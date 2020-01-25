#pragma once

#include "Common.h"
#include "BuildingManager.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <windows.h>

using namespace BWAPI;



struct BuildQueue{
	Unit scv;
	UnitType type;
    TilePosition buildLocation;
	Unit building;
	bool Started;
	int FrameStarted;
};

struct BO {
	UnitType type;
	int supply;
};

class ProductionManager{
public:

	bool useMultiThread;

	int reservedMinerals;
	int reservedGas;

	std::vector<UnitType> BuildingOrder;
	std::vector<BO> BuildingSupplyOrder;
	std::vector< BuildQueue > BuildingsQueue;
	std::vector< BWAPI::TechType > Research;
	std::vector<UnitType> BuildingsQueueType; //record the type of buildings in the queue

	ProductionManager();
	void onFrame();
	void onFrameMain();

	//void onFrameMarineRush();
	//void onFrameWorkerRush();
	void onFrameMacro();
	void onFrameMacroRandom();
	void onFrameMacroZerg();
	void onFrameMacroProtoss();
	void onFrameMacroTerran();

	//void onFrameBunkerRush();

	void AddCCTerran();

	void returnBuilderSCV( BWAPI::Unit scv );
	BWAPI::Unit getSCVBuilder(  BWAPI::UnitType type, BWAPI::TilePosition position  );
	void checkSCVbuild();
	void onUnitComplete(BWAPI::Unit unit);
	int TotalTypeInQueue(BWAPI::UnitType type);
	int TotalUnitInProduction(BWAPI::UnitType type);
	//Unit bSCV;

	HANDLE addQueueMutex; 

	//addToQueueMultiThread(BWAPI::UnitType type);
	void addToQueue(BWAPI::UnitType type);
	void addToQueueTile(BWAPI::UnitType type, BWAPI::TilePosition tile);
	void addToQueueClose(BWAPI::UnitType type, BWAPI::TilePosition tile, bool inBase=true);
	void addToQueueClose(BWAPI::UnitType type, BWAPI::TilePosition tile, BWTA::Region* region);
	void addSupplyBO(BWAPI::UnitType type, int supply = 0);

	void addToQueueBegin(BWAPI::UnitType type, BWAPI::TilePosition tileClose = BWAPI::TilePositions::Unknown );
	void addToQueueBeginTile(BWAPI::UnitType type, BWAPI::TilePosition tile);


	void addToBuildOrderBegin(BWAPI::UnitType type, int supply = 0);
	void removeFromQueue(BWAPI::UnitType type);
	void removeFromBuildOrder(BWAPI::UnitType type);

	void onUnitDestroy(BWAPI::Unit  unit);

	int getAvailableMinerals();
	int getAvailableGas();

	void PlaceTurrets();

	//Production buildings
	std::vector<Unit> CommandCenters;
	std::vector<Unit> FactoryNoAddon;
	std::vector<Unit> FactoryShop;
	std::vector<Unit> StarportNoAddon;
	std::vector<Unit> StarportCtower;
    std::vector<Unit> Barracks;
    std::vector<Unit> Armory;
	Unit Academy;
	Unit EngineeringBay;
	Unit ScienceFacility;

};