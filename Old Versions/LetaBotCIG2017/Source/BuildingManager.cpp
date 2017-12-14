#include "BuildingManager.h"


#include <vector>
#include <fstream>
#include <queue>          // std::priority_queue

#include <windows.h>

#include <ctime>
//#include <boost/chrono/thread_clock.hpp>

using namespace BWAPI;

#define BLOCKED 0
#define BUILDABLE 1

ExtraSpace SpaceArray[10];

//bool WallSound = false;
//bool useWallIO = true;


bool Walled = false;
int Visited[129*4][129*4];
std::vector<BWAPI::TilePosition> BuildingPlace;



DWORD WINAPI AnalyzeWall()
{
	bManager->WallOff();
    return 0;
}

void BuildingManager::AddWallData(std::string mapHashI,int startX,int startY,int Supply1X,int Supply1Y,int Supply2X,int Supply2Y,int BarrackX,int BarrackY){

  				WallData newWD;
				//int x;
				//int y;
				//fin>>newWD.mapHash;
				newWD.mapHash = mapHashI;
				newWD.startTile = BWAPI::TilePosition(startX,startY);
				newWD.Supply1 = BWAPI::TilePosition(Supply1X,Supply1Y);
				newWD.Supply2 = BWAPI::TilePosition(Supply2X,Supply2Y);
				newWD.Barracks1 = BWAPI::TilePosition(BarrackX,BarrackY);
				WD.push_back(  newWD );

}

void BuildingManager::ReadWallData()
{
  if( useWallIO == false ){
	  return;
  }
  //Broodwar->printf("%i %i",InfoMan->OurBase.x(), InfoMan->OurBase.y() );
  Broodwar->printf("%s",Broodwar->mapHash().c_str() ); //print the map hash
  		WD.clear();
  //CIG Wall maps
		/*
  			AddWallData("cd5d907c30d58333ce47c88719b6ddb2cba6612f",117,83,116,61,116 ,63,112, 60);
  			AddWallData("cd5d907c30d58333ce47c88719b6ddb2cba6612f",7 ,83 ,6 ,63 ,9 ,63 ,5 ,60);

  			AddWallData("b10e73a252d5c693f19829871a01043f0277fd58",93 ,85 ,90, 63 ,91 ,61 ,90, 65);
  			AddWallData("b10e73a252d5c693f19829871a01043f0277fd58",31, 83, 36, 96, 39, 96, 32, 94);
  			AddWallData("b10e73a252d5c693f19829871a01043f0277fd58",70, 25, 57 ,28 ,57, 26, 56 ,30);

  			AddWallData("9bfc271360fa5bab3707a29e1326b84d0ff58911",7, 44 ,6 ,26, 11, 30, 7 ,28);
  			AddWallData("9bfc271360fa5bab3707a29e1326b84d0ff58911",117 ,9, 113, 35 ,113, 38 ,109, 36);
  			AddWallData("9bfc271360fa5bab3707a29e1326b84d0ff58911",93, 118, 72 ,113 ,70, 115, 68, 111);

  			AddWallData("1e983eb6bcfa02ef7d75bd572cb59ad3aab49285",117, 119, 112, 94, 114, 96, 108, 92);
  			AddWallData("1e983eb6bcfa02ef7d75bd572cb59ad3aab49285",117, 7, 114, 31, 116, 29, 110, 32);
			*/
			// Fighting spirit default 1.3
  			AddWallData("5731c103687826de48ba3cc7d6e37e2537b0e902",117, 117, 116, 100, 118, 98, 112, 101);
  			AddWallData("5731c103687826de48ba3cc7d6e37e2537b0e902",7 ,6, 7, 26 ,10 ,26, 4 ,28);
  			AddWallData("5731c103687826de48ba3cc7d6e37e2537b0e902",7, 116, 22, 118, 28, 121, 24, 120);
  			AddWallData("5731c103687826de48ba3cc7d6e37e2537b0e902",117, 7, 97, 5, 100, 7, 102, 9  );
			// Fighting spirit ICCUP
  			AddWallData("d2f5633cc4bb0fca13cd1250729d5530c82c7451",117, 117, 116, 100, 118, 98, 112, 101);
  			AddWallData("d2f5633cc4bb0fca13cd1250729d5530c82c7451",7 ,6, 7, 26 ,10 ,26, 4 ,28);
  			AddWallData("d2f5633cc4bb0fca13cd1250729d5530c82c7451",7, 116, 22, 118, 28, 121, 24, 120);
  			AddWallData("d2f5633cc4bb0fca13cd1250729d5530c82c7451",117, 7, 97, 5, 100, 7, 102, 9  );


		std::ifstream fin("bwapi-data/read/wall.txt");
		if( !fin.fail() ){
			std::string mapHas;
			while( fin>>mapHas ){
				WallData newWD;
				int x;
				int y;
				//fin>>newWD.mapHash;
				newWD.mapHash = mapHas;
				fin>>x>>y;
				newWD.startTile = BWAPI::TilePosition(x,y);
				fin>>x>>y;
				newWD.Supply1 = BWAPI::TilePosition(x,y);
				fin>>x>>y;
				newWD.Supply2 = BWAPI::TilePosition(x,y);
				fin>>x>>y;
				newWD.Barracks1 = BWAPI::TilePosition(x,y);
				//fin>>x>>y;
				//newWD.CC1 = BWAPI::TilePosition(x,y);
				//newWD.Academy1 = BWAPI::TilePosition(x,y);


				WD.push_back(  newWD );
			}
		}
			//check if the wall data already exists
			bool already = false;
			for(unsigned int i=0; i<WD.size(); i++){
				//Broodwar->printf("%s",WD[i].mapHash);
				//Broodwar->printf("%d", WD[i].startTile.x() );
				//Broodwar->printf("%d", WD[i].startTile.y() );
				if( WD[i].mapHash == Broodwar->mapHash() &&
					WD[i].startTile == InfoMan->OurBase ){
                   already = true;
				   Broodwar->printf("Wall in found in IO");
				   
				   //if(   CurrentStrategy == One_Fact_3_Rax ){
					if(CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss  ){
					   //if( bManager != NULL ){
						/*
					   bManager->SupplyWall1 = WD[i].Supply1;
					   bManager->SupplyWall2 = WD[i].Supply2;
					   bManager->BarracksWall = WD[i].Barracks1;
					   //bManager->CC1 = WD[i].CC1;
					   bManager->Academy1 =  WD[i].Academy1;
					   bManager->WallCalculated = true;
					   */
					   SupplyWall1 = WD[i].Supply1;
					   SupplyWall2 = WD[i].Supply2;
					   BarracksWall = WD[i].Barracks1;
					   //bManager->CC1 = WD[i].CC1;
					   Academy1 =  WD[i].Academy1;
					   WallCalculated = true;
					   WallSound = true;
					   //Broodwar->printf("Wall in found in IO");
					   //}
				   }
				   
				   break;
				}
			}



			if( already == false ){
              //WallOff();
				Broodwar->printf("Wall not in found in IO");
			  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeWall, NULL, 0, NULL);	
			}

			if(  (CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss  )
				//|| (CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Zerg )
				    ){ //&& MacroMan->CurrentStrat == "2 Port Wraith"

			} else {
					   WallSound = false;
			}

}


