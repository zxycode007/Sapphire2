#include "Vector3.h"
#include <cstdio>

#include "DebugNew.h"


namespace Sapphire
{

	const Vector3 Vector3::ZERO;
	const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::DOWN(0.0f, -1.0f, 0.0f);
	const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::BACK(0.0f, 0.0f, -1.0f);
	const Vector3 Vector3::ONE(1.0f, 1.0f, 1.0f);

	String Vector3::ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g", x_, y_, z_);
		return String(tempBuffer);
	}
}