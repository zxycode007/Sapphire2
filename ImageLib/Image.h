#pragma once
#include "stdafx.h"
#include "Color.h"


enum EIMAGE_FORMAT
{
	EIF_JPEG,
	EIF_PNG,
	EIF_BMP,
	EIF_GIF,
	EIF_TGA
};

class Image
{
public:
	Image(ECOLOR_FORMAT format, uint32 width, uint32 height, uint32 depth);
	Image(ECOLOR_FORMAT format, uint32 width, uint32 height, uint32 depth, uint8* data);
	~Image();

	void release();
	Color getPixel(int x, int y);
	//Color getPixel(int x, int y, int z);
	void  setPixel(int x, int y, Color&, bool blend);

	int   getHeight();
	int   getWidth();
	uint8* getImageData();
	void  setSize(int width, int height, int depth);
	void  setImageData(uint8* data);
	ulong getImageDataSize();
	void  fillColor(Color& c);
	ulong getRowSize();

	ECOLOR_FORMAT getColorFormat();

	//Image* convertToOtherECF(const ECOLOR_FORMAT format);

	static uint32 getBitsPerPixelFromFormat(const ECOLOR_FORMAT format)
	{
		switch (format)
		{
		case ECF_A1R5G5B5:
			return 16;
		case ECF_R5G6B5:
			return 16;
		case ECF_R8G8B8:
			return 24;
		case ECF_A8R8G8B8:
			return 32;
		case ECF_R8G8B8A8:
			return 32;
		case ECF_R16F:
			return 16;
		case ECF_G16R16F:
			return 32;
		case ECF_A16B16G16R16F:
			return 64;
		case ECF_R32F:
			return 32;
		case ECF_G32R32F:
			return 64;
		case ECF_A32B32G32R32F:
			return 128;
		default:
			return 0;
		}
	}


private:

	uint8* mData;
	int mWidth;
	int mHeight;
	int mDepth;
	ulong  mDataSize;
	ulong  mRowSize;
	ulong  mRows;
	ECOLOR_FORMAT mFormat;
	uint32   mPerPixelBytes;

	void initData();
	


};

