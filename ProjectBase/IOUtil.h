#pragma once
#include "stdafx.h"
#include <string>



string readTextFile(const char* filename);

bool CreateDir(const char* path);

bool DeleteDir(const char* path);

size_t GetFileSize(const char* fileName);

void FileCopy(const char* src, const char* dest, int bufSize);

string  GetCurrentDir();

