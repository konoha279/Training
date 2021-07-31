#pragma once
#include <Windows.h>
#include <stdio.h>
#include <shlobj_core.h>
#include <string>
#include <iostream>
#include <codecvt>
using namespace std;

class Konoha
{
private:

public:
	wstring utf8_to_wstring(const std::string& str);
	string wstring_to_utf8(const std::wstring& str);
};

