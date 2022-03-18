//-------------------------------------------------------------------------------------------------------------
// objects.h
//
// Created: JohnL
//
// Game objects.
//
// Copyright(c) Ninja Theory 2010. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------------------
#include "ntpoint.h"

//-------------------------------------------------------------------------------------------------------------
// CelestialBody
// The base class for all game objects.
//-------------------------------------------------------------------------------------------------------------
class CelestialBody
{
public:
	CelestialBody() {}
	CelestialBody(const NTPoint& position);

	virtual void Update() = 0;
	virtual void Draw(HDC hdc) = 0;

	NTPoint GetPosition() { return m_Position; }
	void ApplyTheGravityFromSuns(const std::list<Sun*>& AllSuns);

	NTPoint m_Position;
	NTPoint m_Velocity;
};


//-------------------------------------------------------------------------------------------------------------
// Sun
// A star, a simple unmoving object.
//-------------------------------------------------------------------------------------------------------------
class Sun : public CelestialBody
{
public:
	Sun(int x, int y);
	virtual void Update() {}
	virtual void Draw(HDC hdc);
	NTPoint GetGravityOfOutsidePoint(const NTPoint& point);

	static const int RADIUS;
	static const int GRAVITY;
};


//-------------------------------------------------------------------------------------------------------------
// Missile
// Fired by a player ship.
//-------------------------------------------------------------------------------------------------------------
class Missile : public CelestialBody
{
public:
	Missile(NTPoint& FromPosition, NTPoint& ToPosition);

	virtual void Update();
	virtual void Draw(HDC hdc);

	bool IsOutOfFuel()
	{
		return m_ShotTime < 0.f;
	}

private:
	float m_ShotTime;
};

//-------------------------------------------------------------------------------------------------------------
// Ship
// Controlled by the player.
//-------------------------------------------------------------------------------------------------------------
class Ship : public CelestialBody
{
public:
	Ship();

	virtual void Update();
	virtual void Draw(HDC hdc);

	void Explode();

	float m_Angle;
	float m_TimeSinceLastShot;

};
