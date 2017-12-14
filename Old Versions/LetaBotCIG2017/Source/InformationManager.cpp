#include "Common.h"
#include "InformationManager.h"

#define SELF_INDEX 0
#define ENEMY_INDEX 1

// constructor
InformationManager::InformationManager() 
{

	EnemyBuildings.clear();

	EnemyRace = Broodwar->enemy()->getRace();

	//initialization
	TotalSunken = 0;
	
	Broodwar->getStartLocations();
	OurBase = BWAPI::Broodwar->self()->getStartLocation();
	PosOurBase = BWAPI::Position( OurBase );
	OurMainregion = BWTA::getRegion(PosOurBase);
	//EnemyBase = BWTA::getStartLocation(BWAPI::Broodwar->enemy());
	EnemyBase = BWAPI::TilePositions::Unknown;
	PosEnemyBase = BWAPI::Positions::Unknown;
	EnemyNatural = BWAPI::TilePositions::Unknown;



	//our natural
	//Get enemy natural
	int closest = 999999999;
	BOOST_FOREACH( BWTA::BaseLocation* base, BWTA::getBaseLocations() ){
		//check if it is not our base
		if( base->getGroundDistance(  BWTA::getNearestBaseLocation(OurBase) ) < 90 ){
			continue;
		}
		//natural has gas
		if(  base->getGroundDistance(  BWTA::getNearestBaseLocation(OurBase) ) < closest
			&&  base->getGeysers().size() > 0  ){
			closest = base->getGroundDistance(  BWTA::getNearestBaseLocation(OurBase) );
			OurNat = base->getTilePosition();
			//Broodwar->printf("Base distance %d" , closest);
		}
	}
	PosOurNat = BWAPI::Position( OurNat );
	NatRegion = BWTA::getRegion( PosOurNat);

	BOOST_FOREACH(BWAPI::TilePosition  startLocation, Broodwar->getStartLocations()) 
	{
      if(  startLocation !=  BWAPI::Broodwar->self()->getStartLocation() ){
        PossibleBase.push_back(startLocation);
	  }
	}

	Broodwar->printf(  "Total possible bases:%d", PossibleBase.size() );
	if(  OurBase != NULL  ){
	 //Broodwar->printf(  "Our base:%d", OurBase.x() );
	}
	if( PossibleBase.size() == 1 ){ //only one possibility
		//EnemyBase =  PossibleBase[0];
		setLocationEnemyBase( PossibleBase[0] );
	}


	/*
    for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
    {
      Minerals.push_back( *m);
    }
	for(std::set<Unit*>::iterator g=Broodwar->getGeysers().begin();g!=Broodwar->getGeysers().end();g++)
    {
      Geysers.push_back(*g);
    }
	*/
    for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
    {
      Minerals.push_back( *m);
    }
	for(std::set<Unit*>::iterator g=Broodwar->getGeysers().begin();g!=Broodwar->getGeysers().end();g++)
    {
      Geysers.push_back(*g);
    }

	//add information about all gas geysers
	for(UnitSet::iterator g = Broodwar->getGeysers().begin(); g != Broodwar->getGeysers().end(); ++g) {
		GeyserLocations.push_back((*g)->getTilePosition());
	}

	/*
	//The ID of all mineral trick in Fighting Spirit
	int FSminID [] = {20,24,25,9,10,51,15,14,50,30,31,29};
	int sizeFSminID = sizeof(FSminID);
	for(int i=0; i<sizeFSminID; i++){
		FSmineral.MineralID.push_back(FSminID[i]);
	}
	*/




	//BWAPI::Position natPos = BWAPI::Position(InfoMan->OurNat);
	//BWAPI::Position mainPos = BWAPI::Position(InfoMan->PosOurBase);
	BWTA::Region* natRegion = BWTA::getRegion( PosOurNat );
	BWTA::Region* mainRegion = BWTA::getRegion( PosOurBase );

	BWTA::Chokepoint* BunkerChoke = NULL;
   BOOST_FOREACH( BWTA::Chokepoint*  choke, BWTA::getChokepoints() ){
	   if( (choke->getRegions().first == natRegion && choke->getRegions().second == mainRegion) ||
		   (choke->getRegions().second == natRegion && choke->getRegions().first == mainRegion) ){
           BunkerChoke = choke;
	   }
   }
   
   if( BunkerChoke == NULL && mainRegion != NULL && natRegion != NULL ){
	   int closestNat = 99999;
       BOOST_FOREACH( BWTA::Chokepoint*  choke, natRegion->getChokepoints() ){
		   if( BWTA::getGroundDistance(  BWAPI::TilePosition(choke->getCenter() ), OurBase ) < closestNat ){
			   BunkerChoke = choke;
			   closestNat = BWTA::getGroundDistance(  BWAPI::TilePosition(choke->getCenter() ), OurBase );
		   }
	   }
	   //BunkerChoke = mainRegion->getChokepoints()
	   //return;
   }

   /*
   //if the choke is too width, get the choke from the natural
   if( BunkerChoke->getWidth() > 4*32 ){
     BOOST_FOREACH( BWTA::Chokepoint*  choke, BWTA::getChokepoints() ){
	   if( (choke->getRegions().first == natRegion && choke->getRegions().second != mainRegion) ||
		   (choke->getRegions().second == natRegion && choke->getRegions().first != mainRegion) ){
           BunkerChoke = choke;
	   }
     }
   }
   */
   if( BunkerChoke != NULL){
     PosMainChoke = BunkerChoke->getCenter();
   }
   //BWAPI::TilePosition TileChoke = BWAPI::TilePosition( BunkerChoke->getCenter() );
   //TileChoke = bManager->getBuildLocationNear( TileChoke, BWAPI::UnitTypes::Terran_Bunker, natRegion);


}