BuildingManager::BuildingManager(){

	useWallIO = true;


	WallCalculated = false;
	WallSound = false;
	UseTimer = false;

	DrawBuildableData = false;
	DrawWalkableData = false;

	DrawWallData = false;

	//Init build map
	mapW = Broodwar->mapWidth();
	mapH = Broodwar->mapHeight();

	for(int x=0; x<128; x++){
	  for(int y=0; y<128; y++){
		  BuildMap[x][y]=BUILDABLE;
		  WallMap[x][y]=BUILDABLE;
		  	if (!Broodwar->isBuildable(x, y)) {
				BuildMap[x][y] = BLOCKED;
				WallMap[x][y] = BLOCKED;
			}

			WalkMap[x][y] = false;
			int totWalk = 0;
			if ( Broodwar->isWalkable(x*4,y*4) ){
				//WalkMap[x][y] = true;
				totWalk++;
			}
			if ( Broodwar->isWalkable(x*4 + 1,y*4) ){
				//WalkMap[x][y] = true;
				totWalk++;
			}
			if ( Broodwar->isWalkable(x*4,y*4 + 1) ){
				//WalkMap[x][y] = true;
				totWalk++;
			}
			if ( Broodwar->isWalkable(x*4 + 1,y*4 + 1) ){
				//WalkMap[x][y] = true;
				totWalk++;
			}
			if( totWalk == 4 ){
				WalkMap[x][y] = true;
			}
	  }
	}

	//Block minerals locations
	for(UnitSet::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); ++m) {
		MyRectangle mineral = getBuildRectangle((*m)->getTilePosition(), (*m)->getType());
		mapArea(mineral, BLOCKED, 1);
		mapWalkable(mineral, false);
		mapWall2(mineral, BLOCKED, 0);
	}

	//Block gas locations
	for(UnitSet::iterator g = Broodwar->getGeysers().begin(); g != Broodwar->getGeysers().end(); ++g) {
		MyRectangle gas = getBuildRectangle((*g)->getTilePosition(), (*g)->getType());
		mapArea(gas, BLOCKED, 2);
		mapWalkable(gas, false);
		mapWall2(gas, BLOCKED, 0);

	}

  //Block base locations
  for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++)
  {
    TilePosition p=(*i)->getTilePosition();
    Position c=(*i)->getPosition();

	// either 0 or 1 extra size, preferably 1 in case you want to send units to the CC to wait

	MyRectangle possibleCC = getBuildRectangle((*i)->getTilePosition(), BWAPI::UnitTypes::Terran_Command_Center );
	mapArea(possibleCC, BLOCKED, 1);
	mapWalkable(possibleCC, false);
	mapWall2(possibleCC, BLOCKED, 1);
  }


	//No building means no gap
	ExtraSpace NoBuilding;
	NoBuilding.Top = 16;
	NoBuilding.Bottom = 16;
	NoBuilding.Left = 16;
	NoBuilding.Right = 16;
	NoBuilding.width = 1;
	NoBuilding.heigth = 1;

	//Fill up extra space array
	ExtraSpace Barracks;
	Barracks.Top = 8;
	Barracks.Bottom = 15;
	Barracks.Left = 16;
	Barracks.Right = 7;
	Barracks.width = 4;
	Barracks.heigth = 3;

	ExtraSpace SupplyDepot;
	SupplyDepot.Top = 10;
	SupplyDepot.Bottom = 5;
	SupplyDepot.Left = 10;
	SupplyDepot.Right = 9;
	SupplyDepot.width = 3;
	SupplyDepot.heigth = 2;

	ExtraSpace CCsize;
	CCsize.Top = 7;
	CCsize.Bottom = 6;
	CCsize.Left = 6;
	CCsize.Right = 5;
	CCsize.width = 4;
	CCsize.heigth = 3;

	ExtraSpace ACsize;
	ACsize.Top = 0;
	ACsize.Bottom = 7;
	ACsize.Left = 8;
	ACsize.Right = 3;
	ACsize.width = 3;
	ACsize.heigth = 2;

	SpaceArray[0] = NoBuilding;
	SpaceArray[1] = NoBuilding;
	SpaceArray[2] = Barracks;
	SpaceArray[3] = SupplyDepot;
	SpaceArray[4] = SupplyDepot;
	SpaceArray[5] = ACsize;
	//WallOff();
	
	// || CurrentStrategy == Eight_rax
    //&& Broodwar->enemy()->getRace() == BWAPI::Races::Protoss 
	if( useWallIO == false &&
		(  ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss  )
		//|| ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Zerg   )   ) //&& MacroMan->CurrentStrat == "2 Port Wraith"
		  ) ){
      //compute a wall off
      WallOff();
	  //CheckWall();
				Broodwar->printf("Wall not in found in IO");
			  //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeWall, NULL, 0, NULL);	
	} 
}

void BuildingManager::mapWalkable(MyRectangle c, bool label, int expand){
	for (int x = c.x1-expand; x <= c.x2+expand; x++) {
		for (int y = c.y1-expand; y <= c.y2+expand; y++) {
			if (x >= 0 && x < mapW && y >= 0 && y < mapH) {
				WalkMap[x][y] = label;
			}
		}
	}
}


void BuildingManager::mapArea(MyRectangle c, int label, int expand){
	for (int x = c.x1-expand; x <= c.x2+expand; x++) {
		for (int y = c.y1-expand; y <= c.y2+expand; y++) {
			if (x >= 0 && x < mapW && y >= 0 && y < mapH) {
				BuildMap[x][y] = label;
			}
		}
	}
}

//Based on Nova source code
MyRectangle BuildingManager::getBuildRectangle(TilePosition position, UnitType type)
{
	MyRectangle area;
	area.x1 = position.x();
	area.y1 = position.y();
	area.x2 = position.x() + type.tileWidth() - 1;
	area.y2 = position.y() + type.tileHeight() - 1;

	//make sure we leave space for add-ons
	if (type==BWAPI::UnitTypes::Terran_Command_Center ||
		type==BWAPI::UnitTypes::Terran_Factory || 
		type==BWAPI::UnitTypes::Terran_Starport ||
		type==BWAPI::UnitTypes::Terran_Science_Facility)
	{
		area.x2+=2;
	}

	//leave more space for production buildings
	//   type==BWAPI::UnitTypes::Terran_Barracks   no need for this one
	if (type==BWAPI::UnitTypes::Terran_Command_Center ||
		type==BWAPI::UnitTypes::Terran_Factory 
		)
	{
		//area.x1--;
		area.y2++;
	}

	return area;
}






void BuildingManager::constructionPlaced(Unit* build)
{
	//Block buildMap
	MyRectangle area = getBuildRectangle(build->getTilePosition(), build->getType());
	mapArea(area, BLOCKED,0);
	mapWalkable(area, false);

	//Remove from toConstruct list
	/*
	for (std::vector<UnitType>::iterator it = _toConstruct.begin(); it != _toConstruct.end(); ++it) {
		if (it->getID() == build->getType().getID()) {
			_toConstruct.erase(it);
			return;
		}
	}
	*/
}


void BuildingManager::onBuildingDestroy(BWAPI::Unit* build)
{
	//Free buildMap
	if (build->getType() == UnitTypes::Terran_Refinery || //don't free space from refinery, command center or addons
		build->getType() == UnitTypes::Terran_Comsat_Station ||
		build->getType() == UnitTypes::Terran_Nuclear_Silo ||
		build->getType() == UnitTypes::Terran_Machine_Shop ||
		build->getType() == UnitTypes::Terran_Control_Tower ||
		build->getType() == UnitTypes::Terran_Physics_Lab ||
		build->getType() == UnitTypes::Terran_Covert_Ops ||
		build->getType() == UnitTypes::Terran_Command_Center) 
		return;
	MyRectangle area = getBuildRectangle(build->getTilePosition(), build->getType());
	mapArea(area, BUILDABLE, 0);
	mapWalkable(area, true);
}

bool BuildingManager::canBuildHere(TilePosition buildPosition, UnitType type) {
	if (!Broodwar->canBuildHere(NULL, buildPosition, type))
		return false;

	if (Broodwar->mapHash() == "4e24f217d2fe4dbfa6799bc57f74d8dc939d425b") { // Map Destination
		BWTA::Region* buildRegion = BWTA::getRegion(buildPosition);
		if (BWTA::getRegion(TilePosition(80,5)) == buildRegion ||
			BWTA::getRegion(TilePosition(20,125)) == buildRegion) {
			return false;
		}
	}

	MyRectangle area = getBuildRectangle(buildPosition, type);

	//Check buildMap
	for (int x = area.x1; x <= area.x2; x++) {
		for (int y = area.y1; y <= area.y2; y++) {
			if (x >= 0 && x < mapW && y >= 0 && y < mapH) {
				if (BuildMap[x][y] != BUILDABLE) {
					return false; //Cant build here.
				}
			} else {
				return false; //Out of map
			}
		}
	}

	//Check if we have free space surrounding us
	MyRectangle buildArea;
	buildArea.x1 = buildPosition.x() - 1;
	buildArea.y1 = buildPosition.y() - 1;
	buildArea.x2 = buildPosition.x() + type.tileWidth();
	buildArea.y2 = buildPosition.y() + type.tileHeight();
	// Check top and bottom
	bool topFree = true;
	for (int y = buildArea.y1; y <= buildArea.y2; y++) {
		if (buildArea.x1 >= 0 && buildArea.x1 < mapW && y >= 0 && y < mapH) {
			//if (!Broodwar->isBuildable(buildArea.x1, y)) {
			if (BuildMap[buildArea.x1][y] != BUILDABLE) {
				topFree = false; //Cant build here.
				break;
			}
		} else {
			topFree = false; //Out of map
			break;
		}
	}
	if (!topFree) {
		for (int y = buildArea.y1; y <= buildArea.y2; y++) {
			if (buildArea.x2 >= 0 && buildArea.x2 < mapW && y >= 0 && y < mapH) {
				//if (!Broodwar->isBuildable(buildArea.x2, y)) {
				if (BuildMap[buildArea.x2][y] != BUILDABLE) {
					return false; //Cant build here.
				}
			} else {
				return false; //Out of map
			}
		}
	}
	// Check left and right
	bool leftFree = true;
	for (int x = buildArea.x1; x <= buildArea.x2; x++) {
		if (buildArea.y1 >= 0 && buildArea.y1 < mapH && x >= 0 && x < mapW) {
			//if (!Broodwar->isBuildable(x, buildArea.y1)) {
			if (BuildMap[x][buildArea.y1] != BUILDABLE) {
				leftFree = false; //Cant build here.
				break;
			}
		} else {
			leftFree = false; //Out of map
			break;
		}
	}
	if (!leftFree) {
		for (int x = buildArea.x1; x <= buildArea.x2; x++) {
			if (buildArea.y2 >= 0 && buildArea.y2 < mapH && x >= 0 && x < mapW) {
				//if (!Broodwar->isBuildable(x, buildArea.y2)) {
				if (BuildMap[x][buildArea.y2] != BUILDABLE) {
					return false; //Cant build here.
				}
			} else {
				return false; //Out of map
			}
		}
	}


	return true;
}

