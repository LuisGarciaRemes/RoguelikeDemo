#pragma once
#include "Matrix4x4.h"
#include "SmartPointer.h"
#include "GameObject.h"
#include "Point2D.h"
#include "Physics.h"
#include <vector>
#include "ConsolePrint.h"
#include <cstdlib> 
#include<time.h>
#include "Timing.h"
#include<random>

class Collider
{
	//box collider
	struct BoxCollider
	{
		Point2D origin;
		float width;
		float height;
		float zRot;
	};

private:
	WeakPointer<GameObject> gameObject;
	BoxCollider col;


public:
	Collider(SmartPointer<GameObject> i_gameObject,float i_width, float i_height)
	{
		gameObject = i_gameObject;
		//The origin of the collider is not equal to the origin point of the sprite which is at the mid bottom position. Setting the boxes origin to this lets the two match.
		col.origin = Point2D(i_gameObject->GetPosition().x()-i_width/2, i_gameObject->GetPosition().y());
		col.width = i_width;
		col.height = i_height;
		col.zRot = i_gameObject->GetZRot();
	}

	Collider()
	{
		gameObject = nullptr;
		//The origin of the collider is not equal to the origin point of the sprite which is at the mid bottom position. Setting the boxes origin to this lets the two match.
		col.origin = Point2D(0.0f,0.0f);
		col.width = 0.0f;
		col.height = 0.0f;
		col.zRot = 0.0f;
	}

	~Collider()
	{
		
	}

	//Checks if an object is colliding with another object
	bool CheckCollisions(const Collider & other ,double& dT)
	{	
		Matrix4x4 bTrans = Matrix4x4::TranslationTransform(other.col.origin.x(), other.col.origin.y(),0.0f);
		Matrix4x4 bRots = Matrix4x4::ZAxisRotationTransform(other.col.zRot);
		Matrix4x4 aTrans = Matrix4x4::TranslationTransform(col.origin.x(), other.col.origin.y(), 0.0f);
		Matrix4x4 aRots = Matrix4x4::ZAxisRotationTransform(col.zRot);

		Matrix4x4 bToWorld = bTrans * bRots;
		Matrix4x4 worldToB = bToWorld.Inverse();

		Matrix4x4 aToWorld = aTrans * aRots;
		Matrix4x4 worldToA = aToWorld.Inverse();

		Matrix4x4 aToB = worldToB * aToWorld;
		Matrix4x4 bToA = worldToA * bToWorld;

		//A projects to B

		Point4D temp;
		temp = Point4D(col.origin.x(), col.origin.y(), 0.0f, 1.0f);
		Point4D aOriginToB = aToB * temp;
		temp = Point4D(col.width, 0.0f, 0.0f, 0.0f);
		Point4D aWidthToB = aToB * temp;
		temp = Point4D(0.0f, col.height, 0.0f, 0.0f);
		Point4D aHeightToB = aToB * temp;


		if (aOriginToB[0] + col.width*2 < other.col.origin.x() || aOriginToB[0] > other.col.origin.x() + other.col.width*2)
		{
			return false;
		}

		if (aOriginToB[1] + col.height < other.col.origin.y() || aOriginToB[1] > other.col.origin.y() + other.col.height)
		{
			return false;
		}

		//B projects to A
		temp = Point4D(other.col.origin.x(), other.col.origin.y(), 0.0f, 1.0f);
		Point4D bOriginToA = bToA * temp; 
		temp = Point4D(other.col.width, 0.0f, 0.0f, 0.0f);
		Point4D bWidthToA = bToA * temp;
		temp = Point4D(0.0f, other.col.height, 0.0f, 0.0f);
		Point4D bHeightToA = bToA * temp;

		if (bOriginToA[0] + other.col.width*2 < col.origin.x() || bOriginToA[0] > col.origin.x() + col.width*2)
		{
			return false;
		}


		if (bOriginToA[1] + other.col.height < col.origin.y() || bOriginToA[1] > col.origin.y() + col.height)
		{
			return false;
		}
	
		return true;
	}

	//returns the gameobject attached to the collider
	SmartPointer<GameObject> GetGameObject()
	{
		return gameObject.CreateSmartPointer();
	}

	//updates the position of the collider
	void UpdateOrigin(Point2D origin)
	{
		col.origin = origin;
	}

	//Returns the position of the collider
	Point2D GetOrigin()
	{
		return col.origin;
	}

	//gets the width of the collider
	float GetWidth()
	{
		return col.width;
	}

	//gets the height of the collider
	float GetHeight()
	{
		return col.height;
	}

};

