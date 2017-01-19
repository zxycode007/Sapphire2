#pragma once 

#include "Serializer.h"
#include "Deserializer.h"


namespace Sapphire
{

	/// Memory area that can be read and written to as a stream.
	class SAPPHIRE_API MemoryBuffer : public Deserializer, public Serializer
	{
	public:
		/// Construct with a pointer and size.
		MemoryBuffer(void* data, unsigned size);
		/// Construct as read-only with a pointer and size.
		MemoryBuffer(const void* data, unsigned size);
		/// Construct from a vector, which must not go out of scope before MemoryBuffer.
		MemoryBuffer(PODVector<unsigned char>& data);
		/// Construct from a read-only vector, which must not go out of scope before MemoryBuffer.
		MemoryBuffer(const PODVector<unsigned char>& data);

		/// Read bytes from the memory area. Return number of bytes actually read.
		virtual unsigned Read(void* dest, unsigned size);
		/// Set position from the beginning of the memory area.
		virtual unsigned Seek(unsigned position);
		/// Write bytes to the memory area.
		virtual unsigned Write(const void* data, unsigned size);

		/// Return memory area.
		unsigned char* GetData() { return buffer_; }

		/// Return whether buffer is read-only.
		bool IsReadOnly() { return readOnly_; }

	private:
		/// Pointer to the memory area.
		unsigned char* buffer_;
		/// Read-only flag.
		bool readOnly_;
	};
}