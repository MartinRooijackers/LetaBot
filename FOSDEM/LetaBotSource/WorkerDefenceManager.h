#pragma once

#include "Common.h"


class WorkerDefenceManager{
public:
	std::vector<Unit> DefenceSCVS;
	std::vector<Unit> RepairSCVS;
	BWAPI::Position ToRetreat;
	BWAPI::Unit ToRetreatM;

	WorkerDefenceManager();
	void OnFrame();
	void AddSCV( BWAPI::Unit scv);
	//~ScoutManager() {};

};