#pragma once

#include "RefCounted.h"

#include <cassert>
#include <cstddef>


namespace Sapphire
{
	/// Shared pointer template class with intrusive reference counting.
	// 侵入式引用计数的shared pointer模板类
	template <class T> class SharedPtr
	{
	public:
		/// Construct a null shared pointer.
		SharedPtr() :
			ptr_(0)
		{
		}

		/// Copy-construct from another shared pointer.
		//复制构造函数
		SharedPtr(const SharedPtr<T>& rhs) :
			ptr_(rhs.ptr_)
		{
			AddRef();
		}

		/// Construct from a raw pointer.
		//从一个原始指针构造
		explicit SharedPtr(T* ptr) :
			ptr_(ptr)
		{
			AddRef();
		}

		/// Destruct. Release the object reference.
		~SharedPtr()
		{
			ReleaseRef();
		}

		/// Assign from another shared pointer.
		//从另外一个共享指针分配
		SharedPtr<T>& operator =(const SharedPtr<T>& rhs)
		{
			if (ptr_ == rhs.ptr_)
				return *this;
			//释放原对象的引用
			ReleaseRef();
			ptr_ = rhs.ptr_;
			//添加新对象的引用
			AddRef();

			return *this;
		}

		/// Assign from a raw pointer.
		//从一个原指针分配
		SharedPtr<T>& operator =(T* ptr)
		{
			if (ptr_ == ptr)
				return *this;

			ReleaseRef();
			ptr_ = ptr;
			AddRef();

			return *this;
		}

		/// Point to the object.
		// 指向原始对象
		T* operator ->() const
		{
			assert(ptr_);
			return ptr_;
		}

		/// Dereference the object.
		//间接引用对象
		T& operator *() const
		{
			assert(ptr_);
			return *ptr_;
		}

		/// Subscript the object if applicable.
		//如可用的话，取对象的下标
		T& operator [](const int index)
		{
			assert(ptr_);
			return ptr_[index];
		}

		/// Test for less than with another shared pointer.
		//  shared pointer 比较
		bool operator <(const SharedPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		/// Test for equality with another shared pointer.
		bool operator ==(const SharedPtr<T>& rhs) const { return ptr_ == rhs.ptr_; }

		/// Test for inequality with another shared pointer.
		bool operator !=(const SharedPtr<T>& rhs) const { return ptr_ != rhs.ptr_; }

		/// Convert to a raw pointer.
		//  转换到原始指针
		operator T*() const { return ptr_; }

		/// Reset to null and release the object reference.
		//  重置到空并且释放对象引用
		void Reset() { ReleaseRef(); }

		/// Detach without destroying the object even if the refcount goes zero. To be used for scripting language interoperation.
		//  如果引用计数到0脱离，但不销毁对象。  用于脚本语言的交互
		void Detach()
		{
			if (ptr_)
			{
				RefCount* refCount = RefCountPtr();
				++refCount->refs_; // 2 refs  +1
				Reset(); // 1 ref   -1
				--refCount->refs_; // 0 refs  -1
			}
		}

		/// Perform a static cast from a shared pointer of another type.
		//从另外一个类型的shared pointer静态转换
		template <class U> void StaticCast(const SharedPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			AddRef();
		}

		/// Perform a dynamic cast from a shared pointer of another type.
		//从另外一个类型的shared pointer动态转换
		template <class U> void DynamicCast(const SharedPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = dynamic_cast<T*>(rhs.Get());
			AddRef();
		}

		/// Check if the pointer is null.
		//  检查指针是否为空
		bool Null() const { return ptr_ == 0; }

		/// Check if the pointer is not null.
		// 检查指针是否不为空
		bool NotNull() const { return ptr_ != 0; }

		/// Return the raw pointer.
		//  返回原始指针
		T* Get() const { return ptr_; }

		/// Return the object's reference count, or 0 if the pointer is null.
		//  返回对象的引用次数, 如果为0指针为空
		int Refs() const { return ptr_ ? ptr_->Refs() : 0; }

		/// Return the object's weak reference count, or 0 if the pointer is null.
		//  返回对象的弱引用
		int WeakRefs() const { return ptr_ ? ptr_->WeakRefs() : 0; }

		/// Return pointer to the RefCount structure.
		//  返回引用计数结构体
		RefCount* RefCountPtr() const { return ptr_ ? ptr_->RefCountPtr() : 0; }

		/// Return hash value for HashSet & HashMap.
		// 返回对于HashSet和HashMap的hash值
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		/// Prevent direct assignment from a shared pointer of another type.
		template <class U> SharedPtr<T>& operator =(const SharedPtr<U>& rhs);

		/// Add a reference to the object pointed to.
		void AddRef()
		{
			if (ptr_)
				ptr_->AddRef();
		}

		/// Release the object reference and delete it if necessary.
		//释放对象的引用，如果必要删除对象
		void ReleaseRef()
		{
			if (ptr_)
			{
				ptr_->ReleaseRef();
				ptr_ = 0;
			}
		}

		/// Pointer to the object.
		//对象的指针
		T* ptr_;
	};

	/// Perform a static cast from one shared pointer type to another.
	//静态转换
	template <class T, class U> SharedPtr<T> StaticCast(const SharedPtr<U>& ptr)
	{
		SharedPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a dynamic cast from one weak pointer type to another.
	// 动态转换
	template <class T, class U> SharedPtr<T> DynamicCast(const SharedPtr<U>& ptr)
	{
		SharedPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}

	/// Weak pointer template class with intrusive reference counting. Does not keep the object pointed to alive.
	// 侵入式引用计数的弱引用指针模板类。  不能保证对象指针存活
	template <class T> class WeakPtr
	{
	public:
		/// Construct a null weak pointer.
		WeakPtr() :
			ptr_(0),
			refCount_(0)
		{
		}

		/// Copy-construct from another weak pointer.
		WeakPtr(const WeakPtr<T>& rhs) :
			ptr_(rhs.ptr_),
			refCount_(rhs.refCount_)
		{
			AddRef();
		}

		/// Construct from a shared pointer.
		WeakPtr(const SharedPtr<T>& rhs) :
			ptr_(rhs.Get()),
			refCount_(rhs.RefCountPtr())
		{
			AddRef();
		}

		/// Construct from a raw pointer.
		explicit WeakPtr(T* ptr) :
			ptr_(ptr),
			refCount_(ptr ? ptr->RefCountPtr() : 0)
		{
			AddRef();
		}

		/// Destruct. Release the weak reference to the object.
		~WeakPtr()
		{
			ReleaseRef();
		}

		/// Assign from a shared pointer.
		//  从shared pointer 分配
		WeakPtr<T>& operator =(const SharedPtr<T>& rhs)
		{
			//判断是不是同一个对象
			if (ptr_ == rhs.Get() && refCount_ == rhs.RefCountPtr())
				return *this;

			ReleaseRef();
			ptr_ = rhs.Get();
			refCount_ = rhs.RefCountPtr();
			AddRef();

			return *this;
		}

		/// Assign from a weak pointer.
		// 从一个弱引用指针分配
		WeakPtr<T>& operator =(const WeakPtr<T>& rhs)
		{
			if (ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_)
				return *this;

			ReleaseRef();
			ptr_ = rhs.ptr_;
			refCount_ = rhs.refCount_;
			AddRef();

			return *this;
		}

		/// Assign from a raw pointer.
		//  从一个原始指针分配
		WeakPtr<T>& operator =(T* ptr)
		{
			RefCount* refCount = ptr ? ptr->RefCountPtr() : 0;

			if (ptr_ == ptr && refCount_ == refCount)
				return *this;

			ReleaseRef();
			ptr_ = ptr;
			refCount_ = refCount;
			AddRef();

			return *this;
		}

		/// Convert to a shared pointer. If expired, return a null shared pointer.
		//  转换到一个shared pointer。 如果失效，返回空的shared pointer
		SharedPtr<T> Lock() const
		{
			if (Expired())
				return SharedPtr<T>();
			else
				return SharedPtr<T>(ptr_);
		}

		/// Return raw pointer. If expired, return null.
		//  返回原始指针，如果失效，返回空
		T* Get() const
		{
			if (Expired())
				return 0;
			else
				return ptr_;
		}

		/// Point to the object.
		//  指向原始指针对象
		T* operator ->() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return rawPtr;
		}

		/// Dereference the object.
		//  间接引用对象
		T& operator *() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return *rawPtr;
		}

		/// Subscript the object if applicable.
		// 如果可以访问对象下标
		T& operator [](const int index)
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return (*rawPtr)[index];
		}

