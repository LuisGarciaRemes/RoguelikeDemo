#include "OverloadNewAndDelete.h"
#define PRINT_CALLED

const size_t 		sizeHeap = 1024 * 1024;
const unsigned int 	numDescriptors = 2048;
void * pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
HeapManager* testHeapManager = HeapManager::create(pHeapMemory, sizeHeap, numDescriptors);


// New
void *operator new(size_t size)
{
#ifdef PRINT_CALLED
	std::cout << "Overloaded new called\n";
#endif
	assert(size);
	assert(testHeapManager);
	return HeapManagerProxy::alloc(testHeapManager, size);

}

// New[]
void *operator new[](size_t size)
{
#ifdef PRINT_CALLED
	std::cout << "Overaloaded  new[] called\n";
#endif  
	assert(size);
	assert(testHeapManager);
	return HeapManagerProxy::alloc(testHeapManager, size);
}

// Delete
void operator delete(void* i_ptr)
{
#ifdef PRINT_CALLED
	std::cout << "Overaloaded delete called\n";
#endif 
	assert(i_ptr);
	assert(testHeapManager);
	HeapManagerProxy::free(testHeapManager, i_ptr);
}

// Delete[]
void operator delete[](void* i_ptr)
{
#ifdef PRINT_CALLED
	std::cout << "Overaloaded delete[] called\n";
#endif 

	HeapManagerProxy::free(testHeapManager, i_ptr);
}

// Aligned new
void *operator new(size_t i_size, size_t i_align)
{
#ifdef PRINT_CALLED
	std::cout << "Aligned new called\n";
#endif 
	return HeapManagerProxy::alloc(testHeapManager, i_size, i_align);
}

// Aligned delete
void operator delete(void * i_ptr, size_t i_align)
{
#ifdef PRINT_CALLED
	std::cout << "Aligned delete called\n";
#endif 
	HeapManagerProxy::free(testHeapManager, i_ptr);
}

// Aligned new
void *operator new[](size_t i_size, size_t i_align)
{
#ifdef PRINT_CALLED
	std::cout << "Aligned new[] called\n";
#endif 
	return HeapManagerProxy::alloc(testHeapManager, i_size, i_align);
}

// Aligned delete
void operator delete[](void* i_ptr, size_t i_align)
{
#ifdef PRINT_CALLED
	std::cout << "Aligned delete[] called\n";
#endif 
	HeapManagerProxy::free(testHeapManager, i_ptr);
}




//Overrides for using a specific heapmanager


// New
void *operator new(size_t size, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific new called\n";
#endif
	assert(size);
	assert(heapManager);
	return HeapManagerProxy::alloc(heapManager, size);

}

// New[]
void *operator new[](size_t size, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific new[] called\n";
#endif  
	assert(size);
	assert(heapManager);
	return HeapManagerProxy::alloc(heapManager, size);
}

// Delete
void operator delete(void* i_ptr, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific delete called\n";
#endif 
	assert(i_ptr);
	assert(heapManager);
	HeapManagerProxy::free(heapManager, i_ptr);
}

// Delete[]
void operator delete[](void* i_ptr, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specififc delete[] called\n";
#endif 

	HeapManagerProxy::free(heapManager, i_ptr);
}

// Aligned new
void *operator new(size_t i_size, size_t i_align, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific Aligned new called\n";
#endif 
	return HeapManagerProxy::alloc(heapManager, i_size, i_align);
}

// Aligned delete
void operator delete(void * i_ptr, size_t i_align, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific Aligned delete called\n";
#endif 
	HeapManagerProxy::free(heapManager, i_ptr);
}

// Aligned new
void *operator new[](size_t i_size, size_t i_align, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific Aligned new[] called\n";
#endif 
	return HeapManagerProxy::alloc(heapManager, i_size, i_align);
}

// Aligned delete
void operator delete[](void* i_ptr, size_t i_align, HeapManager* heapManager)
{
#ifdef PRINT_CALLED
	std::cout << "Specific Aligned delete[] called\n";
#endif 
	HeapManagerProxy::free(heapManager, i_ptr);
}