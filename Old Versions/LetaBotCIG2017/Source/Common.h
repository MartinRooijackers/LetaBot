#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include <BWAPI.h>
#include <BWTA.h>

#include <boost/foreach.hpp>

#include "ProductionManager.h"
#include "BaseManager.h"
#include "InformationManager.h"
#include "ScoutManager.h"
#include "WorkerDefenceManager.h"
#include "GeneralManager.h"
#include "RepairManager.h"
#include "ScannerManager.h"
#include "WraithManager.h"
#include "SKManager.h"
#include "WraithMicro.h"
#include "MacroManager.h"
#include "BuildOrderManager.h"
#include "WorkerAttackManager.h"
#include "MarineRushManager.h"

#include "MicroManagerZerg.h"
#include "MicroManagerProtoss.h"
#include "MicroManagerTerran.h"

#include <vector>
#include <string>

enum Strategies{
	Macro_Strat,
	Worker_Rush,
	BBS_Rush
};


extern bool LanLatency;

extern Strategies CurrentStrategy;
extern Strategies InitialStrategy;

typedef std::set<BWAPI::Unit*> UnitSet;

class ProductionManager;
class BuildingManager;
class BaseManager;
class InformationManager;

extern ProductionManager* ProdMan;
extern BuildingManager* bManager;
extern std::vector<BaseManager*> CCmanager;
extern InformationManager* InfoMan;
extern ScoutManager* ScoutMan;
extern WorkerDefenceManager* WDMan;
extern GeneralManager* GMan;
extern RepairManager* RPMan;
extern ScannerManager* ScanMan;
extern WraithManager* WraithMan;
extern SKManager* SKMan;
extern WraithMicro* WMicro;
extern MacroManager* MacroMan;
extern BuildOrderManager* BOMan;
extern WorkerAttackManager* WAMan;
extern MarineRushManager* MarineRush;


extern MicroManagerZerg *MicroZerg;
extern MicroManagerProtoss *MicroProtoss;
extern MicroManagerTerran *MicroTerran;

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