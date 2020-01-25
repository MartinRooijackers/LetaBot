#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <vector>

using namespace BWAPI;


enum MicroStatesProtoss{
	Protoss_Defend_Base,
	Protoss_Hold,
	Protoss_Hunt,
	Protoss_Attack_Base
};


struct MicroSquadProtoss{
	
	//the three units used in SK terran
	std::vector<BWAPI::Unit> Marines;
	std::vector<BWAPI::Unit> Medics;
	std::vector<BWAPI::Unit> FireBats;
	std::vector<BWAPI::Unit> ScienceVessels;
	std::vector<BWAPI::Unit> Tanks;//tank support
	std::vector<BWAPI::Unit> Goliaths;
	std::vector<BWAPI::Unit> Vultures;

	MicroStatesProtoss currentState;
	BWAPI::Position AttackTarget;
	BWAPI::Unit AttackBuilding;
	//BWAPI::Position CurrentPosition;

};


struct MoveGraph{
	BWTA::Region* reg;
	BWAPI::Position Center;
	std::vector<int> Edges;

};


struct InfluenceBaseP{
	BWTA::Region* reg;
	BWAPI::TilePosition CClocation;
	//std::vector<int> Edges;
	int DefenceScoreNeeded;
	MicroSquadProtoss defenceSquad;
};


class MicroManagerProtoss{
public:
	std::vector< MicroSquadProtoss > Squads;
	std::vector< MoveGraph > Graph;

	std::vector< InfluenceBaseP > DefenceBases;

	std::vector< BWAPI::Unit > Bunkers;

	MicroManagerProtoss();
	void onFrame();
	void drawInfo();
	void AddUnit( Unit unit);


	void EarlyDefend();

	void onDefend( MicroSquadProtoss* SQuad );
	void onAttack( MicroSquadProtoss* SQuad );
	
	void NextBuilding( MicroSquadProtoss* SQuad );
	void onBuildingDestroyed(Unit unit);

	void OnFrameProtossSquadDefenceExpand( MicroSquadProtoss squad );

	void InitialPush();


	BWAPI::UnitType RecommendBio();// what Bio unit to build next

	int MarineCount();
	int MedicCount();
	int VesselCount();
    int FireBatCount();
    int GoliathCount();
};