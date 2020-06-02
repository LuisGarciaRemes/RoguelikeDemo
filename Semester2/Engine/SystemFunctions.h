#pragma once
#include "GameObject.h"
#include <vector>
namespace SystemFunctions {

	void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);
	SmartPointer<GameObject> CreateGameObjectSmartPointer(const char * fileName);
	std::vector<SmartPointer<GameObject>>CreateMulitpleGameObjectSmartPointers(const char * fileName);
	void TestMatrixClass();
 }
