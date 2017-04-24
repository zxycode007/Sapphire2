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
		FILL_POINT,
		MAX_FILLMODE
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

	//纹理寻址模式
	enum ETextureAddressingMode
	{
		EAD_WRAP = 0,
		EAD_MIRROR,
		EAD_CLAMP,
		EAD_BORDER,
		MAX_EAD_MODES
	};

	//纹理坐标
	enum ETextureCoordinate
	{
		ECOORD_U = 0,
		ECOORD_V,
		ECOORD_W,
		MAX_COORDS
	};

	//纹理滤波模式
	enum ETextureFilterMode
	{
		EFILTER_NEAREST = 0,
		FILTER_BILINEAR,
		FILTER_TRILINEAR,
		FILTER_ANISOTROPIC,
		FILTER_DEFAULT,
		MAX_EFILTERMODES
	};

	//纹理单元定义
	enum ETextureUnit
	{
		TU_DIFFUSE = 0,
		TU_ALBEDOBUFFER = 0,
		TU_NORMAL = 1,
		TU_NORMALBUFFER = 1,
		TU_SPECULAR = 2,
		TU_EMISSIVE = 3,
		TU_ENVIRONMENT = 4,
#ifdef SAPPHIRE_DESKTOP_GRAPHICS
		TU_VOLUMEMAP = 5,
		TU_CUSTOM1 = 6,
		TU_CUSTOM2 = 7,
		TU_LIGHTRAMP = 8,
		TU_LIGHTSHAPE = 9,
		TU_SHADOWMAP = 10,
		TU_FACESELECT = 11,
		TU_INDIRECTION = 12,
		TU_DEPTHBUFFER = 13,
		TU_LIGHTBUFFER = 14,
		TU_ZONE = 15,
		MAX_MATERIAL_TEXTURE_UNITS = 8,
		MAX_TEXTURE_UNITS = 16
#else
		TU_LIGHTRAMP = 5,
		TU_LIGHTSHAPE = 6,
		TU_SHADOWMAP = 7,
		MAX_MATERIAL_TEXTURE_UNITS = 5,
		MAX_TEXTURE_UNITS = 8
#endif
	};
}