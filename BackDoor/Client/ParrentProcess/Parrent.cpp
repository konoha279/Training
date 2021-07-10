#include "Parrent.h"

void Parrent::Setup(void)
{
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		ErrorExit(PTSTR(L"StdoutRd CreatePipe"));

	// Ensure the read handle to the pipe for STDOUT is not inherited.

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		ErrorExit(PTSTR(L"Stdout SetHandleInformation"));

	// Create a pipe for the child process's STDIN. 
	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		ErrorExit(PTSTR(L"Stdin CreatePipe"));

	// Ensure the write handle to the pipe for STDIN is not inherited. 

	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		ErrorExit(PTSTR(L"Stdin SetHandleInformation"));
}

void Parrent::CreateChildProcess(void)
{
	TCHAR szCmdline[] = TEXT("ChildProcess.exe");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bSuccess = CreateProcess(NULL,
		szCmdline,     
		NULL,          
		NULL,          
		TRUE,          
		0,            
		NULL,          
		NULL,        
		&siStartInfo,  
		&piProcInfo); 

	if (!bSuccess)
		ErrorExit(PTSTR(L"CreateProcess"));

	else
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		// Close handles to the stdin and stdout pipes no longer needed by the child process.
		// If they are not explicitly closed, there is no way to recognize that the child process has ended.

		CloseHandle(g_hChildStd_OUT_Wr);
		CloseHandle(g_hChildStd_IN_Rd);
	}
}

void Parrent::WriteToPipe(void)
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[] = "ipconfig";
	BOOL bSuccess = FALSE;

	bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, sizeof(chBuf), &dwWritten, NULL);
	if (!bSuccess) ErrorExit(PTSTR(L"Errror Writefile !"));

	//// Close the pipe handle so the child process stops reading. 

	//if (!CloseHandle(g_hChildStd_IN_Wr))
	//	ErrorExit(PTSTR(L"StdInWr CloseHandle"));
}

void Parrent::WriteToPipeWithText()
{
	DWORD dwRead, dwWritten;
	BOOL bSuccess = FALSE;

	bSuccess = WriteFile(g_hChildStd_IN_Wr, text, strlen(text), &dwWritten, NULL);
	if (!bSuccess) ErrorExit(PTSTR(L"Errror Writefile !"));
}

void Parrent::ReadFromPipe(void)
{
	DWORD dwRead, dwWritten;
	
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	do
	{
		ZeroMemory(bufRead, sizeof(bufRead));
		//read it and print to stdout
		ReadFile(g_hChildStd_OUT_Rd, bufRead, sizeof(bufRead), &dwRead, NULL);

		bufRead[dwRead] = 0;
		WriteFile(hParentStdOut, bufRead, dwRead, &dwWritten, NULL);
	} while (dwRead >= sizeof(bufRead));
}

int Parrent::SendResult(SOCKET socket)
{
	DWORD dwRead, dwWritten;

	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for (;;)
	{
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, bufRead, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = WriteFile(hParentStdOut, bufRead, dwRead, &dwWritten, NULL);
		if (!bSuccess) break;

		if (send(socket, bufRead, dwRead, 0) == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(socket);
			WSACleanup();
			return -1;
		}
	}
}

void Parrent::ErrorExit(PTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}
