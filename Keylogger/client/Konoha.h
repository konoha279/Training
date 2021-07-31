#pragma once
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <shlobj_core.h>
#include <string>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <filesystem>
#include <atlstr.h>
using namespace std;

class Konoha
{
private:

public: 
	wstring utf8_to_wstring(const std::string& str);
	string wstring_to_utf8(const std::wstring& str);
	bool changeValueRegistryUAC(int value);
	bool changeValueRegistryShowALL(int value);
	bool setKeyStartupWithWindows(string name, string src);
	wstring s2ws(const std::string& s); //string to wstring
	wstring getCurrentFolderAddress();
	void MoveFile(experimental::filesystem::path src, experimental::filesystem::path des);
	wstring getPathCurrent();
};

