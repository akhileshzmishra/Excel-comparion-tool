#ifndef _POSITION_MANAGER_H_
#define _POSITION_MANAGER_H_

#include "IDualMap.h"

class CellIdentifier
{
	int mX;
	int mY;
	static const int Multiplier = 2048;
	static const int MultiplierBits = 16;
public:
	int& X() {return mX;}
	int& Y() {return mY;}
	CellIdentifier(int x = 0, int y = 0): mX(x), mY(y) {}
	CellIdentifier(const CellIdentifier& other): mX(other.mX), mY(other.mY) {}
	CellIdentifier& operator = (const CellIdentifier& other)
	{
		if(this == &other)
		{
			return *this;
		}
		mX = other.mX;
		mY = other.mY;
		return *this;
	}
	bool operator < (CellIdentifier other)	const
	{
		return ((Multiply(mX) + mY) < (Multiply(other.mX) + other.mY));
	}
	bool operator == (CellIdentifier other)
	{
		return ((mX == other.mX)  && (mY == other.mY));
	}
	int Multiply(int X)	const
	{
		return X<<MultiplierBits;
	}
	void Set(int x, int y)
	{
		mX = x;
		mY = y;
	}
	void Reset()
	{
		mX = 0;
		mY = 0;
	}
};

class CellIdentifierComparator
{
public:
	bool operator () (CellIdentifier first, CellIdentifier second)   const
	{
		return (first < second);
	}
};


typedef DualMap<CellIdentifier, CellIdentifier, CellIdentifierComparator, CellIdentifierComparator> PositionMap;


#endif