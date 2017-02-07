#include "ImageLoader.h"
#include "Image.h"
#include <Windows.h>
#include "ximage.h"


Image * ImageLoader::LoadImageW(const wchar_t * filePath, EIMAGE_FORMAT format)
{
	uint32 width = 0;
	uint32 height = 0;
	uint32 depth = 0;
	bool ret = false;
	CxImage cimage;

	switch (format)
	{
	case EIF_BMP:
		ret = cimage.Load(filePath, CXIMAGE_FORMAT_BMP);
		break;
	case EIF_JPEG:
		ret = cimage.Load(filePath, CXIMAGE_FORMAT_JPG);
		break;
	case EIF_PNG:
		ret = cimage.Load(filePath, CXIMAGE_FORMAT_PNG);
		break;
	case EIF_TGA:
		ret = cimage.Load(filePath, CXIMAGE_FORMAT_TGA);
		break;
	case EIF_GIF:
		ret = cimage.Load(filePath, CXIMAGE_FORMAT_GIF);
		break;
	default:
		break;
	}
	if (ret)
	{
		width = cimage.GetWidth();
		height = cimage.GetHeight();
		depth = cimage.GetBpp();
		long size = width*height*Image::getBitsPerPixelFromFormat(ECF_R8G8B8A8)/8;
		uint8* buf = NULL;
		Image* img = new Image(ECF_R8G8B8A8, width, height, Image::getBitsPerPixelFromFormat(ECF_R8G8B8A8)/8,buf);
		ret = cimage.Encode2RGBA(buf, size);
		cimage.Clear();
		return img;
		
	}

	return NULL;
}

Image * ImageLoader::LoadImageW(const wchar_t * filePath, EIMAGE_FORMAT format, uint32 width, uint32 height)
{
	return nullptr;
}
