//-------------------------------------------------------------------------------------------------------------
// ntpoint.h
//
// Created: JohnL
//
// A 2D point class.
//
// Copyright(c) Ninja Theory 2010. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------

#include <math.h>

class NTPoint
{
public:
	NTPoint()
	{
	}

	NTPoint(float _x, float _y)
	: x(_x)
	, y(_y)
	{
	}

	NTPoint& operator=(const NTPoint& pt)      
	{
		x = pt.x;
		y = pt.y;
		return *this;
	}

	NTPoint operator+(const NTPoint& pt) const
	{
		return NTPoint(x + pt.x, y + pt.y);
	}

	NTPoint operator-(const NTPoint& pt) const
	{
		return NTPoint(x - pt.x, y - pt.y);
	}

	NTPoint operator*(float f) const
	{
		return NTPoint(x * f, y * f);
	}

	NTPoint operator/(float f) const
	{
		return NTPoint(x / f, y / f);
	}

	float GetLength() const
	{
		return sqrt((float)(x * x + y * y));
	}

	void Normalise()
	{
		float f = GetLength();
		x /= f;
		y /= f;
	}

public:
	float x;
	float y;
};

//--------------------------------------------------------------------------------------------------------------
// operator==
// Comparison operator for two point objects.
//--------------------------------------------------------------------------------------------------------------
static bool operator==(const NTPoint& lhs, const NTPoint& rhs) 
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
