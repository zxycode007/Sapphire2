#pragma once
#include "Vertex.h"
#include <vector>
#include "EmptyRefException.h"

namespace Sapphire
{

	class VertexBuffer
	{

		class IVertexArray
		{
		public:
			virtual void AddVertex(Vertex& v) = 0;
			virtual int  getElementSize() = 0;
			virtual bool getData(byte* buf, ULONG& size, UINT32& stride) = 0;
			virtual Vertex&      operator[] (const UINT32 index) const = 0;
			virtual int  getStride() = 0;
			

		};

		template<class T>
		class VertexArray : public IVertexArray
		{
		public:

			VertexArray() {};

			void AddVertex(Vertex& v)
			{
				mVertices.push_back((T&)v);
			};
			int  getElementSize()
			{
				return mVertices.size();
			}
			bool getData(byte* buf, ULONG& size, UINT32& stride)
			{
				if (mVertices.size() > 0)
				{
					/*buf = new byte[sizeof(T)*mVertices.size()];
					memcpy(buf, mVertices.begin(), mVertices.size() * sizeof(T));*/
					buf = (byte*)mVertices.begin()._Ptr;
					size = mVertices.size() * sizeof(T);
					stride = getStride();
					return true;
				}
				else
				{
					return false;
				}

			}
			T&  operator[] (const UINT32 index) const
			{
				if (index < mVertices.size())
				{
					return (T&)mVertices[index];
				}
				else
				{
					throw EmptyRefException("INVALID REF!");
				}
			}
			int getStride() { return sizeof(T); };

		private:

			std::vector<T> mVertices;

		};


		typedef VertexArray<Vertex>  VertexList;
		typedef VertexArray<VertexColor>  VertexColorList;
		typedef VertexArray<VertexNormal>  VertexNormalList;
		typedef VertexArray<VertexTcoord>  VertexTcoordList;

	public:

		VertexBuffer(EVertexType type)
		{
			setType(type);
		}

		//注意改变这个缓冲区类型，会清空内容
		void setType(EVertexType type)
		{
			IVertexArray* vertices = 0;
			mVertexType = type;
			switch (mVertexType)
			{
			case EVT_STARNDRD:
				vertices = new VertexList();
				break;
			case EVT_COLOR:
				vertices = new VertexColorList();
				break;
			case EVT_NORMAL:
				vertices = new VertexNormalList();
				break;
			case EVT_TCOORD:
				vertices = new VertexTcoordList();
				break;
			default:
				vertices = new VertexList();
				break;
			}
			if (mVertices)
			{
				delete mVertices;
			}
			mVertices = vertices;
		}
		EVertexType getType()
		{
			return mVertexType;
		}

		int  getStride()
		{
			return mVertices->getStride();
		}

		bool getData(byte* buf, ULONG& size, UINT32& stride)
		{
			return mVertices->getData(buf, size, stride);
		}

		UINT getBufferSize()
		{
			return mVertices->getElementSize();
		}

		void AddVertex(Vertex& v)
		{
			mVertices->AddVertex(v);
		}

	private:

		IVertexArray* mVertices;
		EVertexType   mVertexType;



	};
	

}