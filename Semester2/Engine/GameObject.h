#pragma once
#include "SmartPointer.h"
#include "Point2D.h"
#include "../Exports/GLib/GLib.h"

// This is our base class for a GameObject, something in our world that moves.
class GameObject
{
public:
	enum ObjectType{Player,Enemy,Projectile,Environment,None};

	GameObject(const Point2D& i_InitialPosition, GLib::Sprites::Sprite* i_sprite, ObjectType objType);

	GameObject(const Point2D& i_InitialPosition, GLib::Sprites::Sprite* i_sprite, ObjectType objType,const Point2D& i_dimensions);

	GameObject(const Point2D & i_InitialPosition, GLib::Sprites::Sprite * i_sprite, bool state, ObjectType objType);

	GameObject(const Point2D & i_InitialPosition, GLib::Sprites::Sprite * i_sprite, bool state, ObjectType objType, const Point2D& i_dimensions);

	GameObject();

	~GameObject();

	void UpdatePosition(const Point2D & i_Movement, float xBound, float yBound);

	void UpdatePosition(const Point2D & i_Movement);

	void UpdateVelocity(const Point2D & i_Direction);

	void UpdateAcceleration(const Point2D & i_acceleration);

	Point2D const GetPosition();

	Point2D const GetVelocity();

	Point2D const GetAcceleration();

	Point2D const GetDimensions();

	void SetMass(float mass);

	float GetMass();

	float GetZRot();

	void SetZRot(float i_zRot);

	bool GetState();

	void SetState(bool state);

	ObjectType GetType();

	GLib::Sprites::Sprite* GetSprite();

	void SetSprite(GLib::Sprites::Sprite* i_sprite);

	void ResetRigidBody()
	{
		m_Velocity = Point2D(0.0f, 0.0f);
		m_Acceleration = Point2D(0.0f, 0.0f);
	}

private:
	Point2D	m_Position;
	Point2D m_Velocity;
	Point2D m_Acceleration;
	Point2D m_Dimensions;
	float mass;
	float zRot;
	bool isEnabled;
	GLib::Sprites::Sprite* sprite;
	ObjectType type;
};

