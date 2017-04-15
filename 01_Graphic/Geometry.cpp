#include "Geometry.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



namespace Sapphire
{
	Geometry::Geometry(VertexBuffer * pVb, IndexBuffer * pIb, Material* material, EPRIMITIVE_TYPE type)
	{
		mVertexBuffer = pVb;
		mIndexBuffer = pIb;
		mType = type;
		mMaterial = material;
	}


	void Geometry::setVertexBuffer(VertexBuffer * pBuf)
	{
		mVertexBuffer = pBuf;
	}

	VertexBuffer * Geometry::getVertexBuffer()
	{
		return mVertexBuffer;
	}

	void Geometry::setIndexBuffer(IndexBuffer * pBuf)
	{
		mIndexBuffer = pBuf;
	}

	IndexBuffer * Geometry::getIndexBuffer()
	{
		return mIndexBuffer;
	}

	EPRIMITIVE_TYPE Geometry::getType()
	{
		return mType;
	}

	void Geometry::setType(EPRIMITIVE_TYPE type)
	{
		mType = type;
	}



	void Geometry::release()
	{
		if (mVertexBuffer)
		{
			delete mVertexBuffer;
			mVertexBuffer = 0;
		}
		if (mIndexBuffer)
		{
			delete mIndexBuffer;
			mIndexBuffer = 0;
		}
		if (mMaterial)
		{
			delete mMaterial;
			mMaterial = 0;
		}
	}

	void Geometry::setMaterial(Material * material)
	{
		mMaterial = material;
	}

	Material * Geometry::getMaterial()
	{
		return mMaterial;
	}

}