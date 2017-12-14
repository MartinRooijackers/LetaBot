#pragma once

#include <BWAPI.h>
#include <vector>

using namespace BWAPI;


enum WraithStates{
	W_Defend,
	W_Hunt,
	W_Repair,
	W_Attack_Base
};


struct WraithAgent{
	
	Unit* Wraith;
	WraithStates currentState;
	BWAPI::Position MovingTo;
	//BWAPI::Position CurrentPosition;
	int stopped_frames;//total amount of frames in which the wraith has the same position
};


class WraithManager{
public:
	std::vector< WraithAgent > Wraiths;

	BWAPI::Unit* RepairSCV;

	WraithManager();
	void onFrame();
	void AddWraith( Unit* unit);

	void NextBuilding();
	void onBuildingDestroyed(Unit* unit);

	BWAPI::Position AttackTarget; //the piece of land to attack
    BWAPI::Unit* AttackBuilding;

};