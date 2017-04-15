#pragma once
#include "GraphicDefs.h"
#include "Material.h"

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
		Geometry(VertexBuffer* pVb, IndexBuffer* pIb, Material* material, EPRIMITIVE_TYPE type);
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
		void   setMaterial(Material* material);
		Material*  getMaterial();


	private:

		VertexBuffer*  mVertexBuffer;
		IndexBuffer*   mIndexBuffer;
		EPRIMITIVE_TYPE  mType;
		Material*     mMaterial;


	};
}