//Based on Nova source code
TilePosition BuildingManager::getBuildLocationNear(TilePosition position, UnitType type, BWTA::Region* inRegion)
{
	//returns a valid build location near the specified tile position.
	//searches outward in a spiral.
	int x      = position.x();
	int y      = position.y();
	int length = 1;
	int j      = 0;
	bool first = true;
	int dx     = 0;
	int dy     = 1;	
	while (length < Broodwar->mapWidth()) //We'll ride the spiral to the end
	{
		//if we can build here, return this tile position
		if (x >= 0 && x < Broodwar->mapWidth() && y >= 0 && y < Broodwar->mapHeight())
			if (inRegion==0 || inRegion==BWTA::getRegion(TilePosition(x, y)))
				if (canBuildHere(TilePosition(x, y), type))
					return TilePosition(x, y);

		//otherwise, move to another position
		x = x + dx;
		y = y + dy;
		//count how many steps we take in this direction
		j++;
		if (j == length) { //if we've reached the end, its time to turn
			j = 0;	//reset step counter

			//Spiral out. Keep going.
			if (!first)
				length++; //increment step counter if needed

			
			first =! first; //first=true for every other turn so we spiral out at the right rate

			//turn counter clockwise 90 degrees:
			if (dx == 0) {
				dx = dy;
				dy = 0;
			} else {
				dy = -dx;
				dx = 0;
			}
		}
		//Spiral out. Keep going.
	}
	return TilePositions::None;
}



TilePosition BuildingManager::getBuildLocationBase(TilePosition position, UnitType type, BWTA::Region* inRegion)
{

	if(  (CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss  )  ){

	} else {
	  WallSound = false;
	}

	//check if it is part of a wall
	if( WallCalculated == true && WallSound == true){
	  if(  type == BWAPI::UnitTypes::Terran_Supply_Depot &&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Supply_Depot ) == 0 ){
		  mapArea( getBuildRectangle(SupplyWall1, BWAPI::UnitTypes::Terran_Supply_Depot) , 0,1 );
		  return SupplyWall1;
	  }
	  
	  if(  type == BWAPI::UnitTypes::Terran_Supply_Depot &&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Supply_Depot ) == 1 ){
		  mapArea( getBuildRectangle(SupplyWall2, BWAPI::UnitTypes::Terran_Supply_Depot) , 0,1 );
		  return SupplyWall2;
	  }
	  
	  //  && ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 0  
	  // && BuildMap[ BarracksWall.x() ][ BarracksWall.y() ] == BUILDABLE
	  if(  type == BWAPI::UnitTypes::Terran_Barracks 
		  &&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Barracks ) == 0
		   ){
		   mapArea( getBuildRectangle(BarracksWall, BWAPI::UnitTypes::Terran_Barracks) , 0,1 );
		   return BarracksWall;
	  }

	  /*
	  if(  type == BWAPI::UnitTypes::Terran_Academy &&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Academy ) == 0 ){
		  return Academy1;
	  }
	  */

	  /*
	  if(  type == BWAPI::UnitTypes::Terran_Command_Center &&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) == 1 ){
		  return CC1;
	  }
	  */

	}
	if( type == BWAPI::UnitTypes::Terran_Refinery ){

		//check for an empty refinery
		for( int i=0; i<CCmanager.size(); i++){
			if( CCmanager[i]->Geysers.size() > 0
				&&  CCmanager[i]->Refinerys.size() == 0 ){
				return CCmanager[i]->Geysers[0]->getTilePosition();
			}
		}

		return BWAPI::TilePositions::None;

	}
	//otherwise do regular build location
	return getBuildLocationNear(position, type, inRegion);
}





BWAPI::TilePosition TopLeft;
BWAPI::TilePosition BottomRight;

BWAPI::TilePosition Top1Left;
BWAPI::TilePosition Bottom1Right;

BWAPI::TilePosition Furthest;
BWAPI::TilePosition Closest;

void BuildingManager::drawBuildMap()
{
	
	if( DrawBuildableData == true ){
	  for(int x=0; x < mapW; ++x) {
		for(int y=0; y < mapH; ++y) {
			if ( BuildMap[x][y] == BUILDABLE){
			//if ( WalkMap[x][y] == true ){
				Broodwar->drawCircleMap(x*TILE_SIZE+16,y*TILE_SIZE+16,3,Colors::Green,true);
			}
			else {
				Broodwar->drawCircleMap(x*TILE_SIZE+16,y*TILE_SIZE+16,3,Colors::Red,true);
			}
		}
	  }
	}

	if( DrawWalkableData == true ){
	  //draw walkable data
	  for(int x=0; x < mapW*4; ++x) {
		for(int y=0; y < mapH*4; ++y) {


			if(  Visited[x][y] == 1 ){
				Broodwar->drawCircleMap(x*8+4,y*8+4,2,Colors::Purple,true);
				continue;
			}

			if ( Broodwar->isWalkable(x,y)){
				Broodwar->drawCircleMap(x*8+4,y*8+4,2,Colors::Green,true);
			}
			else {
				Broodwar->drawCircleMap(x*8+4,y*8+4,2,Colors::Red,true);
			}
		}
	  }
	}


	for(unsigned int i=0; i<ProdMan->BuildingsQueue.size(); i++ ){
		Broodwar->drawTextScreen( 10, i*20 + 10, "%s", ProdMan->BuildingsQueue[i].type.c_str() );
		Broodwar->drawBox(CoordinateType::Map,ProdMan->BuildingsQueue[i].buildLocation.x()*32,ProdMan->BuildingsQueue[i].buildLocation.y()*32,ProdMan->BuildingsQueue[i].buildLocation.x()*32+3*32,ProdMan->BuildingsQueue[i].buildLocation.y()*32+2*32,Colors::Cyan,false);
		
		/*
		if( ProdMan->BuildingsQueue[i].scv == NULL || !ProdMan->BuildingsQueue[i].scv->exists() ){
			Broodwar->printf("SCV does not exists");
		} else {
		Broodwar->drawCircleMap( ProdMan->BuildingsQueue[i].scv->getPosition().x(), ProdMan->BuildingsQueue[i].scv->getPosition().y(), 5, Colors::Orange,true );
		}
		*/

	}
	
	//if(  WallCalculated  &&  DrawWallData ) {
	if(   DrawWallData ) {	
	  //Draw wall locations
		Broodwar->drawBox(CoordinateType::Map,BarracksWall.x()*32,BarracksWall.y()*32,BarracksWall.x()*32+4*32,BarracksWall.y()*32+3*32,Colors::Orange,false);
		Broodwar->drawBox(CoordinateType::Map,SupplyWall1.x()*32,SupplyWall1.y()*32,SupplyWall1.x()*32+3*32,SupplyWall1.y()*32+2*32,Colors::Orange,false);
		Broodwar->drawBox(CoordinateType::Map,SupplyWall2.x()*32,SupplyWall2.y()*32,SupplyWall2.x()*32+3*32,SupplyWall2.y()*32+2*32,Colors::Orange,false);

		//Broodwar->drawBox(CoordinateType::Map,CC1.x()*32,CC1.y()*32,CC1.x()*32+4*32,CC1.y()*32+3*32,Colors::Orange,false);
		//Broodwar->drawBox(CoordinateType::Map,Academy1.x()*32,Academy1.y()*32,Academy1.x()*32+3*32,Academy1.y()*32+2*32,Colors::Orange,false);


		Broodwar->drawBox(CoordinateType::Map,TopLeft.x()*32,TopLeft.y()*32,BottomRight.x()*32,BottomRight.y()*32,Colors::Purple,false);	
		Broodwar->drawBox(CoordinateType::Map,Furthest.x()*32,Furthest.y()*32,Furthest.x()*32+32,Furthest.y()*32+32,Colors::White,false);	
		Broodwar->drawBox(CoordinateType::Map,Closest.x()*32,Closest.y()*32,Closest.x()*32+32,Closest.y()*32+32,Colors::White,false);
	    
	}
	

}


