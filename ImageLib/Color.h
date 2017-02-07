#pragma once
#include "stdafx.h"


//! 一个纹理的颜色格式的枚举
enum ECOLOR_FORMAT
{
	//! 16位色彩格式：用于软件驱动
	/** 因此它可以为引擎所有其它视频驱动的首选
	每个颜色分量有5位，alpha信息由单个位组成 */
	ECF_A1R5G5B5 = 0,

	//! 标准的16位颜色格式
	ECF_R5G6B5,

	//! 标准的24位颜色格式，没有alpha通道，红绿蓝均8位
	ECF_R8G8B8,

	//! 32位颜色格式,红绿蓝均8位
	ECF_R8G8B8A8,

	//! 默认的32位颜色格式，每个分量均8位
	ECF_A8R8G8B8,

	/** 浮点格式。下列格式只能用于渲染目标纹理 */

	//! 16位浮点格式，使用16位的红色通道
	ECF_R16F,

	//! 32位浮点格式，用16位红色通道和16位绿色通道
	ECF_G16R16F,

	//! 64位浮点格式，用16位红，绿，蓝和alpha通道
	ECF_A16B16G16R16F,

	//! 32位浮点格式，用32位的红色通道
	ECF_R32F,

	//! 64位浮点格式，用32位的红色和绿色通道
	ECF_G32R32F,

	//! 128位浮点格式，用32位的红绿蓝和透明格式
	ECF_A32B32G32R32F,


	ECF_UNKNOWN
};


//! 创建一个16位的A1R5G5B5颜色
inline uint16 RGBA16(uint32 r, uint32 g, uint32 b, uint32 a = 0xFF)
{
	return (uint16)((a & 0x80) << 8 |
		(r & 0xF8) << 7 |
		(g & 0xF8) << 2 |
		(b & 0xF8) >> 3);
}


//! 创建一个16位的A1R5G5B5颜色 
inline uint16 RGB16(uint32 r, uint32 g, uint32 b)
{
	return RGBA16(r, g, b);
}


//!  创建一个16位的A1R5G5B5颜色 ,基于16位输入值
inline uint16 RGB16from16(uint16 r, uint16 g, uint16 b)
{
	return (0x8000 |
		(r & 0x1F) << 10 |
		(g & 0x1F) << 5 |
		(b & 0x1F));
}


//! 转换一个32位(X8R8G8B8)颜色到16位的A1R5G5B5
inline uint16 X8R8G8B8toA1R5G5B5(uint32 color)
{
	return (uint16)(0x8000 |
		(color & 0x00F80000) >> 9 |
		(color & 0x0000F800) >> 6 |
		(color & 0x000000F8) >> 3);
}


//! 转换一个32位(A8R8G8B8)颜色到16位的A1R5G5B5
inline uint16 A8R8G8B8toA1R5G5B5(uint32 color)
{
	return (uint16)((color & 0x80000000) >> 16 |
		(color & 0x00F80000) >> 9 |
		(color & 0x0000F800) >> 6 |
		(color & 0x000000F8) >> 3);
}


//! 转换一个32位(A8R8G8B8) 颜色到16位的R5G6B5
inline uint16 A8R8G8B8toR5G6B5(uint32 color)
{
	return (uint16)((color & 0x00F80000) >> 8 |
		(color & 0x0000FC00) >> 5 |
		(color & 0x000000F8) >> 3);
}

//! 转换一个32位(A8R8G8B8) 颜色到32位的R8G8B8A8
inline uint16 A8R8G8B8toR8G8B8A8(uint32 color)
{
	return (uint32)((color & 0x00FF0000) << 8 |
		(color & 0x0000FF00) << 8 |
		(color & 0x000000FF) << 8 |
		(color & 0xFF000000) >> 24
		);
}


//! 从A1R5G5B5转换到A8R8G8B8
inline uint32 A1R5G5B5toA8R8G8B8(uint16 color)
{
	return (((-((sint32)color & 0x00008000) >> (sint32)31) & 0xFF000000) |
		((color & 0x00007C00) << 9) | ((color & 0x00007000) << 4) |
		((color & 0x000003E0) << 6) | ((color & 0x00000380) << 1) |
		((color & 0x0000001F) << 3) | ((color & 0x0000001C) >> 2)
		);
}


//! 返回R5G6B5的A8R8G8B8的颜色
inline uint32 R5G6B5toA8R8G8B8(uint16 color)
{
	return 0xFF000000 |
		((color & 0xF800) << 8) |
		((color & 0x07E0) << 5) |
		((color & 0x001F) << 3);
}


//! 返回从A1R5G5B5的R5G6B5的颜色 
inline uint16 R5G6B5toA1R5G5B5(uint16 color)
{
	return 0x8000 | (((color & 0xFFC0) >> 1) | (color & 0x1F));
}