		/// Test for equality with another weak pointer.
		//  比较弱引用指针是否相同
		bool operator ==(const WeakPtr<T>& rhs) const { return ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_; }

		/// Test for inequality with another weak pointer.
		//  比较弱引用指针是否不相同
		bool operator !=(const WeakPtr<T>& rhs) const { return ptr_ != rhs.ptr_ || refCount_ != rhs.refCount_; }

		/// Test for less than with another weak pointer.
		bool operator <(const WeakPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		/// Convert to a raw pointer, null if the object is expired.
		// 转换到原始指针，如果无效，返回空
		operator T*() const { return Get(); }

		/// Reset to null and release the weak reference.
		//  重新设置到空并释放弱引用
		void Reset() { ReleaseRef(); }

		/// Perform a static cast from a weak pointer of another type.
		/// 从一个其它类型应用一个静态转换
		template <class U> void StaticCast(const WeakPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			refCount_ = rhs.refCount_;
			AddRef();
		}

		/// Perform a dynamic cast from a weak pointer of another type.
		/// 从一个其它类型应用一个动态转换
		template <class U> void DynamicCast(const WeakPtr<U>& rhs)
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
		//  检测指针是否为空
		bool Null() const { return refCount_ == 0; }

		/// Check if the pointer is not null.
		//  检测指针是否不为空
		bool NotNull() const { return refCount_ != 0; }

		/// Return the object's reference count, or 0 if null pointer or if object has expired.
		//  返回对象的引用数量， 如果为0，对象已经失效
		int Refs() const { return (refCount_ && refCount_->refs_ >= 0) ? refCount_->refs_ : 0; }

		/// Return the object's weak reference count.
		//  返回对象的弱引用数量
		int WeakRefs() const
		{
			if (!Expired())
				return ptr_->WeakRefs();
			else
				return refCount_ ? refCount_->weakRefs_ : 0;
		}

		/// Return whether the object has expired. If null pointer, always return true.
		//  返回对象是否无效， 如果引用小于0，无效
		bool Expired() const { return refCount_ ? refCount_->refs_ < 0 : true; }

		/// Return pointer to the RefCount structure.
		//  返回引用计数结构体
		RefCount* RefCountPtr() const { return refCount_; }

		/// Return hash value for HashSet & HashMap.
		//  返回hash值
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		/// Prevent direct assignment from a weak pointer of different type.
		template <class U> WeakPtr<T>& operator =(const WeakPtr<U>& rhs);

		/// Add a weak reference to the object pointed to.
		//  为对象添加一个弱引用
		void AddRef()
		{
			if (refCount_)
			{
				assert(refCount_->weakRefs_ >= 0);
				++(refCount_->weakRefs_);
			}
		}

		/// Release the weak reference. Delete the Refcount structure if necessary.
		// 释放一个弱引用，  如必要删除引用结构体
		void ReleaseRef()
		{
			if (refCount_)
			{
				assert(refCount_->weakRefs_ > 0);
				--(refCount_->weakRefs_);

				if (Expired() && !refCount_->weakRefs_)
					delete refCount_;
			}

			ptr_ = 0;
			refCount_ = 0;
		}

		/// Pointer to the object.
		T* ptr_;
		/// Pointer to the RefCount structure.
		RefCount* refCount_;
	};

	/// Perform a static cast from one weak pointer type to another.
	template <class T, class U> WeakPtr<T> StaticCast(const WeakPtr<U>& ptr)
	{
		WeakPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a dynamic cast from one weak pointer type to another.
	template <class T, class U> WeakPtr<T> DynamicCast(const WeakPtr<U>& ptr)
	{
		WeakPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}
}