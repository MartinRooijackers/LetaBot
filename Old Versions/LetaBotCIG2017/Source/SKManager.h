#pragma once

#include <BWAPI.h>
#include <vector>

using namespace BWAPI;


enum SKStates{
	SK_Defend_Base,
	SK_Hold,
	SK_Hunt,
	SK_Attack_Base
};


struct SKSquad{
	
	//the three units used in SK terran
	std::vector<BWAPI::Unit*> Marines;
	std::vector<BWAPI::Unit*> Medics;
	std::vector<BWAPI::Unit*> FireBats;
	std::vector<BWAPI::Unit*> ScienceVessels;
	std::vector<BWAPI::Unit*> Tanks;//tank support
	std::vector<BWAPI::Unit*> Goliaths;

	SKStates currentState;
	BWAPI::Position AttackTarget;
	BWAPI::Unit* AttackBuilding;
	//BWAPI::Position CurrentPosition;

};


class SKManager{
public:
	std::vector< SKSquad* > Squads;
	SKManager();
	void onFrame();
	void AddUnit( Unit* unit);

	void onDefend( SKSquad* SQuad );
	void onAttack( SKSquad* SQuad );
	
	void NextBuilding( SKSquad* SQuad );
	void onBuildingDestroyed(Unit* unit);

	void InitialPush();

	BWAPI::UnitType RecommendBio();// what Bio unit to build next

	int MarineCount();
	int MedicCount();
	int VesselCount();
    int FireBatCount();
    int GoliathCount();
};