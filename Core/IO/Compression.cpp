#include "Predefined.h"

#include "../Container/ArrayPtr.h"
#include "Compression.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "VectorBuffer.h"

#include <LZ4/lz4.h>
#include <LZ4/lz4hc.h>


namespace Sapphire
{

	unsigned EstimateCompressBound(unsigned srcSize)
	{
		return (unsigned)LZ4_compressBound(srcSize);
	}

	unsigned CompressData(void* dest, const void* src, unsigned srcSize)
	{
		if (!dest || !src || !srcSize)
			return 0;
		else
			return (unsigned)LZ4_compressHC((const char*)src, (char*)dest, srcSize);
	}

	unsigned DecompressData(void* dest, const void* src, unsigned destSize)
	{
		if (!dest || !src || !destSize)
			return 0;
		else
			return (unsigned)LZ4_decompress_fast((const char*)src, (char*)dest, destSize);
	}

	bool CompressStream(Serializer& dest, Deserializer& src)
	{
		unsigned srcSize = src.GetSize() - src.GetPosition();
		// Prepend the source and dest. data size in the stream so that we know to buffer & uncompress the right amount
		if (!srcSize)
		{
			dest.WriteUInt(0);
			dest.WriteUInt(0);
			return true;
		}

		unsigned maxDestSize = (unsigned)LZ4_compressBound(srcSize);
		SharedArrayPtr<unsigned char> srcBuffer(new unsigned char[srcSize]);
		SharedArrayPtr<unsigned char> destBuffer(new unsigned char[maxDestSize]);

		if (src.Read(srcBuffer, srcSize) != srcSize)
			return false;

		unsigned destSize = (unsigned)LZ4_compressHC((const char*)srcBuffer.Get(), (char*)destBuffer.Get(), srcSize);
		bool success = true;
		success &= dest.WriteUInt(srcSize);
		success &= dest.WriteUInt(destSize);
		success &= dest.Write(destBuffer, destSize) == destSize;
		return success;
	}

	bool DecompressStream(Serializer& dest, Deserializer& src)
	{
		if (src.IsEof())
			return false;

		unsigned destSize = src.ReadUInt();
		unsigned srcSize = src.ReadUInt();
		if (!srcSize || !destSize)
			return true; // No data

		if (srcSize > src.GetSize())
			return false; // Illegal source (packed data) size reported, possibly not valid data

		SharedArrayPtr<unsigned char> srcBuffer(new unsigned char[srcSize]);
		SharedArrayPtr<unsigned char> destBuffer(new unsigned char[destSize]);

		if (src.Read(srcBuffer, srcSize) != srcSize)
			return false;

		LZ4_decompress_fast((const char*)srcBuffer.Get(), (char*)destBuffer.Get(), destSize);
		return dest.Write(destBuffer, destSize) == destSize;
	}

	VectorBuffer CompressVectorBuffer(VectorBuffer& src)
	{
		VectorBuffer ret;
		src.Seek(0);
		CompressStream(ret, src);
		ret.Seek(0);
		return ret;
	}

	VectorBuffer DecompressVectorBuffer(VectorBuffer& src)
	{
		VectorBuffer ret;
		src.Seek(0);
		DecompressStream(ret, src);
		ret.Seek(0);
		return ret;
	}
}