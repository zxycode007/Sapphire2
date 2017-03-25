#pragma once

#include "../Variant.h"
#include "BoundingBox.h"
#include "Rect.h"

namespace Sapphire
{
	/// 抽象的读取流
	class SAPPHIRE_API Deserializer
	{
	public:
		/// 用0大小构造
		Deserializer();
		/// 用指定的大小构造
		Deserializer(unsigned size);
		virtual ~Deserializer();

		/// 从这个流中读取字节，返回实际读取的字节数
		virtual unsigned Read(void* dest, unsigned size) = 0;
		/// 从这个流的起始处开始设置位置
		virtual unsigned Seek(unsigned position) = 0;
		/// 返回这个流的名称
		virtual const String& GetName() const;
		/// 如果合适检查校验和
		virtual unsigned GetChecksum();

		/// 返回当前位置
		unsigned GetPosition() const { return position_; }

		/// 返回大小
		unsigned GetSize() const { return size_; }

		/// 返回是否到达流的末尾
		bool IsEof() const { return position_ >= size_; }

		/// Read a 32-bit integer.
		int ReadInt();
		/// Read a 16-bit integer.
		short ReadShort();
		/// Read an 8-bit integer.
		signed char ReadByte();
		/// Read a 32-bit unsigned integer.
		unsigned ReadUInt();
		/// Read a 16-bit unsigned integer.
		unsigned short ReadUShort();
		/// Read an 8-bit unsigned integer.
		unsigned char ReadUByte();
		/// Read a bool.
		bool ReadBool();
		/// Read a float.
		float ReadFloat();
		/// Read a double.
		double ReadDouble();
		/// Read an IntRect.
		IntRect ReadIntRect();
		/// Read an IntVector2.
		IntVector2 ReadIntVector2();
		/// Read a Rect.
		Rect ReadRect();
		/// Read a Vector2.
		Vector2 ReadVector2();
		/// Read a Vector3.
		Vector3 ReadVector3();
		/// Read a Vector3 packed into 3 x 16 bits with the specified maximum absolute range.
		Vector3 ReadPackedVector3(float maxAbsCoord);
		/// Read a Vector4.
		Vector4 ReadVector4();
		/// Read a quaternion.
		Quaternion ReadQuaternion();
		/// Read a quaternion with each component packed in 16 bits.
		Quaternion ReadPackedQuaternion();
		/// Read a Matrix3.
		Matrix3 ReadMatrix3();
		/// Read a Matrix3x4.
		Matrix3x4 ReadMatrix3x4();
		/// Read a Matrix4.
		Matrix4 ReadMatrix4();
		/// Read a color.
		Color ReadColor();
		/// Read a bounding box.
		BoundingBox ReadBoundingBox();
		/// Read a null-terminated string.
		String ReadString();
		/// Read a four-letter file ID.
		String ReadFileID();
		/// Read a 32-bit StringHash.
		StringHash ReadStringHash();
		/// Read a buffer with size encoded as VLE.
		PODVector<unsigned char> ReadBuffer();
		/// Read a resource reference.
		ResourceRef ReadResourceRef();
		/// Read a resource reference list.
		ResourceRefList ReadResourceRefList();
		/// Read a variant.
		Variant ReadVariant();
		/// Read a variant whose type is already known.
		Variant ReadVariant(VariantType type);
		/// Read a variant vector.
		VariantVector ReadVariantVector();
		/// Read a string vector.
		StringVector ReadStringVector();
		/// Read a variant map.
		VariantMap ReadVariantMap();
		/// Read a variable-length encoded unsigned integer, which can use 29 bits maximum.
		unsigned ReadVLE();
		/// Read a 24-bit network object ID.
		unsigned ReadNetID();
		/// Read a text line.
		String ReadLine();

	protected:
		/// Stream position.
		unsigned position_;
		/// Stream size.
		unsigned size_;
	};
}