#pragma once

#include "../Container/HashBase.h"
#include "../Math/StringHash.h"


namespace Sapphire
{
	class Vector3;

#if defined(ANDROID) || defined(IOS) || defined(RPI)
#define MOBILE_GRAPHICS
#else
#define DESKTOP_GRAPHICS
#endif

	/// 图元类型
	enum PrimitiveType
	{
		TRIANGLE_LIST = 0,
		LINE_LIST,
		POINT_LIST,
		TRIANGLE_STRIP,
		LINE_STRIP,
		TRIANGLE_FAN
	};

	/// 几何类型
	enum GeometryType
	{
		GEOM_STATIC = 0,
		GEOM_SKINNED = 1,
		GEOM_INSTANCED = 2,
		GEOM_BILLBOARD = 3,
		GEOM_STATIC_NOINSTANCING = 4,
		MAX_GEOMETRYTYPES = 4,
	};

	/// 混合类型
	enum BlendMode
	{
		BLEND_REPLACE = 0,
		BLEND_ADD,
		BLEND_MULTIPLY,
		BLEND_ALPHA,
		BLEND_ADDALPHA,
		BLEND_PREMULALPHA,
		BLEND_INVDESTALPHA,
		BLEND_SUBTRACT,
		BLEND_SUBTRACTALPHA,
		MAX_BLENDMODES
	};

	///深度和模板比较模式
	enum CompareMode
	{
		CMP_ALWAYS = 0,
		CMP_EQUAL,
		CMP_NOTEQUAL,
		CMP_LESS,
		CMP_LESSEQUAL,
		CMP_GREATER,
		CMP_GREATEREQUAL,
		MAX_COMPAREMODES
	};

	/// 剔除模式
	enum CullMode
	{
		CULL_NONE = 0,
		CULL_CCW,
		CULL_CW,
		MAX_CULLMODES
	};

