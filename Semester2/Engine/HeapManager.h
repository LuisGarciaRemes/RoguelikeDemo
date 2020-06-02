#pragma once
#include "OverloadNewAndDelete.h";
class HeapManager
{
	struct BlockDescriptor {
		BlockDescriptor * next;
		void * blockStart;
		size_t blockSize;
	};

public:
	static HeapManager * create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	void destroy();
	void * _alloc(size_t i_size);
	void * _alloc(size_t i_size, size_t i_alignment);
	bool _free(void * i_ptr);
	void collect();
	bool Contains(void * i_ptr) const;
	bool IsAllocated(void * i_ptr) const;
	size_t getLargestFreeBlock() const;
	size_t getTotalFreeMemory() const;
	void ShowFreeBlocks() const;
	void ShowOutstandingAllocations() const;

private:
	BlockDescriptor * outstandingBlockHead;
	BlockDescriptor * freeBlockHead;
	BlockDescriptor * descriptorPoolHead;

	void * heapMemory;
	size_t sizeHeap;
	unsigned int numDescriptors;
};
