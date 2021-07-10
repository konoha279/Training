#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <string>
#define BUFSIZE 4096 
std::wstring exec(const char* cmd) {
	TCHAR buffer[128];
	std::wstring result = L"";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgetws(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}
