#pragma once
#include <vector> 
#include "GameObject.h"
class Physics
{
public:
	static void ApplyForce(SmartPointer<GameObject>& object ,Point2D force);
	static void UpdatePhysics(SmartPointer<GameObject>& object,float friction, double dt, float xBound, float yBound);
	static void UpdatePhysics(std::vector<SmartPointer<GameObject>> objectList, float friction, double dt, float xBound, float yBound);
	static void ElasticCollision(SmartPointer<GameObject>& objectA, SmartPointer<GameObject>& objectB);
};

