#pragma once

//#include "Common.h"


struct RBuilding{
	Unit* toRepair;
	std::vector<Unit*> RepairSCVS;
};

class RepairManager{
public:
	//std::vector<Unit*> RepairSCVS;
	std::vector< RBuilding > RB;

	RepairManager();
	void OnFrame();
	void AddSCV( BWAPI::Unit* scv);
	BWAPI::Unit* getRepairSCV( BWAPI::TilePosition locRepair ); //get an SCV as close to the repair as possible
	void returnRepairSCV( BWAPI::Unit* scv ); //return the scv when it is no longer needed
	//~ScoutManager() {};

};