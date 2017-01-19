#pragma once

#include "Sapphire.h"


namespace Sapphire
{

	/// Set the random seed. The default seed is 1.
	SAPPHIRE_API void SetRandomSeed(unsigned seed);
	/// Return the current random seed.
	SAPPHIRE_API unsigned GetRandomSeed();
	/// Return a random number between 0-32767. Should operate similarly to MSVC rand().
	SAPPHIRE_API int Rand();
	/// Return a standard normal distributed number.
	SAPPHIRE_API float RandStandardNormal();

}