int MaxHighGround = 0;
bool useHighGround = true; //set to false when highground wall fails

bool BuildingManager::CanWall(int BuildType,int xPos, int yPos){
	bool canPlace = true;
	bool NextToB = false;//Check if it is next to a building
	for(int x=xPos; x<xPos+ SpaceArray[BuildType].width; x++ ){
		for(int y=yPos; y<yPos+ SpaceArray[BuildType].heigth; y++ ){
           if(  (x >= mapW) || (y >= mapH) || (x < 0) || (y < 0) ){
			   return false;//out of map
		   }
		   if( WallMap[x][y] != BUILDABLE ){
			   return false;
		   }
		   // Do not build over the end goal,otherwise it cannot be reached
		   if(  Furthest.x() == x && Furthest.y() == y ){
			   return false;
		   }
		   // Do not build over the start goal,otherwise it cannot be reached
		   if(  Closest.x() == x && Closest.y() == y ){
			   return false;
		   }

		   //Only build on highest ground
		   if( useHighGround && Broodwar->getGroundHeight(x,y) != MaxHighGround ){
			   return false;
		   }

		   if( BuildType == 2 ){//Barracks gets put down first
			   NextToB = true;//
		   }
		   //Check if thr building is next to another building
		   int dx[8] = {1,-1,0,0,1,-1,1,-1};
		   int dy[8] = {0,0,1,-1,1,-1,-1,1};
		   for(int i=0; i<8; i++){
			   int newX = x+dx[i];
			   int newY = y+dy[i];
			   if( (newX < 0) || (newX >= mapW) || (newY < 0) || (newY >= mapH)  ){
				   continue;
			   }
		       if( WallMap[newX][newY] > 1 && WallMap[newX][newY] != BuildType ){
				   NextToB = true;
		       }
		   }
		   /*
		   if( NextToB == false ){
			   //return false;
		   }
		   */
		}
	}
	if( NextToB == false ){
	   return false;
	}
	return true;
}


void BuildingManager::mapWall(int xPos, int yPos, int BuildType, int place){
	for(int x=xPos; x<xPos+ SpaceArray[BuildType].width; x++ ){
		for(int y=yPos; y<yPos+ SpaceArray[BuildType].heigth; y++ ){
           WallMap[x][y] = place;
		}
	}
}





/*
	int getGap = SpaceArray[WallMap[x][y+1]].Top +  SpaceArray[WallMap[x][y]].Bottom;
	if(  WallMap[x][y+1] != WallMap[x][y] && maxGap < getGap && WallMap[x][y+1] > 1 ){
		maxGap = getGap;
	}
	getGap = SpaceArray[WallMap[x][y-1]].Bottom + SpaceArray[WallMap[x][y]].Top;
	if( WallMap[x][y-1] != WallMap[x][y] && maxGap < getGap && WallMap[x][y-1] > 1 ){
		maxGap = getGap;
	}
	getGap = SpaceArray[WallMap[x+1][y]].Left + SpaceArray[WallMap[x][y]].Right;
	if(  WallMap[x+1][y] != WallMap[x][y] && maxGap < getGap && WallMap[x+1][y] > 1 ){
		maxGap = getGap;
	}
	getGap = SpaceArray[WallMap[x-1][y]].Right + SpaceArray[WallMap[x][y]].Left;
	if(  WallMap[x-1][y] != WallMap[x][y] && maxGap < getGap && WallMap[x+1][y] > 1 ){
		maxGap = getGap;
	}

	if( maxGap > 1 ){
      return maxGap;
	}

	for(int i=-1; i<=1; i++){
		if( i == 0 ){
			continue;
		}
		getGap = SpaceArray[WallMap[x-1][ y + i]].Right +  SpaceArray[WallMap[x][y]].Left;
		if(  WallMap[x-1][y+i] != WallMap[x][y] && maxGap < getGap && WallMap[x-1][y+i] > 1){
			maxGap = getGap;
		}
		getGap = SpaceArray[WallMap[x+1][ y + i]].Left +  SpaceArray[WallMap[x][y]].Right;
		if(  WallMap[x+1][y+i] != WallMap[x][y]  && maxGap < getGap && WallMap[x+1][y+i] > 1 ){
			maxGap = getGap;
		}
		if( i == -1 ){//diagonal is the maximum of left/rigth and top/bottom
			getGap = SpaceArray[WallMap[x-1][y+i]].Bottom +  SpaceArray[WallMap[x][y]].Top;
		  if(  WallMap[x-1][y+i] != WallMap[x][y] && maxGap < getGap && WallMap[x-1][y+i] > 1 ){
			maxGap = getGap;
		  }
		  getGap = SpaceArray[WallMap[x+1][y+i]].Bottom +  SpaceArray[WallMap[x][y]].Top;
		  if(  WallMap[x+1][y+i] != WallMap[x][y]  && maxGap < getGap && WallMap[x+1][y+i] > 1 ){
			maxGap = getGap;
		  }
		}
		if( i == 1 ){//diagonal is the maximum of left/rigth and top/bottom
			getGap = SpaceArray[WallMap[x-1][y+i]].Top +  SpaceArray[WallMap[x][y]].Bottom;
		  if(  WallMap[x-1][y+i] != WallMap[x][y] && maxGap < getGap && WallMap[x-1][y+i] > 1 ){
			maxGap = getGap;
		  }
		  getGap = SpaceArray[WallMap[x+1][y+i]].Top +  SpaceArray[WallMap[x][y]].Bottom;
		  if(  WallMap[x+1][y+i] != WallMap[x][y]  && maxGap < getGap && WallMap[x+1][y+i] > 1 ){
			maxGap = getGap;
		  }
		}

	}
*/

int Hgap = 0;
int Vgap = 0;

