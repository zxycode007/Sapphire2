#pragma once

#include "Deserializer.h"
#include "Serializer.h"

namespace Sapphire
{

	/// Dynamically sized buffer that can be read and written to as a stream.
	class SAPPHIRE_API VectorBuffer : public Deserializer, public Serializer
	{
	public:
		/// Construct an empty buffer.
		VectorBuffer();
		/// Construct from another buffer.
		VectorBuffer(const PODVector<unsigned char>& data);
		/// Construct from a memory area.
		VectorBuffer(const void* data, unsigned size);
		/// Construct from a stream.
		VectorBuffer(Deserializer& source, unsigned size);

		/// Read bytes from the buffer. Return number of bytes actually read.
		virtual unsigned Read(void* dest, unsigned size);
		/// Set position from the beginning of the buffer.
		virtual unsigned Seek(unsigned position);
		/// Write bytes to the buffer. Return number of bytes actually written.
		virtual unsigned Write(const void* data, unsigned size);

		/// Set data from another buffer.
		void SetData(const PODVector<unsigned char>& data);
		/// Set data from a memory area.
		void SetData(const void* data, unsigned size);
		/// Set data from a stream.
		void SetData(Deserializer& source, unsigned size);
		/// Reset to zero size.
		void Clear();
		/// Set size.
		void Resize(unsigned size);

		/// Return data.
		const unsigned char* GetData() const { return size_ ? &buffer_[0] : 0; }

		/// Return non-const data.
		unsigned char* GetModifiableData() { return size_ ? &buffer_[0] : 0; }

		/// Return the buffer.
		const PODVector<unsigned char>& GetBuffer() const { return buffer_; }

	private:
		/// Dynamic data buffer.
		PODVector<unsigned char> buffer_;
	};

}