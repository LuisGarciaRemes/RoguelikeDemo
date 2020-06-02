#include "HeapManager.h"
#include <iostream>
#include <assert.h>

HeapManager * HeapManager::create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{
	HeapManager * heapManager = reinterpret_cast<HeapManager*>((static_cast<char*>(i_pMemory) + i_sizeMemory -( i_numDescriptors * sizeof(BlockDescriptor)) - sizeof(HeapManager)));
	heapManager->heapMemory = i_pMemory;
	heapManager->sizeHeap = i_sizeMemory;
	heapManager->numDescriptors = i_numDescriptors;
	heapManager->freeBlockHead = reinterpret_cast<BlockDescriptor*>((static_cast<char*>(i_pMemory) + i_sizeMemory - (i_numDescriptors * sizeof(BlockDescriptor))));
	heapManager->freeBlockHead->blockStart = i_pMemory;
	heapManager->freeBlockHead->blockSize = i_sizeMemory - (i_numDescriptors * sizeof(BlockDescriptor)) - sizeof(HeapManager);
	heapManager->freeBlockHead->next = nullptr;
	heapManager->outstandingBlockHead = nullptr;
	heapManager->descriptorPoolHead = reinterpret_cast<BlockDescriptor*>(heapManager->freeBlockHead + sizeof(BlockDescriptor));

	//creates the next block descripter after the original.
	BlockDescriptor * tmpBlock = heapManager->descriptorPoolHead;

	//creates the rest of the block descriptors
	for (int i = 1; i < i_numDescriptors; i++)
	{
		tmpBlock->blockSize = 0;
		tmpBlock->blockStart = nullptr;
		tmpBlock->next = reinterpret_cast<BlockDescriptor*>((reinterpret_cast<char*>(tmpBlock) + sizeof(BlockDescriptor)));
		tmpBlock = tmpBlock->next;
	}

	return heapManager;
}

void HeapManager::destroy()
{
	outstandingBlockHead = nullptr;
	freeBlockHead = nullptr;
	descriptorPoolHead = nullptr;
	heapMemory = nullptr;
	sizeHeap = 0;
	numDescriptors =0;
}

void * HeapManager::_alloc(size_t i_size)
{
	if (freeBlockHead == nullptr)
	{
		return nullptr;
	}

	BlockDescriptor* curr = freeBlockHead;
	BlockDescriptor* prev = nullptr;

	//checks if a block is available.
	while (curr != nullptr)
	{
		if (curr->blockSize == i_size)
		{
			if (prev == nullptr)
			{
				freeBlockHead = curr->next;
				curr->next = outstandingBlockHead;
				outstandingBlockHead = curr;
				return curr->blockStart;
			}			
			else
			{
				prev->next = curr->next;
				curr->next = outstandingBlockHead;
				outstandingBlockHead = curr;
				return curr->blockStart;
			}
		}
		prev = curr;
		curr = curr->next;
	}

	curr = freeBlockHead;
	prev = nullptr;

	//if no block is available. Creates one from a bigger block.
	while (curr != nullptr)
	{
		if (curr->blockSize > i_size)
		{
			BlockDescriptor* newBlock;
			if (descriptorPoolHead != nullptr)
			{
				//gets a free descriptor from pool
				newBlock = descriptorPoolHead;
				//removes the just grabbed descriptor from the pool
				descriptorPoolHead = descriptorPoolHead->next;
			}
			else
			{
				return nullptr;
			}

			curr->blockSize -= i_size;
			newBlock->blockSize = i_size;
			newBlock->blockStart = static_cast<char*>(curr->blockStart) + curr->blockSize;

			//adds block to outstanding list
			newBlock->next = outstandingBlockHead;
			outstandingBlockHead = newBlock;

			return newBlock->blockStart;
		}
		curr = curr->next;
	}
	return nullptr;
}

void * HeapManager::_alloc(size_t i_size, size_t i_alignment)
{
	if (freeBlockHead == nullptr)
	{
		return nullptr;
	}

	BlockDescriptor* curr = freeBlockHead;
	BlockDescriptor* prev = nullptr;

	//checks if a block is available.
	while (curr != nullptr)
	{
		if (curr->blockSize == i_size && reinterpret_cast<uint8_t>(curr->blockStart) % i_alignment == 0)
		{
			if (prev == nullptr)
			{
				freeBlockHead = curr->next;
				curr->next = outstandingBlockHead;
				outstandingBlockHead = curr;
				return curr->blockStart;
			}
			else
			{
				prev->next = curr->next;
				curr->next = outstandingBlockHead;
				outstandingBlockHead = curr;
				return curr->blockStart;
			}
		}
		prev = curr;
		curr = curr->next;
	}

	curr = freeBlockHead;
	prev = nullptr;

	//if no block is available. Creates one from a bigger block.
	while (curr != nullptr)
	{
		unsigned int padding = (((reinterpret_cast<uint8_t>(curr->blockStart) + curr->blockSize) - i_size)%i_alignment);
		if (curr->blockSize > i_size + padding)
		{
			BlockDescriptor* newBlock = nullptr;
			if (descriptorPoolHead != nullptr)
			{
				//gets a free descriptor from pool
				newBlock = descriptorPoolHead;
				//removes the just grabbed descriptor from the pool
				descriptorPoolHead = descriptorPoolHead->next;
			}
			else
			{
				return nullptr;
			}

			//Divides the given block from the end up
			curr->blockSize -= i_size + padding;
			newBlock->blockSize = i_size + padding;
			newBlock->blockStart = static_cast<char*>(curr->blockStart) + curr->blockSize;

			//adds block to outstanding list
			newBlock->next = outstandingBlockHead;
			outstandingBlockHead = newBlock;

			return newBlock->blockStart;
		}
		prev = curr;
		curr = curr->next;
	}
	return nullptr;
}

