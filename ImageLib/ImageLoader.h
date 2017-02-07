#pragma once
#include "stdafx.h"
#include "Image.h"




class ImageLoader
{
public:
	static Image*  LoadImageW(const wchar_t* filePath, EIMAGE_FORMAT format);

	static Image*  LoadImageW(const wchar_t* filePath, EIMAGE_FORMAT format, uint32 width, uint32 height);

};