namespace CollisionsSys
{
	//updates the position of the colliders and checks for collisions
	void Update(std::vector<Collider> colliderList, double dT)
	{
		for (auto itA = colliderList.begin(); itA != colliderList.end(); itA++)
		{
				SmartPointer<GameObject> objA = (itA)->GetGameObject();
				(itA)->UpdateOrigin(Point2D(objA->GetPosition().x() - (itA)->GetWidth() / 2, objA->GetPosition().y()));

			for (auto itB = itA + 1; itB != colliderList.end(); itB++)
			{
				SmartPointer<GameObject>  objB = (itB)->GetGameObject();
				(itB)->UpdateOrigin(Point2D(objB->GetPosition().x() - (itB)->GetWidth() / 2, objB->GetPosition().y()));

				if (objA->GetState() && objB->GetState()) {		

					if ((objB->GetType() == GameObject::ObjectType::Environment) && (objA->GetType() != GameObject::ObjectType::Environment) && (objA->GetType() != GameObject::ObjectType::Player))
					{
						if ((itA)->CheckCollisions(*(itB), dT))
						{
							Physics::ElasticCollision(objA, objB);
							(itA)->UpdateOrigin(Point2D(objA->GetPosition().x() - (itA)->GetWidth() / 2, objA->GetPosition().y()));
							(itB)->UpdateOrigin(Point2D(objB->GetPosition().x() - (itB)->GetWidth() / 2, objB->GetPosition().y()));
						}
					}
					else if ((objA->GetType() == GameObject::ObjectType::Player && objB->GetType() == GameObject::ObjectType::Enemy))
					{
						if ((itA)->CheckCollisions(*(itB), dT))
						{
								objA->SetState(false);
								objB->SetState(false);
						}
					}
					else if (((objB->GetType() == GameObject::ObjectType::Player && objA->GetType() == GameObject::ObjectType::Projectile) && (abs(objA->GetAcceleration().x()) <= 0.0f && abs(objA->GetAcceleration().y()) <= 0.0f)))
					{
						if ((itA)->CheckCollisions(*(itB), dT))
						{
								objA->SetState(false);
						}
					}
					else if ((objB->GetType() == GameObject::ObjectType::Enemy && objA->GetType() == GameObject::ObjectType::Projectile))
					{
						if ((itA)->CheckCollisions(*(itB), dT))
						{
							objB->SetState(false);
						}
					}
					else if (objB->GetType() == GameObject::ObjectType::Environment && objA->GetType() == GameObject::ObjectType::Player)
					{
						if ((itA)->CheckCollisions(*(itB), dT))
						{
						objA->SetState(false);
						}
					}
				}
			}
		}
	}

	Point2D GenerateRandomVector(Point2D posMin, Point2D posMax)
	{
		return Point2D(fmod(rand(), (posMax.x() + 1 - posMin.x()) + posMin.x()), fmod(rand(), (posMax.y() + 1 - posMax.y()) + posMax.y()));
	}

	//test of collision system
	void CollisionPoolCheck()
	{
		Timing timing = Timing::Timing();
		double dT = 0;
		std::vector<Collider*> colliderList;
		const float AreaExtent = 10.0f;
		Point2D PositionMin(-AreaExtent, -AreaExtent);
		Point2D PositionMax(AreaExtent, AreaExtent);

		Point2D VelocityMin(-1.0f, -1.0f);
		Point2D VelocityMax(1.0f, 1.0f);

		const size_t numObjects = 100;

		srand(time(0));

		for (size_t i = 0; i < numObjects; i++)
		{
			SmartPointer<GameObject> pObj = SmartPointer<GameObject>(new GameObject());

			pObj->UpdatePosition(GenerateRandomVector(PositionMin, PositionMax));
			pObj->UpdateVelocity(GenerateRandomVector(VelocityMin, VelocityMax));

			DEBUG_PRINT("ObjectA at postion (%f)", pObj->GetPosition().x());

			float randNum = fmod(rand(),(90.0f + 1 - 0.0f) + 0.0f);

			pObj->SetZRot(randNum);
			
			colliderList.push_back(&Collider(pObj, 32.0f, 32.0f));
		}

		timing.RunTimer(dT);
		while (dT > 1.0) {
			timing.RunTimer(dT);
		}

		while (1)
		{
			timing.RunTimer(dT);

			for (auto itA = colliderList.begin(); itA != colliderList.end(); itA++)
			{
				SmartPointer<GameObject> objA = (*itA)->GetGameObject();
				objA->UpdatePosition(Point2D(objA->GetPosition().x() + objA->GetVelocity().x(), objA->GetPosition().y() + objA->GetVelocity().y()));
			}

			for (auto itA = colliderList.begin(); itA != colliderList.end(); itA++)
			{
				SmartPointer<GameObject> objA = (*itA)->GetGameObject();
				for (auto itB = itA + 1; itB != colliderList.end(); itB++)
				{
					SmartPointer<GameObject> objB = (*itB)->GetGameObject();
					if ((*itA)->CheckCollisions(*(*itB), dT))
					{
						DEBUG_PRINT("ObjectA at postion (%f,%f) collided with ObjectB at position (%f,%f) at time: %f. Object w",objA->GetPosition().x(), objA->GetPosition().y(), objB->GetPosition().x(), objB->GetPosition().y(), dT);
					}
				}
			}		
		}
	}
}



