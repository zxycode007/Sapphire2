#pragma once

#include "Image.h"

namespace Sapphire
{
	/// 解压DXT到RGBA
	SAPPHIRE_API void
		DecompressImageDXT(unsigned char* dest, const void* blocks, int width, int height, int depth, CompressedFormat format);
	/// 解压ETC1到RGBA
	SAPPHIRE_API void DecompressImageETC(unsigned char* dest, const void* blocks, int width, int height);
	/// 解压PVRTC到RGBA
	SAPPHIRE_API void DecompressImagePVRTC(unsigned char* dest, const void* blocks, int width, int height, CompressedFormat format);
	/// 垂直翻转一块压缩块
	SAPPHIRE_API void FlipBlockVertical(unsigned char* dest, unsigned char* src, CompressedFormat format);
	/// 水平翻转一个压缩块
	SAPPHIRE_API void FlipBlockHorizontal(unsigned char* dest, unsigned char* src, CompressedFormat format);
}