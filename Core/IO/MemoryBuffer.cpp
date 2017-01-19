#include "Predefined.h"

#include "MemoryBuffer.h"


namespace Sapphire
{

	MemoryBuffer::MemoryBuffer(void* data, unsigned size) :
		Deserializer(size),
		buffer_((unsigned char*)data),
		readOnly_(false)
	{
		if (!buffer_)
			size_ = 0;
	}

	MemoryBuffer::MemoryBuffer(const void* data, unsigned size) :
		Deserializer(size),
		buffer_((unsigned char*)data),
		readOnly_(true)
	{
		if (!buffer_)
			size_ = 0;
	}

	MemoryBuffer::MemoryBuffer(PODVector<unsigned char>& data) :
		Deserializer(data.Size()),
		buffer_(data.Begin().ptr_),
		readOnly_(false)
	{
	}

	MemoryBuffer::MemoryBuffer(const PODVector<unsigned char>& data) :
		Deserializer(data.Size()),
		buffer_(data.Begin().ptr_),
		readOnly_(true)
	{
	}

	unsigned MemoryBuffer::Read(void* dest, unsigned size)
	{
		if (size + position_ > size_)
			size = size_ - position_;
		if (!size)
			return 0;

		unsigned char* srcPtr = &buffer_[position_];
		unsigned char* destPtr = (unsigned char*)dest;
		position_ += size;

		unsigned copySize = size;
		while (copySize >= sizeof(unsigned))
		{
			*((unsigned*)destPtr) = *((unsigned*)srcPtr);
			srcPtr += sizeof(unsigned);
			destPtr += sizeof(unsigned);
			copySize -= sizeof(unsigned);
		}
		if (copySize & sizeof(unsigned short))
		{
			*((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
			srcPtr += sizeof(unsigned short);
			destPtr += sizeof(unsigned short);
		}
		if (copySize & 1)
			*destPtr = *srcPtr;

		return size;
	}

	unsigned MemoryBuffer::Seek(unsigned position)
	{
		if (position > size_)
			position = size_;

		position_ = position;
		return position_;
	}

	unsigned MemoryBuffer::Write(const void* data, unsigned size)
	{
		if (size + position_ > size_)
			size = size_ - position_;
		if (!size)
			return 0;

		unsigned char* srcPtr = (unsigned char*)data;
		unsigned char* destPtr = &buffer_[position_];
		position_ += size;

		unsigned copySize = size;
		while (copySize >= sizeof(unsigned))
		{
			*((unsigned*)destPtr) = *((unsigned*)srcPtr);
			srcPtr += sizeof(unsigned);
			destPtr += sizeof(unsigned);
			copySize -= sizeof(unsigned);
		}
		if (copySize & sizeof(unsigned short))
		{
			*((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
			srcPtr += sizeof(unsigned short);
			destPtr += sizeof(unsigned short);
		}
		if (copySize & 1)
			*destPtr = *srcPtr;

		return size;
	}
}