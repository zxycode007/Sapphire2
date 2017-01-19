#include "Vector4.h"
#include <cstdio>

#include "DebugNew.h"


namespace Sapphire
{
	const Vector4 Vector4::ZERO;
	const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

	String Vector4::ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g", x_, y_, z_, w_);
		return String(tempBuffer);
	}
}