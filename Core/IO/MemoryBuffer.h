#pragma once 

#include "Serializer.h"
#include "Deserializer.h"


namespace Sapphire
{

	/// 可以作为一个Stream被读写的内存区域
	class SAPPHIRE_API MemoryBuffer : public Deserializer, public Serializer
	{
	public:
		/// 用一个指针和大小构造
		MemoryBuffer(void* data, unsigned size);
		/// 构造一个只读的缓冲区
		MemoryBuffer(const void* data, unsigned size);
	    ///从一个Vector构造
		MemoryBuffer(PODVector<unsigned char>& data);
		///从一个Vector构造
		MemoryBuffer(const PODVector<unsigned char>& data);

		/// 从内存区域读取字节数，返回实际读取的字节数
		virtual unsigned Read(void* dest, unsigned size);
		/// 设置内存区域的起始位置 
		virtual unsigned Seek(unsigned position);
		/// 写字节到内存区域
		virtual unsigned Write(const void* data, unsigned size);

		/// 返回内存区域指针
		unsigned char* GetData() { return buffer_; }

		/// 返回是否只读
		bool IsReadOnly() { return readOnly_; }

	private:
		/// 内存区域指针
		unsigned char* buffer_;
		/// 是否只读
		bool readOnly_;
	};
}