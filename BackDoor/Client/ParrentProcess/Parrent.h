#pragma once
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#define BUFSIZE 4096 
class Parrent
{
private:
	
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;

	HANDLE g_hInputFile = NULL;
	SECURITY_ATTRIBUTES saAttr;
public:
	char text[BUFSIZE];
	CHAR bufRead[BUFSIZE] = { 0 };

	void Setup(void);
	void CreateChildProcess(void);
	void WriteToPipe(void);
	void WriteToPipeWithText(void);
	void ReadFromPipe(void);
	int SendResult(SOCKET socket);
	void ErrorExit(PTSTR lpszFunction);
};

