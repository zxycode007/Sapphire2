#pragma once

#include "stdafx.h"
#include "Image.h"


class ImageWriter
{
public:
	static bool ImageWriterW(Image* imag, const wchar_t*  filePath, EIMAGE_FORMAT format);
};
