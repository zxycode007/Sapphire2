#pragma once
#include "stdafx.h"
#include <string>

int Replace(string& source, const string& replaceThis, const string& replaceWith);

std::string ConvertWStringToString(std::wstring & src, const char* locale);

std::wstring ConvertStringToWString(std::string& src, const char* locale);

