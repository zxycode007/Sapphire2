#pragma once

#include "HashBase.h"
#include "RefCounted.h"

#include <cassert>

namespace Sapphire
{

	// 共享的数组指针模板类. 用于非侵入式引用计数
	template <class T> class SharedArrayPtr
	{
	public:
		/// 构造一个非空共享数组指针
		SharedArrayPtr() :
			ptr_(0),
			refCount_(0)
		{
		}

		SharedArrayPtr(const SharedArrayPtr<T>& rhs) :
			ptr_(rhs.ptr_),
			refCount_(rhs.refCount_)
		{
			AddRef();
		}

		/// 从一个原始指针构造
		explicit SharedArrayPtr(T* ptr) :
			ptr_(ptr),
			refCount_(new RefCount())
		{
			AddRef();
		}

		// 析构函数，释放的数组引用
		~SharedArrayPtr()
		{
			ReleaseRef();
		}

		// 从一个另外的共享数组指针来分配
		SharedArrayPtr<T>& operator =(const SharedArrayPtr<T>& rhs)
		{
			if (ptr_ == rhs.ptr_)
				return *this;

			ReleaseRef();
			ptr_ = rhs.ptr_;
			refCount_ = rhs.refCount_;
			AddRef();

			return *this;
		}

		/// 从一个另外的原始指针来分配
		SharedArrayPtr<T>& operator =(T* ptr)
		{
			if (ptr_ == ptr)
				return *this;

			ReleaseRef();

			if (ptr)
			{
				ptr_ = ptr;
				refCount_ = new RefCount();
				AddRef();
			}

			return *this;
		}

		///执行数组
		T* operator ->() const
		{
			assert(ptr_);
			return ptr_;
		}

		/// 对数组解引用,返回原始指针
		T& operator *() const
		{
			assert(ptr_);
			return *ptr_;
		}

		/// 数组索引
		T& operator [](const int index)
		{
			assert(ptr_);
			return ptr_[index];
		}

		// 测试另外一个数组指针是否相等
		bool operator ==(const SharedArrayPtr<T>& rhs) const { return ptr_ == rhs.ptr_; }

		/// 测试另外一个数组指针是否不相等
		bool operator !=(const SharedArrayPtr<T>& rhs) const { return ptr_ != rhs.ptr_; }

		/// Test for less than with another array pointer.
		//  测试是否小于另外一个数组指针
		bool operator <(const SharedArrayPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		/// 转换到原始指针
		operator T*() const { return ptr_; }

		/// 回复到null和释放所有引用
		void Reset() { ReleaseRef(); }

		/// Perform a static cast from a shared array pointer of another type.
		// 对一个数组指针应用一个静态转换到另外一种类型
		template <class U> void StaticCast(const SharedArrayPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			refCount_ = rhs.RefCountPtr();
			AddRef();
		}

		/// Perform a reinterpret cast from a shared array pointer of another type.
		//对一个数组指针应用一个重解释转换到另外的类型
		template <class U> void ReinterpretCast(const SharedArrayPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = reinterpret_cast<T*>(rhs.Get());
			refCount_ = rhs.RefCountPtr();
			AddRef();
		}

		/// Check if the pointer is null.
		//检查指针是否为空
		bool Null() const { return ptr_ == 0; }

		/// Check if the pointer is not null.
		//检查指针是否非空
		bool NotNull() const { return ptr_ != 0; }

		/// Return the raw pointer.
		// 返回原始指针
		T* Get() const { return ptr_; }

		/// Return the array's reference count, or 0 if the pointer is null.
		// 返回数组的引用次数，如果指针位NULL返回0
		int Refs() const { return refCount_ ? refCount_->refs_ : 0; }

		/// Return the array's weak reference count, or 0 if the pointer is null.
		// 返回数组的弱引用数量， 如果为NULL返回0
		int WeakRefs() const { return refCount_ ? refCount_->weakRefs_ : 0; }

		/// Return pointer to the RefCount structure.
		// 返回引用计数结构的指针
		RefCount* RefCountPtr() const { return refCount_; }

		/// Return hash value for HashSet & HashMap.
		//  返回hashSet和HashMap的hash值
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		/// Prevent direct assignment from a shared array pointer of different type.
		// 阻止从一个不同类型的shared array pointer分配
		template <class U> SharedArrayPtr<T>& operator =(const SharedArrayPtr<U>& rhs);

		/// Add a reference to the array pointed to.
		//添加引用
		void AddRef()
		{
			if (refCount_)
			{
				assert(refCount_->refs_ >= 0);
				++(refCount_->refs_);
			}
		}

		/// Release the array reference and delete it and the RefCount structure if necessary.
		void ReleaseRef()
		{
			if (refCount_)
			{
				assert(refCount_->refs_ > 0);
				--(refCount_->refs_);
				if (!refCount_->refs_)
				{
					refCount_->refs_ = -1;
					delete[] ptr_;
				}

				if (refCount_->refs_ < 0 && !refCount_->weakRefs_)
					delete refCount_;
			}

			ptr_ = 0;
			refCount_ = 0;
		}

		/// Pointer to the array.
		T* ptr_;
		/// Pointer to the RefCount structure.
		RefCount* refCount_;
	};

	/// Perform a static cast from one shared array pointer type to another.
	template <class T, class U> SharedArrayPtr<T> StaticCast(const SharedArrayPtr<U>& ptr)
	{
		SharedArrayPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a reinterpret cast from one shared array pointer type to another.
	template <class T, class U> SharedArrayPtr<T> ReinterpretCast(const SharedArrayPtr<U>& ptr)
	{
		SharedArrayPtr<T> ret;
		ret.ReinterpretCast(ptr);
		return ret;
	}

	/// Weak array pointer template class. Uses non-intrusive reference counting.
	//弱引用指针
	template <class T> class WeakArrayPtr
	{
	public:
		/// Construct a null weak array pointer.
		WeakArrayPtr() :
			ptr_(0),
			refCount_(0)
		{
		}

		/// Copy-construct from another weak array pointer.
		WeakArrayPtr(const WeakArrayPtr<T>& rhs) :
			ptr_(rhs.ptr_),
			refCount_(rhs.refCount_)
		{
			AddRef();
		}

		/// Construct from a shared array pointer.
		WeakArrayPtr(const SharedArrayPtr<T>& rhs) :
			ptr_(rhs.Get()),
			refCount_(rhs.RefCountPtr())
		{
			AddRef();
		}

		/// Destruct. Release the weak reference to the array.
		~WeakArrayPtr()
		{
			ReleaseRef();
		}

		/// Assign from a shared array pointer.
		WeakArrayPtr<T>& operator =(const SharedArrayPtr<T>& rhs)
		{
			if (ptr_ == rhs.Get() && refCount_ == rhs.RefCountPtr())
				return *this;

			ReleaseRef();
			ptr_ = rhs.Get();
			refCount_ = rhs.RefCountPtr();
			AddRef();

			return *this;
		}

		/// Assign from another weak array pointer.
		WeakArrayPtr<T>& operator =(const WeakArrayPtr<T>& rhs)
		{
			if (ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_)
				return *this;

			ReleaseRef();
			ptr_ = rhs.ptr_;
			refCount_ = rhs.refCount_;
			AddRef();

			return *this;
		}

		/// Convert to shared array pointer. If expired, return a null shared array pointer.
		SharedArrayPtr<T> Lock() const
		{
			if (Expired())
				return SharedArrayPtr<T>();
			else
				return SharedArrayPtr<T>(ptr_, refCount_);
		}

		/// Return raw pointer. If expired, return null.
		T* Get() const
		{
			if (Expired())
				return 0;
			else
				return ptr_;
		}

		/// Point to the array.
		T* operator ->() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return rawPtr;
		}

		/// Dereference the array.
		T& operator *() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return *rawPtr;
		}

		/// Subscript the array.
		T& operator [](const int index)
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return (*rawPtr)[index];
		}

