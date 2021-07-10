#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>

#define MAX_BUFFER_SIZE 512
class cmd
{
private:
	STARTUPINFO sti = { 0 };
	SECURITY_ATTRIBUTES sats = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
	DWORD writ, excode, read, available;
	bool isRunning = false;
public:
	char buffer[MAX_BUFFER_SIZE];

	int CreateProcess();
	bool isCmdStillRunning();
	int readCmd(std::string &result);
	int writeCMD(char *data);
	bool KillCurrentCmdProcess();
	void clean();
};