// a 2x2 part of the walkable array
//check for gaps between each of the tiles
//TODO: special case for zerglings
int BuildingManager::MaxGap(int x, int y){
	int maxGap = 0;

	Hgap = 0;
	Vgap = 0;
	//inside building, not possible to walk here
	if(  WallMap[(x/4)][(y/4)] == WallMap[((x+1)/4)][(y/4)] && WallMap[(x/4)][(y/4)] == WallMap[(x/4)][((y+1)/4)]
	     && WallMap[(x/4)][(y/4)] > 1){
			 Hgap = 0;
			 Vgap = 0;
			 return 0;
	}

   bool OtherBuilding = false;
	int Xm[8] = {-1,1,0,0,-1,1,1,-1};
	int Ym[8] = {0,0,-1,1,-1,-1,1,1};
	for(int i=0; i<8; i++){
		int checkX = x + Xm[i];
		int checkY = y + Ym[i];
		if( WallMap[(checkX/4)][(checkY/4)] > 1 && WallMap[(x/4)][(y/4)] !=  WallMap[(checkX/4)][(checkY/4)]  ){
			OtherBuilding = true;
		}
	}

    if( OtherBuilding == false ){
			 Hgap = 0;
			 Vgap = 0;
			 return 0;
	}


	bool BGhorizontal = false;//check if there is a horizontal building gap. Needed for diagonal gap
	bool BGvertical = false;//check if there is a vertical building gap. Needed for diagonal gap

	bool Diagonal = false;

	Hgap = 32;

	/*
	//no need to check horizontal
	if( WallMap[(x/4)][(y/4)] == WallMap[((x+1)/4)][(y/4)] ){
		BGhorizontal = true;
	}
	*/


	int horizontal1 = 32;
	//building gap 1 to the left
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][(y/4)] && WallMap[((x+1)/4)][(y/4)] > 1
		 ){
		horizontal1 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][(y/4)]].Left;
		//BGhorizontal = true;
	}
	/*
	if(  !Broodwar->isWalkable(x+1,y) ){
		horizontal1 = 16;
	}
	*/
	if(  !Broodwar->isWalkable(x+1,y) ){
		horizontal1 = SpaceArray[WallMap[(x/4)][(y/4)]].Right;
	}
	if( Hgap > horizontal1 ){
		 Hgap = horizontal1;
		 BGhorizontal = true;
	}
	//y+1
	int horizontal2 = 32;
	//building gap diagonal 1 to right and 1 down 
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][((y+1)/4)] && WallMap[((x+1)/4)][((y+1)/4)] > 1
		&&   WallMap[(x/4)][((y+1)/4)] <= 1 &&   WallMap[((x+1)/4)][(y/4)] <= 1
		 && BGhorizontal == false  ){
		horizontal2 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][((y+1)/4)]].Left;
		//horizontal2 = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom + SpaceArray[WallMap[((x+1)/4)][((y+1)/4)]].Top;
		BGhorizontal = true;
		Diagonal = true;
	}
	if(  !Broodwar->isWalkable(x+1,y+1)  ){
		horizontal2 = SpaceArray[WallMap[(x/4)][(y/4)]].Right;
	}
	if( Hgap > horizontal2 ){
		 Hgap = horizontal2;
		 BGhorizontal = true;
	}
	int horizontal3 = 32;
	//building gap diagonal 1 to left and 1 up
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][((y-1)/4)] && WallMap[((x+1)/4)][((y-1)/4)] > 1
		&&   WallMap[(x/4)][((y-1)/4)] <= 1 && WallMap[((x+1)/4)][(y/4)] <= 1
		 && BGhorizontal == false  ){
		horizontal3 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][((y-1)/4)]].Left;
		BGhorizontal = true;
		Diagonal = true;
	}
	if(  !Broodwar->isWalkable(x+1,y-1)  ){
		horizontal3 = SpaceArray[WallMap[(x/4)][(y/4)]].Right;
	}
	if( Hgap > horizontal3 ){
		 Hgap = horizontal3;
		 BGhorizontal = true;
	}
	/*
	//TODO: shorter way of writing this down with  ?  macro
	Hgap = horizontal1;
	if(  horizontal2 > horizontal1 ){
		Hgap =  horizontal2;
	}
    */

	Vgap = 32;
	/*
	//no need to check vertical
	if( WallMap[(x/4)][(y/4)] == WallMap[(x/4)][((y+1)/4)] ){
		BGvertical = true;
	}
	*/

	//Now vertical gap
	int vertical1 = 32;
	//building gap
	if(  WallMap[(x/4)][(y/4)] != WallMap[(x/4)][((y+1)/4)] && WallMap[(x/4)][((y+1)/4)] > 1
		 && BGvertical == false ){
		vertical1 = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom + SpaceArray[WallMap[(x/4)][((y+1)/4)]].Top;
		BGvertical = true;
	}
	if(  !Broodwar->isWalkable(x,y+1) ){
		//vertical1 = 16;
		vertical1 = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom;
	}
	if( Vgap > vertical1 ){
		 Vgap = vertical1;
		 BGvertical = true;
	}
	//x+1
	int vertical2 = 32;
	//building gap diagonal 1 right 1 down
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][((y+1)/4)] && WallMap[((x+1)/4)][((y+1)/4)] > 1
		&&   WallMap[(x/4)][((y+1)/4)] <= 1 && WallMap[((x+1)/4)][(y/4)] <= 1
		 && BGvertical == false ){
		vertical2 = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom + SpaceArray[WallMap[((x+1)/4)][((y+1)/4)]].Top;
		//vertical2 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][((y+1)/4)]].Left;
		BGvertical = true;
		Diagonal = true;
	}
	if(  !Broodwar->isWalkable(x+1,y+1) ){
		vertical2 = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom;
	}
	if( Vgap > vertical2 ){
		 Vgap = vertical2;
		 BGvertical = true;
	}


	int vertical3 = 32;
	//building gap diagonal 1 right  1 up
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][((y-1)/4)] && WallMap[((x+1)/4)][((y-1)/4)] > 1
		&&   WallMap[(x/4)][((y-1)/4)] <= 1 && WallMap[((x+1)/4)][(y/4)] <= 1
		&& BGvertical == false  ){
		vertical3 = SpaceArray[WallMap[(x/4)][(y/4)]].Top + SpaceArray[WallMap[((x+1)/4)][((y-1)/4)]].Bottom;
		//vertical3 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][((y-1)/4)]].Left;
		BGvertical = true;
		Diagonal = true;
	}
	if(  !Broodwar->isWalkable(x+1,y-1) ){
		vertical3 = SpaceArray[WallMap[(x/4)][(y/4)]].Top;
	}
	if( Vgap > vertical3 ){
		 Vgap = vertical3;
		 BGvertical = true;
	}

	/*
	int vertical4 = 32;
	//building gap 1 up
	if(  WallMap[(x/4)][(y/4)] != WallMap[(x/4)][((y-1)/4)] && WallMap[(x/4)][((y-1)/4)] > 1
		 && BGvertical == false ){
		vertical4 = SpaceArray[WallMap[(x/4)][(y/4)]].Top + SpaceArray[WallMap[(x/4)][((y-1)/4)]].Bottom;
		BGvertical = true;
	}
	if(  !Broodwar->isWalkable(x,y-1) ){
		vertical4 = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom;
	}
	if( Vgap > vertical4 ){
		 Vgap = vertical4;
		 BGvertical = true;
	}
	*/

	/*
	//no gap horizontal
	if( BGhorizontal == false ){
		//Hgap = 0;
	}
	//no gap vertical
	if( BGvertical == false ){
		//Vgap = 0;
	}
	*/

	if( Vgap == 32 ){
		if(  !Broodwar->isWalkable(x-1,y+1) ){
			Vgap = SpaceArray[WallMap[(x/4)][(y/4)]].Bottom;
		}
	}




     if( Diagonal == true ){
        int bigest = Vgap;
		if( Hgap > Vgap ){
			bigest = Hgap;
		}
		Hgap = bigest;
		Vgap = bigest;
	 }

	/*
	//TODO: shorter way of writing this down with  ?  macro
	Vgap = vertical1;
	if(  vertical2 > vertical1 ){
		Vgap =  horizontal2;
	}
	*/



	/*
	//Now diagonal gap
	int diagonal1 = 0;//diagonal gap is only relevant in building gap
	//diagonal gap between buildings is equal to horizontal gap
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][((y+1)/4)] && WallMap[(x/4)][(y/4)] > 1 && WallMap[((x+1)/4)][((y+1)/4)] > 1){
		diagonal1 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][((y+1)/4)]].Left;
	}
	//x+1
	int diagonal2 = 0;
	//	if(  WallMap[(x/4)][((y+1)/4)] != WallMap[((x+1)/4)][(y/4)] && WallMap[(x/4)][((y+1)/4)] > 1 && WallMap[((x+1)/4)][(y/4)] > 1){
	if(  WallMap[(x/4)][(y/4)] != WallMap[((x+1)/4)][((y-1)/4)] && WallMap[(x/4)][(y/4)] > 1 && WallMap[((x+1)/4)][((y-1)/4)] > 1){
		diagonal2 = SpaceArray[WallMap[(x/4)][(y/4)]].Right + SpaceArray[WallMap[((x+1)/4)][((y-1)/4)]].Left;
	}
	//TODO: shorter way of writing this down with  ?  macro
	int MinDiagonal = diagonal1;
	if(  diagonal2 < diagonal1 || diagonal1 == 0 ){
		MinDiagonal =  diagonal2;
	}
	
	
	//diagonal gap counts as a horizontal gap only if no other horizontal gap is present
	if(  MinDiagonal > Hgap &&  MinDiagonal != 0 && BGhorizontal == false && BGvertical == false){
		Hgap = MinDiagonal;
	}
	*/


	/*
	maxGap = MinVertical;
	if(  MinHorizontal != 0 &&  (MinHorizontal < MinVertical || MinVertical == 0) ){
		maxGap = MinHorizontal;
	}
	*/

	return maxGap;
}

//A star version of the wall off
//Ignore = check if it is passable when barrcks is removed


/*
bool BuildingManager::WalledOffAstar(int x, int y, int Ignore ){
	bool isWalked = false;
	if( Visited[x][y] == 1 ){
		return false;
	}
	Visited[x][y] = 1;
	if( Furthest.x() == (int)(x/4) && Furthest.y() == (int)(y/4) ){
		return true;
	}
	int Xm[4] = {-1,1,0,0};
	int Ym[4] = {0,0,-1,1};
	for(int i=0; i<4; i++){
	  int newX = ( x + Xm[i] )/4;
	  int newY = (y + Ym[i] )/4;
	  int realX = x + Xm[i];
	  int realY = y + Ym[i];
	  if( newX < 0 || newX == mapW || newY < 0 || newY == mapH ){//bound check
	 	continue;
	  }
	  //check 16x16 bound
	  if( newX < TopLeft.x()-1 || newX > BottomRight.x()+1 || newY < TopLeft.y()-1 || newY > BottomRight.y()+1 ){//bound check
	 	continue;
	  }
	  if( Broodwar->isWalkable(x,y) ){
		  //Check if there is a building
		  bool Walkable = false;
		  if( WallMap[newX][newY] <= 1 ||  WallMap[newX][newY] == Ignore ){
			  Walkable = true;
		  }
		  //if there is a building, check if you can walk between the gap
		  if( Walkable == false  ){
			  
			  //int gap = MaxGap( realX, realY);
			  //if( gap >= 23 ){
				//  Walkable = true;
			  //}
			  
			  MaxGap( realX, realY);
			  if( Hgap >= unitH && Vgap >= unitW ){
				  Walkable = true;
			  }
		  }
		  if( Walkable == true ){
		    if(  WalledOffAstar(realX,realY,Ignore) ){
			  isWalked = true;
		    }
		  }
	  } 
	  
	 // if( WallMap[newX][newY] <= 1 && WalkMap[newX][newY] ){
		//  if(  WalledOff(newX,newY) ){
		//	  isWalked = true;
		 // }
	 // }
	  
	}
	return isWalked;
}

*/