bool HeapManager::_free(void * i_ptr)
{
	assert(i_ptr);
	//std::cout << "Before Total: " << getTotalFreeMemory() << "\n";
	BlockDescriptor* curr = outstandingBlockHead;
	BlockDescriptor* prev = nullptr;
	while (curr != nullptr)
	{
		if (static_cast<char*>(i_ptr) == static_cast<char*>(curr->blockStart))
		{
			if (prev == nullptr)
			{
				outstandingBlockHead = curr->next;
				curr->next = freeBlockHead;
				freeBlockHead = curr;
				collect();
				//std::cout << "After Total: " << getTotalFreeMemory() << "\n";
				return true;
			}
			else
			{
				prev->next = curr->next;
				curr->next = freeBlockHead;
				freeBlockHead = curr;
				collect();
				//std::cout << "After Total: " << getTotalFreeMemory() << "\n";
				return true;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	return false;
}

void HeapManager::collect()
{
	//std::cout << "Before Largest: " << getLargestFreeBlock() << "\n";
	BlockDescriptor* currFree = freeBlockHead;
	BlockDescriptor* prevCheck = nullptr;
	BlockDescriptor* ptrCheck = freeBlockHead;

	while (currFree != nullptr)
	{
		while (ptrCheck != nullptr)
		{
			if ((static_cast<char*>(currFree->blockStart) + currFree->blockSize) == static_cast<char*>(ptrCheck->blockStart))
			{		
				//merge memory
				currFree->blockSize += ptrCheck->blockSize;

				//free blockdescriptor
				ptrCheck->blockStart = nullptr;
				ptrCheck->blockSize = 0;

				if (prevCheck == nullptr)
				{
					freeBlockHead = ptrCheck->next;
					ptrCheck->next = descriptorPoolHead;
					descriptorPoolHead = ptrCheck;
				}
				else
				{
					prevCheck->next = ptrCheck->next;
					ptrCheck->next = descriptorPoolHead;
					descriptorPoolHead = ptrCheck;
				}
				prevCheck = nullptr;
				ptrCheck = freeBlockHead;
			}
				prevCheck = ptrCheck;
				ptrCheck = ptrCheck->next;		
		}
		currFree = currFree->next;
		prevCheck = nullptr;
		ptrCheck = freeBlockHead;
	}
	//std::cout << "After Largest: " << getLargestFreeBlock() << "\n";
}

bool HeapManager::Contains(void * i_ptr) const
{
	assert(i_ptr);
	if (i_ptr >= heapMemory && i_ptr < static_cast<char*>(heapMemory) + sizeHeap)
	{
		return true;
	}
	return false;
}

bool HeapManager::IsAllocated(void * i_ptr) const
{
	assert(i_ptr);
	BlockDescriptor* curr = outstandingBlockHead;

	while (curr != nullptr)
	{
		if (i_ptr >= curr->blockStart && i_ptr < (static_cast<char*>(curr->blockStart) + curr->blockSize))
		{
			return true;
		}
		curr = curr->next;
	}
	return false;
}

size_t HeapManager::getLargestFreeBlock() const
{
	size_t size_t = 0;
	BlockDescriptor* currFreeBlock = freeBlockHead;

	while (currFreeBlock != nullptr)
	{
		if (size_t < currFreeBlock->blockSize)
		{
			size_t = currFreeBlock->blockSize;
		}
		currFreeBlock = currFreeBlock->next;
	}
	return size_t;
}

size_t HeapManager::getTotalFreeMemory() const
{
	size_t size_t = 0;
	BlockDescriptor* currFreeBlock = freeBlockHead;
	while (currFreeBlock != nullptr)
	{
		size_t += currFreeBlock->blockSize;
		currFreeBlock = currFreeBlock->next;
	}
	return size_t;
}

void HeapManager::ShowFreeBlocks() const
{
	BlockDescriptor* currFreeBlock = freeBlockHead;
	while (currFreeBlock != nullptr)
	{
		std::cout << currFreeBlock->blockStart << "\n";
		currFreeBlock = currFreeBlock->next;
	}
}

void HeapManager::ShowOutstandingAllocations() const
{
	BlockDescriptor* currOutStandingBlock = outstandingBlockHead;
	while (currOutStandingBlock != nullptr)
	{
		std::cout << currOutStandingBlock->blockStart << "\n";
		currOutStandingBlock = currOutStandingBlock->next;
	}
}
