#pragma once

#include <BWAPI.h>
#include <vector>


using namespace BWAPI;

enum ProtossStates{
	P_Wall,
	P_Defend_Nat,
	P_Push
};

enum ZergStates{
	Z_Init,
	Z_Pool_Defend,
	Z_Plug_Ramp,  //against 9 pool
	Z_Choke_Defend,
	Z_SparksTerran, //go sparks terran vs 1 base zerg
	Z_Expand_Defend,
	Z_Expand_Defend_2_hatch,
	Z_Expand_Defend_3_hatch,
	Z_Push
};

enum TerranStates{
	//T_Init,
	T_Choke_Defend,
	T_Hold_BBS,
	//T_Expand_Defend,//no need with 3 fact
	T_Push
};


enum SquadStates{
	SQ_Defend_Base,
	SQ_Defend_Position,
	SQ_Attack_Base
};


struct MacroSquad{
	
	//the units used
	std::vector<BWAPI::Unit> Marines;
	std::vector<BWAPI::Unit> Medics;
	std::vector<BWAPI::Unit> FireBats;
	std::vector<BWAPI::Unit> ScienceVessels;
	std::vector<BWAPI::Unit> Tanks;//tank support
	std::vector<BWAPI::Unit> Goliaths;
	std::vector<BWAPI::Unit> SCVs;
	std::vector<BWAPI::Unit> Wraiths;
	std::vector<BWAPI::Unit> Valkryies;
	std::vector<BWAPI::Unit> Vultures;
	std::vector<BWAPI::Unit> BattleCruisers;
	std::vector<BWAPI::Unit> Ghosts;

	//bunkers used for defence
	std::vector<BWAPI::Unit> Bunkers;

	BWAPI::Position AttackTarget;
	BWAPI::Unit AttackBuilding;
	//BWAPI::Position CurrentPosition;

	SquadStates SQstate;

};


struct LurkerPos{
	BWAPI::Position position;
	int FramesLastSeen;
	int UnitID;
	BWAPI::Unit lurker;
};

//also has micro, this refers to the macro build orders
class MacroManager{
public:

	BWAPI::Position ClosestTankProtoss;
	int DistTankProtoss;
	bool AllowSDestroy; //should the supply depot be destroyed to create space for tanks?
	bool TankClose;
	int StartAttackFrame;
	
	bool ZealotRushExpected;
	bool TurretPlaced; //place turrets when factory starts

	BWAPI::Position chokePos;
	bool defend;
	bool EarlyPool;

	bool ZealotRushBunker;

	bool WallBunkerPlaced;
	bool NatBunkerPlace;
	bool NatLurkerTurretPlace;
	bool MutaBuild;
	BWAPI::TilePosition TileBunkerPos;
	Unit NatBunkerChoke;

	//attack position for Zerg
	Unit currentBuilding;
	BWAPI::Position attackPos;
	bool Hunt;
	bool HuntAllowed;

	Unit currentBuildingP;
	BWAPI::Position attackPosP;
	bool HuntP;
	bool HuntAllowedP;

	bool NoNeedTank;
	bool EnemyTerranHasBunker;

	Unit currentBuildingT2;
	BWAPI::Position attackPosT2;
	bool HuntT2;
	bool HuntAllowedT2;

	Unit FloatingBarracks;
	std::vector< BWAPI::Position > ETankPositions; //where enemy tanks are located

	std::vector< LurkerPos > ELurkerPositions; //where enemy lurkers are located

	ProtossStates PState;
	ZergStates ZState;
	TerranStates TState;

	std::string CurrentStrat;

	std::vector<MacroSquad> Squads;
	std::vector<BWAPI::UnitType> FollowupP;
	std::vector<BWAPI::UnitType> FollowupZ;
	std::vector<BWAPI::UnitType> FollowupT;

	MacroManager();
	void OnFrame();
	void RemoveDead();
	void MarineDefence();
	void VultureAttack();
	void WraithMoves();
	void AddUnit( Unit unit);
	void AddSCV( Unit unit);

	void NextBuildingMM();
	void NextBuildingMMp();
	void NextBuildingMMt();
	void onUnitDestroy(Unit unit);

	void OnFrameProtoss();
	void OnFrameProtossSquad();
	void OnFrameProtossSquadAttack(int id);
	void OnFrameProtossSquadDefence(int id);
	void OnFrameProtossSquadDefenceExpansion(int id);
	void OnFramePushOut();

	void OnFrameZerg();
	void OnFrameZergSquad();
	void OnFrameZergSquadAttack(int id);
	void OnFrameZergSquadDefence(int id);
	void OnFrameZergSquadDefenceChoke(int id);
	void OnFrameZergFlowChart();
	void OnFrameZergFlowChartPlugRamp();
	void OnFrameZergFlowChartSparksTerran();

	void OnFrameTerran();
	void OnFrameTerranSquad();
	void OnFrameTerranSquadDefence(int id);
	void OnFrameTerranSquadAttack(int id);

	void PlaceNatBunker();

	BWAPI::UnitType BarrackSuggest();
	BWAPI::UnitType FactorySuggest();
	BWAPI::UnitType FactorySuggestShop();
	BWAPI::UnitType StarPortCTSuggest();
	BWAPI::UnitType StarPortNoTSuggest();

};