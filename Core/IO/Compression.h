#pragma once

#include "Sapphire.h"

namespace Sapphire
{

	class Deserializer;
	class Serializer;
	class VectorBuffer;

	/// 对于给定的输入大小估算并且返回最坏情况的LZ4 压缩输出大小
	SAPPHIRE_API unsigned EstimateCompressBound(unsigned srcSize);
	/// 用LZ4压缩算法压缩数据并且返回压缩数据的大小.它需要目标缓冲由EstimateCompressBound()给定的大小
	SAPPHIRE_API unsigned CompressData(void* dest, const void* src, unsigned srcSize);
	/// 用LZ4算法解压缩数据。 未压缩数据的大小必须知道。返回已经解压缩数据大小
	SAPPHIRE_API unsigned DecompressData(void* dest, const void* src, unsigned destSize);
	/// 压缩一个流(从源到目标)，成功返回true
	SAPPHIRE_API bool CompressStream(Serializer& dest, Deserializer& src);
	/// 解压缩一个流到目标流，成功返回true
	SAPPHIRE_API bool DecompressStream(Serializer& dest, Deserializer& src);
	/// 用LZ4算法压缩一个可变缓冲区，返回压缩结果缓冲区
	SAPPHIRE_API VectorBuffer CompressVectorBuffer(VectorBuffer& src);
	/// 解压缩一个可变缓冲区
	SAPPHIRE_API VectorBuffer DecompressVectorBuffer(VectorBuffer& src);


}