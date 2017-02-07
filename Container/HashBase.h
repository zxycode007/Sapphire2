#pragma once

#include "Sapphire.h"

#include "Allocator.h"
#include "Hash.h"
#include "Swap.h"


namespace Sapphire
{
	/// Hash set/map node base class.
	//  hash set/map 节点基类
	struct HashNodeBase
	{
		/// Construct.
		HashNodeBase() :
			down_(0),
			prev_(0),
			next_(0)
		{
		}

		/// Next node in the bucket.
		//这个桶里的下个节点 
		HashNodeBase* down_;
		/// Previous node.
		//  前一个节点
		HashNodeBase* prev_;
		/// Next node.
		//  下个节点
		HashNodeBase* next_;
	};

	/// Hash set/map iterator base class.
	//  hash set/map 迭代器基类
	struct HashIteratorBase
	{
		/// Construct.
		HashIteratorBase() :
			ptr_(0)
		{
		}

		/// Construct with a node pointer.
		//  用一个节点指针的构造器
		explicit HashIteratorBase(HashNodeBase* ptr) :
			ptr_(ptr)
		{
		}

		/// Test for equality with another iterator.
		//  比较指针值
		bool operator ==(const HashIteratorBase& rhs) const { return ptr_ == rhs.ptr_; }

		/// Test for inequality with another iterator.
		// 比较指针值
		bool operator !=(const HashIteratorBase& rhs) const { return ptr_ != rhs.ptr_; }

		/// Go to the next node.
		//  到下一节点
		void GotoNext()
		{
			if (ptr_)
				ptr_ = ptr_->next_;
		}

		/// Go to the previous node.
		//  到上一节点
		void GotoPrev()
		{
			if (ptr_)
				ptr_ = ptr_->prev_;
		}

		/// Node pointer.
		//  节点指针
		HashNodeBase* ptr_;
	};

	/// Hash set/map base class.
	//  hash set/map 基类
	/** Note that to prevent extra memory use due to vtable pointer, %HashBase intentionally does not declare a virtual destructor
	and therefore %HashBase pointers should never be used.
	    注意
	*/
	class SAPPHIRE_API HashBase
	{
	public:
		/// Initial amount of buckets.
		//  初始化一定数量的桶
		static const unsigned MIN_BUCKETS = 8;
		/// Maximum load factor.
		//  最大加载因子
		static const unsigned MAX_LOAD_FACTOR = 4;

		/// Construct.
		HashBase() :
			ptrs_(0),
			allocator_(0)
		{
		}

		/// Swap with another hash set or map.
		//从其它的进行交换
		void Swap(HashBase& rhs)
		{
			Sapphire::Swap(head_, rhs.head_);
			Sapphire::Swap(tail_, rhs.tail_);
			Sapphire::Swap(ptrs_, rhs.ptrs_);
			Sapphire::Swap(allocator_, rhs.allocator_);
		}

		/// Return number of elements.
		// 返回元素数量
		unsigned Size() const { return ptrs_ ? (reinterpret_cast<unsigned*>(ptrs_))[0] : 0; }

		/// Return number of buckets.
		//  返回桶数量
		unsigned NumBuckets() const { return ptrs_ ? (reinterpret_cast<unsigned*>(ptrs_))[1] : 0; }

		/// Return whether has no elements.
		bool Empty() const { return Size() == 0; }

	protected:
		/// Allocate bucket head pointers + room for size and bucket count variables.
		void AllocateBuckets(unsigned size, unsigned numBuckets);

		/// Reset bucket head pointers.
		void ResetPtrs();

		/// Set new size.
		void SetSize(unsigned size) { if (ptrs_) (reinterpret_cast<unsigned*>(ptrs_))[0] = size; }

		/// Return bucket head pointers.
		HashNodeBase** Ptrs() const { return ptrs_ ? ptrs_ + 2 : 0; }

		/// List head node pointer.
		//  头节点指针
		HashNodeBase* head_;
		/// List tail node pointer.
		//  尾部节点指针
		HashNodeBase* tail_;
		/// Bucket head pointers.
		//  桶指针
		HashNodeBase** ptrs_;
		/// Node allocator.
		//  节点分配器
		AllocatorBlock* allocator_;
	};
}