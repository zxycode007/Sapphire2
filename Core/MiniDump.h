#pragma once

#include "Predefined.h"
#include "Sapphire.h"

namespace Sapphire
{
#if defined(_MSC_VER) && defined(SAPPHIRE_MINIDUMPS)
	/// Write a minidump. Needs to be called from within a structured exception handler.
	SAPPHIRE_API int WriteMiniDump(const char* applicationName, void* exceptionPointers);
#endif
}