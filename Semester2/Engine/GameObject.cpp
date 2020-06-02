#include "GameObject.h"
#include <math.h>;

GameObject::GameObject(const Point2D & i_InitialPosition, GLib::Sprites::Sprite * i_sprite, ObjectType objType)
{
	m_Position = i_InitialPosition;
	m_Velocity = Point2D(0.0f, 0.0f);
	m_Acceleration = Point2D(0.0f, 0.0f);
	zRot = 0.0f;
	isEnabled = true;
	sprite = i_sprite;
	type = objType;
	
	if (objType == Environment)
	{
		mass = 10000.0f;
	}
	else if (objType == Projectile)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 50.0f;
	}
}

GameObject::GameObject(const Point2D & i_InitialPosition, GLib::Sprites::Sprite * i_sprite, ObjectType objType, const Point2D& i_dimensions)
{
	m_Position = i_InitialPosition;
	m_Velocity = Point2D(0.0f, 0.0f);
	m_Acceleration = Point2D(0.0f, 0.0f);
	m_Dimensions = i_dimensions;
	zRot = 0.0f;
	isEnabled = true;
	sprite = i_sprite;
	type = objType;

	if (objType == Environment)
	{
		mass = 10000.0f;
	}
	else if (objType == Projectile)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 50.0f;
	}
}

GameObject::GameObject(const Point2D & i_InitialPosition, GLib::Sprites::Sprite * i_sprite, bool state, ObjectType objType)
{
	m_Position = i_InitialPosition;
	m_Velocity = Point2D(0.0f, 0.0f);
	m_Acceleration = Point2D(0.0f, 0.0f);
	zRot = 0.0f;
	isEnabled = state;
	sprite = i_sprite;
	type = objType;

	if (objType == Environment)
	{
		mass = 10000.0f;
	}
	else if (objType == Projectile)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 50.0f;
	}
}


GameObject::GameObject(const Point2D & i_InitialPosition, GLib::Sprites::Sprite * i_sprite, bool state, ObjectType objType, const Point2D & i_dimensions)
{
	m_Position = i_InitialPosition;
	m_Velocity = Point2D(0.0f, 0.0f);
	m_Acceleration = Point2D(0.0f, 0.0f);
	zRot = 0.0f;
	isEnabled = state;
	sprite = i_sprite;
	type = objType;
	m_Dimensions = i_dimensions;

	if (objType == Environment)
	{
		mass = 10000.0f;
	}
	else if (objType == Projectile)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 50.0f;
	}
}

GameObject::GameObject(){
	m_Position = Point2D(0.0f,0.0f);
	mass = 10.0f;
	zRot = 0.0f;
	isEnabled = true;
	sprite = nullptr;
}

GameObject::~GameObject()
{
}


//Updates the position of the gameobject with bounds
void GameObject::UpdatePosition(const Point2D & i_Movement, float xBound, float yBound)
{
	float x = 0.0f;
	float y = 0.0f;
	float Offset = 64;

	if (i_Movement.x() < xBound/2 - m_Dimensions.x()/2 && i_Movement.x() > -xBound / 2 + m_Dimensions.x()/2)
	{
		x = i_Movement.x();
	}
	else
	{
		m_Acceleration.x(0.0f);
		m_Velocity.x(0.0f);
		x = m_Position.x() - 1 * (m_Position.x()/ fabs(m_Position.x()));
	}

	if (i_Movement.y() < yBound/2 - m_Dimensions.y() && i_Movement.y() > -yBound / 2 +Offset)
	{
		y = i_Movement.y();
	}
	else
	{
		m_Acceleration.y(0.0f);
		m_Velocity.y(0.0f);
		y = m_Position.y() - 1 * (m_Position.y() / fabs(m_Position.y()));
	}

	m_Position = Point2D(x,y);
}


//Updates the position of the gameobject without bounds
void GameObject::UpdatePosition(const Point2D & i_Movement)
{
	m_Position = i_Movement;
}

//Updates the veloctiy of the gameobject
void GameObject::UpdateVelocity(const Point2D & i_Velocity)
{
	m_Velocity = i_Velocity;
}

//Updates the acceleration of the gameobject
void GameObject::UpdateAcceleration(const Point2D & i_acceleration)
{
	m_Acceleration = i_acceleration;
}

//returns position
Point2D const GameObject::GetPosition()
{
	return m_Position;
}

//returns veloctiy
Point2D const GameObject::GetVelocity()
{
	return m_Velocity;
}

//returns acceleration
Point2D const GameObject::GetAcceleration()
{
	return m_Acceleration;
}

//returns the dimentions of the sprite of the gameobject
Point2D const GameObject::GetDimensions()
{
	return m_Dimensions;
}
//sets the mass of the object
void GameObject::SetMass(float i_mass)
{
	mass = i_mass;
}

//gets the mass of the object
float GameObject::GetMass()
{
	return mass;
}

//gets the rotaion in the z-axis of the object
float GameObject::GetZRot()
{
	return zRot;
}

//sets the rotaion in the z-axis of the object
void GameObject::SetZRot(float i_zRot)
{
	zRot = i_zRot;
}

//gets the state of an object - dead or alive
bool GameObject::GetState()
{
	return isEnabled;
}

//sets the state of an object - dead or alive
void GameObject::SetState(bool state)
{
	isEnabled = state;
}

//gets type of the gameobject
GameObject::ObjectType GameObject::GetType()
{
	return type;
}

//gets the sprites of a gameobject
GLib::Sprites::Sprite * GameObject::GetSprite()
{
	return sprite;
}

//sets the sprites of a gameobject
void GameObject::SetSprite(GLib::Sprites::Sprite * i_sprite)
{
	sprite = i_sprite;
}


