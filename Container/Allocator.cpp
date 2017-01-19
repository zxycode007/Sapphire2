#include "Allocator.h"
#include "DebugNew.h"


namespace Sapphire
{
	
	AllocatorBlock* AllocatorReserveBlock(AllocatorBlock* allocator, unsigned nodeSize, unsigned capacity)
	{
		if (!capacity)
			capacity = 1;
		//块大小=分配块数据结构大小 + 所有的分配节点总大小
		unsigned char* blockPtr = new unsigned char[sizeof(AllocatorBlock) + capacity * (sizeof(AllocatorNode) + nodeSize)];
		AllocatorBlock* newBlock = reinterpret_cast<AllocatorBlock*>(blockPtr);
		newBlock->nodeSize_ = nodeSize;
		newBlock->capacity_ = capacity;
		newBlock->free_ = 0;
		newBlock->next_ = 0;

		if (!allocator)
			allocator = newBlock;
		else
		{
			newBlock->next_ = allocator->next_;
			allocator->next_ = newBlock;
		}

	 
		// 初始化节点，空节点链接到首个分配器后面
		unsigned char* nodePtr = blockPtr + sizeof(AllocatorBlock);
		AllocatorNode* firstNewNode = reinterpret_cast<AllocatorNode*>(nodePtr);

		for (unsigned i = 0; i < capacity - 1; ++i)
		{
			AllocatorNode* newNode = reinterpret_cast<AllocatorNode*>(nodePtr);
			newNode->next_ = reinterpret_cast<AllocatorNode*>(nodePtr + sizeof(AllocatorNode) + nodeSize);
			nodePtr += sizeof(AllocatorNode) + nodeSize;
		}
		// i == capacity - 1
	{
		AllocatorNode* newNode = reinterpret_cast<AllocatorNode*>(nodePtr);
		newNode->next_ = 0;
	}

	allocator->free_ = firstNewNode;

	return newBlock;
	}

	AllocatorBlock* AllocatorInitialize(unsigned nodeSize, unsigned initialCapacity)
	{
		AllocatorBlock* block = AllocatorReserveBlock(0, nodeSize, initialCapacity);
		return block;
	}

	void AllocatorUninitialize(AllocatorBlock* allocator)
	{
		while (allocator)
		{
			AllocatorBlock* next = allocator->next_;
			delete[] reinterpret_cast<unsigned char*>(allocator);
			allocator = next;
		}
	}

	void* AllocatorReserve(AllocatorBlock* allocator)
	{
		if (!allocator)
			return 0;

		if (!allocator->free_)
		{
			// Free节点耗尽，分配一个新块
			//进行预分配
			unsigned newCapacity = (allocator->capacity_ + 1) >> 1;
			AllocatorReserveBlock(allocator, allocator->nodeSize_, newCapacity);
			allocator->capacity_ += newCapacity;
		}

		
		AllocatorNode* freeNode = allocator->free_;
		void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(AllocatorNode);
		allocator->free_ = freeNode->next_;
		freeNode->next_ = 0;

		return ptr;
	}

	void AllocatorFree(AllocatorBlock* allocator, void* ptr)
	{
		if (!allocator || !ptr)
			return;

		unsigned char* dataPtr = static_cast<unsigned char*>(ptr);
		AllocatorNode* node = reinterpret_cast<AllocatorNode*>(dataPtr - sizeof(AllocatorNode));

		 
		node->next_ = allocator->free_;
		allocator->free_ = node;
	}
}