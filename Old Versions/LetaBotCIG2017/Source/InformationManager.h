#pragma once

#include "Common.h"
#include "BWTA.h"

struct BaseInfo;
typedef std::vector<BaseInfo> BaseInfoVector;

struct BuildingInfo{
	BWAPI::Unit * building;
	BWAPI::Position position;
	BWAPI::UnitType type;
};


struct MineralCoOpPath{
	std::string mapHash; //hash of the map
	std::vector<int> MineralID; //ID of merals that can be used for coop path
};

class InformationManager {
	public:

	InformationManager();

	//location of bases;
	BWAPI::TilePosition				OurBase;
	BWAPI::Position				    PosOurBase;
	BWTA::Region*					OurMainregion;
	BWAPI::TilePosition				OurNat;
	BWAPI::Position				    PosOurNat;
	BWTA::Region*					NatRegion;
	BWAPI::TilePosition	 			EnemyBase;
	BWAPI::Position	 			    PosEnemyBase;
	BWAPI::TilePosition	 			EnemyNatural;
	BWAPI::Position	 			    PosMainChoke;

	BWAPI::Race  EnemyRace;

	MineralCoOpPath FSmineral;//coop path for Fighting Spirit map

	std::vector<BWAPI::TilePosition>   PossibleBase;

	std::vector<BuildingInfo>  EnemyBuildings;
	std::vector<Unit*>  Sunkens;
	std::vector<Unit*>  EnemyCombatUnits;
    int TotalSunken;

	void setLocationEnemyBase( BWAPI::TilePosition Here );
	void NoBaseHere( BWAPI::TilePosition notHere);
    void GetNaturalEnemy();
	void onUnitShow(BWAPI::Unit * unit);
	void onUnitDestroy(BWAPI::Unit * unit);

	//all gas geyser locations
	std::vector<Unit*>   Minerals;
	std::vector<Unit*>   Geysers;
	std::vector<BWAPI::TilePosition>   GeyserLocations;

};


