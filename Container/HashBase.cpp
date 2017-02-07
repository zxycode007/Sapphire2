#include "HashBase.h"
#include "DebugNew.h"


namespace Sapphire
{

	void HashBase::AllocateBuckets(unsigned size, unsigned numBuckets)
	{
		if (ptrs_)
			delete[] ptrs_;
		//new 一个HashNodeBase数组
		HashNodeBase** ptrs = new HashNodeBase*[numBuckets + 2];
		unsigned* data = reinterpret_cast<unsigned*>(ptrs);
		data[0] = size;
		data[1] = numBuckets;
		ptrs_ = ptrs;

		ResetPtrs();
	}

	void HashBase::ResetPtrs()
	{
		// Reset bucket pointers
		if (!ptrs_)
			return;

		unsigned numBuckets = NumBuckets();
		HashNodeBase** ptrs = Ptrs();
		for (unsigned i = 0; i < numBuckets; ++i)
			ptrs[i] = 0;
	}
}