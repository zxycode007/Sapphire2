#include "StringUtil.h"
#include <string>


int Replace(string& source, const string& replaceThis, const string& replaceWith)
{
	unsigned nextPos = 0;
	int ret = 0;
	while (nextPos < source.length())
	{
		unsigned pos = source.find(replaceThis, nextPos);
		if (pos == std::string::npos)
			break;
		source.replace(pos, replaceThis.length(), replaceWith);
		nextPos = pos + replaceWith.length();
		ret++;
	}
	return ret;
}

std::string ConvertWStringToString(std::wstring & src, const char* locale)
{
	char* oldLoc = setlocale(LC_ALL, "");
	setlocale(LC_ALL, locale);
	const wchar_t *WStr = src.c_str();
	//计算出字符串长度
	size_t len = wcslen(src.c_str())*2 + 1;
	size_t converted = 0;
	char *CStr;
	CStr = (char*)malloc(len * sizeof(char));
	wcstombs_s(&converted, CStr, len, WStr, _TRUNCATE);
	std::string retStr(CStr);
	free(CStr);
	setlocale(LC_ALL, oldLoc);
	return  retStr;
}

std::wstring ConvertStringToWString(std::string& src, const char* locale)
{
	char* oldLoc = setlocale(LC_ALL, "");
	setlocale(LC_ALL, locale);
	const char *CStr = src.c_str();
	size_t len = strlen(CStr) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
	std::wstring retStr(WStr);
	free(WStr);
	setlocale(LC_ALL, oldLoc);
	return retStr;
}