#pragma once
#include "GraphicDefs.h"
#include "IMaterial.h"

namespace Sapphire
{
	class VertexBuffer;
	class IndexBuffer;

	class Geometry
	{
	public:

		Geometry()
		{
			mVertexBuffer = 0;
			mIndexBuffer = 0;
			mType = EPT_TRIANGLES;
			mMaterial = 0;
		};
		Geometry(VertexBuffer* pVb, IndexBuffer* pIb, IMaterial* material, EPRIMITIVE_TYPE type);
		~Geometry()
		{
			release();
		};
		void setVertexBuffer(VertexBuffer* pBuf);
		VertexBuffer* getVertexBuffer();
		void setIndexBuffer(IndexBuffer* pBuf);
		IndexBuffer*  getIndexBuffer();
		EPRIMITIVE_TYPE  getType();
		void          setType(EPRIMITIVE_TYPE type);
		void release();
		void   setMaterial(IMaterial* material);
		IMaterial*  getMaterial();


	private:

		VertexBuffer*  mVertexBuffer;
		IndexBuffer*   mIndexBuffer;
		EPRIMITIVE_TYPE  mType;
		IMaterial*     mMaterial;


	};
}