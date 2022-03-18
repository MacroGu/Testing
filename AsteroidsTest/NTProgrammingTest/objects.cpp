//-------------------------------------------------------------------------------------------------------------
// objects.cpp
//
// Created: JohnL
//
// Implementation of game objects.
//
// Copyright(c) Ninja Theory 2010. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "game.h"
#include "objects.h"
#include "timer.h"
#include <cmath>

//--------------------------------------------------------------------------------------------------------------
// CelestialBody
//--------------------------------------------------------------------------------------------------------------
CelestialBody::CelestialBody(const NTPoint& position)
: m_Position(position)
{
}

//--------------------------------------------------------------------------------------------------------------
// CelestialBody
// calculate the gravity of all suns and apply it to velocity
//--------------------------------------------------------------------------------------------------------------
void CelestialBody::ApplyTheGravityFromSuns(const std::list<Sun*>& AllSuns)
{
	NTPoint resultGravity(0.f, 0.f);

	for (auto OneSun : AllSuns)
	{
		NTPoint OneSunGravity = OneSun->GetGravityOfOutsidePoint(m_Position);
		resultGravity = resultGravity + OneSunGravity;
	}

	m_Velocity = m_Velocity + resultGravity;
}

const int Sun::RADIUS = 15;
const int Sun::GRAVITY = 1;

//--------------------------------------------------------------------------------------------------------------
// Sun
// Construct a sun.  Let there be light.
//--------------------------------------------------------------------------------------------------------------
Sun::Sun(int x, int y)
: CelestialBody(NTPoint((float)x,(float)y))
{
}

//--------------------------------------------------------------------------------------------------------------
// Draw
// Draw the sun.
//--------------------------------------------------------------------------------------------------------------
void Sun::Draw(HDC hdc)
{
	Ellipse(hdc, (int)m_Position.x - RADIUS, (int)m_Position.y - RADIUS, (int)m_Position.x + RADIUS, (int)m_Position.y + RADIUS);
}

//--------------------------------------------------------------------------------------------------------------
// Gravity
// calculate the gravity for the point outside the sun
//--------------------------------------------------------------------------------------------------------------
NTPoint Sun::GetGravityOfOutsidePoint(const NTPoint& point)
{
	NTPoint targetVector = m_Position - point;
	float distance = targetVector.GetLength();
	targetVector.Normalise();
	// the distance if bigger , the gravity is smaller
	return targetVector * GRAVITY / distance;
}



//--------------------------------------------------------------------------------------------------------------
// Missile
// Constructs a missile. Fired from FromPosition at ToPosition.
//--------------------------------------------------------------------------------------------------------------
Missile::Missile(NTPoint& FromPosition, NTPoint& ToPosition)
{
	m_Position = FromPosition;
	m_Velocity = ToPosition - FromPosition;
	m_Velocity.Normalise();
	m_Velocity = m_Velocity * 300.f;
	m_ShotTime = 10.f;
}

//--------------------------------------------------------------------------------------------------------------
// Update
// Updates the position & velocity.
//--------------------------------------------------------------------------------------------------------------
void Missile::Update()
{
	float timeDelta = g_Game.m_Timer.GetTimeDelta();

	m_ShotTime -= timeDelta;

	float speed = m_Velocity.GetLength();
	if (speed > 500.f)
	{
		m_Velocity.Normalise();
		m_Velocity = m_Velocity * 500.f;
	}
	else if (speed < 50.f && speed != 0.f)
	{
		m_Velocity.Normalise();
		m_Velocity = m_Velocity * 50.f;
	}

	m_Position = m_Position + m_Velocity * timeDelta;
}

//--------------------------------------------------------------------------------------------------------------
// Draw
// Draws a missile.
//--------------------------------------------------------------------------------------------------------------
void Missile::Draw(HDC hdc)
{
	Ellipse(hdc, (int)m_Position.x - 2, (int)m_Position.y - 2, (int)m_Position.x + 2, (int)m_Position.y + 2);
}

//--------------------------------------------------------------------------------------------------------------
// Ship
// Constructs a player ship.
//--------------------------------------------------------------------------------------------------------------
Ship::Ship()
: CelestialBody(NTPoint(250.f, 250.f))
, m_Angle(0.f)
, m_TimeSinceLastShot(10.f)
{
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;
}

