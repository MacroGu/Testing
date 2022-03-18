//-------------------------------------------------------------------------------------------------------------
// game.cpp
//
// Created: JohnL
//
// Implementation of the game class.
//
// Copyright(c) Ninja Theory 2010. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "game.h"
#include "objects.h"
#include "timer.h"

//-------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------
static const int MIN_SUNS = 10;
static const int MAX_SUNS = 25;
static const int X_SAFEREGION_MIN = 100;
static const int X_SAFEREGION_MAX = 1500;
static const int Y_SAFEREGION_MIN = 100;
static const int Y_SAFEREGION_MAX = 1000;
static const int PLACE_ATTEMPTS_PER_SUN = 20;
static const float MINIMUM_DISTANCE_BETWEEN_SUNS = 150.0f;
static const float DRAW_TIME = 0.05f;

//--------------------------------------------------------------------------------------------------------------
// Destructor
// Clean up any remaining objects being held in our lists.
//--------------------------------------------------------------------------------------------------------------
Game::~Game()
{
       m_LocalShip = NULL;

       for ( std::list< Sun * >::iterator itSun = m_Suns.begin(); itSun != m_Suns.end(); itSun++ )
       {
              delete *itSun;
       }
       m_Suns.clear();

       for ( std::list< Ship * >::iterator itShip = m_Ships.begin(); itShip != m_Ships.end(); itShip++ )
       {
              delete *itShip;
       }
       m_Ships.clear();

       for ( std::list< Missile * >::iterator itMissile = m_Missiles.begin(); itMissile != m_Missiles.end(); itMissile++ )
       {
              delete *itMissile;
       }
       m_Missiles.clear();
}

//--------------------------------------------------------------------------------------------------------------
// Initialise
// Set up the playing field and spawn the player's ship. Returns true if initialisation was successful.
//--------------------------------------------------------------------------------------------------------------
bool Game::Initialise()
{
	// Generate a random number of suns
	int numberOfSuns = RandomRange(MIN_SUNS, MAX_SUNS);

	for (int sunIndex = 0; sunIndex < numberOfSuns; sunIndex++)
	{
		// Try to place each sun a limited number of times, to ensure the function doesn't get stuck in an infinite loop
		for (int attemptNumber = 0; attemptNumber < PLACE_ATTEMPTS_PER_SUN; attemptNumber++)
		{
			// Generate a random position
			int sunX = RandomRange(X_SAFEREGION_MIN, X_SAFEREGION_MAX);
			int sunY = RandomRange(Y_SAFEREGION_MIN, Y_SAFEREGION_MAX);
	
			// Check the position is safe
			bool positionIsSafe = true;
			for (std::list<Sun*>::iterator itSun = m_Suns.begin(); itSun != m_Suns.end(); itSun++)
			{
				if (NTPoint(NTPoint((float)sunX, (float)sunY) - (*itSun)->GetPosition()).GetLength() < MINIMUM_DISTANCE_BETWEEN_SUNS)
				{
					positionIsSafe = false;
					break;
				}
			}

			if (positionIsSafe)
			{
				// Found a safe position, so create the sun and break out of the attempt loop
				m_Suns.push_back(new Sun(sunX, sunY));
				break;
			}
		}
	}

	// Launch the player ship
	m_LocalShip = new Ship();
	m_Ships.push_back(m_LocalShip);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
// Draw
// Draw the game.
//--------------------------------------------------------------------------------------------------------------
void Game::Draw(HDC hdc, PAINTSTRUCT* ps)
{
	// Make some pens
	HPEN penBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
	HPEN penGreen = CreatePen(PS_SOLID, 1, RGB(0,255,0));
	HPEN penRed  = CreatePen(PS_SOLID, 1, RGB(255,0,0));

	// Draw suns
	SelectObject(hdc, penRed);
	for (std::list<Sun*>::iterator itSun = m_Suns.begin(); itSun != m_Suns.end(); itSun++)
	{
		(*itSun)->Draw(hdc);
	}

	// Draw missiles
	SelectObject(hdc, penBlue);
	for (std::list<Missile*>::iterator itMissile = m_Missiles.begin(); itMissile != m_Missiles.end(); itMissile++)
	{
		(*itMissile)->Draw(hdc);
	}

	// Draw ships
	SelectObject(hdc, penBlue);
	for (std::list<Ship*>::iterator itShip = m_Ships.begin(); itShip != m_Ships.end(); itShip++)
	{
		(*itShip)->Draw(hdc);
	}

	// Delete our pens
	DeleteObject(penRed);
	DeleteObject(penGreen);
	DeleteObject(penBlue);

}


//--------------------------------------------------------------------------------------------------------------
// Update
// Update the game.
//--------------------------------------------------------------------------------------------------------------
void Game::Update(bool& outNeedRedraw)
{
	// Update the timer before doing anything else.
	m_Timer.Update();

	// Update missiles
	for (std::list<Missile*>::iterator itMissile = m_Missiles.begin(); itMissile != m_Missiles.end(); itMissile++)
	{
		(*itMissile)->Update();

		// Remove old missiles
		if ((*itMissile)->IsOutOfFuel())
		{
			m_Missiles.remove(*itMissile);
			delete (*itMissile);
		}
	}

	// Update player ships
	for (std::list<Ship*>::iterator itShip = m_Ships.begin(); itShip != m_Ships.end(); itShip++)
	{
		(*itShip)->Update();
	}

	// Check if we need a redraw
	static float fNextDraw = 0.f;
	fNextDraw -= m_Timer.GetTimeDelta();
	if (fNextDraw < 0.f)
	{
		fNextDraw = DRAW_TIME;
		outNeedRedraw = true;
	}
	else
	{
		outNeedRedraw = false;
	}
}


//--------------------------------------------------------------------------------------------------------------
// Fire
// Fire weapons
//--------------------------------------------------------------------------------------------------------------
void Game::Fire(int x, int y)
{
	m_Missiles.push_back(new Missile(m_LocalShip->GetPosition(), NTPoint((float)x, (float)y)));
}
