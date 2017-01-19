#include "VectorBase.h"
#include <DebugNew.h>

namespace Sapphire
{
	unsigned char* VectorBase::AllocateBuffer(unsigned size)
	{
		return new unsigned char[size];
	}
}