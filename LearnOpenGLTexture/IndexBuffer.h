#pragma once

#include "Vertex.h"
#include <vector>

namespace Sapphire
{
	class IndexBuffer
	{
		class IIndexArray
		{
		public:

			virtual void AddIndex(UINT32 index) = 0;
			virtual UINT32 getSize() = 0;
			virtual int getStride() = 0;
			virtual bool getData(byte* buf, ULONG& size, int& stride) = 0;
			virtual UINT32  operator[] (UINT32 index) = 0;
			virtual bool setValue(UINT32 index, ULONG value) = 0;

		};

		template<class T>
		class IndexArray : public IIndexArray
		{
		public:

			void AddIndex(UINT32 index)
			{
				mIndexs.push_back((T&)index);
			}

			UINT32 getSize()
			{
				return mIndexs.size();
			}

			int getStride()
			{
				return sizeof(T);
			}

			bool getData(byte* buf, ULONG& size, int& stride)
			{
				if (mIndexs.size() > 0)
				{
					buf = (byte*)mIndexs.begin()._Ptr;
					size = getSize()*getStride();
					stride = getStride();
					return true;
				}
				return false;
			};

			UINT32   operator[](UINT32 index)
			{
				return mIndexs[index];
			}

			bool setValue(UINT32 index, ULONG value)
			{
				if (index > mIndexs.size())
				{
					return false;
				}
				mIndexs[index] = value;
			}

		private:

			std::vector<T> mIndexs;

		};

		typedef IndexArray<int>    IndexArray32;
		typedef IndexArray<short>  IndexArray16;

	public:

		IndexBuffer(EIndexType type)
		{
			setType(type);

		}

		void setType(EIndexType type)
		{
			IIndexArray* indexList = 0;
			switch (type)
			{
			case Sapphire::EIT_16BIT:
				indexList = new IndexArray16();
				break;
			case Sapphire::EIT_32BIT:
				indexList = new IndexArray32();
				break;
			default:
				break;
			}
			if (mIndexList)
			{
				delete mIndexList;
			}
			mIndexList = indexList;
		}

		UINT32   operator[](UINT32 index)
		{
			if (index > mIndexList->getSize())
			{
			}
			return  (*mIndexList)[index];
		}

		int getStride()
		{
			return mIndexList->getStride();
		}

		int size()
		{
			return mIndexList->getSize();
		}

		void AddIndex(UINT32 index)
		{
			mIndexList->AddIndex(index);
		}

		void setValue(UINT32 index, ULONG value)
		{
			mIndexList->setValue(index, value);
		}

		bool getData(byte* buf, ULONG& size, int& stride)
		{
			return mIndexList->getData(buf, size, stride);
		}

	private:

		
		
		EIndexType mType;
		IIndexArray* mIndexList;

	};

}
