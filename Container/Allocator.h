#pragma once 

#include "Sapphire.h"

namespace Sapphire
{

	struct AllocatorBlock;
	struct AllocatorNode;

	//内存分配块
	struct AllocatorBlock
	{
		//节点大小
		unsigned nodeSize_;
		// 这个内存块的节点数量
		unsigned capacity_;
		//  第一个空闲的节点
		AllocatorNode* free_;
		// 下一个分配块的指针
		AllocatorBlock* next_;
	};

	//内存分配节点
	struct AllocatorNode
	{
		/// 下一个空闲节点
		AllocatorNode* next_;
	};

	//用指定的节点大小和初始容量来初始化一个固定大小的分配器
	SAPPHIRE_API AllocatorBlock* AllocatorInitialize(unsigned nodeSize, unsigned initialCapacity = 1);
	//反初始化一个固定大小的分配器，释放所有链表里的所有块
	SAPPHIRE_API void AllocatorUninitialize(AllocatorBlock* allocator);
	//预定一个节点。如果必要创建一个新块
	SAPPHIRE_API void* AllocatorReserve(AllocatorBlock* allocator);
	/// 释放一个节点，不释放任何块
	SAPPHIRE_API void AllocatorFree(AllocatorBlock* allocator, void* ptr);

	// 分配器模板类。
	template <class T> class Allocator
	{
	public:
		
		Allocator(unsigned initialCapacity = 0) :
			allocator_(0)
		{
			if (initialCapacity)
				allocator_ = AllocatorInitialize((unsigned)sizeof(T), initialCapacity);
		}

		~Allocator()
		{
			AllocatorUninitialize(allocator_);
		}

		//  用默认的构造器预分配一个对象
		T* Reserve()
		{
			if (!allocator_)
				allocator_ = AllocatorInitialize((unsigned)sizeof(T));
			T* newObject = static_cast<T*>(AllocatorReserve(allocator_));
			new(newObject)T();

			return newObject;
		}

		// 用复制构造器来预分配一个对象
		T* Reserve(const T& object)
		{
			if (!allocator_)
				allocator_ = AllocatorInitialize((unsigned)sizeof(T));
			T* newObject = static_cast<T*>(AllocatorReserve(allocator_));
			new(newObject)T(object);

			return newObject;
		}

		// 销毁和释放一个对象
		void Free(T* object)
		{
			(object)->~T();
			AllocatorFree(allocator_, object);
		}

	private:

		Allocator(const Allocator<T>& rhs);

		Allocator<T>& operator =(const Allocator<T>& rhs);

		// 内存分配块指针
		AllocatorBlock* allocator_;
	};
}