void InformationManager::NoBaseHere( BWAPI::TilePosition notHere)
{
	for(unsigned int i=0; i<PossibleBase.size(); i++){
		if( PossibleBase[i] == notHere  ){
			PossibleBase.erase( PossibleBase.begin() + i );
			break;
		}
	}
}

void InformationManager::setLocationEnemyBase( BWAPI::TilePosition Here ){
	EnemyBase = Here;
	PosEnemyBase = BWAPI::Position( Here );
	GetNaturalEnemy();

	//add default attack location
	if( MacroMan != NULL ){
	  MacroMan->attackPosT2 = BWAPI::Position( Here );
	  MacroMan->attackPosP = BWAPI::Position( Here );
	  MacroMan->attackPos = BWAPI::Position( Here );
	}

}

void InformationManager::GetNaturalEnemy()
{
	if( EnemyBase == BWAPI::TilePositions::Unknown ){
		Broodwar->printf("Enemy base location unknown");
		return;
	}
	//Get enemy natural
	int closest = 999999999;
	BOOST_FOREACH( BWTA::BaseLocation* base, BWTA::getBaseLocations() ){
		if( base->getGroundDistance(  BWTA::getNearestBaseLocation(EnemyBase) ) < 90 ){
			continue;
		}
		if(  base->getGroundDistance(  BWTA::getNearestBaseLocation(EnemyBase) ) < closest){
			closest = base->getGroundDistance(  BWTA::getNearestBaseLocation(EnemyBase) );
			EnemyNatural = base->getTilePosition();
			//Broodwar->printf("Base distance %d" , closest);
		}
	}

	//EnemyNatural = BWTA::getNearestBaseLocation(EnemyBase)->getTilePosition();

}


void InformationManager::onUnitShow(BWAPI::Unit * unit)
{

	/*
	if(  unit->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
		for(unsigned int i=0; i<Sunkens.size(); i++){
			if( unit->getID() == Sunkens[i]->getID() ){
				//return;//already known
				continue;
			}
		}
		TotalSunken++;
		//Broodwar->printf("seen sunken");
		Sunkens.push_back(unit);
	}
	*/


	if( unit->getPlayer() == Broodwar->enemy() && !unit->getType().isBuilding()
		&& !unit->getType().isWorker()  ){
      for(unsigned int i=0; i<EnemyCombatUnits.size(); i++){
		if( unit->getID() == EnemyCombatUnits[i]->getID() ){
		  return;//already known
		}
      }
	  EnemyCombatUnits.push_back( unit );
	}



	if( unit->getPlayer() == Broodwar->enemy() && unit->getType().isBuilding() ){
      for(unsigned int i=0; i<EnemyBuildings.size(); i++){
		if( unit->getID() == EnemyBuildings[i].building->getID() ){
		  return;//already known
		}
      }
	  BuildingInfo newBuilding;
	  newBuilding.building = unit;
	  newBuilding.position = unit->getPosition();
	  newBuilding.type = unit->getType();
	  EnemyBuildings.push_back( newBuilding );
	}

}

void InformationManager::onUnitDestroy(BWAPI::Unit * unit)
{

	if(  unit->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
		for(unsigned int i=0; i<Sunkens.size(); i++){
			if( unit->getID() == Sunkens[i]->getID() ){
				Sunkens.erase( Sunkens.begin() + i );
				TotalSunken--;
				return;//already known
			}
		}
	}



	if( unit->getPlayer() == Broodwar->enemy() && !unit->getType().isBuilding()
		&& !unit->getType().isWorker()  ){
      for(unsigned int i=0; i<EnemyCombatUnits.size(); i++){
		if( unit->getID() == EnemyCombatUnits[i]->getID() ){
				EnemyCombatUnits.erase( EnemyCombatUnits.begin() + i );
				return;//already known
		}
      }
	  EnemyCombatUnits.push_back( unit );
	}



	if( unit->getPlayer() == Broodwar->enemy() && unit->getType().isBuilding() ){
      for(unsigned int i=0; i<EnemyBuildings.size(); i++){
		  if( unit->getID() == EnemyBuildings[i].building->getID() ){
		  EnemyBuildings.erase(  EnemyBuildings.begin() + i );
		  return;//already known
		}
      }
	}


}