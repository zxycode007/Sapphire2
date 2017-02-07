#include "ImageWriter.h"
#include <Windows.h>
#include "ximage.h"

bool ImageWriter::ImageWriterW(Image * imag, const wchar_t * filePath, EIMAGE_FORMAT format)
{
	if (!imag)
	{
		return false;
	}
	CxImage cimage;
	if (!imag)
	{
		return false;
	}
	DWORD fmt = 0;
	switch (format)
	{
	case EIF_JPEG:
		fmt = CXIMAGE_FORMAT_JPG;
		break;
	case EIF_PNG:
		fmt = CXIMAGE_FORMAT_PNG;
		break;
	case EIF_BMP:
		fmt = CXIMAGE_FORMAT_BMP;
		break;
	case EIF_GIF:
		fmt = CXIMAGE_FORMAT_GIF;
		break;
	case EIF_TGA:
		fmt = CXIMAGE_FORMAT_TGA;
		break;
	default:
		break;
	}
	bool ret = cimage.Decode(imag->getImageData(), imag->getImageDataSize(), fmt);
	if (ret)
	{
		cimage.Save(filePath, fmt);
		cimage.Clear();
	}
	return ret;

}
