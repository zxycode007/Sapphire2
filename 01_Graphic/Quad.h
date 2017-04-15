#pragma once

#include "Geometry.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include  "IndexBuffer.h"


namespace Sapphire
{

	template<class T>
	class Quad
	{
	public:
		T mLeftTop;
		T mRightTop;
		T mLeftBottom;
		T mRightBottom;

		Quad(T leftTop, T rightTop, T leftBottom, T rightBottom) :mLeftTop(leftTop), mRightTop(rightTop),
			mLeftBottom(leftBottom), mRightBottom(rightBottom)
		{
			mType = leftTop.getType();
		}
		~Quad()
		{}

		Geometry* toGeometry()
		{
			VertexBuffer* pVb = new VertexBuffer(mType);
			IndexBuffer* pIb = new IndexBuffer(EIT_32BIT);
			pVb->AddVertex(mLeftTop);
			pVb->AddVertex(mRightTop);
			pVb->AddVertex(mLeftBottom);
			pVb->AddVertex(mRightBottom);
			pIb->AddIndex(0);
			pIb->AddIndex(1);
			pIb->AddIndex(2);
			pIb->AddIndex(2);
			pIb->AddIndex(1);
			pIb->AddIndex(3);
			Geometry* geometry = new Geometry(pVb, pIb, NULL, EPT_TRIANGLES);
			return geometry;
		}

	private:

		EVertexType  mType;

	};

	typedef Quad<Vertex>  StandrdQuad;
	typedef Quad<VertexColor>  ColorQuad;
	typedef Quad<VertexNormal>  NormalQuad;
	typedef Quad<VertexTcoord>  TexturedQuad;

}
