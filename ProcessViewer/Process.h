#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <list> 
#include <iostream>
#include <string>
#include <vector>
class Process
{
protected:
	
public:
	DWORD pid;
	std::wstring name;
	std::wstring command;
	std::vector<std::wstring> Modules;
};