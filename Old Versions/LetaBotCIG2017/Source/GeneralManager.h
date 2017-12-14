#pragma once

//#include "Common.h"

#include <BWAPI.h>
#include <BWTA.h>

using namespace BWAPI;

class GeneralManager{
public:

	bool BarracksLift;
	bool putBdown;
	int StartLift;

	bool stillLifting;
	bool onlyOnce;
	bool alreadySupply;
	BWAPI::TilePosition toMove;

	GeneralManager();
	void onFrame();
	void GateManage();//check if the barracks should be lifted up
	void GateSCV();//Lift the barracks for an scv


	void StartLiftBarracks();//check if the barracks should be lifted up

};