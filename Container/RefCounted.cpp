#include <DebugNew.h>
#include <assert.h>
#include "RefCounted.h"


namespace Sapphire
{


	RefCounted::RefCounted() :
		refCount_(new RefCount())
	{
		// Hold a weak ref to self to avoid possible double delete of the refcount
		//保存一个弱引用到自己避免引用的重复删除
		(refCount_->weakRefs_)++;
	}

	RefCounted::~RefCounted()
	{
		assert(refCount_);
		assert(refCount_->refs_ == 0);
		assert(refCount_->weakRefs_ > 0);

		//标记对象为过期，如果没有别的弱引用存在释放他自己的弱引用并删除引用计数
		// Mark object as expired, release the self weak ref and delete the refcount if no other weak refs exist
		refCount_->refs_ = -1;
		(refCount_->weakRefs_)--;
		if (!refCount_->weakRefs_)
			delete refCount_;

		refCount_ = 0;
	}

	void RefCounted::AddRef()
	{
		assert(refCount_->refs_ >= 0);
		(refCount_->refs_)++;
	}

	void RefCounted::ReleaseRef()
	{
		assert(refCount_->refs_ > 0);
		(refCount_->refs_)--;
		if (!refCount_->refs_)
			delete this;
	}

	int RefCounted::Refs() const
	{
		return refCount_->refs_;
	}

	int RefCounted::WeakRefs() const
	{
		return refCount_->weakRefs_ - 1;
	}
}