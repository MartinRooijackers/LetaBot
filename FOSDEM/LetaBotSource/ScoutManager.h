#pragma once

#include "Common.h"


struct Scout{
	Unit scv;
	BWAPI::Position Target;
	BWAPI::TilePosition TileTarget;
	BWAPI::Position CurrentNatural; //natural to go towards to not get stuck by neutral buildings
	BWAPI::Position TargetNatural; //scout the other natural first
	bool CurrentNatScouted;
	bool TargetNatScouted;
};

class ScoutManager {
public:
	std::vector<Scout> workerScouts;


	int directionScoutMain;

	ScoutManager( int totalSCV );
	void OnFrame();
	void addSCV(BWAPI::Unit unit);

	void ScoutMain( Unit scv ); //circle around the enemy base to scout

	BWAPI::Position GetNaturalLocScout( BWAPI::Position Target ); 

	//~ScoutManager() {};

};