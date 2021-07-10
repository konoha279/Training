#include "cmd.h"

int cmd::CreateProcess()
{
	sats.nLength = sizeof(sats);
	sats.bInheritHandle = TRUE;
	sats.lpSecurityDescriptor = NULL;

	//create child's stdout pipes
	if (!CreatePipe(&pipout_r, &pipout_w, &sats, 0)) return 0;
	//and its stdin pipes
	if (!CreatePipe(&pipin_r, &pipin_w, &sats, 0)) return 0;
	//printf("Created pipes\n");

	//now set STARTUPINFO struct fields (from the child's point of view)
	sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	sti.wShowWindow = SW_HIDE;
	sti.hStdInput = pipin_r;
	sti.hStdOutput = pipout_w;
	sti.hStdError = pipout_w;

	//create the process...
	if (!CreateProcessA(NULL, (LPSTR) "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, (LPSTARTUPINFOA)&sti, &pi)) return 0;
	isRunning = true;
	Sleep(500);
	return 1;
}

bool cmd::isCmdStillRunning()
{
	DWORD excode;
	if (!isRunning)
		return false;
	GetExitCodeProcess(pi.hProcess, &excode);
	if (excode != STILL_ACTIVE)
		isRunning = false;
	return isRunning;
}

int cmd::readCmd(std::string &result)
{
	bool ret = false;
	if (!isRunning)
	{
		return 0;
	}
	result = "";
	if (!PeekNamedPipe(pipout_r, buffer, sizeof(buffer), &read, &available, NULL)) return 0;
	
	//is there anything to be read in the pipe?
	if (read)
	{
		do
		{
			ZeroMemory(buffer, sizeof(buffer));
			//read it and print to stdout
			if (!ReadFile(pipout_r, buffer, sizeof(buffer), &read, NULL) || !read)
				ret = true;
			buffer[read] = 0;
			result += buffer;
			//fprintf(stdout, "%s", buffer);
			if (ret) break;
		} while (read >= sizeof(buffer));
	}
	else
		return 0;	
	return 1;
}

int cmd::writeCMD(char * data)
{
	DWORD writ;
	if (!WriteFile(pipin_w, data, strlen(data), &writ, NULL) || !isRunning)
		return false;
	else
	{
		Sleep(100);
		return true;
	}
}

bool cmd::KillCurrentCmdProcess()
{
	if (isRunning)
		TerminateProcess(pi.hProcess, 0);
	isRunning = false;
	return true;
}

void cmd::clean()
{
	//clean up any unfinished business
	if (pipin_w != NULL) CloseHandle(pipin_w);
	if (pipin_r != NULL) CloseHandle(pipin_r);
	if (pipout_w != NULL) CloseHandle(pipout_w);
	if (pipout_r != NULL) CloseHandle(pipout_r);
	if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
	if (pi.hThread != NULL) CloseHandle(pi.hThread);
}
