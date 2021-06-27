#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <list> 
#include <iostream>
#include <string>
#include <vector>
#include <winternl.h>
#include "Process.h"

class ProcessList
{
public:
	std::vector<std::wstring> getModules(DWORD processID);
	std::vector<Process> getRunningProcesses();
};