//--------------------------------------------------------------------------------------------------------------
// Update
// Update for a player ship.
//--------------------------------------------------------------------------------------------------------------
void Ship::Update()
{
	float timeDelta = g_Game.m_Timer.GetTimeDelta();

	bool bCollision = false;

	if (GetKeyState(VK_LEFT) & 0x800)
	{
		m_Angle += timeDelta * 3.14f;
		if (m_Angle > 3.14f) m_Angle -= 3.14f * 2.f;
	}
	if (GetKeyState(VK_RIGHT) & 0x800)
	{
		m_Angle -= timeDelta * 3.14f;
		if (m_Angle < -3.14f) m_Angle += 3.14f * 2.f;
	}
	
	if (GetKeyState(VK_UP) & 0x800)
	{
		NTPoint pt(1.f * sinf(m_Angle), 1.f * cosf(m_Angle));
		m_Velocity = m_Velocity + pt * 40.f * timeDelta;
	}
	if (GetKeyState(VK_DOWN) & 0x800)
	{
		NTPoint pt(-1.f * sinf(m_Angle), -1.f * cosf(m_Angle));
		m_Velocity = m_Velocity + pt * 40.f * timeDelta;
	}

	if (m_TimeSinceLastShot < 0.5f)
	{
		m_TimeSinceLastShot += timeDelta;
	}
	else if (GetKeyState(' ') & 0x800)
	{
		NTPoint pt(1.f * sinf(m_Angle), 1.f * cosf(m_Angle));
		g_Game.m_Missiles.push_back(new Missile(m_Position + pt * 10.f, m_Position + pt * 20.f));
		m_TimeSinceLastShot = 0.f;
	}

	for (std::list<Missile*>::iterator itMissile = g_Game.m_Missiles.begin(); 
		itMissile != g_Game.m_Missiles.end(); itMissile++)
	{
		NTPoint pt = (*itMissile)->GetPosition();
		NTPoint dir = pt - m_Position;
		float dist = dir.GetLength();
		if (dist < 3.f)
		{
			bCollision = true;
		}
	}

	for (std::list<Sun*>::iterator itSun = g_Game.m_Suns.begin(); 
		itSun != g_Game.m_Suns.end(); itSun++)
	{
		NTPoint pt = (*itSun)->GetPosition();
		NTPoint dir = pt - m_Position;
		float dist = dir.GetLength();
		if (dist < 15.f)
		{
			bCollision = true;
		}
	}

	if (bCollision)
	{
		Explode();
	}

	float speed = m_Velocity.GetLength();
	if (fabsf(speed) > 50.f)
	{
		m_Velocity.Normalise();
		m_Velocity = m_Velocity * 50.f;
	}

	m_Position = m_Position + m_Velocity * timeDelta;
}


//--------------------------------------------------------------------------------------------------------------
// Draw
// Draw a player ship.
//--------------------------------------------------------------------------------------------------------------
void Ship::Draw(HDC hdc)
{
	const int iLong = 12;
	const int iShort = 4;

	int aiPoints[4][2] =
	{
		{(int)(m_Position.x),									(int)(m_Position.y)},
		{(int)(m_Position.x+sinf(m_Angle-3.14f/2.f)*iShort),	(int)(m_Position.y+cosf(m_Angle-3.14f/2.f)*iShort)},
		{(int)(m_Position.x+sinf(m_Angle)*iLong),				(int)(m_Position.y+cosf(m_Angle)*iLong)},
		{(int)(m_Position.x+sinf(m_Angle+3.14f/2.f)*iShort),	(int)(m_Position.y+cosf(m_Angle+3.14f/2.f)*iShort)}
	};
	
	MoveToEx(hdc, aiPoints[0][0], aiPoints[0][1], 0);
	LineTo(hdc, aiPoints[1][0], aiPoints[1][1]);
	LineTo(hdc, aiPoints[2][0], aiPoints[2][1]);
	LineTo(hdc, aiPoints[3][0], aiPoints[3][1]);
	LineTo(hdc, aiPoints[0][0], aiPoints[0][1]);

}


//--------------------------------------------------------------------------------------------------------------
// Explode
// Destroy the players ship.
//--------------------------------------------------------------------------------------------------------------
void Ship::Explode()
{
	m_Position = NTPoint(float(((double)rand() / RAND_MAX) * 600 + 100), float(((double)rand() / RAND_MAX) * 400 + 100));
	m_Velocity = NTPoint(0, 0);
	m_Angle = 0.f;
}
