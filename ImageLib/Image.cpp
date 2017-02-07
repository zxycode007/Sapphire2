#include <iostream>
#include "Image.h"


Image::Image(ECOLOR_FORMAT format, uint32 width, uint32 height, uint32 depth)
{
	mFormat = format;
	mWidth = mWidth;
	mHeight = mHeight;
	mDepth = depth;
	mData = NULL;
	mDataSize = 0;
	mPerPixelBytes = 0;
	mRows = 0;
	mRowSize = 0;

	initData();
	
}

Image::Image(ECOLOR_FORMAT format, uint32 width, uint32 height, uint32 depth, uint8 * data)
{
	mFormat = format;
	mWidth = mWidth;
	mHeight = mHeight;
	mDepth = depth;
	mData = NULL;
	mDataSize = 0;
	mPerPixelBytes = 0;
	mRows = 0;
	mRowSize = 0;

	if (!mData)
	{
		mData = (uint8*)0xbadf00d;
		initData();
		mData = data;
	}
	else
	{
		initData();
		memcpy(mData, data, mRowSize*mHeight);
	}
}

Image::~Image()
{
	if (mData)
	{
		delete[] mData;
		mData = NULL;
	}
}

void Image::release()
{
	if (mData)
	{
		delete[] mData;
		mData = NULL;
	}
	delete this;
}

Color Image::getPixel(int x, int y)
{

	Color c;
	if (x >= mWidth || y >= mHeight)
		return c;

	switch (mFormat)
	{
	case ECF_A1R5G5B5:
		c.setAsARGB(A1R5G5B5toA8R8G8B8(((uint16*)mData)[y*mWidth + x]));
		return c; //A1R5G5B5toA8R8G8B8(((uint16*)Data)[y*Size.Width + x]);
	case ECF_R5G6B5:
		c.setAsARGB(R5G6B5toA8R8G8B8(((uint16*)mData)[y*mWidth + x]));
		return c;//R5G6B5toA8R8G8B8(((uint16*)Data)[y*Size.Width + x]);
	case ECF_A8R8G8B8:
		c.setAsARGB(((uint32*)mData)[y*mWidth + x]);
		return c;// ((UINT32*)Data)[y*Size.Width + x];
	case ECF_R8G8B8A8:
		c.setAsRGBA(((uint32*)mData)[y*mWidth + x]);
		return c;// ((UINT32*)Data)[y*Size.Width + x];
	case ECF_R8G8B8:
	{
		uint8* p = mData + (y * 3)*mWidth + (x * 3);
		c.setAlpha(255);
		c.setRed(p[0]);
		c.setBlue(p[1]);
		c.setGreen(p[2]);
		return c;
	}
	default:
		break;
	}

	return Color(0);
}

void Image::setPixel(int x, int y, Color & color, bool blend)
{
	Color c = color;
	if (x >= mWidth || y >= mHeight)
		return;

	switch (mFormat)
	{
	case ECF_A1R5G5B5:
	{
		uint16 * dest = (uint16*)(mData + (y * mRowSize) + (x << 1));  //2字节 16位
		*dest = A8R8G8B8toA1R5G5B5(color.getAsARGB());
	} break;

	case ECF_R5G6B5:
	{
		uint16 * dest = (uint16*)(mData + (y * mRowSize) + (x << 1));   //2字节 16位
		*dest = A8R8G8B8toR5G6B5(color.getAsARGB());
	} break;

	case ECF_R8G8B8:                                              //3字节  24位
	{
		uint8* dest = mData + (y * mRowSize) + (x * 3);

		dest[0] = (uint8)c.getRed();
		dest[1] = (uint8)c.getGreen();
		dest[2] = (uint8)c.getBlue();
	} break;

	case ECF_A8R8G8B8:                                 ////4字节 32位
	{
		uint32 * dest = (uint32*)(mData + (y * mRowSize) + (x << 2));
		*dest = blend ? PixelBlend32(*dest, color.getAsARGB()) : color.getAsARGB();
	} break;

	case ECF_R8G8B8A8:                                 ////4字节 32位
	{
		uint32 * dest = (uint32*)(mData + (y * mRowSize) + (x << 2));
		*dest = blend ? PixelBlend32(*dest, color.getAsRGBA()) : color.getAsRGBA();
	} break;
	default:
		break;
	}
}

int Image::getHeight()
{
	return mHeight;
}

int Image::getWidth()
{
	return mWidth;
}

uint8 * Image::getImageData()
{
	return nullptr;
}

void Image::setSize(int width, int height, int depth)
{
	mWidth = width;
	mHeight = height;
	mDepth = depth;
}

void Image::setImageData(uint8 * data)
{
	if (data)
	{
		mData = data;
	}
}

ulong Image::getImageDataSize()
{
	return mDataSize;
}

void Image::fillColor(Color & color)
{
	uint32 c;

	switch (mFormat)
	{
	case ECF_A1R5G5B5:
		c = A8R8G8B8toA1R5G5B5(color.getAsARGB());
		c |= c << 16;
		break;
	case ECF_R5G6B5:
		c = A8R8G8B8toR5G6B5(color.getAsARGB());
		c |= c << 16;
		break;
	case ECF_A8R8G8B8:
		c = color.getAsARGB();
		break;
	case ECF_R8G8B8A8:
		c = color.getAsRGBA();
		break;
	case ECF_R8G8B8:   //24位不好处理, 每个字节单个处理
	{
		uint8 rgb[3];
		convert_A8R8G8B8toR8G8B8(&color, 1, rgb);
		const uint32 size = getImageDataSize();
		for (uint32 i = 0; i<size; i += 3)
		{
			memcpy(mData + i, rgb, 3);
		}
		return;
	}
	break;
	default:
		// 其他格式，待定
		return;
	}
	memset32bit(mData, c, getImageDataSize());
}

ulong Image::getRowSize()
{
	return mRowSize;
}

ECOLOR_FORMAT Image::getColorFormat()
{
	return mFormat;
}

void Image::initData()
{
	mPerPixelBytes = getBitsPerPixelFromFormat(mFormat);
	mRowSize = mWidth * mPerPixelBytes;

	mData = new uint8[mRowSize * mHeight];

}


