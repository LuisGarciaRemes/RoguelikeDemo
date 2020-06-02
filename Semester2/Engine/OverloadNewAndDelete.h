#pragma once
#include "HeapManagerProxy.h"
#include "Windows.h"
#include "iostream"
#include "ConsolePrint.h"
#include <assert.h>
#include "HeapManager.h"

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void* i_ptr);
void operator delete[](void* i_ptr);
void *operator new(size_t i_size, size_t i_align);
void operator delete(void * i_ptr, size_t i_align);
void *operator new[](size_t i_size, size_t i_align);
void operator delete[](void* i_ptr, size_t i_align);
void *operator new(size_t size, HeapManager* heapManager);
void *operator new[](size_t size, HeapManager* heapManager);
void operator delete(void* i_ptr, HeapManager* heapManager);
void operator delete[](void* i_ptr, HeapManager* heapManager);
void *operator new(size_t i_size, size_t i_align, HeapManager* heapManager);
void operator delete(void * i_ptr, size_t i_align, HeapManager* heapManager);
void *operator new[](size_t i_size, size_t i_align, HeapManager* heapManager);
void operator delete[](void* i_ptr, size_t i_align, HeapManager* heapManager);