//Ignore = check if it is passable when barrcks is removed
bool BuildingManager::WalledOff(int x, int y, int Ignore, int unitH, int unitW){
	bool isWalked = false;
	if( Visited[x][y] == 1 ){
		return false;
	}
	Visited[x][y] = 1;
	if( Furthest.x() == (int)(x/4) && Furthest.y() == (int)(y/4) ){
		return true;
	}
	int Xm[4] = {-1,1,0,0};
	int Ym[4] = {0,0,-1,1};
	for(int i=0; i<4; i++){
	  int newX = ( x + Xm[i] )/4;
	  int newY = (y + Ym[i] )/4;
	  int realX = x + Xm[i];
	  int realY = y + Ym[i];
	  if( newX < 0 || newX == mapW || newY < 0 || newY == mapH ){//bound check
	 	continue;
	  }
	  //check 16x16 bound
	  if( newX < TopLeft.x()-1 || newX > BottomRight.x()+1 || newY < TopLeft.y()-1 || newY > BottomRight.y()+1 ){//bound check
	 	continue;
	  }
	  //if( Broodwar->isWalkable(x,y) ){
	  if( Broodwar->isWalkable(realX,realY) ){
		  //Check if there is a building
		  bool Walkable = false;
		  if( WallMap[newX][newY] <= 1 ||  WallMap[newX][newY] == Ignore ){
			  Walkable = true;
		  }
		  //if there is a building, check if you can walk between the gap
		  if( Walkable == false  ){
			  /*
			  int gap = MaxGap( realX, realY);
			  if( gap >= 23 ){
				  Walkable = true;
			  }
			  */
			  MaxGap( realX, realY);
			  if( Hgap >= unitW && Vgap >= unitH ){
				  Walkable = true;
			  }
		  }
		  if( Walkable == true ){
		    if(  WalledOff(realX,realY,Ignore, unitH, unitW) ){
			  isWalked = true;
		    }
		  }
	  } 
	  /*
	  if( WallMap[newX][newY] <= 1 && WalkMap[newX][newY] ){
		  if(  WalledOff(newX,newY) ){
			  isWalked = true;
		  }
	  }
	  */
	}
	return isWalked;
}


//struct used for A* priority queue
struct PrioAstar{
	int x;
	int y;
	int ManDist; //manhattan distance
  bool operator()(const PrioAstar& lhs, const PrioAstar& rhs) const
  {
    return lhs.ManDist > rhs.ManDist;
  }

};


bool BuildingManager::WalledOffAstar(int Startx, int Starty, int Ignore, int unitH, int unitW){
	//Astar using manhattan distance only. Don't need path minimized, just find a path
	std::priority_queue<PrioAstar, std::vector<PrioAstar>, PrioAstar> pqAstar;
	PrioAstar start;
	start.x = Startx;
	start.y = Starty;
	int TileX = (int)(Startx/4); 
	int TileY = (int)(Starty/4);
	start.ManDist =  abs( TileX - Furthest.x() ) + abs( TileY - Furthest.y() );
	pqAstar.push( start );
	while( !pqAstar.empty() ){
	  PrioAstar cur = pqAstar.top();
	  pqAstar.pop();
	  Visited[cur.x][cur.y] = 1;

	  if( Furthest.x() == (int)(cur.x/4) && Furthest.y() == (int)(cur.y/4) ){
		return true;
	  }
	  int Xm[4] = {-1,1,0,0};
	  int Ym[4] = {0,0,-1,1};
	  for(int i=0; i<4; i++){
	    int newX = ( cur.x + Xm[i] )/4;
	    int newY = (cur.y + Ym[i] )/4;
	    int realX = cur.x + Xm[i];
	    int realY = cur.y + Ym[i];
		if(  Visited[realX][realY]  == 1 ){ //already visited
			continue;
		}
	    if( newX < 0 || newX == mapW || newY < 0 || newY == mapH ){//bound check
	 	continue;
	    }
	    //check 16x16 bound
	    if( newX < TopLeft.x()-1 || newX > BottomRight.x()+1 || newY < TopLeft.y()-1 || newY > BottomRight.y()+1 ){//bound check
	 	  continue;
	    }
	    if( Broodwar->isWalkable(realX,realY) ){
		  //Check if there is a building
		  bool Walkable = false;
		  if( WallMap[newX][newY] <= 1 ||  WallMap[newX][newY] == Ignore ){
			  Walkable = true;
		  }
		  //if there is a building, check if you can walk between the gap
		  if( Walkable == false  ){
			  /*
			  int gap = MaxGap( realX, realY);
			  if( gap >= 23 ){
				  Walkable = true;
			  }
			  */
			  MaxGap( realX, realY);
			  if( Hgap >= unitW && Vgap >= unitH ){
				  Walkable = true;
			  }
		  }
		  if( Walkable == true ){ //can add to prio queue
			  PrioAstar newTile;
			  newTile.x = realX;
			  newTile.y = realY;
			  int newTileX = (int)(newTile.x/4); 
			  int newTileY = (int)(newTile.y/4);
			  newTile.ManDist =  abs( newTileX - Furthest.x() ) + abs( newTileY - Furthest.y() );
			  pqAstar.push( newTile );
		  }
	  } 
	  /*
	  if( WallMap[newX][newY] <= 1 && WalkMap[newX][newY] ){
		  if(  WalledOff(newX,newY) ){
			  isWalked = true;
		  }
	  }
	  */
	}





	}

	//tile not reachable, so no wall
	return false;

}


clock_t begin_time_func;

void BuildingManager::RecursiveWall(std::vector<int> Buildings, int depth){

	
	if( WallSound == true ){ //wall in found
		return;
	}
	
	int timedifFunc = (float( clock () - begin_time_func ) /  CLOCKS_PER_SEC) * 1000.0;
	if( timedifFunc > 6000  && UseTimer ){
		Broodwar->printf("Wall calculation out of time" );
		return;
	}


	//Broodwar->printf(" recurse" );
	if( depth ==  Buildings.size() ){
		//Broodwar->printf(" end" );
		/*
		for(int x=TopLeft.x(); x<=BottomRight.x(); x++ ){
	      for(int y=TopLeft.y(); y<=BottomRight.y(); y++ ){
			   Visited[x][y] = 0;
	         }
			}
			*/

		const int SizeWipe = 4*2;

		for(int x=(TopLeft.x()*4)-SizeWipe; x<=(BottomRight.x()*4)+SizeWipe; x++ ){
	      for(int y=(TopLeft.y()*4)-SizeWipe; y<=(BottomRight.y()*4)+SizeWipe; y++ ){
			  if( x < 0 || y < 0 || x >= mapW*4 || y >= mapH*4 ){
				  continue;
			  }
			   Visited[x][y] = 0;
	         }
			}


		//bool isWalked = WalledOff(Closest.x()*4,Closest.y()*4,1, unitHeight , unitWidth);//check if this is a wall
		bool isWalked = WalledOffAstar(Closest.x()*4,Closest.y()*4,1, unitHeight , unitWidth);//check if this is a wall
		if( isWalked == false ){
		  for(int x=(TopLeft.x()*4)-SizeWipe; x<=(BottomRight.x()*4)+SizeWipe; x++ ){
	        for(int y=(TopLeft.y()*4)-SizeWipe; y<=(BottomRight.y()*4)+SizeWipe; y++ ){
			    if( x < 0 || y < 0 || x >= mapW*4 || y >= mapH*4 ){
				  continue;
			    }
			     Visited[x][y] = 0;
	           }
			  }
		   //Check if lifting the barracks allows passage
		    //bool Gate = WalledOff(Closest.x()*4,Closest.y()*4,2, unitHeight , unitWidth);
		    bool Gate = WalledOffAstar(Closest.x()*4,Closest.y()*4,2, unitHeight , unitWidth);
		    //bool Gate = WalledOffAstar(Closest.x()*4,Closest.y()*4,2, 32 ,32); //tank passage

			bool MarineFit = true;

		    for(int x=(TopLeft.x()*4)-SizeWipe; x<=(BottomRight.x()*4)+SizeWipe; x++ ){
	          for(int y=(TopLeft.y()*4)-SizeWipe; y<=(BottomRight.y()*4)+SizeWipe; y++ ){
			    if( x < 0 || y < 0 || x >= mapW*4 || y >= mapH*4 ){
				  continue;
			    }
			     Visited[x][y] = 0;
	           }
			  }

			/*
			//check if a marine can fit through the gaps
			if( CurrentStrategy == One_Fact_3_Rax ){
				MarineFit = WalledOff(Closest.x()*4,Closest.y()*4,0,17,17);
			}
			*/

			//Check if the Command center is used to fill the building gap, not the terrain gap
			//bool CorrectCC = WalledOff(Closest.x()*4,Closest.y()*4,5,23,19);


			if( Gate == true && MarineFit == true ){ //&& CorrectCC == false 
			   if( WallSound == false){
			    Broodwar->printf("Wall in Found" );
			    WallSound = true;
			   }
			   BarracksWall = BuildingPlace[0];
	           SupplyWall1 = BuildingPlace[1];
	           SupplyWall2 = BuildingPlace[2];

			   //CC1 = BuildingPlace[2];
			   //Academy1 = BuildingPlace[3];

			   if( WallSound == true ){
				   return;
			   }

			}
		}
		return;
	}
	BWAPI::TilePosition CurrentPos;
	int BuildType = Buildings[depth];
	//Buildings.pop_back();
	for(int x=TopLeft.x(); x<=BottomRight.x()-SpaceArray[BuildType].width; x++ ){
		for(int y=TopLeft.y(); y<=BottomRight.y()-SpaceArray[BuildType].heigth; y++ ){
			/*
          if(  x == TopLeft.x() && abs( x - TopLeft.x() ) < 3 ){
			  continue;
		  }
          if(  y == TopLeft.y() && abs( y - TopLeft.y() ) < 3 ){
			  continue;
		  }
          if(  x == BottomRight.x()-SpaceArray[BuildType].width && abs( x - (BottomRight.x()-SpaceArray[BuildType].width) ) < 3 ){
			  continue;
		  }
          if(  y == BottomRight.y()-SpaceArray[BuildType].width && abs( y - (BottomRight.y()-SpaceArray[BuildType].width) ) < 3 ){
			  continue;
		  }
		  */
          if(   CanWall( BuildType,x,y) ){ 
			//Broodwar->printf(" Can build" );
            mapWall(x, y, BuildType, BuildType);
			BWAPI::TilePosition thisTile = BWAPI::TilePosition(x,y);
			BuildingPlace.push_back(thisTile);
			RecursiveWall(Buildings, depth+1);
			BuildingPlace.pop_back();
			mapWall(x, y, BuildType, BUILDABLE);
		  } else {
			//Broodwar->printf(" Cant build" );
		  }
	  }
	}
	//Broodwar->printf(" map: %d",Buildings.back() );
	//Buildings.pop_back();
	//RecursiveWall(Buildings);
}