//! 返回A1R5G5B5到R5G6B5的颜色  
inline uint16 A1R5G5B5toR5G6B5(uint16 color)
{
	return (((color & 0x7FE0) << 1) | (color & 0x1F));
}



//! 返回A1R5G5B5的alpha分量

inline uint32 getAlpha(uint16 color)
{
	return ((color >> 15) & 0x1);
}


//! 返回A1R5G5B5的红色分量
/** Shift left by 3 to get 8 bit value. */
inline uint32 getRed(uint16 color)
{
	return ((color >> 10) & 0x1F);
}


//! 返回A1R5G5B5的绿色分量
/** Shift left by 3 to get 8 bit value. */
inline uint32 getGreen(uint16 color)
{
	return ((color >> 5) & 0x1F);
}


//! 返回A1R5G5B5的蓝色分量
/** Shift left by 3 to get 8 bit value. */
inline uint32 getBlue(uint16 color)
{
	return (color & 0x1F);
}


//! 返回16位A1R5G5B5的平均值
inline sint32 getAverage(sint32 color)
{
	return ((getRed(color) << 3) + (getGreen(color) << 3) + (getBlue(color) << 3)) / 3;
}

////32位RGBA
typedef uint32 RGBA;
////32位ARGB
typedef uint32 ARGB;
////32位ABGR
typedef uint32 ABGR;
////32位BGRA
typedef uint32 BGRA;



// 1 - 位alpha混合
inline uint16 PixelBlend16(const uint16 c2, const uint16 c1)
{
	uint16 mask = ((c1 & 0x8000) >> 15) + 0x7fff;
	return (c2 & mask) | (c1 & ~mask);
}

// 1 - 位alpha混合 16Bit SIMD
inline uint32 PixelBlend16_simd(const uint32 c2, const uint32 c1)
{
	uint32 mask = ((c1 & 0x80008000) >> 15) + 0x7fff7fff;
	return (c2 & mask) | (c1 & ~mask);
}

//32位像素Alpha混合
/*!
Pixel = dest * ( 1 - SourceAlpha ) + source * SourceAlpha
*/
inline uint32 PixelBlend32(const uint32 c2, const uint32 c1)
{
	// alpha test
	uint32 alpha = c1 & 0xFF000000;

	if (0 == alpha)
		return c2;

	if (0xFF000000 == alpha)
	{
		return c1;
	}

	alpha >>= 24;

	// 添加alpha高位， 如果(alpha > 127) alpha += 1
	alpha += (alpha >> 7);

	uint32 srcRB = c1 & 0x00FF00FF;  //源：红蓝
	uint32 srcXG = c1 & 0x0000FF00;   //源：绿

	uint32 dstRB = c2 & 0x00FF00FF;  //源：红蓝
	uint32 dstXG = c2 & 0x0000FF00;  //源：绿


	uint32 rb = srcRB - dstRB;    
	uint32 xg = srcXG - dstXG;

	rb *= alpha;
	xg *= alpha;
	rb >>= 8;
	xg >>= 8;

	rb += dstRB;
	xg += dstXG;

	rb &= 0x00FF00FF;
	xg &= 0x0000FF00;

	return (c1 & 0xFF000000) | rb | xg;
}

inline void convert_A8R8G8B8toR8G8B8(const void* sP, sint32 sN, void* dP)
{
	uint8* sB = (uint8*)sP;
	uint8* dB = (uint8*)dP;

	for (sint32 x = 0; x < sN; ++x)
	{
		// sB[3] is alpha
		dB[0] = sB[2];
		dB[1] = sB[1];
		dB[2] = sB[0];

		sB += 4;
		dB += 3;
	}
}

class Color
{
public:

	
	
	Color() :r(1.0f), g(1.0f), b(1.0f), a(1.0f)
	{
	}
	Color(const Color& other) :r(other.r), g(other.g), b(other.b), a(other.a)
	{
	}
	Color(float _r,float _g, float _b, float _a) :a(_a), r(_r), g(_g), b(_b)
	{

	}
	Color(uint32 alpha, uint32 red, uint32 green, uint32 blue)
	{
		setAlpha(alpha);
		setRed(red);
		setGreen(green);
		setBlue(blue);
	}
	Color(uint32 argb)
	{
		uint32 val32 = argb;

		// 转换到32位模式
		// (ARGB = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		b = (val32 & 0xFF) / 255.0f;
	}

	~Color()
	{}

	Color& operator=(const Color& other)
	{
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
		return *this;
	}

	Color operator+(const Color& other)
	{
		return Color(a + other.a, r + other.r, g + other.g, b + other.b);
	}

	Color operator*(const Color& other)
	{
		return Color(a * other.a, r * other.r, g * other.g, b * other.b);
	}

	Color operator*(const float scalar)
	{
		return Color(a * scalar, r * scalar, g * scalar, b * scalar);
	}

