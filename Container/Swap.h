#pragma once 

#include "Sapphire.h"

namespace Sapphire
{
	class HashBase;
	class ListBase;
	class String;
	class VectorBase;

	/// Swap two values.
	template <class T> inline void Swap(T& first, T& second)
	{
		T temp = first;
		first = second;
		second = temp;
	}

	template <> SAPPHIRE_API void Swap<String>(String& first, String& second);
	template <> SAPPHIRE_API void Swap<VectorBase>(VectorBase& first, VectorBase& second);
	template <> SAPPHIRE_API void Swap<ListBase>(ListBase& first, ListBase& second);
	template <> SAPPHIRE_API void Swap<HashBase>(HashBase& first, HashBase& second);

}