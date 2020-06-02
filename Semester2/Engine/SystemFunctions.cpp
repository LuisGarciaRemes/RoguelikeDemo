#include "SystemFunctions.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <Windows.h>
#include "../Lua/src/lua.hpp"
#include "Matrix4x4.h"
#include "ConsolePrint.h"
#include "RenderingSystem.h"

#include <iostream>
using namespace std;


void * SystemFunctions::LoadFile(const char * i_pFilename, size_t & o_sizeFile)
{
	assert(i_pFilename != NULL);

	FILE * pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

	assert(pFile != NULL);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	assert(FileIOError == 0);

	long FileSize = ftell(pFile);
	assert(FileSize >= 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	assert(FileIOError == 0);

	uint8_t * pBuffer = new uint8_t[FileSize];
	assert(pBuffer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	assert(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}


SmartPointer<GameObject> SystemFunctions::CreateGameObjectSmartPointer(const char * fileName)
{
	lua_State * luaState = luaL_newstate();
	assert(luaState);
	luaL_openlibs(luaState);

	char resultString[100];
	strcpy_s(resultString, "LuaData\\");
	strcat_s(resultString, fileName);

	size_t 		fileSize = 0;
	uint8_t * 	fileContents = reinterpret_cast<uint8_t *>(SystemFunctions::LoadFile(resultString, fileSize));

	if (fileContents  && fileSize)
	{
		int 		result = 0;
		result = luaL_loadbuffer(luaState, reinterpret_cast<char *>(fileContents), fileSize, nullptr);
		assert(result == 0);

		result = lua_pcall(luaState, 0, 0, 0);
		assert(result == 0);
	}

	int result = lua_getglobal(luaState, "GameObject");
	assert(resultString == LUA_TNIL || result == LUA_TTABLE);

	lua_pushstring(luaState, "ObjectType");
	int type = lua_gettable(luaState, -2);
	assert(type == LUA_TSTRING);
	string objTypeString = static_cast<string>(lua_tostring(luaState, -1));
	lua_pop(luaState, 1);

	GameObject::ObjectType objType;

	if (objTypeString._Equal("Player"))
	{
		objType = GameObject::ObjectType::Player;
	}
	else if (objTypeString._Equal("Projectile"))
	{
		objType = GameObject::ObjectType::Projectile;
	}
	else if (objTypeString._Equal("Enemy"))
	{
		objType = GameObject::ObjectType::Enemy;
	}
	else if (objTypeString._Equal("Environment"))
	{
		objType = GameObject::ObjectType::Environment;
	}
	else
	{
		objType = GameObject::ObjectType::None;
	}

	lua_pushstring(luaState, "xPos");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float xPos = static_cast<float>(lua_tonumber(luaState, -1))*32;	
	lua_pop(luaState, 1);

	lua_pushstring(luaState, "yPos");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float yPos = static_cast<float>(lua_tonumber(luaState, -1))*32;
	lua_pop(luaState, 1);

	lua_pushstring(luaState, "width");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float width = static_cast<float>(lua_tonumber(luaState, -1));
	lua_pop(luaState, 1);

	lua_pushstring(luaState, "height");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float height = static_cast<float>(lua_tonumber(luaState, -1));
	lua_pop(luaState, 1);

	lua_pushstring(luaState, "sprite");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TSTRING);
	string spriteAddress = static_cast<string>(lua_tostring(luaState, -1));
	lua_pop(luaState, 1);

	Point2D objectPos(xPos, yPos);

	delete fileContents;
	lua_close(luaState);

	// declaring character array 
	char char_array[100];

	// copying the contents of the 
	// string to char array 
	strcpy_s(char_array, spriteAddress.c_str());

	Point2D dimensions(width, height);
	GLib::Sprites::Sprite* tempSprite = RenderingSystem::CreateSprite(char_array);

	return SmartPointer<GameObject>(new GameObject(objectPos, tempSprite, objType,dimensions));
}

std::vector<SmartPointer<GameObject>> SystemFunctions::CreateMulitpleGameObjectSmartPointers(const char * fileName)
{
	lua_State * luaState = luaL_newstate();
	assert(luaState);
	luaL_openlibs(luaState);

	char resultString[100];
	strcpy_s(resultString, "LuaData\\");
	strcat_s(resultString, fileName);

	size_t 		fileSize = 0;
	uint8_t * 	fileContents = reinterpret_cast<uint8_t *>(SystemFunctions::LoadFile(resultString, fileSize));

	if (fileContents  && fileSize)
	{
		int 		result = 0;
		result = luaL_loadbuffer(luaState, reinterpret_cast<char *>(fileContents), fileSize, nullptr);
		assert(result == 0);

		result = lua_pcall(luaState, 0, 0, 0);
		assert(result == 0);
	}

	int result = lua_getglobal(luaState, "GameObject");
	assert(resultString == LUA_TNIL || result == LUA_TTABLE);

	lua_pushstring(luaState, "numOfObjects");
	int type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	int numObjects = static_cast<int>(lua_tonumber(luaState, -1));
	lua_pop(luaState, 1);

	lua_pushstring(luaState, "ObjectType");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TSTRING);
	string objTypeString = static_cast<string>(lua_tostring(luaState, -1));
	lua_pop(luaState, 1);

	GameObject::ObjectType objType;

	if (objTypeString._Equal("Player"))
	{
		objType = GameObject::ObjectType::Player;
	}
	else if (objTypeString._Equal("Projectile"))
	{
		objType = GameObject::ObjectType::Projectile;
	}
	else if (objTypeString._Equal("Enemy"))
	{
		objType = GameObject::ObjectType::Enemy;
	}
	else if (objTypeString._Equal("Environment"))
	{
		objType = GameObject::ObjectType::Environment;
	}
	else
	{
		objType = GameObject::ObjectType::None;
	}

	lua_pushstring(luaState, "initialState");
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TSTRING);
	string isEnabled = static_cast<string>(lua_tostring(luaState, -1));
	lua_pop(luaState, 1);

	bool state = false;

	if (isEnabled._Equal("true"))
	{
		state = true;
	}

	std::vector<SmartPointer<GameObject>> temp;
	for(int i = 0; i < 0 + numObjects;i++)
	{
	char lineToRead[10];
	char letter = 'A' + i;

	strcpy_s(lineToRead, "xPos ");
	lineToRead[4] = letter;
	
	lua_pushstring(luaState, lineToRead);
	int type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float xPos = static_cast<float>(lua_tonumber(luaState, -1))*32;
	lua_pop(luaState, 1);

	strcpy_s(lineToRead, "yPos ");
	lineToRead[4] = letter;

	lua_pushstring(luaState, lineToRead);
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float yPos = static_cast<float>(lua_tonumber(luaState, -1))*32;
	lua_pop(luaState, 1);

	strcpy_s(lineToRead, "width ");
	lineToRead[5] = letter;

	lua_pushstring(luaState, lineToRead);
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float width = static_cast<float>(lua_tonumber(luaState, -1));
	lua_pop(luaState, 1);

	strcpy_s(lineToRead, "height ");
	lineToRead[6] = letter;

	lua_pushstring(luaState, lineToRead);
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TNUMBER);
	float height = static_cast<float>(lua_tonumber(luaState, -1));
	lua_pop(luaState, 1);

	strcpy_s(lineToRead, "sprite ");
	lineToRead[6] = letter;

	lua_pushstring(luaState, lineToRead);
	type = lua_gettable(luaState, -2);
	assert(type == LUA_TSTRING);
	string spriteAddress = static_cast<string>(lua_tostring(luaState, -1));
	lua_pop(luaState, 1);

	Point2D objectPos(xPos, yPos);

	// declaring character array 
	char char_array[100];

	// copying the contents of the 
	// string to char array 
	strcpy_s(char_array, spriteAddress.c_str());

	Point2D dimensions(width, height);
	GLib::Sprites::Sprite* tempSprite = RenderingSystem::CreateSprite(char_array);
	
	temp.push_back(SmartPointer<GameObject>(new GameObject(objectPos, tempSprite,state, objType, dimensions)));
	}

	delete fileContents;
	lua_close(luaState);

	return temp;
}