	Color operator/(const float scalar)
	{
		assert(scalar != 0.0f);
		return Color(a / scalar, r / scalar, g / scalar, b / scalar);
	}

	Color operator/(const Color& other)
	{
		return Color(a / other.a, r / other.r, g / other.g, b / other.b);
	}

	Color operator-(const Color& other)
	{
		return Color(a - other.a, r - other.r, g - other.g, b - other.b);
	}

	bool  operator==(const Color& other)
	{
		return a == other.a && r == other.r && g == other.g && b == other.b;
	}

	bool  operator!=(const Color& other)
	{
		return a != other.a || r != other.r || g != other.g || b != other.b;
	}

	void setAlpha(uint32 alpha)
	{
		a = static_cast<float>(alpha) / 255;
	};

	void setRed(uint32 red)
	{
		r = static_cast<float>(red) / 255;
	};

	void setGreen(uint32 green)
	{
		g = static_cast<float>(green) / 255;
	}

	void setBlue(uint32 blue)
	{
		b = static_cast<float>(blue) / 255;
	};
	
	uint32 getAlpha() const
	{
		return static_cast<uint32>(a * 255);
	}

	uint32 getRed() const
	{
		return static_cast<uint32>(r * 255);
	}

	uint32 getGreen() const
	{
		return static_cast<uint32>(g * 255);
	}

	uint32 getBlue() const
	{
		return static_cast<uint32>(b * 255);
	}

	uint32 getAverage()
	{
		return (getRed() + getGreen() + getBlue()) / 3;
	}

	//! 转换到OpenGL颜色格式
	/** 从ARGB到RGBA
	*/
	void toOpenGLColor(uint8* dest) const
	{
		*dest = (uint8)getRed();
		*++dest = (uint8)getGreen();
		*++dest = (uint8)getBlue();
		*++dest = (uint8)getAlpha();
	}

	void saturate(float min, float max);


	/** 返回RGBA
	*/
	RGBA getAsRGBA(void) const;

	/** 返回ARGB
	*/
	ARGB getAsARGB(void) const;

	/**返回BGRA
	*/
	BGRA getAsBGRA(void) const;

	/** 返回ABGR */
	ABGR getAsABGR(void) const;

	/** 设置 RGBA.
	*/
	void setAsRGBA(const RGBA val);

	/** 设置 ARGB.
	*/
	void setAsARGB(const ARGB val);

	/**设置 BGRA.
	*/
	void setAsBGRA(const BGRA val);

	/** 设置 ABGR.
	*/
	void setAsABGR(const ABGR val);


	void setData(const void *data, ECOLOR_FORMAT format)
	{
		uint32 color;
		switch (format)
		{
		case ECF_A1R5G5B5:
			color = A1R5G5B5toA8R8G8B8(*(uint16*)data);
			break;
		case ECF_R5G6B5:
			color = R5G6B5toA8R8G8B8(*(uint16*)data);
			break;
		case ECF_A8R8G8B8:
			color = *(uint32*)data;
			break;
		case ECF_R8G8B8:
		{
			uint8* p = (uint8*)data;
			//set(255, p[0], p[1], p[2]);
			color = ((255 & 0xff) << 24) | ((p[0] & 0xff) << 16) | ((p[1] & 0xff) << 8) | (p[2] & 0xff);
		}
		break;
		default:
			color = 0xffffffff;
			break;
		}
		setAsARGB(color);
	}

	//! 以指定的颜色格式写入一个颜色数据
	/** \param data: 目标的颜色数据。必须包含足够大的内容一般接受指定格式的颜色数据
	\param format: 告诉要用什么颜色数据
	*/
	void getData(void *data, ECOLOR_FORMAT format)
	{
		uint32 color = getAsARGB();
		switch (format)
		{
		case ECF_A1R5G5B5:
		{
			uint16 * dest = (uint16*)data;
			*dest = A8R8G8B8toA1R5G5B5(color);
		}
		break;

		case ECF_R5G6B5:
		{
			uint16 * dest = (uint16*)data;
			*dest = A8R8G8B8toR5G6B5(color);
		}
		break;

		case ECF_R8G8B8:
		{
			uint8* dest = (uint8*)data;
			dest[0] = (uint8)getRed();
			dest[1] = (uint8)getGreen();
			dest[2] = (uint8)getBlue();
		}
		break;

		case ECF_A8R8G8B8:
		{
			uint32 * dest = (uint32*)data;
			*dest = color;
		}
		break;

		default:
			break;
		}
	}

	float r;
	float g;
	float b;
	float a;

	//定义常用颜色
	static const Color ZERO;
	static const Color Black;   //黑   
	static const Color White;    //白
	static const Color Red;      //红
	static const Color Green;     //绿
	static const Color Blue;        //蓝

private:

	

};