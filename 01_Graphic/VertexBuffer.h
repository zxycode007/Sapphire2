#pragma once

#include "GraphicDefs.h"
#include <vector>
#include "Vertex.h"
#include "../Exception/EmptyRefException.h"

namespace Sapphire
{

	struct VertexAttributeInfo
	{
		EVertexAttribute id;
		int              length;
		int              offset;
		VertexAttributeInfo()
		{
			memset(this, 0, sizeof(VertexAttributeInfo));
		};
		VertexAttributeInfo(EVertexAttribute id, int len, int offset)
		{
			this->id = id;
			this->length = len;
			this->offset = offset;
		}
		int getOffset()
		{
			return offset;
		}
	};

	typedef std::vector<VertexAttributeInfo>  VertexAttributeInfoList;

	class VertexBuffer
	{

		class IVertexArray
		{
		public:
			virtual void AddVertex(Vertex& v) = 0;
			virtual int  getElementSize() = 0;
			virtual void* getData(ULONG& size, UINT32& stride) = 0;
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
			void* getData(ULONG& size, UINT32& stride)
			{
				if (mVertices.size() > 0)
				{
					size = mVertices.size() * sizeof(T);
					stride = getStride();
					return (byte*)mVertices.begin()._Ptr;
				}
				else
				{
					return NULL;
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
		typedef VertexArray<VertexTcoord2>  Vertex2TcoordList;

	public:

		VertexBuffer(EVertexType type)
		{
			mVertices = 0;
			setType(type);
		}

		//注意改变这个缓冲区类型，会清空内容
		void setType(EVertexType type)
		{
			IVertexArray* vertices = 0;
			mVertexType = type;
			mAttributesInfo.clear();
			int seek = 4;
			switch (mVertexType)
			{
			case EVT_STARNDRD:
			{
				vertices = new VertexList();
				VertexAttributeInfo pos(EVA_POSITION, sizeof(Vector3), seek);
				mAttributesInfo.push_back(pos);
			}
			break;
			case EVT_COLOR:
			{
				vertices = new VertexColorList();
				VertexAttributeInfo pos(EVA_POSITION, sizeof(Vector3), seek);
				mAttributesInfo.push_back(pos);
				seek += sizeof(EVertexType) + sizeof(Vector3);
				VertexAttributeInfo color(EVA_COLOR, sizeof(Color), seek);
				mAttributesInfo.push_back(color);
			}
			break;
			case EVT_NORMAL:
			{
				vertices = new VertexNormalList();
				VertexAttributeInfo pos(EVA_POSITION, sizeof(Vector3), seek);
				mAttributesInfo.push_back(pos);
				seek += sizeof(EVertexType) + sizeof(Vector3);
				VertexAttributeInfo color(EVA_COLOR, sizeof(Color), seek);
				mAttributesInfo.push_back(color);
				seek += sizeof(Color);
				VertexAttributeInfo normal(EVA_NORMAL, sizeof(Vector3), seek);
				mAttributesInfo.push_back(normal);
			}
			break;
			case EVT_TCOORD:
			{
				vertices = new VertexTcoordList();
				VertexAttributeInfo pos(EVA_POSITION, sizeof(Vector3), seek);
				mAttributesInfo.push_back(pos);
				seek += sizeof(EVertexType) + sizeof(Vector3);
				VertexAttributeInfo color(EVA_COLOR, sizeof(Color), seek);
				mAttributesInfo.push_back(color);
				seek += sizeof(Color);
				VertexAttributeInfo normal(EVA_NORMAL, sizeof(Vector3), seek);
				mAttributesInfo.push_back(normal);
				seek += sizeof(Vector3);
				VertexAttributeInfo tcoord(EVA_TCOORD, sizeof(Vector2), seek);
				mAttributesInfo.push_back(tcoord);
			}
			break;
			case EVT_2TCOORD:
			{
				vertices = new Vertex2TcoordList();
				VertexAttributeInfo pos(EVA_POSITION, sizeof(Vector3), seek);
				mAttributesInfo.push_back(pos);
				seek += sizeof(EVertexType) + sizeof(Vector3);
				VertexAttributeInfo color(EVA_COLOR, sizeof(Color), seek);
				mAttributesInfo.push_back(color);
				seek += sizeof(Color);
				VertexAttributeInfo normal(EVA_NORMAL, sizeof(Vector3), seek);
				mAttributesInfo.push_back(normal);
				seek += sizeof(Vector3);
				VertexAttributeInfo tcoord(EVA_TCOORD, sizeof(Vector2), sizeof(Vector3));
				mAttributesInfo.push_back(tcoord);
				seek += sizeof(Vector2);
				VertexAttributeInfo tcoord2(EVA_2TCOORD, sizeof(Vector2), seek);
				mAttributesInfo.push_back(tcoord2);
			}
			break;
			default:
			{
				vertices = new VertexList();
				VertexAttributeInfo pos(EVA_POSITION, sizeof(Vector3), 4);
				mAttributesInfo.push_back(pos);
			}
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

		void* getData(ULONG& size, UINT32& stride)
		{
			return mVertices->getData(size, stride);
		}

		ULONG32 getDataSize()
		{
			return getStride()*getBufferSize();
		}

		UINT getBufferSize()
		{
			return mVertices->getElementSize();
		}

		void AddVertex(Vertex& v)
		{
			mVertices->AddVertex(v);
		}

		const VertexAttributeInfoList& getAttributeInfo()
		{
			return mAttributesInfo;
		}

	private:

		IVertexArray* mVertices;
		EVertexType   mVertexType;
		VertexAttributeInfoList  mAttributesInfo;



	};


}