#pragma once

#include "Common.h"



class MarineRushManager {
public:

	BWAPI::Position EnemyPos;
	BWAPI::Position EnemyNat;


	std::vector< BWAPI::Unit* > AttackMarines;
	bool StopMarineRush; //rush should be stopped if it removed the enemy resource depot

	MarineRushManager();
	void OnFrame();
	void AddMarine( BWAPI::Unit* marine);
	//~ScoutManager() {};

};