void SystemFunctions::TestMatrixClass()
{
	//Test matrix mult
	Matrix4x4 mat1 = Matrix4x4(Point4D(5,4,2,1), Point4D(0, 1, -1, -1), Point4D(-1, -1, 3, 0), Point4D(1, 1, -1, 2));
	Matrix4x4 mat2 = Matrix4x4(Point4D(4, 2, 3, 1), Point4D(3, 2, 6, 10), Point4D(1, 6, 5, 4), Point4D(8 ,7, 3, 9));
	Matrix4x4 resultMatrix;

	resultMatrix = mat1*mat2;

	for (int i = 0; i < 4; i++)
	{	
		cout << resultMatrix[i][0] << "," << resultMatrix[i][1] << "," << resultMatrix[i][2] << "," << resultMatrix[i][3];
	}
	 
	//Test vextor mult
	Point4D vect1 = Point4D(1,2,3,4);
	Point4D resultVect;
	resultVect = mat1 * vect1;

	for (int i = 0; i < 4; i++)
	{
		cout << resultVect[0] << "," << resultVect[1] << "," << resultVect[2] << "," << resultVect[3];
	}

	//Test transpose and test undo after the second transpose
	resultMatrix = mat1.Transpose();

	for (int i = 0; i < 4; i++)
	{
		cout << resultMatrix[i][0] << "," << resultMatrix[i][1] << "," << resultMatrix[i][2] << "," << resultMatrix[i][3];
	}

	resultMatrix = mat1.Transpose();

	for (int i = 0; i < 4; i++)
	{
		cout << resultMatrix[i][0] << "," << resultMatrix[i][1] << "," << resultMatrix[i][2] << "," << resultMatrix[i][3];
	}

	//Test determinant
	float det = mat2.Determinant();
	cout << det;

	//Test adjugate matrix
	resultMatrix = mat1.Adjugate();
	for (int i = 0; i < 4; i++)
	{
		cout << resultMatrix[i][0] << "," << resultMatrix[i][1] << "," << resultMatrix[i][2] << "," << resultMatrix[i][3];
	}

	//Test inverse matrix
	resultMatrix = mat1.Inverse();
	for (int i = 0; i < 4; i++)
	{
		cout << resultMatrix[i][0] << "," << resultMatrix[i][1] << "," << resultMatrix[i][2] << "," << resultMatrix[i][3];
	}
}
