#include "Physics.h"
#include "ConsolePrint.h"

//applies force to the passed object.

void Physics::ApplyForce(SmartPointer<GameObject> & object, Point2D force)
{
	object->UpdateAcceleration(Point2D((force.x() / object->GetMass()), (force.y() / object->GetMass())));
}

//Updates the physics of the given object
void Physics::UpdatePhysics(SmartPointer<GameObject> & object, float friction, double dt, float xBound, float yBound)
{
	float accx =0;
	float accy=0;
	float xVel=0;
	float yVel=0;


		if (abs(object->GetAcceleration().x()) > abs(friction*9.81))
		{
			accx = (object->GetAcceleration().x() / object->GetMass()) + friction * (-1 * object->GetAcceleration().x() / abs(object->GetAcceleration().x()))*(9.81);
			xVel = object->GetVelocity().x() + accx * dt;
		}

		if (abs(object->GetAcceleration().y()) > abs(friction*9.81))
		{
			accy = (object->GetAcceleration().y() / object->GetMass()) + friction * (-1 * object->GetAcceleration().y() / abs(object->GetAcceleration().y()))*(9.81);
			yVel = object->GetVelocity().y() + accy * dt;
		}

	object->UpdateVelocity(Point2D(xVel, yVel));

	float xPos = object->GetPosition().x() + object->GetVelocity().x()*dt;

	float yPos = object->GetPosition().y() + object->GetVelocity().y()*dt;

	object->UpdateAcceleration(Point2D(accx, accy));
	object->UpdatePosition(Point2D(xPos, yPos), xBound, yBound);
}

//Updates the physics of multiple given objects
void Physics::UpdatePhysics(std::vector<SmartPointer<GameObject>> objectList, float friction, double dt, float xBound, float yBound)
{
	for (auto itA = objectList.begin(); itA != objectList.end(); itA++)
	{
		if ((*itA)->GetState())
		{
			Physics::UpdatePhysics((*itA), friction, dt, xBound, yBound);
		}
	}
}

//Applies an ellastic collision betweeen the two objects given
void Physics::ElasticCollision(SmartPointer<GameObject>& objectA, SmartPointer<GameObject>& objectB)
{
	float objAVelX = (((((objectA->GetMass() - objectB->GetMass()) / (objectA->GetMass()) + objectB->GetMass()))*objectA->GetVelocity().x()) + (((objectB->GetMass() * 2) / (objectA->GetMass() + objectB->GetMass()))*objectB->GetVelocity().x()));
	float objAVelY = (((((objectA->GetMass() - objectB->GetMass())) / (objectA->GetMass() + objectB->GetMass()))*objectA->GetVelocity().y()) + (((objectB->GetMass() * 2) / (objectA->GetMass() + objectB->GetMass()))*objectB->GetVelocity().y()));

	float objBVelX = ((objectA->GetMass()*2)/(objectA->GetMass() + objectB->GetMass()))*objectA->GetVelocity().x() + ((objectB->GetMass() - objectA->GetMass()) / (objectA->GetMass() + objectB->GetMass()))*objectB->GetVelocity().x();
	float objBVelY = ((objectA->GetMass()*2)/(objectA->GetMass() + objectB->GetMass()))*objectA->GetVelocity().y() + ((objectB->GetMass() - objectA->GetMass()) / (objectA->GetMass() + objectB->GetMass()))*objectB->GetVelocity().y();

	if (objectB->GetType() == objectB->Environment)
	{
		float posX = objectA->GetPosition().x();
		float posY = objectA->GetPosition().y();

		if (objectA->GetPosition().x() < objectB->GetPosition().x())
		{
			posX += - 7.0;
		}
		else if(objectA->GetPosition().x()> objectB->GetPosition().x())
		{
			posX +=7.0;
		}

		if (objectA->GetPosition().y() + objectA->GetDimensions().y()/2 < objectB->GetPosition().y() + objectA->GetDimensions().y() / 2)
		{
			posY += - 7.0;
		}
		else if (objectA->GetPosition().y() + objectA->GetDimensions().y() / 2 > objectB->GetPosition().y() + objectA->GetDimensions().y() / 2)
		{
			posY += 7.0;
		}

		objectA->UpdatePosition(Point2D(posX, posY));
	}

	objectA->UpdateVelocity(Point2D(objAVelX,objAVelY));
	objectB->UpdateVelocity(Point2D(objBVelX,objBVelY));
}