void BuildingManager::mapWall2(MyRectangle c, int label, int expand){
	for (int x = c.x1-expand; x <= c.x2+expand; x++) {
		for (int y = c.y1-expand; y <= c.y2+expand; y++) {
			if (x >= 0 && x < mapW && y >= 0 && y < mapH) {
				WallMap[x][y] = label;
			}
		}
	}
}



void BuildingManager::CheckWall()
{


	useWallIO = true;

	ReadWallData();

        BWTA::Region* home  = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
        std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
		//BWAPI::Position ThisChoke; 
		//BWAPI::Position ThisChoke; 
		BWAPI::TilePosition TileChoke;
		int LowestDist = 9999999;

        for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++)
        {
			//Broodwar->printf("X:%d",(*c)->getCenter().x());
			BWAPI::Position Current = BWAPI::Position((*c)->getCenter().x(),(*c)->getCenter().y());
			//check if this choke point is closer to the starting location
			if(  Broodwar->self()->getStartLocation().getDistance( TilePosition(Current) ) < LowestDist ){
			  TileChoke = TilePosition(Current);
			  LowestDist = Broodwar->self()->getStartLocation().getDistance( TilePosition(Current) );
			  //toDefend = (*c);
			}
			//(*c)->getCenter
        }
		
		//Broodwar->printf("X:");
		
		TileChoke = BWAPI::TilePosition( InfoMan->PosMainChoke );

	  std::vector<int> Buildings;
	  Buildings.push_back(2);//Barracks
	  Buildings.push_back(3);//Supply depot
	  Buildings.push_back(4);//Supply depot
	  //Buildings.push_back(5);// Academy

	  int BuildSize = 9;

	  int TopX = TileChoke.x() - BuildSize;
	  if( TopX < 0 ){
		  TopX = 0;
	  }
	  int TopY = TileChoke.y() - BuildSize;
	  if( TopY < 0 ){
		  TopY = 0;
	  }
	  TopLeft = TilePosition( TopX, TopY );
	  Top1Left = TilePosition( TopX+1, TopY+1 );

	  int BottomX = TileChoke.x() + BuildSize;
	  if( BottomX >= mapW ){
		  BottomX = mapW - 1;
	  }
	  int BottomY = TileChoke.y() + BuildSize;
	  if( BottomY >= mapH ){
		  BottomY = mapH - 1;
	  }
	  BottomRight = TilePosition( BottomX, BottomY );
	  Bottom1Right = TilePosition( BottomX-1, BottomY-1 );

	  //Broodwar->printf("X:%d , Y: %d", TopX , TopY );
	  
	  //Get nearest and furthest buildable position

	  BWAPI::TilePosition CommandC;
	  for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
      {
		if ( (*i)->getType().isResourceDepot() ){
			CommandC = BWAPI::TilePosition(  (*i)->getPosition() );
		  }
	  }

	  //getGroundDistance
	  double MaxT = 0;
	  double MinT = 9999;
	  for(int x = TopLeft.x(); x<BottomRight.x(); x++){
	    for(int y = TopLeft.y(); y<BottomRight.y(); y++){
			if( !Broodwar->isBuildable(x, y) || !WalkMap[x][y] ){
				continue;
			}
			if( Broodwar->getGroundHeight(x,y) > MaxHighGround && (Broodwar->getGroundHeight(x,y) % 2  == 0) ){
				MaxHighGround = Broodwar->getGroundHeight(x,y);
			}
			BWAPI::TilePosition Current = BWAPI::TilePosition(x,y);
			double getDist = BWTA::getGroundDistance(CommandC,Current);
			if( getDist < MinT ){
				MinT = getDist;
				Closest = Current;
			}
			/*
			if( getDist > MaxT ){
				MaxT = getDist;
				Furthest = Current;
			}
			*/
	    }
	  }

	  /*
	  if( MaxHighGround % 2 != 0 ){//remove doodads
		  MaxHighGround--;
	  }
	  */

	  for(int x = TopLeft.x(); x<BottomRight.x(); x++){
	    for(int y = TopLeft.y(); y<BottomRight.y(); y++){
			if( !Broodwar->isBuildable(x, y) || !WalkMap[x][y] ){
				continue;
			}
			BWAPI::TilePosition backup = Furthest;
			BWAPI::TilePosition Current = BWAPI::TilePosition(x,y);
			double getDist = BWTA::getGroundDistance(CommandC,Current);
			if( getDist > MaxT ){
				//MaxT = getDist;
				Furthest = Current;
				//Check if furthers is connected
				for(int x=(TopLeft.x()*4)-4; x<=(BottomRight.x()*4)+4; x++ ){
	              for(int y=(TopLeft.y()*4)-4; y<=(BottomRight.y()*4)+4; y++ ){
			        if( x < 0 || y < 0 || x >= mapW*4 || y >= mapH*4 ){
				      continue;
			        }
			       Visited[x][y] = 0;
	             }
			    }
		        bool isWalked = WalledOff(Closest.x()*4,Closest.y()*4,1,32,32);//check if this spot is reachable
				if( isWalked == false ){//if not reachable in 32x32, revert
                   Furthest = backup;
				} else {
					MaxT = getDist;
				}
			}
	    }
	  }

	  //BarracksWall = TilePosition( rand()%2 + 2, rand()%2 + 2);
	  //SupplyWall1 = Closest;
	  //SupplyWall2 = Furthest;
	  //useHighGround = false;


    //using namespace boost::chrono;
    //thread_clock::time_point start = thread_clock::now();
    //thread_clock::time_point stop = thread_clock::now();  
	//int timediff2 = (stop - start).count();
   // std::cout << "duration: " << duration_cast<milliseconds>(stop - start).count() << " ms\n";


	  //set the unit heigth and width
	  unitHeight = 19;
	  unitWidth = 23;


	  mapWall( BarracksWall.x(), BarracksWall.y(), 2 ,2 );
	  mapWall( SupplyWall1.x(), SupplyWall1.y(), 3 ,3 );
	  mapWall( SupplyWall2.x(), SupplyWall2.y(), 4 ,4 );




		for(int x=(TopLeft.x()*4)-4*4; x<=(BottomRight.x()*4)+4*4; x++ ){
	      for(int y=(TopLeft.y()*4)-4*4; y<=(BottomRight.y()*4)+4*4; y++ ){
			  if( x < 0 || y < 0 || x >= mapW*4 || y >= mapH*4 ){
				  continue;
			  }
			   Visited[x][y] = 0;
	         }
			}


		//bool isWalked = WalledOff(Closest.x()*4,Closest.y()*4,1, unitHeight , unitWidth);//check if this is a wall
		bool isWalked = WalledOffAstar(Closest.x()*4,Closest.y()*4,1, unitHeight , unitWidth);//check if this is a wall



}


