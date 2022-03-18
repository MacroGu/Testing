//-------------------------------------------------------------------------------------------------------------
// game.h
//
// Created: JohnL
//
// The game object.  
//
// Copyright(c) Ninja Theory 2010. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------------------
#include <list>
#include "timer.h"

// Externally defined classes.
class Sun;
class Missile;
class Ship;
class Asteroids;

//--------------------------------------------------------------------------------------------------------------
// Random
// Generate a random number in a specified range.
//--------------------------------------------------------------------------------------------------------------
static int RandomRange(int min, int max)
{
	return (int)(((double)rand() / RAND_MAX) * (max - min) + min);
}

//-------------------------------------------------------------------------------------------------------------
// Game
// Top level storage for the game.
//-------------------------------------------------------------------------------------------------------------
class Game
{
public:
	bool Initialise();
	void Update(bool& outNeedRedraw);
	void Draw(HDC hdc, PAINTSTRUCT* ps);

	void Fire(int x, int y);

	~Game();

public:
	Timer				m_Timer;
	std::list<Missile*> m_Missiles;
	std::list<Sun*>     m_Suns;
	std::list<Ship*>    m_Ships;
	std::list<Asteroids*>     m_Asteroids;

protected:
	Ship*				m_LocalShip;
};

extern Game g_Game;