	/// 填充模式
	enum FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME,
		FILL_POINT
	};

	/// 模板操作
	enum StencilOp
	{
		OP_KEEP = 0,
		OP_ZERO,
		OP_REF,
		OP_INCR,
		OP_DECR
	};

	/// 顶点/索引 缓冲区锁状态
	enum LockState
	{
		LOCK_NONE = 0,
		LOCK_HARDWARE,
		LOCK_SHADOW,
		LOCK_SCRATCH
	};

	/// 顶点元素
	enum VertexElement
	{
		ELEMENT_POSITION = 0,             //位置
		ELEMENT_NORMAL,                    //法线
		ELEMENT_COLOR,
		ELEMENT_TEXCOORD1,
		ELEMENT_TEXCOORD2,
		ELEMENT_CUBETEXCOORD1,
		ELEMENT_CUBETEXCOORD2,
		ELEMENT_TANGENT,
		ELEMENT_BLENDWEIGHTS,
		ELEMENT_BLENDINDICES,
		ELEMENT_INSTANCEMATRIX1,            //实例矩阵
		ELEMENT_INSTANCEMATRIX2,
		ELEMENT_INSTANCEMATRIX3,         
		MAX_VERTEX_ELEMENTS
	};

	/// 纹理滤波模式
	enum TextureFilterMode
	{
		FILTER_NEAREST = 0,
		FILTER_BILINEAR,
		FILTER_TRILINEAR,
		FILTER_ANISOTROPIC,
		FILTER_DEFAULT,
		MAX_FILTERMODES
	};

	/// 纹理地址模式
	enum TextureAddressMode
	{
		ADDRESS_WRAP = 0,
		ADDRESS_MIRROR,
		ADDRESS_CLAMP,
		ADDRESS_BORDER,
		MAX_ADDRESSMODES
	};

	/// 纹理坐标
	enum TextureCoordinate
	{
		COORD_U = 0,
		COORD_V,
		COORD_W,
		MAX_COORDS
	};

	/// 纹理使用类型
	enum TextureUsage
	{
		TEXTURE_STATIC = 0,
		TEXTURE_DYNAMIC,
		TEXTURE_RENDERTARGET,
		TEXTURE_DEPTHSTENCIL
	};

	/// 立方体面映射
	enum CubeMapFace
	{
		FACE_POSITIVE_X = 0,
		FACE_NEGATIVE_X,
		FACE_POSITIVE_Y,
		FACE_NEGATIVE_Y,
		FACE_POSITIVE_Z,
		FACE_NEGATIVE_Z,
		MAX_CUBEMAP_FACES
	};

	/// 立方体单图案布局模式
	enum CubeMapLayout
	{
		CML_HORIZONTAL = 0,
		CML_HORIZONTALNVIDIA,
		CML_HORIZONTALCROSS,
		CML_VERTICALCROSS,
		CML_BLENDER
	};

	/// 更新渲染表面视口 
	enum RenderSurfaceUpdateMode
	{
		SURFACE_MANUALUPDATE = 0,
		SURFACE_UPDATEVISIBLE,
		SURFACE_UPDATEALWAYS
	};

	/// Shader 类型.
	enum ShaderType
	{
		VS = 0,
		PS,
	};

	/// 要更新着色器参数组 Shader parameter groups for determining need to update. On APIs that support constant buffers, these correspond to different constant buffers.
	enum ShaderParameterGroup
	{
		SP_FRAME = 0,
		SP_CAMERA,
		SP_ZONE,
		SP_LIGHT,
		SP_MATERIAL,
		SP_OBJECT,
		SP_CUSTOM,
		MAX_SHADER_PARAMETER_GROUPS
	};

	/// 纹理单元
	enum TextureUnit
	{
		TU_DIFFUSE = 0,
		TU_ALBEDOBUFFER = 0,
		TU_NORMAL = 1,
		TU_NORMALBUFFER = 1,
		TU_SPECULAR = 2,
		TU_EMISSIVE = 3,
		TU_ENVIRONMENT = 4,
#ifdef DESKTOP_GRAPHICS
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

	/// Billboard 相机面向模式
	enum FaceCameraMode
	{
		FC_NONE = 0,
		FC_ROTATE_XYZ,
		FC_ROTATE_Y,
		FC_LOOKAT_XYZ,
		FC_LOOKAT_Y
	};

	// shader参数
	extern SAPPHIRE_API const StringHash VSP_AMBIENTSTARTCOLOR;
	extern SAPPHIRE_API const StringHash VSP_AMBIENTENDCOLOR;
	extern SAPPHIRE_API const StringHash VSP_BILLBOARDROT;
	extern SAPPHIRE_API const StringHash VSP_CAMERAPOS;
	extern SAPPHIRE_API const StringHash VSP_CAMERAROT;
	extern SAPPHIRE_API const StringHash VSP_CLIPPLANE;
	extern SAPPHIRE_API const StringHash VSP_NEARCLIP;
	extern SAPPHIRE_API const StringHash VSP_FARCLIP;
	extern SAPPHIRE_API const StringHash VSP_DEPTHMODE;
	extern SAPPHIRE_API const StringHash VSP_DELTATIME;
	extern SAPPHIRE_API const StringHash VSP_ELAPSEDTIME;
	extern SAPPHIRE_API const StringHash VSP_FRUSTUMSIZE;
	extern SAPPHIRE_API const StringHash VSP_GBUFFEROFFSETS;
	extern SAPPHIRE_API const StringHash VSP_LIGHTDIR;
	extern SAPPHIRE_API const StringHash VSP_LIGHTPOS;
	extern SAPPHIRE_API const StringHash VSP_MODEL;
	extern SAPPHIRE_API const StringHash VSP_VIEWPROJ;
	extern SAPPHIRE_API const StringHash VSP_UOFFSET;
	extern SAPPHIRE_API const StringHash VSP_VOFFSET;
	extern SAPPHIRE_API const StringHash VSP_ZONE;
	extern SAPPHIRE_API const StringHash VSP_LIGHTMATRICES;
	extern SAPPHIRE_API const StringHash VSP_SKINMATRICES;
	extern SAPPHIRE_API const StringHash VSP_VERTEXLIGHTS;
	extern SAPPHIRE_API const StringHash PSP_AMBIENTCOLOR;
	extern SAPPHIRE_API const StringHash PSP_CAMERAPOS;
	extern SAPPHIRE_API const StringHash PSP_DELTATIME;
	extern SAPPHIRE_API const StringHash PSP_DEPTHRECONSTRUCT;
	extern SAPPHIRE_API const StringHash PSP_ELAPSEDTIME;
	extern SAPPHIRE_API const StringHash PSP_FOGCOLOR;
	extern SAPPHIRE_API const StringHash PSP_FOGPARAMS;
	extern SAPPHIRE_API const StringHash PSP_GBUFFERINVSIZE;
	extern SAPPHIRE_API const StringHash PSP_LIGHTCOLOR;
	extern SAPPHIRE_API const StringHash PSP_LIGHTDIR;
	extern SAPPHIRE_API const StringHash PSP_LIGHTPOS;
	extern SAPPHIRE_API const StringHash PSP_MATDIFFCOLOR;
	extern SAPPHIRE_API const StringHash PSP_MATEMISSIVECOLOR;
	extern SAPPHIRE_API const StringHash PSP_MATENVMAPCOLOR;
	extern SAPPHIRE_API const StringHash PSP_MATSPECCOLOR;
	extern SAPPHIRE_API const StringHash PSP_NEARCLIP;
	extern SAPPHIRE_API const StringHash PSP_FARCLIP;
	extern SAPPHIRE_API const StringHash PSP_SHADOWCUBEADJUST;
	extern SAPPHIRE_API const StringHash PSP_SHADOWDEPTHFADE;
	extern SAPPHIRE_API const StringHash PSP_SHADOWINTENSITY;
	extern SAPPHIRE_API const StringHash PSP_SHADOWMAPINVSIZE;
	extern SAPPHIRE_API const StringHash PSP_SHADOWSPLITS;
	extern SAPPHIRE_API const StringHash PSP_LIGHTMATRICES;

	// 碰撞盒子对角线缩放计算
	extern SAPPHIRE_API const Vector3 DOT_SCALE;

	static const int QUALITY_LOW = 0;
	static const int QUALITY_MEDIUM = 1;
	static const int QUALITY_HIGH = 2;
	static const int QUALITY_MAX = 15;

	static const int SHADOWQUALITY_LOW_16BIT = 0;
	static const int SHADOWQUALITY_LOW_24BIT = 1;
	static const int SHADOWQUALITY_HIGH_16BIT = 2;
	static const int SHADOWQUALITY_HIGH_24BIT = 3;

	static const unsigned CLEAR_COLOR = 0x1;
	static const unsigned CLEAR_DEPTH = 0x2;
	static const unsigned CLEAR_STENCIL = 0x4;

	static const unsigned MASK_NONE = 0x0;
	static const unsigned MASK_POSITION = 0x1;
	static const unsigned MASK_NORMAL = 0x2;
	static const unsigned MASK_COLOR = 0x4;
	static const unsigned MASK_TEXCOORD1 = 0x8;
	static const unsigned MASK_TEXCOORD2 = 0x10;
	static const unsigned MASK_CUBETEXCOORD1 = 0x20;
	static const unsigned MASK_CUBETEXCOORD2 = 0x40;
	static const unsigned MASK_TANGENT = 0x80;
	static const unsigned MASK_BLENDWEIGHTS = 0x100;
	static const unsigned MASK_BLENDINDICES = 0x200;
	static const unsigned MASK_INSTANCEMATRIX1 = 0x400;
	static const unsigned MASK_INSTANCEMATRIX2 = 0x800;
	static const unsigned MASK_INSTANCEMATRIX3 = 0x1000;
	static const unsigned MASK_DEFAULT = 0xffffffff;
	static const unsigned NO_ELEMENT = 0xffffffff;

	static const int MAX_RENDERTARGETS = 4;
	static const int MAX_VERTEX_STREAMS = 4;
	static const int MAX_CONSTANT_REGISTERS = 256;

	static const int BITS_PER_COMPONENT = 8;

}