//Find locations to wall of the choke
void BuildingManager::WallOff(){

		const clock_t begin_time = clock();
	    begin_time_func = begin_time;

        BWTA::Region* home  = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
        std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
		//BWAPI::Position ThisChoke; 
		BWAPI::TilePosition TileChoke;
		int LowestDist = 9999999;

        for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++)
        {
			//Broodwar->printf("X:%d",(*c)->getCenter().x());
			BWAPI::Position Current = BWAPI::Position((*c)->getCenter().x(),(*c)->getCenter().y());
			//check if this choke point is closer to the starting location
			if(  Broodwar->self()->getStartLocation().getDistance( TilePosition(Current) ) < LowestDist ){
			  TileChoke = TilePosition(Current);
			  LowestDist = Broodwar->self()->getStartLocation().getDistance( TilePosition(Current) );
			  //toDefend = (*c);
			}
			//(*c)->getCenter
        }
		
		//Broodwar->printf("X:");
		
		TileChoke = BWAPI::TilePosition( InfoMan->PosMainChoke );

	  std::vector<int> Buildings;
	  Buildings.push_back(2);//Barracks
	  Buildings.push_back(3);//Supply depot
	  Buildings.push_back(4);//Supply depot
	  //Buildings.push_back(5);// Academy

	  int BuildSize = 9;

	  int TopX = TileChoke.x() - BuildSize;
	  if( TopX < 0 ){
		  TopX = 0;
	  }
	  int TopY = TileChoke.y() - BuildSize;
	  if( TopY < 0 ){
		  TopY = 0;
	  }
	  TopLeft = TilePosition( TopX, TopY );
	  Top1Left = TilePosition( TopX+1, TopY+1 );

	  int BottomX = TileChoke.x() + BuildSize;
	  if( BottomX >= mapW ){
		  BottomX = mapW - 1;
	  }
	  int BottomY = TileChoke.y() + BuildSize;
	  if( BottomY >= mapH ){
		  BottomY = mapH - 1;
	  }
	  BottomRight = TilePosition( BottomX, BottomY );
	  Bottom1Right = TilePosition( BottomX-1, BottomY-1 );

	  //Broodwar->printf("X:%d , Y: %d", TopX , TopY );
	  
	  //Get nearest and furthest buildable position

	  BWAPI::TilePosition CommandC;
	  for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
      {
		if ( (*i)->getType().isResourceDepot() ){
			CommandC = BWAPI::TilePosition(  (*i)->getPosition() );
		  }
	  }

	  //getGroundDistance
	  double MaxT = 0;
	  double MinT = 9999;
	  for(int x = TopLeft.x(); x<BottomRight.x(); x++){
	    for(int y = TopLeft.y(); y<BottomRight.y(); y++){
			if( !Broodwar->isBuildable(x, y) || !WalkMap[x][y] ){
				continue;
			}
			if( Broodwar->getGroundHeight(x,y) > MaxHighGround && (Broodwar->getGroundHeight(x,y) % 2  == 0) ){
				MaxHighGround = Broodwar->getGroundHeight(x,y);
			}
			BWAPI::TilePosition Current = BWAPI::TilePosition(x,y);
			double getDist = BWTA::getGroundDistance(CommandC,Current);
			if( getDist < MinT ){
				MinT = getDist;
				Closest = Current;
			}
			/*
			if( getDist > MaxT ){
				MaxT = getDist;
				Furthest = Current;
			}
			*/
	    }
	  }

	  /*
	  if( MaxHighGround % 2 != 0 ){//remove doodads
		  MaxHighGround--;
	  }
	  */

	  for(int x = TopLeft.x(); x<BottomRight.x(); x++){
	    for(int y = TopLeft.y(); y<BottomRight.y(); y++){
			if( !Broodwar->isBuildable(x, y) || !WalkMap[x][y] ){
				continue;
			}
			BWAPI::TilePosition backup = Furthest;
			BWAPI::TilePosition Current = BWAPI::TilePosition(x,y);
			double getDist = BWTA::getGroundDistance(CommandC,Current);
			if( getDist > MaxT ){
				//MaxT = getDist;
				Furthest = Current;
				//Check if furthers is connected
				for(int x=(TopLeft.x()*4)-4; x<=(BottomRight.x()*4)+4; x++ ){
	              for(int y=(TopLeft.y()*4)-4; y<=(BottomRight.y()*4)+4; y++ ){
			        if( x < 0 || y < 0 || x >= mapW*4 || y >= mapH*4 ){
				      continue;
			        }
			       Visited[x][y] = 0;
	             }
			    }
		        bool isWalked = WalledOff(Closest.x()*4,Closest.y()*4,1,32,32);//check if this spot is reachable
				if( isWalked == false ){//if not reachable in 32x32, revert
                   Furthest = backup;
				} else {
					MaxT = getDist;
				}
			}
	    }
	  }

	  //BarracksWall = TilePosition( rand()%2 + 2, rand()%2 + 2);
	  //SupplyWall1 = Closest;
	  //SupplyWall2 = Furthest;
	  //useHighGround = false;


    //using namespace boost::chrono;
    //thread_clock::time_point start = thread_clock::now();
    //thread_clock::time_point stop = thread_clock::now();  
	//int timediff2 = (stop - start).count();
   // std::cout << "duration: " << duration_cast<milliseconds>(stop - start).count() << " ms\n";


	  //set the unit heigth and width
	  unitHeight = 19;
	  unitWidth = 23;
	  //unitHeight = 17;
	  //unitWidth = 17;

	  //const clock_t begin_time = clock();
	  //begin_time_func = begin_time;
	  RecursiveWall(Buildings,0);
// do something

	  if( WallSound == false ){
		  Broodwar->printf( "No wall found");
	  }

	  
	  if( WallSound == false ){
		useHighGround = false;
	    RecursiveWall(Buildings,0);
	  }
	  
	  //total time it took to calculate wall
	  int timediff = (float( clock () - begin_time ) /  CLOCKS_PER_SEC) * 1000.0;
	  Broodwar->printf( "wall calc time %d", timediff );



	  if( WallSound == true ){
	    WallCalculated = true;
	  }

	  if( useWallIO == true && WallSound == true ){ //temporarly dont output, no IO
		  WallData newWD;
		  newWD.mapHash = Broodwar->mapHash();
		  newWD.startTile = InfoMan->OurBase;
		  newWD.Supply1 = bManager->SupplyWall1;
		  newWD.Supply2 = bManager->SupplyWall2;
		  newWD.Barracks1 = bManager->BarracksWall;
		  //newWD.CC1 = bManager->CC1;
		  newWD.Academy1 = bManager->Academy1;
		  //WD.push_back( newWD );
		  //in the competiton, this should be "bwapi-data/write/wall.txt"
          std::ofstream fout("bwapi-data/read/wall.txt", std::fstream::app);//, std::fstream::app
          if( !fout.fail() ){
			  fout<<Broodwar->mapHash()<<"\n";
			  fout<<InfoMan->OurBase.x()<<" "<<InfoMan->OurBase.y()<<"\n";
			  fout<<bManager->SupplyWall1.x()<<" "<<bManager->SupplyWall1.y()<<"\n";
			  fout<<bManager->SupplyWall2.x()<<" "<<bManager->SupplyWall2.y()<<"\n";
			  fout<<bManager->BarracksWall.x()<<" "<<bManager->BarracksWall.y()<<"\n";
			  //fout<<bManager->CC1.x()<<" "<<bManager->CC1.y()<<"\n";
			  //fout<<bManager->Academy1.x()<<" "<<bManager->Academy1.y()<<"\n";
          }

	  }


	  //also check if the producmanager isn't already building
	  //because if this is the case then don't go for a wall
	  bool StopWall = false;
	  //check if supply depot already build
	  BOOST_FOREACH( BWAPI::Unit* building , Broodwar->self()->getUnits() ){
		  if( building->getType().isBuilding() &&
			  building->getType() != BWAPI::UnitTypes::Terran_Command_Center ){
              StopWall = true;
		  }			  
	  }

	  if( ProdMan != NULL ){
	    if( ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Supply_Depot ) > 0
		    || ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0){
              StopWall = true;
	    }
	  }

	  if( StopWall == true ){
	     WallSound == false;
	     WallCalculated = false;
	  }

}
