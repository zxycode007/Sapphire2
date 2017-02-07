#include "StringUtil.h"
#include "IOUtil.h"
#include <io.h>
#include <direct.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <Windows.h>



string readTextFile(const char* filename)
{
	int ret = _access(filename, 0);
	string str;
	if (ret != -1)
	{
		FILE* f = fopen(filename, "r");
		char ch;
		while (EOF != (ch = fgetc(f)))
		{
			str.push_back(ch);
		}
		fclose(f);
	}
	return str;
}

bool CreateDir(const char* path)
{
	if (_mkdir(path) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DeleteDir(const char* path)
{
	if (_rmdir(path) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

size_t GetFileSize(const char* fileName)
{
	size_t size = 0;
	if (_access(fileName, 0) != -1)
	{
		FILE* f = fopen(fileName, "r+");
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fclose(f);
	}
	return size;
}

void FileCopy(const char* src, const char* dest, int bufSize)
{
	size_t src_size = 0;
	if (_access(src, 0) == -1)
		return;
	src_size = GetFileSize(src);
	byte* buf = new byte[bufSize];
	memset(buf, 0, bufSize);
	FILE* fin = fopen(src, "rb+");
	FILE* fout = fopen(dest, "wb+");
	int ret = fread(buf, sizeof(byte), bufSize, fin);
	while (ret > 0)
	{
		fwrite(buf, sizeof(byte), ret, fout);
		ret = fread(buf, sizeof(byte), bufSize, fin);
	}
	fclose(fin);
	fclose(fout);
	delete[] buf;
}


string  GetCurrentDir()
{
	char   buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	return string(buffer);
}