		/// Test for equality with another weak array pointer.
		bool operator ==(const WeakArrayPtr<T>& rhs) const { return ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_; }

		/// Test for inequality with another weak array pointer.
		bool operator !=(const WeakArrayPtr<T>& rhs) const { return ptr_ != rhs.ptr_ || refCount_ != rhs.refCount_; }

		/// Test for less than with another weak array pointer.
		bool operator <(const WeakArrayPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		/// Convert to a raw pointer, null if array is expired.
		operator T*() const { return Get(); }

		/// Reset to null and release the weak reference.
		void Reset() { ReleaseRef(); }

		/// Perform a static cast from a weak array pointer of another type.
		template <class U> void StaticCast(const WeakArrayPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			refCount_ = rhs.refCount_;
			AddRef();
		}

		/// Perform a dynamic cast from a weak array pointer of another type.
		template <class U> void DynamicCast(const WeakArrayPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = dynamic_cast<T*>(rhs.Get());

			if (ptr_)
			{
				refCount_ = rhs.refCount_;
				AddRef();
			}
			else
				refCount_ = 0;
		}

		/// Check if the pointer is null.
		bool Null() const { return refCount_ == 0; }

		/// Check if the pointer is not null.
		bool NotNull() const { return refCount_ != 0; }

		/// Return the array's reference count, or 0 if null pointer or if array has expired.
		int Refs() const { return (refCount_ && refCount_->refs_ >= 0) ? refCount_->refs_ : 0; }

		/// Return the array's weak reference count.
		int WeakRefs() const { return refCount_ ? refCount_->weakRefs_ : 0; }

		/// Return whether the array has expired. If null pointer, always return true.
		bool Expired() const { return refCount_ ? refCount_->refs_ < 0 : true; }

		/// Return pointer to RefCount structure.
		RefCount* RefCountPtr() const { return refCount_; }

		/// Return hash value for HashSet & HashMap.
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		/// Prevent direct assignment from a weak array pointer of different type.
		template <class U> WeakArrayPtr<T>& operator =(const WeakArrayPtr<U>& rhs);

		/// Add a weak reference to the array pointed to.
		void AddRef()
		{
			if (refCount_)
			{
				assert(refCount_->weakRefs_ >= 0);
				++(refCount_->weakRefs_);
			}
		}

		/// Release the weak reference. Delete the Refcount structure if necessary.
		void ReleaseRef()
		{
			if (refCount_)
			{
				assert(refCount_->weakRefs_ >= 0);

				if (refCount_->weakRefs_ > 0)
					--(refCount_->weakRefs_);

				if (Expired() && !refCount_->weakRefs_)
					delete refCount_;
			}

			ptr_ = 0;
			refCount_ = 0;
		}

		/// Pointer to the array.
		T* ptr_;
		/// Pointer to the RefCount structure.
		RefCount* refCount_;
	};

	/// Perform a static cast from one weak array pointer type to another.
	template <class T, class U> WeakArrayPtr<T> StaticCast(const WeakArrayPtr<U>& ptr)
	{
		WeakArrayPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a reinterpret cast from one weak pointer type to another.
	template <class T, class U> WeakArrayPtr<T> ReinterpretCast(const WeakArrayPtr<U>& ptr)
	{
		WeakArrayPtr<T> ret;
		ret.ReinterpretCast(ptr);
		return ret;
	}

}