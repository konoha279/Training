#include "ChildProcess.h"
#include "cmd.h"
int main(void)
{
	CHAR chBuf[BUFSIZE];
	DWORD dwRead, dwWritten;
	HANDLE hStdin, hStdout;
	BOOL bSuccess;
	DWORD available;
	bool ret = false;
	std::string result = "";
	cmd c;
	if (!c.CreateProcessW())
		printf("\nERROR Create CMD process!");
	
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if ((hStdout == INVALID_HANDLE_VALUE) || (hStdin == INVALID_HANDLE_VALUE))
		ExitProcess(1);
	
	c.readCmd(result);
	WriteFile(hStdout, result.c_str(), (DWORD)result.length() , &dwWritten, NULL);
	while (1)
	{
		do
		{
			ZeroMemory(chBuf, sizeof(chBuf));
			if (!ReadFile(hStdin, chBuf, sizeof(chBuf), &dwRead, NULL) || !dwRead)
				ret = true;
			chBuf[dwRead] = 0;
			if (ret) break;
		} while (dwRead >= sizeof(chBuf));

		//fgets(chBuf, 1024, stdin);
		if (!_strnicmp(chBuf, "exit", 4))
		{
			break;
		}
		
		c.writeCMD(chBuf);
		c.readCmd(result);
		WriteFile(hStdout, result.c_str(),(DWORD) result.length(), &dwWritten, NULL);
	}
	
	c.KillCurrentCmdProcess();
	c.clean();
	return 0;
}