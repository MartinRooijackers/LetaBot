#pragma once

#include <BWAPI.h>
#include <vector>
#include <BWTA.h>

using namespace BWAPI;


enum MicroStatesZerg{
	Zerg_Defend_Base,
	Zerg_Hold,
	Zerg_Hunt,
	Zerg_Attack_Base
};


struct MicroSquadZerg{
	
	//the three units used in SK terran
	std::vector<BWAPI::Unit> Marines;
	std::vector<BWAPI::Unit> Medics;
	std::vector<BWAPI::Unit> FireBats;
	std::vector<BWAPI::Unit> ScienceVessels;
	std::vector<BWAPI::Unit> Tanks;//tank support
	std::vector<BWAPI::Unit> Goliaths;

	MicroStatesZerg currentState;
	BWAPI::Position AttackTarget;
	BWAPI::Unit AttackBuilding;
	//BWAPI::Position CurrentPosition;

};


//use for air superiority/harrasing etc
struct WraithSquadronZerg{

	std::vector<BWAPI::Unit> Wraiths;

};


struct XY{

	int x;
	int y;

};


struct MoveGraphZ{
	BWTA::Region* reg;
	BWAPI::Position Center;
	std::vector<int> Edges;

};


struct InfluenceBaseZ{
	BWTA::Region* reg;
	BWAPI::TilePosition CClocation;
	//std::vector<int> Edges;
	int DefenceScoreNeeded;
	MicroSquadZerg defenceSquad;
};


class MicroManagerZerg{
public:
	//std::vector< MicroSquadZerg > Squads;
	std::vector< MoveGraphZ > Graph;

	std::vector< InfluenceBaseZ > DefenceBases;

	std::vector< XY > InfluenceMapPos;

	bool MMpush;
	bool RetreatIpush;

	MicroManagerZerg();
	void onFrame();


	void AddUnit( Unit unit);

	void onDefend( MicroSquadZerg* SQuad );
	void onAttack( MicroSquadZerg* SQuad );
	
	void NextBuilding( MicroSquadZerg* SQuad );
	void onBuildingDestroyed(Unit unit);

	void InitialPush(); //initial M&M push to force sunkens
	void StartIPush(); //initial M&M push to force sunkens

	void OnFrameZergSquadDefendExpand( MicroSquadZerg squad );

	BWAPI::UnitType RecommendBio();// what Bio unit to build next

	int MarineCount();
	int MedicCount();
	int VesselCount();
    int FireBatCount();
    int GoliathCount();
};