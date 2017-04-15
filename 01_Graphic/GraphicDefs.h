#pragma once

namespace Sapphire
{
	//! 枚举图元类型
	// 决定绘制图元的方法
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

	//填充模式
	enum FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME,
		FILL_POINT
	};

	enum EVertexAttribute
	{
		EVA_NULL,
		EVA_POSITION,
		EVA_COLOR,
		EVA_NORMAL,
		EVA_TCOORD,
		EVA_2TCOORD,
		EVA_TANGENT,
		MAX_EVA
	};

	enum EVertexType
	{
		EVT_STARNDRD,
		EVT_COLOR,
		EVT_NORMAL,
		EVT_TCOORD,
		EVT_2TCOORD,
		EVT_TANGENTS,
		MAX_EVT
	};

	enum EIndexType
	{
		EIT_16BIT,
		EIT_32BIT
	};

	
}