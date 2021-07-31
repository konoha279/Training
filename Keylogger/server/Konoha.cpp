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