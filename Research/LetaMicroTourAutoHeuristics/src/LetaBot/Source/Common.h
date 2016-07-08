#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include <BWAPI.h>
#include <BWTA.h>

#include <boost/foreach.hpp>


#include "VultureManager.h"
#include "LevelSelector.h"
#include "MicroModule.h"

#include <vector>



typedef std::set<BWAPI::Unit*> UnitSet;

extern VultureManager* VMan;
extern LevelSelector *LvlSelect;
extern MicroModule* MicroMod;

//#define M_PI 3.1415926

struct double2
{
	double x,y;

	double2() {}
	double2(double x, double y) : x(x), y(y) {}
	double2(const BWAPI::Position & p) : x(p.x()), y(p.y()) {}

	operator BWAPI::Position()				const { return BWAPI::Position(static_cast<int>(x),static_cast<int>(y)); }

	double2 operator + (const double2 & v)	const { return double2(x+v.x,y+v.y); }
	double2 operator - (const double2 & v)	const { return double2(x-v.x,y-v.y); }
	double2 operator * (double s)			const { return double2(x*s,y*s); }
	double2 operator / (double s)			const { return double2(x/s,y/s); }

	double dot(const double2 & v)			const { return x*v.x + y*v.y; }
	double lenSq()							const { return x*x + y*y; }
	double len()							const { return sqrt(lenSq()); }
	double2 normal()						const { return *this / len(); }

	void normalise() { double s(len()); x/=s; y/=s; } 
	void rotate(double angle) 
	{ 	
		angle = angle*M_PI/180.0;
		*this = double2(x * cos(angle) - y * sin(angle), y * cos(angle) + x * sin(angle));
	}
};