#pragma once


namespace Sapphire
{
	class VertexBuffer;
	class IndexBuffer;
	//! 枚举图元类型
	enum EPRIMITIVE_TYPE
	{
		//单个顶点
		EPT_POINTS = 0,

		//所有顶点链接成一条单独线段
		EPT_LINE_STRIP,

		//!同上，但是首位相接
		EPT_LINE_LOOP,

		//! 每两个顶点链接为N/2条线段
		EPT_LINES,

		//!  自前两个顶点后的每个顶点构成一个新三角形N-2
		EPT_TRIANGLE_STRIP,

		//!  自前两个顶点后的每个顶点构成一个新三角形N-2
		//! 所有的三角形都环绕第一个顶点
		EPT_TRIANGLE_FAN,

		//! 设置所有顶点到每一个三角形
		EPT_TRIANGLES,

		//! 每四个顶点构成一个四边形
		EPT_QUADS,

		//! 就是一个LINE_LOOP,但填充
		EPT_POLYGON,

		//! 在GPU由一个顶点扩展到一个四边形的公告牌
		EPT_POINT_SPRITES
	};


	class Geometry
	{
		

	public:

		Geometry()
		{
			mVertexBuffer = 0;
			mIndexBuffer = 0;
			mType = EPT_TRIANGLES;
		};
		Geometry(VertexBuffer* pVb, IndexBuffer* pIb, EPRIMITIVE_TYPE type);
		~Geometry();
		void draw();
		void setVertexBuffer(VertexBuffer* pBuf);
		VertexBuffer* getVertexBuffer();
		void setIndexBuffer(IndexBuffer* pBuf);
		IndexBuffer*  getIndexBuffer();
		EPRIMITIVE_TYPE  getType();
		void          setType(EPRIMITIVE_TYPE type);
		void release();


	private:

		VertexBuffer*  mVertexBuffer;
		IndexBuffer*   mIndexBuffer;
		EPRIMITIVE_TYPE  mType;
		
		
	};
}