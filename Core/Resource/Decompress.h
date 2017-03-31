#pragma once

#include "Image.h"

namespace Sapphire
{
	/// Decompress a DXT compressed image to RGBA.
	SAPPHIRE_API void
		DecompressImageDXT(unsigned char* dest, const void* blocks, int width, int height, int depth, CompressedFormat format);
	/// Decompress an ETC1 compressed image to RGBA.
	SAPPHIRE_API void DecompressImageETC(unsigned char* dest, const void* blocks, int width, int height);
	/// Decompress a PVRTC compressed image to RGBA.
	SAPPHIRE_API void DecompressImagePVRTC(unsigned char* dest, const void* blocks, int width, int height, CompressedFormat format);
	/// Flip a compressed block vertically.
	SAPPHIRE_API void FlipBlockVertical(unsigned char* dest, unsigned char* src, CompressedFormat format);
	/// Flip a compressed block horizontally.
	SAPPHIRE_API void FlipBlockHorizontal(unsigned char* dest, unsigned char* src, CompressedFormat format);
}