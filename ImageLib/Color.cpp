
#include "Color.h"


const Color Color::ZERO = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::Black = Color(0.0f, 0.0f, 0.0f,1.0f);
const Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);

void Color::saturate(float min, float max)
{
	if (a > max)
	{
		a = 1.0f;
	}
	else if (a < min)
	{
		a = 0.0f;
	}
	if (r > max)
	{
		r = 1.0f;
	}
	else if (r < min)
	{
		r = 0.0f;
	}
	if (g > max)
	{
		g = 1.0f;
	}
	else if (g < min)
	{
		g = 0.0f;
	}
	if (b > max)
	{
		b = 1.0f;
	}
	else if (a < min)
	{
		b = 0.0f;
	}
}




RGBA Color::getAsRGBA(void) const
{
	uint8 val8;
	uint32 val32 = 0;

	// 转换到32位模式
	// (RGBA = 8888)

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 16;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 += val8;

	return val32;
}


ARGB Color::getAsARGB(void) const
{
	uint8 val8;
	uint32 val32 = 0;

	// 转换到32位模式
	// (ARGB = 8888)

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 = val8 << 24;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 8;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8;


	return val32;
}


BGRA Color::getAsBGRA(void) const
{
	uint8 val8;
	uint32 val32 = 0;

	// 转换到32位模式
	// (ARGB = 8888)

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 16;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 += val8;


	return val32;

}


ABGR Color::getAsABGR(void) const
{
	uint8 val8;
	uint32 val32 = 0;

	// 转换到32位模式
	// (ABRG = 8888)

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 = val8 << 24;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 8;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8;


	return val32;
}


void Color::setAsRGBA(const RGBA val)
{
	uint32 val32 = val;

	// 转换到32位模式
	// (RGBA = 8888)

	// Red
	r = ((val32 >> 24) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 16) & 0xFF) / 255.0f;

	// Blue
	b = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	a = (val32 & 0xFF) / 255.0f;
}


void Color::setAsARGB(const ARGB val)
{
	uint32 val32 = val;

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


void Color::setAsBGRA(const BGRA val)
{
	uint32 val32 = val;

	// 转换到32位模式
	// (ARGB = 8888)

	// Blue
	b = ((val32 >> 24) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 16) & 0xFF) / 255.0f;

	// Red
	r = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	a = (val32 & 0xFF) / 255.0f;

}


void Color::setAsABGR(const ABGR val)
{
	uint32 val32 = val;

	// 转换到32位模式
	// (ABGR = 8888)

	// Alpha
	a = ((val32 >> 24) & 0xFF) / 255.0f;

	// Blue
	b = ((val32 >> 16) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 8) & 0xFF) / 255.0f;

	// Red
	r = (val32 & 0xFF) / 255.0f;
}