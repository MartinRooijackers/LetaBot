#pragma once

#include <BWAPI.h>
#include <vector>
#include <BWTA.h>

using namespace BWAPI;


enum MicroStatesTerran{
	Terran_Defend_Base,
	Terran_Hold,
	Terran_Hunt,
	Terran_Attack_Base
};


struct MicroSquadTerran{
	

	std::vector<BWAPI::Unit> Marines;
	std::vector<BWAPI::Unit> Medics;
	std::vector<BWAPI::Unit> FireBats;
	std::vector<BWAPI::Unit> ScienceVessels;
	std::vector<BWAPI::Unit> Tanks;//tank support
	std::vector<BWAPI::Unit> Goliaths;
	std::vector<BWAPI::Unit> Vultures;
	std::vector<BWAPI::Unit> SCVS;
	std::vector<BWAPI::Unit> Wraiths;
	std::vector<BWAPI::Unit> BattleCruisers;

	MicroStatesTerran currentState;
	BWAPI::Position AttackTarget;
	BWAPI::Unit AttackBuilding;
	//BWAPI::Position CurrentPosition;

};

//use for air superiority/harrasing etc
struct WraithSquadron{

	std::vector<BWAPI::Unit> Wraiths;

};


enum controller{
	Self,
	Enemy,
	Contested,
	Neutral

};

struct MoveGraphT{
	BWTA::Region* reg;
	BWAPI::Position Center;
	std::vector<int> Edges;

	controller control; //who controls this node?
};



struct InfluenceBaseT{
	BWTA::Region* reg;
	BWAPI::TilePosition CClocation;
	//std::vector<int> Edges;
	int DefenceScoreNeeded;
	MicroSquadTerran defenceSquad;
};



class MicroManagerTerran{
public:
	std::vector< MicroSquadTerran* > Squads;
	std::vector< MoveGraphT > Graph;

	std::vector< InfluenceBaseT > DefenceBases;

	MicroManagerTerran();
	void onFrame();
	void AddUnit( Unit unit);

	void drawInfo();

	void onDefend( MicroSquadTerran* SQuad );
	void onAttack( MicroSquadTerran* SQuad );
	
	void NextBuilding( MicroSquadTerran* SQuad );
	void onBuildingDestroyed(Unit unit);

	void InitialPush();
	void OnFrameTerranSquadDefendExpand( MicroSquadTerran squad );


	BWAPI::UnitType RecommendBio();// what Bio unit to build next



	int MarineCount();
	//int MedicCount();
	int VesselCount();
    //int FireBatCount();
    int GoliathCount();
};