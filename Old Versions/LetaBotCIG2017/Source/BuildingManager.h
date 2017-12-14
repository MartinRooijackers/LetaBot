#pragma once

#include "Common.h"
#include <BWAPI.h>
#include <BWTA.h>

using namespace BWAPI;

struct MyRectangle {
	int x1;
	int y1;
	int x2;
	int y2;
};


struct ExtraSpace{
	int Top;
	int Bottom;
	int Left;
	int Right;
	int width;
	int heigth;
};

struct WallData{
	std::string mapHash;
	BWAPI::TilePosition startTile;
	BWAPI::TilePosition Supply1;
	BWAPI::TilePosition Supply2;
	BWAPI::TilePosition Barracks1;
	BWAPI::TilePosition CC1;
	BWAPI::TilePosition Academy1;
};

//Building manager is based on Nova source code
class BuildingManager{
public:
	int mapW;
	int mapH;

	bool DrawWalkableData;
	bool DrawBuildableData;
	bool DrawWallData;
	bool useWallIO;
	bool UseTimer; //include a maximum timer for wall calculation
	std::vector<WallData> WD;
	void ReadWallData();
	void AddWallData(std::string mapHashI,int startX,int startY,int Supply1X,int Supply1Y,int Supply2X,int Supply2Y,int BarrackX,int BarrackY);

	//ExtraSpace SpaceArray[10];

	bool WallCalculated;//True when the map calculation is done
	bool WallSound;
	BWAPI::TilePosition BarracksWall;
	BWAPI::TilePosition SupplyWall1;
	BWAPI::TilePosition SupplyWall2;
	BWAPI::TilePosition CC1;
	BWAPI::TilePosition Academy1;
	//BWTA::Chokepoint* toDefend;

	int BuildMap[129][129];
	int WallMap[129][129];
	bool WalkMap[129][129];

	int unitHeight;
	int unitWidth;

	BuildingManager();
	//Based on Nova source code
	MyRectangle getBuildRectangle(TilePosition position, UnitType type);
	void mapArea(MyRectangle c, int label, int expand  = 0);
	void mapWalkable(MyRectangle c, bool label, int expand  = 0);
	void drawBuildMap();

	TilePosition getBuildLocationBase(TilePosition position, UnitType type, BWTA::Region* inRegion);
	//Based on Nova source code
	TilePosition getBuildLocationNear(TilePosition position, UnitType type, BWTA::Region* inRegion);
	bool canBuildHere(TilePosition buildPosition, UnitType type);
	void constructionPlaced(Unit* build);
	void onBuildingDestroy(BWAPI::Unit* build);

	//Walling off functions
	void CheckWall();

	void WallOff();
	bool CanWall(int BuildType,int xPos, int yPos);
	void RecursiveWall(std::vector<int> Buildings, int depth);
	void mapWall(int xPos, int yPos, int BuildType, int place);
	bool WalledOff(int x, int y, int Ignore, int unitH, int unitW);
	bool WalledOffAstar(int x, int y, int Ignore, int unitH, int unitW);
	void mapWall2(MyRectangle c, int label, int expand);
	int MaxGap(int x, int y);


};

