

#include "ProductionManager.h"

#include <boost/foreach.hpp>
#include <windows.h>

ProductionManager::ProductionManager(){


	useMultiThread = false;

	addQueueMutex = CreateMutex( NULL,FALSE,NULL);

	reservedMinerals = 0;
	reservedGas = 0;

	BuildingsQueueType.clear();

	BuildingsQueue.clear();
	//bSCV = NULL;
	EngineeringBay = NULL;
	Academy = NULL;
	ScienceFacility = NULL;
	Armory.clear();

	//Build order for 2 port wraith
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot);
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );

	if( CurrentStrategy  == Macro_Strat){
		BuildingOrder.clear();
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){

			/*
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 13);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 19);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 21);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */

			/*
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 12);	
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 21);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */

		}
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
		  addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 21);
		}
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	      addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
		}

	}

}


int ProductionManager::getAvailableMinerals(){
	return Broodwar->self()->minerals() - reservedMinerals;
}

int ProductionManager::getAvailableGas(){
	return Broodwar->self()->gas() - reservedGas;
}

void ProductionManager::addSupplyBO(BWAPI::UnitType type, int supply){
	BO newBO;
	newBO.type = type;
	newBO.supply = supply;
	BuildingSupplyOrder.push_back(newBO);

}

void ProductionManager::addToBuildOrderBegin(BWAPI::UnitType type, int supply )
{

	    BO newBO;
	    newBO.type = type;
	    newBO.supply = supply;
	    ProdMan->BuildingSupplyOrder.insert( ProdMan->BuildingSupplyOrder.begin(), newBO );


}


void ProductionManager::removeFromQueue(BWAPI::UnitType type)
{

	for( int i=0; i<BuildingsQueue.size(); i++){

		if( BuildingsQueue[i].type == type ){
			if( BuildingsQueue[i].Started == false  ){
			  if( BuildingsQueue[i].scv != NULL ){
				  returnBuilderSCV( BuildingsQueue[i].scv );
			  }
			  BuildingsQueue.erase( BuildingsQueue.begin() + i );
			  break;
			}
		}

	}
}



void ProductionManager::removeFromBuildOrder(BWAPI::UnitType type)
{

	for( int i=0; i<BuildingSupplyOrder.size(); i++){

		if( BuildingSupplyOrder[i].type == type ){
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() + i );
			  break;
		}
	}
}





void ProductionManager::addToQueueBeginTile(BWAPI::UnitType type, BWAPI::TilePosition tile)
{
		reservedMinerals +=  type.mineralPrice();
		reservedGas += type.gasPrice();
		BuildQueue addQueue;

		if( tile == BWAPI::TilePositions::Unknown ){
		  addQueue.buildLocation = bManager->getBuildLocationBase( CCmanager[0]->CommandCenter->getTilePosition(),type,0);
		} else {
		  addQueue.buildLocation = tile;
		}
        //reserve space
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		BuildingsQueue.insert( BuildingsQueue.begin(), addQueue );
		Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );

}



void ProductionManager::addToQueueBegin(BWAPI::UnitType type, BWAPI::TilePosition tileClose)
{
		reservedMinerals +=  type.mineralPrice();
		reservedGas += type.gasPrice();
		BuildQueue addQueue;
		if( tileClose == BWAPI::TilePositions::Unknown ){
		  addQueue.buildLocation = bManager->getBuildLocationBase( CCmanager[0]->CommandCenter->getTilePosition(),type,0);
		} else {
		  addQueue.buildLocation = bManager->getBuildLocationBase( tileClose,type, BWTA::getRegion( InfoMan->PosOurBase ) );
		}
        //reserve space
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		BuildingsQueue.insert( BuildingsQueue.begin(), addQueue );
		Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );

}


DWORD WINAPI addToQueueMultiThread(LPVOID lpParameter)// BWAPI::UnitType type
{

	   DWORD dwCount=0, dwWaitResult; 

       dwWaitResult = WaitForSingleObject( 
            ProdMan->addQueueMutex,    // handle to mutex
            INFINITE);  // no time-out interval

	   BWAPI::UnitType type = *(BWAPI::UnitType*)lpParameter;
	   
		BuildQueue addQueue;
		addQueue.buildLocation = bManager->getBuildLocationBase( CCmanager[0]->CommandCenter->getTilePosition(),type,0);
        //reserve space
		if( addQueue.buildLocation == BWAPI::TilePositions::None ){
		  ProdMan->reservedMinerals -=  type.mineralPrice();
		  ProdMan->reservedGas -= type.gasPrice();
		  ReleaseMutex(ProdMan->addQueueMutex); //release mutex
		  delete lpParameter; //otherwise memory will be leaked
		  return 0;
		}
		//ProdMan->reservedMinerals +=  type.mineralPrice();
		//ProdMan->reservedGas += type.gasPrice();
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = ProdMan->getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		ProdMan->BuildingsQueue.push_back( addQueue );
		//Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );
		ReleaseMutex(ProdMan->addQueueMutex); //release mutex
		delete lpParameter; //otherwise memory will be leaked
		return 0;
}


void ProductionManager::addToQueue(BWAPI::UnitType type)
{

	   //Broodwar->printf("Next building added: %s", type.c_str() );

	   BWAPI::UnitType* Storetype = new BWAPI::UnitType( type );


		//ProdMan->reservedMinerals +=  type.mineralPrice();
		//ProdMan->reservedGas += type.gasPrice();

		BuildingsQueueType.push_back( type );

		if( useMultiThread == true) {
		  ProdMan->reservedMinerals +=  type.mineralPrice();
		  ProdMan->reservedGas += type.gasPrice();
	      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)addToQueueMultiThread, Storetype, 0, NULL);	
		}
		else { 
	   
		BuildQueue addQueue;
		addQueue.buildLocation = bManager->getBuildLocationBase( CCmanager[0]->CommandCenter->getTilePosition(),type,0);
        //reserve space
		if( addQueue.buildLocation == BWAPI::TilePositions::None ){
			return;
		}
		reservedMinerals +=  type.mineralPrice();
		reservedGas += type.gasPrice();
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		BuildingsQueue.push_back( addQueue );
		Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );
		

		}
}

void ProductionManager::addToQueueTile(BWAPI::UnitType type, BWAPI::TilePosition tile)
{
		reservedMinerals +=  type.mineralPrice();
		reservedGas += type.gasPrice();
		BuildQueue addQueue;
		addQueue.buildLocation = tile;
        //reserve space
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		BuildingsQueue.push_back( addQueue );
		Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );

}


void ProductionManager::addToQueueClose(BWAPI::UnitType type, BWAPI::TilePosition tile, BWTA::Region* region)
{
	   //yea I know  NULL == 0 , but just in case of some compiler setting or something like that
	    if( region == NULL ){
			region = 0;
		}
		reservedMinerals +=  type.mineralPrice();
		reservedGas += type.gasPrice();
		BuildQueue addQueue;
		  addQueue.buildLocation = bManager->getBuildLocationBase( tile,type, region );
        //reserve space
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		BuildingsQueue.push_back( addQueue );
		Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );

}

void ProductionManager::addToQueueClose(BWAPI::UnitType type, BWAPI::TilePosition tile, bool inBase)
{
		reservedMinerals +=  type.mineralPrice();
		reservedGas += type.gasPrice();
		BuildQueue addQueue;
		if( inBase ){
		  addQueue.buildLocation = bManager->getBuildLocationBase( tile,type, BWTA::getRegion( InfoMan->PosOurBase ) );
		} else {
		  addQueue.buildLocation = bManager->getBuildLocationBase( tile,type, 0 );
		}
        //reserve space
		bManager->mapArea( bManager->getBuildRectangle(addQueue.buildLocation, type) , 0,0);
		
		addQueue.type = type;
		//addQueue.scv = CCmanager[0]->getBuilder();
		addQueue.scv = getSCVBuilder( type, addQueue.buildLocation );
		addQueue.building = NULL;
		addQueue.Started = false;
		addQueue.FrameStarted = Broodwar->getFrameCount();
		BuildingsQueue.push_back( addQueue );
		Broodwar->printf("Next building added: %s", type.c_str() );
		//BuildingOrder.erase( BuildingOrder.begin() );

}


int ProductionManager::TotalTypeInQueue(BWAPI::UnitType type){
	int total = 0;
		for(unsigned int i=0; i<BuildingsQueue.size(); i++){
			if( BuildingsQueue[i].type == type ){
				total++;
			}
		}
		
		//maybe there is one in BuildingsQueueType
		if( total == 0 && BuildingsQueueType.size() > 0){
		  for(unsigned int i=0; i<BuildingsQueueType.size(); i++){
			if( BuildingsQueueType[i] == type ){
				total++;
			}
		  }
		}
		

	return total;
}

int ProductionManager::TotalUnitInProduction(BWAPI::UnitType type)
{
	int total = 0;
	if( type == BWAPI::UnitTypes::Terran_Science_Vessel){
		for(int i=0; i<StarportCtower.size(); i++){
			if( StarportCtower[i]->getTrainingQueue().size() > 0){
				if( StarportCtower[i]->getTrainingQueue().front() == BWAPI::UnitTypes::Terran_Science_Vessel ){
					total++;
				}

			}
		}

	}
	return total;
}


BWAPI::Unit* ProductionManager::getSCVBuilder( BWAPI::UnitType type, BWAPI::TilePosition position )
{

	//TODO: make a better selection for both the CC and other buildings
	if( type == BWAPI::UnitTypes::Terran_Command_Center ){
		if(  BWTA::getRegion( position ) == InfoMan->OurMainregion
			||  BWTA::getRegion( position ) == InfoMan->NatRegion ){
				return CCmanager[0]->getBuilder();
		} else {
			if( CCmanager.size() > 1 ){
				return CCmanager[1]->getBuilder();
			} else {
				return CCmanager[0]->getBuilder();
			}
		}
	} else {
		
		if(  BWTA::getRegion( position ) == InfoMan->OurMainregion ){
				return CCmanager[0]->getBuilder();
		} else {
			if( CCmanager.size() > 1 ){
				return CCmanager[1]->getBuilder();
			} else {
				return CCmanager[0]->getBuilder();
			}
		}

	}


}




void ProductionManager::returnBuilderSCV( BWAPI::Unit* scv )
{

	if( scv == NULL ){
		return;
	}

	//get nearest base
	int closesBaseDist = 9999;
	int closestID = -1;
	for( int i=0; i< CCmanager.size(); i++ ){

		if(  CCmanager[i]->CommandCenter->getPosition().getDistance( scv->getPosition() ) < closesBaseDist\
			&& CCmanager[i]->BaseReady == true){
			closesBaseDist =  CCmanager[i]->CommandCenter->getPosition().getDistance( scv->getPosition() );
			closestID = i;
		}

	}
	
	if( closestID != -1 ){
		CCmanager[closestID]->addSCV( scv );
	}

}


void ProductionManager::checkSCVbuild()
{

    if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }

	for(unsigned int i=0; i<BuildingsQueue.size(); i++){
		if(  BuildingsQueue[i].scv == NULL || !BuildingsQueue[i].scv->exists() ){
			//Broodwar->printf("No scv assigned. Reassigning");
			//BuildingsQueue[i].scv = CCmanager[0]->getBuilder();
			BuildingsQueue[i].scv = getSCVBuilder( BuildingsQueue[i].type, BuildingsQueue[i].buildLocation);
			continue;
		}
		BWAPI::Position moveTo = BWAPI::Position( BuildingsQueue[i].buildLocation );

		// start frame not set
		if( BuildingsQueue[i].FrameStarted == 0 ){
			BuildingsQueue[i].FrameStarted = Broodwar->getFrameCount();
		}

		//check if the scv is stuck or otherwise unable to build
		if( Broodwar->getFrameCount() - BuildingsQueue[i].FrameStarted > 2500
			||  ( (Broodwar->getFrameCount() - BuildingsQueue[i].FrameStarted > 1500) 
			&& Broodwar->self()->minerals() >= BuildingsQueue[i].type.mineralPrice()
			&& Broodwar->self()->gas() >= BuildingsQueue[i].type.gasPrice()  )  ){
			bool buildingExists = true;
			if( BuildingsQueue[i].building == NULL ){
				buildingExists = false;
			}
			if( buildingExists){
				if( !BuildingsQueue[i].building->exists() ){
					buildingExists = false;
				}
			}
			if( !buildingExists ){
			Broodwar->printf("SCV stuck. Reassigning");
			Unit* stuck = BuildingsQueue[i].scv;
			//BuildingsQueue[i].scv = CCmanager[0]->getBuilder();
			BuildingsQueue[i].scv = getSCVBuilder( BuildingsQueue[i].type, BuildingsQueue[i].buildLocation);
			//CCmanager[0]->addSCV( stuck );
			returnBuilderSCV(stuck);
			BuildingsQueue[i].FrameStarted = Broodwar->getFrameCount();
			continue;
			}
		}

		/*
		if(  !Broodwar->isVisible( BuildingsQueue[i].buildLocation ) ){
			BuildingsQueue[i].scv->move(moveTo);
		} else {
			BuildingsQueue[i].scv->build( BuildingsQueue[i].buildLocation , BuildingsQueue[i].type );
		}
		*/
		BWAPI::TilePosition BotRight( BuildingsQueue[i].buildLocation.x() + BuildingsQueue[i].type.tileWidth(),BuildingsQueue[i].buildLocation.y() + BuildingsQueue[i].type.tileHeight() );
		BWAPI::TilePosition TopRight( BuildingsQueue[i].buildLocation.x() + BuildingsQueue[i].type.tileWidth(),BuildingsQueue[i].buildLocation.y()  );
		BWAPI::TilePosition BotLeft( BuildingsQueue[i].buildLocation.x() ,BuildingsQueue[i].buildLocation.y() + BuildingsQueue[i].type.tileHeight() );

		if(  !Broodwar->isVisible( BuildingsQueue[i].buildLocation )
			 || !Broodwar->isVisible( BotRight )  || !Broodwar->isVisible( TopRight ) || !Broodwar->isVisible( BotLeft )  ){
			BuildingsQueue[i].scv->move(moveTo);
		} else if ( Broodwar->isVisible( BuildingsQueue[i].buildLocation ) && BuildingsQueue[i].building == NULL ) {
			BuildingsQueue[i].scv->build( BuildingsQueue[i].buildLocation , BuildingsQueue[i].type );
		} else { //building already placed down
			//if it has been destroyed, rebuild it
			if( !BuildingsQueue[i].building->exists() ) {
			  BuildingsQueue[i].scv->build( BuildingsQueue[i].buildLocation , BuildingsQueue[i].type );
			  BuildingsQueue[i].building = NULL;
			}
			else if( BuildingsQueue[i].scv->getOrder() != BWAPI::Orders::ConstructingBuilding  ){
				BuildingsQueue[i].scv->rightClick( BuildingsQueue[i].building );
			}
		}

		//The scv is currently building
		if(  BuildingsQueue[i].building == NULL &&  BuildingsQueue[i].scv->getBuildUnit() != NULL  ){
			 BuildingsQueue[i].building =  BuildingsQueue[i].scv->getBuildUnit();
		}
		if(  BuildingsQueue[i].building != NULL){
			if( BuildingsQueue[i].Started == false ){
			  reservedMinerals -= BuildingsQueue[i].type.mineralPrice();
			  reservedGas -= BuildingsQueue[i].type.gasPrice();
			  BuildingsQueue[i].Started = true;
			}
			if(BuildingsQueue[i].building->isCompleted()  ){
			  //CCmanager[0]->addSCV(BuildingsQueue[i].scv);
			  returnBuilderSCV( BuildingsQueue[i].scv );
			  //reservedMinerals -= BuildingsQueue[i].type.mineralPrice();
			  //reservedGas -= BuildingsQueue[i].type.gasPrice();

			  /*
			  if(BuildingsQueue[i].type == BWAPI::UnitTypes::Terran_Refinery){
				  Broodwar->printf("Refinery done");
				  CCmanager[0]->Refinerys.push_back(BuildingsQueue[i].building);
		         CCmanager[0]->toGas();
			  }
			  */
			  if(BuildingsQueue[i].type == BWAPI::UnitTypes::Terran_Refinery){
		        int closestCC = 9999;
		        int closeID = 0;
		        for( unsigned int j=0; j<CCmanager.size(); j++ ){
			      if(  CCmanager[j]->CommandCenter->getPosition().getDistance(BuildingsQueue[i].building->getPosition()) < closestCC ){
					closestCC = CCmanager[j]->CommandCenter->getPosition().getDistance(BuildingsQueue[i].building->getPosition());
					closeID = j;
			      }
		        }

		        Broodwar->printf("Refinery done");
				CCmanager[closeID]->Refinerys.push_back( BuildingsQueue[i].building );
		        CCmanager[closeID]->toGas();
			  }

			  // remove from BuildingsQueueType and BuildingsQueue
			  for(int j=0; j<BuildingsQueueType.size(); j++){
				  if( BuildingsQueueType[j] == BuildingsQueue[i].type ){
					  BuildingsQueueType.erase( BuildingsQueueType.begin() + j);
					  break;
				  }
			  }

			  BuildingsQueue.erase(  BuildingsQueue.begin() + i );
			  i--;
			  Broodwar->printf("job done");
			}	
		}
	}



}

void ProductionManager::onFrameWorkerRush(){

	// && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation 
	if(  (Broodwar->self()->minerals() - reservedMinerals) >= 50){
		CCmanager[0]->buildSCV();
	}

	checkSCVbuild();

	if( WAMan != NULL){
		if( WAMan->StopWorkerRush == false){

	     if( ProdMan->getAvailableMinerals() > 150 
		   && Broodwar->self()->supplyTotal() ==  Broodwar->self()->supplyUsed() ){ // && bManager->WallCalculated
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
	     }
		}
	}

	if( WAMan != NULL){
		if( WAMan->StopWorkerRush == true){
	  if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		    addToQueue( BuildingSupplyOrder[0].type );
			//addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );

	     }
		}

		for(int i=0; i<StarportNoAddon.size(); i++){

			if( StarportNoAddon[i]->getTrainingQueue().size() == 0){
				StarportNoAddon[i]->train(  BWAPI::UnitTypes::Terran_Wraith );
			}

		}

	}

}



void ProductionManager::onFrameMarineRush(){


	for(int i=0; i<Barracks.size(); i++ ){
		if( Barracks[i]->getTrainingQueue().size() == 0 ){
			Barracks[i]->train( BWAPI::UnitTypes::Terran_Marine );
		}

	}


	// && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation 
	if(  Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_SCV) < 8   ){
		CCmanager[0]->buildSCV();
	}



	checkSCVbuild();

	if( MarineRush != NULL){


		if( MarineRush->StopMarineRush == false){


			if( ScoutMan == NULL && Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_SCV) == 8   ){
				ScoutMan = new ScoutManager(1);
	         }


		if( Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Barracks) == 0
			&& TotalTypeInQueue(BWAPI::UnitTypes::Terran_Barracks) == 0  ){
			BuildingSupplyOrder.clear();
			addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 8);
			addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 8);
			addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
		}

				  if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		    addToQueue( BuildingSupplyOrder[0].type );
			//addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );

	     }

	     if( ProdMan->getAvailableMinerals() > 150 
		   && Broodwar->self()->supplyTotal() ==  Broodwar->self()->supplyUsed() ){ // && bManager->WallCalculated
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
	     }


		}
	}

	if( MarineRush != NULL){
		if( MarineRush->StopMarineRush == true){
	  if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		    addToQueue( BuildingSupplyOrder[0].type );
			//addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );

	     }
		}

		for(int i=0; i<StarportNoAddon.size(); i++){

			if( StarportNoAddon[i]->getTrainingQueue().size() == 0){
				StarportNoAddon[i]->train(  BWAPI::UnitTypes::Terran_Wraith );
			}

		}

	}



	// && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation 
	if(  getAvailableMinerals() >= 50 && MarineRush->StopMarineRush == true   ){
		CCmanager[0]->buildSCV();
	}


}


void ProductionManager::onFrameMain(){

    checkSCVbuild();
	
	if( BuildingOrder.size() > 0 && BuildingOrder[0].mineralPrice() <=  ( Broodwar->self()->minerals() - reservedMinerals) ){
		//Broodwar->printf("adding possible");
	}
	if( BuildingOrder.size() > 0 && BuildingOrder[0].mineralPrice() <=  ( Broodwar->self()->minerals() - reservedMinerals) && BuildingOrder[0].gasPrice() <=  ( Broodwar->self()->gas() - reservedGas )){ // && bManager->WallCalculated
		addToQueue( BuildingOrder[0] );
		BuildingOrder.erase( BuildingOrder.begin() );
	}
	
	//build as many wraiths as possible
	if( StarportNoAddon.size() > 0 || StarportCtower.size() > 0 ){
		bool buildingDepot = false;
		for(unsigned int i=0; i<BuildingsQueue.size(); i++){
			if( BuildingsQueue[i].type == BWAPI::UnitTypes::Terran_Supply_Depot ){
				buildingDepot = true;
			}
		}
		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 10 && !buildingDepot ){
		//if( !buildingDepot ){
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}

		if( StarportNoAddon.size() > 0 && StarportCtower.size() == 0 ){
		  if( StarportNoAddon[0]->getAddon() == NULL){
			StarportNoAddon[0]->buildAddon( BWAPI::UnitTypes::Terran_Control_Tower );
		  }
		  /*
		  if( StarportNoAddon[0]->getAddon() != NULL){
			StarportNoAddon[0]->getAddon()->research( BWAPI::TechTypes::Cloaking_Field);
		  }
		  */
		}

		//train wraith
		for(unsigned int i=0; i<StarportNoAddon.size(); i++){
			if( StarportNoAddon[i]->getTrainingQueue().size() == 0 ){
		     StarportNoAddon[i]->train( BWAPI::UnitTypes::Terran_Wraith);
		  }
		}
		//train wraith
		for(unsigned int i=0; i<StarportCtower.size(); i++){
			if( StarportCtower[i]->getTrainingQueue().size() == 0 ){
		     StarportCtower[i]->train( BWAPI::UnitTypes::Terran_Wraith);
		  }
		}
	}
	
	// && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation
	if(  (Broodwar->self()->minerals() - reservedMinerals) >= 50 ){
		CCmanager[0]->buildSCV();
	}

	//time to start training marines
	if(  getAvailableMinerals() >= 200 ){
		bool trainMore = true;
		if( Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Factory) == 0 ){
			trainMore = false;
		}
		for(unsigned int i=0; i<Barracks.size(); i++){
		  if( Barracks[i]->getTrainingQueue().size() == 0 && trainMore ){
		    Barracks[i]->train(BWAPI::UnitTypes::Terran_Marine);
		  }
		}
		//if we have to many minerals, build another barracks
		if( StarportNoAddon.size() > 0 &&  (Broodwar->self()->minerals() - reservedMinerals) >= 250 && BuildingsQueue.size() == 0 
			&&  BuildingOrder.size() == 0 && Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Barracks) < 3 ){
			BuildingOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
		}
	}

}



void ProductionManager::onFrameMacroProtoss()
{


	checkSCVbuild();

	//if( BuildingSupplyOrder.size() > 0 && BuildingSupplyOrder[0].supply <=  Broodwar->self()->supplyUsed()/2 ){ // && bManager->WallCalculated
	if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		if( BuildingSupplyOrder[0].type != BWAPI::UnitTypes::Terran_Command_Center){
		  addToQueue( BuildingSupplyOrder[0].type );
		  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
		} else {

			bool CCready = false;
			if( FactoryShop.size() > 0 ){
				if( FactoryShop[0]->getAddon() != NULL ){
					if( (FactoryShop[0]->getAddon()->isResearching() || Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) )						
						&& FactoryShop[0]->getTrainingQueue().size() > 0){
						CCready = true;
					}
				}

				if( getAvailableMinerals() > 500 ){
					CCready = true;
				}

			}

			if( CCready ){
				bManager->mapArea( bManager->getBuildRectangle( BWAPI::TilePosition( MacroMan->chokePos ), BuildingSupplyOrder[0].type) , 0,1);
			  addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
		      //addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			}
		}

	}

	//future macro build
	if( BuildingSupplyOrder.size() == 0 ){ 

		int SupplyBeforeDouble = 100;

		bool buildingDepot = false;
		if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Supply_Depot ) > 0 ){
			buildingDepot = true;
		}

		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 10 && !buildingDepot
			 && Broodwar->self()->supplyUsed() < SupplyBeforeDouble ){
		//if( !buildingDepot ){
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	
		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 20 && !buildingDepot
			&& Broodwar->self()->supplyUsed() >= SupplyBeforeDouble
			&&  Broodwar->self()->supplyTotal() != 400 ){
		//if( !buildingDepot ){
				Broodwar->printf("Double supply");
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	


		if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){


		  if( MacroMan->FollowupP.size() > 0){ // && MacroMan->PState == P_Push
			  if( MacroMan->FollowupP[0] == BWAPI::UnitTypes::Terran_Command_Center 
				  &&  MacroMan->FollowupP[0].mineralPrice() + 75 < getAvailableMinerals() ){
					  addToQueueTile( MacroMan->FollowupP[0], InfoMan->OurNat );
				 MacroMan->FollowupP.erase( MacroMan->FollowupP.begin() );
			  } else if( MacroMan->FollowupP[0].mineralPrice() + 25 <= getAvailableMinerals()  
				&& MacroMan->FollowupP[0].gasPrice() <= getAvailableGas()
				&& MacroMan->FollowupP[0] != BWAPI::UnitTypes::Terran_Command_Center){
				addToQueue( MacroMan->FollowupP[0] );
				 MacroMan->FollowupP.erase( MacroMan->FollowupP.begin() );
			}

		  } else {


			  bool ForceExpand = false; //force expansion when a CC is about to run out of minerals
			  for(int c=0; c<CCmanager.size(); c++ ){
				  if( CCmanager[c]->InitialMinerals != CCmanager[c]->Minerals.size() ){
					  if( c + 2 >= CCmanager.size() ){
                         ForceExpand = true;
					  }
				  }
			  }
			  

			  //  if( ( getAvailableMinerals() > 470 &&  MacroMan->PState == P_Push )||  ForceExpand ){

			  //expand
			  if(   MacroMan->PState == P_Push &&  ForceExpand ){
				   if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0){
					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;

						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
						   }
						   BOOST_FOREACH( BWAPI::Unit* unit, Broodwar->self()->getUnits() ){
							   if( unit->getType() == BWAPI::UnitTypes::Terran_Command_Center
								   &&  unit->getDistance(loc->getPosition() ) < 7*32 ){
									   already = true;
							   }
						   }

						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() )
							   &&  !loc->isIsland()  ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 

				}
			   }
			   


			  /*
			   if( getAvailableMinerals() > 550 && getAvailableGas() > 300 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Factory ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Factory );
				}
			   }
			   */

			  /*
			  //add science vessel for DTs
			  if( getAvailableMinerals() > 450 && getAvailableGas() > 300
				  && ( StarportCtower.size() + StarportNoAddon.size() ) == 0 ){
					  if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Starport ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Starport );
				}
			  }
			  */

			  /*
			  if( ( StarportCtower.size() + StarportNoAddon.size() ) >= 1
				  && ScienceFacility == NULL && TotalTypeInQueue( BWAPI::UnitTypes::Terran_Science_Facility ) == 0 
				  && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Science_Facility ) == 0   ){
				  addToQueue(  BWAPI::UnitTypes::Terran_Science_Facility );
			  }
			  */


			  /*
			   if( getAvailableMinerals() > 350 && Barracks.size()  < (CCmanager.size() * 2 + 1) ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }
			  */


		  }

		}


	}



		//&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation
		for(int i=0; i<CCmanager.size(); i++){
			
			bool earlyRaxNeeded = false;
			if( Broodwar->getFrameCount() < 3000 && TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0){
				earlyRaxNeeded = true;
			}
			//prioritise building a barracks
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && earlyRaxNeeded && getAvailableMinerals() < 50){
				continue;
			}

			//if( (CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation )
			//	&& (getAvailableMinerals() >= 50 || MacroMan->ZState == Z_Pool_Defend )){
			if( CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation  ){
		   CCmanager[i]->buildSCV();
	      }

		}

		for(int i=0; i<Barracks.size(); i++){

			//&&  (GMan->stillLifting == true || GMan->BarracksLift == true )

		  if( i == 0 
			&&  (GMan->stillLifting == true )
			&& bManager->WallSound ){
			if( Barracks[i]->getTrainingQueue().size() != 0){
			Barracks[i]->cancelTrain();
			}
			continue;
		  }


			BWAPI::UnitType toTrain = MacroMan->BarrackSuggest();
			if( toTrain != BWAPI::UnitTypes::None && Barracks[i]->getTrainingQueue().size() == 0 ){
				Barracks[i]->train( toTrain );
			}
		}





		for(int i=0; i<FactoryNoAddon.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggest();
			if(  getSug == BWAPI::UnitTypes::None ){
				continue;
			}
			if( getSug == BWAPI::UnitTypes::Terran_Machine_Shop){
				FactoryNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Machine_Shop);
				continue;
			}
			if( FactoryNoAddon[i]->getTrainingQueue().size() == 0 ){
				FactoryNoAddon[i]->train( getSug );
			}
		}

		for(int i=0; i<FactoryShop.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggestShop();
			if( FactoryShop[i]->getTrainingQueue().size() == 0 ){
				FactoryShop[i]->train( getSug );
			}
			if( FactoryShop[i]->getAddon() != NULL){
				if(  Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
				  if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }
				}

			}
		}

		for(int i=0; i<StarportNoAddon.size(); i++){
			StarportNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Control_Tower );
			/*
			if( StarportNoAddon[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortNoTSuggest();
				StarportNoAddon[i]->train( getSug);
			}
			*/ 
		}

		for(int i=0; i<StarportCtower.size(); i++){
			if( StarportCtower[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortCTSuggest();
				StarportCtower[i]->train( getSug);
			}
		}

		if( Academy != NULL){
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Stim_Packs) ){
					Academy->research( BWAPI::TechTypes::Stim_Packs);
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::U_238_Shells) == 0){
					Academy->upgrade( BWAPI::UpgradeTypes::U_238_Shells);
				}
			}
		}

		if(EngineeringBay != NULL){
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
				if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Weapons) == 0 ){
					EngineeringBay->upgrade( BWAPI::UpgradeTypes::Terran_Infantry_Weapons );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Armor) == 0){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Armor );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Armor) == 1){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Armor );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Armor) == 2){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Armor );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Weapons) == 1){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Weapons );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Weapons) == 2){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Weapons );
				}


			}
		}
		
		if(ScienceFacility != NULL){


				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::EMP_Shockwave )
					&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Science_Vessel  ) > 0 ){
				   ScienceFacility->research( BWAPI::TechTypes::EMP_Shockwave );
				}

			/*	
			if( ScienceFacility->getAddon() == NULL ){
				ScienceFacility->buildAddon( BWAPI::UnitTypes::Terran_Covert_Ops);
			}
			if( ScienceFacility->getAddon() != NULL ){
				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::EMP_Shockwave ) ){
				   ScienceFacility->research( BWAPI::TechTypes::EMP_Shockwave );
				}
				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Lockdown ) ){
				   ScienceFacility->getAddon()->research( BWAPI::TechTypes::Lockdown );
				}


			}
			*/
		}



}

void ProductionManager::onFrameMacro(){

	checkSCVbuild();

	
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg
		 && BuildingSupplyOrder.size() > 0 ){
			 if( BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Barracks
				  && Broodwar->self()->supplyUsed()/2 == 10 ){
		      addToQueue( BuildingSupplyOrder[0].type );
		      BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			 }
	}
	


	//if( BuildingSupplyOrder.size() > 0 && BuildingSupplyOrder[0].supply <=  Broodwar->self()->supplyUsed()/2 ){ // && bManager->WallCalculated
	if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		if( BuildingSupplyOrder[0].type != BWAPI::UnitTypes::Terran_Command_Center){
		  addToQueue( BuildingSupplyOrder[0].type );
		  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
		} else {
			if( 
				( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg &&
				MacroMan->EarlyPool == false)
				||
				( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss
				&& MacroMan->CurrentStrat != "Siege Expand"  )
				||
				( Broodwar->enemy()->getRace() == BWAPI::Races::Terran )  ){
		      addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			} else {
				//bManager->mapArea( bManager->getBuildRectangle( BWAPI::TilePosition( MacroMan->chokePos ), BuildingSupplyOrder[0].type) , 0,1);
			  addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
		      //addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			}
		}

	}

	//future macro build
	if( BuildingSupplyOrder.size() == 0 ){ 

		bool buildingDepot = false;
		if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Supply_Depot ) > 0 ){
			buildingDepot = true;
		}

        int DoubleSupplyCount = 140;

		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 10 && !buildingDepot
			 && Broodwar->self()->supplyUsed() < DoubleSupplyCount ){
		//if( !buildingDepot ){
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	
		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 20 && !buildingDepot
			&& Broodwar->self()->supplyUsed() >= DoubleSupplyCount
			&&  Broodwar->self()->supplyTotal() != 400 ){
		//if( !buildingDepot ){
				Broodwar->printf("Double supply");
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	


		//extra buildings
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		  if( MacroMan->FollowupZ.size() > 0){ // && MacroMan->PState == P_Push
			  if( MacroMan->FollowupZ[0] == BWAPI::UnitTypes::Terran_Command_Center 
				  &&  MacroMan->FollowupZ[0].mineralPrice() + 75 < getAvailableMinerals() ){
					  addToQueueTile( MacroMan->FollowupZ[0], InfoMan->OurNat );
				 MacroMan->FollowupZ.erase( MacroMan->FollowupZ.begin() );
			  } else if( MacroMan->FollowupZ[0].mineralPrice() + 25 <= getAvailableMinerals()  
				&& MacroMan->FollowupZ[0].gasPrice() <= getAvailableGas()
				&& MacroMan->FollowupZ[0] != BWAPI::UnitTypes::Terran_Command_Center){
				addToQueue( MacroMan->FollowupZ[0] );
				 MacroMan->FollowupZ.erase( MacroMan->FollowupZ.begin() );
			}

		  } else {
			  

			  //expand
			   if( getAvailableMinerals() > 500  ){
				   if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0){
					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;
						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
						   }
						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() ) ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 

				}
			   }


			  if( getAvailableMinerals() > 420 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }
			  if( MacroMan->CurrentStrat == "Goliath M&M"){
			    if( getAvailableMinerals() > 310 && getAvailableGas() > 300 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Factory ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Factory );
				}
			    }
			  }
		  }
		}

		if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){


		  if( MacroMan->FollowupP.size() > 0){ // && MacroMan->PState == P_Push
			  if( MacroMan->FollowupP[0] == BWAPI::UnitTypes::Terran_Command_Center 
				  &&  MacroMan->FollowupP[0].mineralPrice() + 75 < getAvailableMinerals() ){
					  addToQueueTile( MacroMan->FollowupP[0], InfoMan->OurNat );
				 MacroMan->FollowupP.erase( MacroMan->FollowupP.begin() );
			  } else if( MacroMan->FollowupP[0].mineralPrice() + 25 <= getAvailableMinerals()  
				&& MacroMan->FollowupP[0].gasPrice() <= getAvailableGas()
				&& MacroMan->FollowupP[0] != BWAPI::UnitTypes::Terran_Command_Center){
				addToQueue( MacroMan->FollowupP[0] );
				 MacroMan->FollowupP.erase( MacroMan->FollowupP.begin() );
			}

		  } else {


			  //expand
			   if( getAvailableMinerals() > 500  ){
				   if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0){
					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;
						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
						   }
						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() ) ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 

				}
			   }



			   if( getAvailableMinerals() > 310 && getAvailableGas() > 300 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Factory ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Factory );
				}
			   }
			  if( getAvailableMinerals() > 350 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }

		  }

		}



		if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){


		  if( MacroMan->FollowupT.size() > 0){ // && MacroMan->PState == P_Push
			  if( MacroMan->FollowupT[0] == BWAPI::UnitTypes::Terran_Command_Center 
				  &&  MacroMan->FollowupT[0].mineralPrice() + 75 < getAvailableMinerals() ){
					  addToQueueTile( MacroMan->FollowupT[0], InfoMan->OurNat );
				 MacroMan->FollowupT.erase( MacroMan->FollowupT.begin() );
			  } else if( MacroMan->FollowupT[0].mineralPrice() + 25 <= getAvailableMinerals()  
				&& MacroMan->FollowupT[0].gasPrice() <= getAvailableGas()
				&& MacroMan->FollowupT[0] != BWAPI::UnitTypes::Terran_Command_Center){
				addToQueue( MacroMan->FollowupT[0] );
				 MacroMan->FollowupT.erase( MacroMan->FollowupT.begin() );
			}

		  } else {

			   if( getAvailableMinerals() > 600  ){
				   if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0){
					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;
						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
							   //dont take gasless expansions
							   if( loc->getGeysers().size() == 0 ){
                                  already = true;
							   }
						   }
						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() ) ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 

				}
			   }
			  if( getAvailableMinerals() > 350  && getAvailableGas() > 400 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Starport ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Starport );
				}
			  }

		  }

		}


	}



		//&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation
		for(int i=0; i<CCmanager.size(); i++){
			
			bool earlyRaxNeeded = false;
			if( Broodwar->getFrameCount() < 3000 && TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0){
				earlyRaxNeeded = true;
			}
			//prioritise building a barracks
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && earlyRaxNeeded && getAvailableMinerals() < 50){
				continue;
			}

			//if( (CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation )
			//	&& (getAvailableMinerals() >= 50 || MacroMan->ZState == Z_Pool_Defend )){
			if( CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation  ){
		   CCmanager[i]->buildSCV();
	      }

		}

		for(int i=0; i<Barracks.size(); i++){

			//&&  (GMan->stillLifting == true || GMan->BarracksLift == true )

		  if( i == 0 
			&&  (GMan->stillLifting == true )
			&& bManager->WallSound ){
			if( Barracks[i]->getTrainingQueue().size() != 0){
			Barracks[i]->cancelTrain();
			}
			continue;
		  }


			BWAPI::UnitType toTrain = MacroMan->BarrackSuggest();
			if( toTrain != BWAPI::UnitTypes::None && Barracks[i]->getTrainingQueue().size() == 0 ){
				Barracks[i]->train( toTrain );
			}
		}





		for(int i=0; i<FactoryNoAddon.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggest();
			if(  getSug == BWAPI::UnitTypes::None ){
				continue;
			}
			if( getSug == BWAPI::UnitTypes::Terran_Machine_Shop){
				FactoryNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Machine_Shop);
				continue;
			}
			if( FactoryNoAddon[i]->getTrainingQueue().size() == 0 ){
				FactoryNoAddon[i]->train( getSug );
			}
		}

		for(int i=0; i<FactoryShop.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggestShop();
			if( FactoryShop[i]->getTrainingQueue().size() == 0 ){
				FactoryShop[i]->train( getSug );
			}
			if( FactoryShop[i]->getAddon() != NULL){
				if(  Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
				  if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }
				}
				if( MacroMan->CurrentStrat == "2 rax FE"){
				  if( MacroMan->Squads[0].Tanks.size() > 0 && !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }
				} else if( MacroMan->CurrentStrat == "Goliath M&M"){
				  if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Charon_Boosters ) == 0){
					FactoryShop[i]->getAddon()->upgrade( BWAPI::UpgradeTypes::Charon_Boosters);
				  } else if(  !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }
				}

				if(  Broodwar->enemy()->getRace() == BWAPI::Races::Terran){
					 if(  !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  } else if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Spider_Mines ) ){
				 	FactoryShop[i]->getAddon()->research( BWAPI::TechTypes::Spider_Mines );
				  }
				}

			}
		}

		for(int i=0; i<StarportNoAddon.size(); i++){
			StarportNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Control_Tower );
			/*
			if( StarportNoAddon[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortNoTSuggest();
				StarportNoAddon[i]->train( getSug);
			}
			*/ 
		}

		for(int i=0; i<StarportCtower.size(); i++){
			if( StarportCtower[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortCTSuggest();
				StarportCtower[i]->train( getSug);
			}
		}

		if( Academy != NULL){
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Stim_Packs) ){
					Academy->research( BWAPI::TechTypes::Stim_Packs);
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::U_238_Shells ) == 0){
					Academy->upgrade( BWAPI::UpgradeTypes::U_238_Shells );
				}
			}

			if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Stim_Packs) ){
					Academy->research( BWAPI::TechTypes::Stim_Packs);
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::U_238_Shells) == 0){
					Academy->upgrade( BWAPI::UpgradeTypes::U_238_Shells);
				}
			}
		}

		if(EngineeringBay != NULL){
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
				if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Weapons) == 0 ){
					EngineeringBay->upgrade( BWAPI::UpgradeTypes::Terran_Infantry_Weapons );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Armor) == 0){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Armor );
				}
			}
			if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
				if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Weapons) == 0 ){
					EngineeringBay->upgrade( BWAPI::UpgradeTypes::Terran_Infantry_Weapons );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Armor) == 0){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Armor );
				}
			}
		}
		
		if(ScienceFacility != NULL){

			if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Irradiate) ){
					ScienceFacility->research( BWAPI::TechTypes::Irradiate);
				} else if( ScienceFacility->getAddon() == NULL ){
					ScienceFacility->buildAddon( BWAPI::UnitTypes::Terran_Physics_Lab);
				}
				if( ScienceFacility->getAddon() != NULL){
					//ScienceFacility->getAddon()->research( BWAPI::TechTypes::Yamato_Gun);
				}
			}

			if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
				if( ScienceFacility->getAddon() == NULL ){
					ScienceFacility->buildAddon( BWAPI::UnitTypes::Terran_Physics_Lab);
				}
				if( ScienceFacility->getAddon() != NULL){
					//ScienceFacility->getAddon()->research( BWAPI::TechTypes::Yamato_Gun);
				}
			}

		}

}

void ProductionManager::AddCCTerran()
{


					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;

						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
						   }
						   BOOST_FOREACH( BWAPI::Unit* unit, Broodwar->self()->getUnits() ){
							   if( unit->getType() == BWAPI::UnitTypes::Terran_Command_Center
								   &&  unit->getDistance(loc->getPosition() ) < 7*32 ){
									   already = true;
							   }
						   }

						   for(int j=0; j<InfoMan->EnemyBuildings.size(); j++ ){

							   if( InfoMan->EnemyBuildings[j].type == BWAPI::UnitTypes::Terran_Command_Center ){
								   if( InfoMan->EnemyBuildings[j].position.getDistance(loc->getPosition() ) < 7*32 ){
									   already = true;
								   }
							   }
						   }

						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() )
							   &&  !loc->isIsland()
							   &&  loc->getGeysers().size() > 0 ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 


					   /*
					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;
						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
							   //dont take gasless expansions
							   if( loc->getGeysers().size() == 0 ){
                                  already = true;
							   }
						   }
						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() ) ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 
					   */



}


void ProductionManager::onFrameMacroTerran(){

	checkSCVbuild();

	//build barracks at 10 supply to help vs BBS
	if(  BuildingSupplyOrder.size() > 0 ){
			 if( BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Barracks
				  && Broodwar->self()->supplyUsed()/2 == 10 ){
		      addToQueue( BuildingSupplyOrder[0].type );
		      BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			 }
	}
	


	//if( BuildingSupplyOrder.size() > 0 && BuildingSupplyOrder[0].supply <=  Broodwar->self()->supplyUsed()/2 ){ // && bManager->WallCalculated
	if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		if( BuildingSupplyOrder[0].type != BWAPI::UnitTypes::Terran_Command_Center){
		  addToQueue( BuildingSupplyOrder[0].type );
		  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
		} else {
			if( ( Broodwar->enemy()->getRace() == BWAPI::Races::Terran )  ){
		      addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			} else {
				//bManager->mapArea( bManager->getBuildRectangle( BWAPI::TilePosition( MacroMan->chokePos ), BuildingSupplyOrder[0].type) , 0,1);
			  addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
		      //addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			}
		}

	}

	//future macro build
	if( BuildingSupplyOrder.size() == 0 ){ 

		bool buildingDepot = false;
		if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Supply_Depot ) > 0 ){
			buildingDepot = true;
		}

		int DoubleSupplyCount = 160;

		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 10 && !buildingDepot
			 && Broodwar->self()->supplyUsed() < DoubleSupplyCount ){
		//if( !buildingDepot ){
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	
		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 20 && !buildingDepot
			&& Broodwar->self()->supplyUsed() >= DoubleSupplyCount
			&&  Broodwar->self()->supplyTotal() != 400 ){
		//if( !buildingDepot ){
				Broodwar->printf("Double supply");
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	


		  if( MacroMan->FollowupT.size() > 0){ // && MacroMan->PState == P_Push
			  if( MacroMan->FollowupT[0] == BWAPI::UnitTypes::Terran_Command_Center 
				  &&  MacroMan->FollowupT[0].mineralPrice() + 75 < getAvailableMinerals() ){
				   //addToQueueTile( MacroMan->FollowupT[0], InfoMan->OurNat );
				   //MacroMan->FollowupT.erase( MacroMan->FollowupT.begin() );
					  AddCCTerran();
					  MacroMan->FollowupT.erase( MacroMan->FollowupT.begin() );
			  } else if( MacroMan->FollowupT[0].mineralPrice() + 25 <= getAvailableMinerals()  
				&& MacroMan->FollowupT[0].gasPrice() <= getAvailableGas()
				&& MacroMan->FollowupT[0] != BWAPI::UnitTypes::Terran_Command_Center){
				addToQueue( MacroMan->FollowupT[0] );
				 MacroMan->FollowupT.erase( MacroMan->FollowupT.begin() );
			}

		  } else {

			   if( getAvailableMinerals() > 600  ){
				   if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0){

					   AddCCTerran();

				}
			   }
			  if( getAvailableMinerals() > 350  && getAvailableGas() > 400 ){
				  if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Starport ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Starport );
				}
			  }

		  }

		


	}



		//&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation
		for(int i=0; i<CCmanager.size(); i++){
			
			bool earlyRaxNeeded = false;
			if( Broodwar->getFrameCount() < 3000 && TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0){
				earlyRaxNeeded = true;
			}
			//prioritise building a barracks
			if( earlyRaxNeeded && getAvailableMinerals() < 50){
				continue;
			}

			//if( (CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation )
			//	&& (getAvailableMinerals() >= 50 || MacroMan->ZState == Z_Pool_Defend )){
			if( CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation  ){
		   CCmanager[i]->buildSCV();
	      }

		}

		for(int i=0; i<Barracks.size(); i++){

			//&&  (GMan->stillLifting == true || GMan->BarracksLift == true )

		  if( i == 0 
			&&  (GMan->stillLifting == true )
			&& bManager->WallSound ){
			if( Barracks[i]->getTrainingQueue().size() != 0){
			Barracks[i]->cancelTrain();
			}
			continue;
		  }


			BWAPI::UnitType toTrain = MacroMan->BarrackSuggest();
			if( toTrain != BWAPI::UnitTypes::None && Barracks[i]->getTrainingQueue().size() == 0 ){
				Barracks[i]->train( toTrain );
			}
		}





		for(int i=0; i<FactoryNoAddon.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggest();
			if(  getSug == BWAPI::UnitTypes::None ){
				continue;
			}
			if( getSug == BWAPI::UnitTypes::Terran_Machine_Shop){
				FactoryNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Machine_Shop);
				continue;
			}
			if( FactoryNoAddon[i]->getTrainingQueue().size() == 0 ){
				FactoryNoAddon[i]->train( getSug );
			}
		}

		for(int i=0; i<FactoryShop.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggestShop();
			if( FactoryShop[i]->getTrainingQueue().size() == 0 ){
				FactoryShop[i]->train( getSug );
			}
			if( FactoryShop[i]->getAddon() != NULL && FactoryShop[i]->getTrainingQueue().size() > 0 ){

				/* else if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Spider_Mines ) ){
				 	FactoryShop[i]->getAddon()->research( BWAPI::TechTypes::Spider_Mines );
				  }
				  */

				  if(  !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode )
					  &&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ) > 0 ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }  else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Charon_Boosters ) == 0 ){
					  if( Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Wraith ) > 0 ){
						  FactoryShop[i]->getAddon()->upgrade( BWAPI::UpgradeTypes::Charon_Boosters );
					  }
				  }


			}
		}

		for(int i=0; i<StarportNoAddon.size(); i++){
			StarportNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Control_Tower );
			/*
			if( StarportNoAddon[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortNoTSuggest();
				StarportNoAddon[i]->train( getSug);
			}
			*/ 
		}

		for(int i=0; i<StarportCtower.size(); i++){
			if( StarportCtower[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortCTSuggest();
				StarportCtower[i]->train( getSug);
			}
		}

		if( Armory.size() > 0 ){
			if(  Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Vehicle_Weapons ) == 0){
				Armory[0]->upgrade( BWAPI::UpgradeTypes::Terran_Vehicle_Weapons );
			} else if (  Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Vehicle_Weapons ) == 1){
				Armory[0]->upgrade( BWAPI::UpgradeTypes::Terran_Vehicle_Weapons );
			}

		}

		if( Academy != NULL){
			// no academy upgrades used, only build for comsat scanner
		}

		if(EngineeringBay != NULL){
			// no engineering bay used, only build for turrets and floating scout building
		}
		
		if(ScienceFacility != NULL){

			if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
				if( ScienceFacility->getAddon() == NULL ){
					ScienceFacility->buildAddon( BWAPI::UnitTypes::Terran_Physics_Lab);
				}
				if( ScienceFacility->getAddon() != NULL){
					//ScienceFacility->getAddon()->research( BWAPI::TechTypes::Yamato_Gun);
				}
			}

		}

}



void ProductionManager::onFrameMacroZerg(){

	checkSCVbuild();

	int BuildBarrackAtSup = 10; //the supply count where you  build a rax early to stop 4 pool
	if(  BuildingSupplyOrder.size() > 0 ){
			 if( BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Barracks
				  && Broodwar->self()->supplyUsed()/2 == BuildBarrackAtSup ){


               if (  CCmanager.size() > 0 && CCmanager[0]->Geysers.size() > 0 ){ 
			       //place the first barracks close to the refinery
					//Broodwar->printf("Placing Barracks close to geyser");
					addToQueueClose( BuildingSupplyOrder[0].type, CCmanager[0]->Geysers[0]->getTilePosition(), true );
		            BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
				  } else {
		            addToQueue( BuildingSupplyOrder[0].type );
		            BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
				  }

		     // addToQueue( BuildingSupplyOrder[0].type );
		     // BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );

			 }
	}
	


	//if( BuildingSupplyOrder.size() > 0 && BuildingSupplyOrder[0].supply <=  Broodwar->self()->supplyUsed()/2 ){ // && bManager->WallCalculated
	if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
		if( BuildingSupplyOrder[0].type != BWAPI::UnitTypes::Terran_Command_Center){

			if(  BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Supply_Depot
				&& Broodwar->getFrameCount() < 1500
				&& Broodwar->self()->supplyUsed()/2 < 9 ){
              //do nothing, fixes the waaay tooo early supply depot
			} else if (   MacroMan->ZState == Z_Pool_Defend
				&& BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Barracks
				&&  getAvailableMinerals() < 250 ) {

					//do nothing, wait for the bunker to finish first

			} else {
		    addToQueue( BuildingSupplyOrder[0].type );
		    BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			}

		} else {
			if( 
				( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg &&
				MacroMan->EarlyPool == false)  ){
		      addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			} else {
				//bManager->mapArea( bManager->getBuildRectangle( BWAPI::TilePosition( MacroMan->chokePos ), BuildingSupplyOrder[0].type) , 0,1);
			  addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
		      //addToQueueTile( BuildingSupplyOrder[0].type, InfoMan->OurNat );
			  //addToQueueClose(BuildingSupplyOrder[0].type, BWAPI::TilePosition( InfoMan->PosMainChoke ) );
			  BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			}
		}

			  if( getAvailableMinerals() > 420 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }

			  if( getAvailableMinerals() > 520 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 1){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }

	}

	//future macro build
	if( BuildingSupplyOrder.size() == 0 ){ 

		bool buildingDepot = false;
		if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Supply_Depot ) > 0 ){
			buildingDepot = true;
		}

		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 10 && !buildingDepot
			 && Broodwar->self()->supplyUsed() < 140 ){
		//if( !buildingDepot ){
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	
		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 15 && !buildingDepot
			&& Broodwar->self()->supplyUsed() >= 140
			&&  Broodwar->self()->supplyTotal() != 400 ){
		//if( !buildingDepot ){
				Broodwar->printf("Double supply");
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	


		//extra buildings
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		  if( MacroMan->FollowupZ.size() > 0){ // && MacroMan->PState == P_Push
			  if( MacroMan->FollowupZ[0] == BWAPI::UnitTypes::Terran_Command_Center 
				  &&  MacroMan->FollowupZ[0].mineralPrice() + 75 < getAvailableMinerals() ){
					  addToQueueTile( MacroMan->FollowupZ[0], InfoMan->OurNat );
				 MacroMan->FollowupZ.erase( MacroMan->FollowupZ.begin() );
			  } else if( MacroMan->FollowupZ[0].mineralPrice() + 25 <= getAvailableMinerals()  
				&& MacroMan->FollowupZ[0].gasPrice() <= getAvailableGas()
				&& MacroMan->FollowupZ[0] != BWAPI::UnitTypes::Terran_Command_Center){
				addToQueue( MacroMan->FollowupZ[0] );
				 MacroMan->FollowupZ.erase( MacroMan->FollowupZ.begin() );
			}

		  } else {
			  
               //expand
			  if( getAvailableMinerals() > 500 &&  MacroMan->ZState == Z_Push ){
				   if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0){

					   
					   int ClosestToBase = 99999;
					   BWAPI::TilePosition newBaseLoc = BWAPI::TilePositions::None;
					   BOOST_FOREACH( BWTA::BaseLocation * loc, BWTA::getBaseLocations() ){
						   bool already = false;

						   for( int j=0; j<CCmanager.size(); j++){
							   if( BWAPI::Position( CCmanager[j]->LandLocation ).getDistance(loc->getPosition() ) < 7*32  ){
								   already = true;
							   }
						   }
						   BOOST_FOREACH( BWAPI::Unit* unit, Broodwar->self()->getUnits() ){
							   if( unit->getType() == BWAPI::UnitTypes::Terran_Command_Center
								   &&  unit->getDistance(loc->getPosition() ) < 7*32 ){
									   already = true;
							   }
						   }

						   if( already == false 
							   &&  ClosestToBase > loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() )
							   &&  !loc->isIsland()  ){
                              ClosestToBase = loc->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() );
							  newBaseLoc = loc->getTilePosition();
						   }
					   }
					   if( newBaseLoc != BWAPI::TilePositions::None){
						   addToQueueTile( BWAPI::UnitTypes::Terran_Command_Center ,newBaseLoc );
					   } 



				}
			   }
	




			  if( getAvailableMinerals() > 540 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }
		

			  if( getAvailableMinerals() > 6000 && getAvailableGas() > 350 ){
				  if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Factory ) == 0){
				  addToQueue(  BWAPI::UnitTypes::Terran_Factory );
				}
			  }
		


			  if( getAvailableMinerals() > 620 ){
				if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) == 1){
				  addToQueue(  BWAPI::UnitTypes::Terran_Barracks );
				}
			  }


		  }
		}


	}



		//&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation
		for(int i=0; i<CCmanager.size(); i++){
			
			bool earlyRaxNeeded = false;
			if( Broodwar->getFrameCount() < 3000 && TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0){
				earlyRaxNeeded = true;
			}
			//prioritise building a barracks
			if(  earlyRaxNeeded && getAvailableMinerals() < 50){
				continue;
			}

			//if( (CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation )
			//	&& (getAvailableMinerals() >= 50 || MacroMan->ZState == Z_Pool_Defend )){
			if( CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation){
		   CCmanager[i]->buildSCV();
	      }

		}

		for(int i=0; i<Barracks.size(); i++){

		  if( i == 0 
			&&  (GMan->stillLifting == true || GMan->BarracksLift == true )
			&& bManager->WallSound ){
			if( Barracks[i]->getTrainingQueue().size() != 0){
			Barracks[i]->cancelTrain();
			}
			continue;
		  }


			BWAPI::UnitType toTrain = MacroMan->BarrackSuggest();
			if( toTrain != BWAPI::UnitTypes::None && Barracks[i]->getTrainingQueue().size() == 0 ){
				Barracks[i]->train( toTrain );
			}
		}





		for(int i=0; i<FactoryNoAddon.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggest();
			if(  getSug == BWAPI::UnitTypes::None ){
				continue;
			}
			if( getSug == BWAPI::UnitTypes::Terran_Machine_Shop){
				FactoryNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Machine_Shop);
				continue;
			}
			if( FactoryNoAddon[i]->getTrainingQueue().size() == 0 ){
				FactoryNoAddon[i]->train( getSug );
			}
		}

		for(int i=0; i<FactoryShop.size(); i++){
			BWAPI::UnitType getSug = MacroMan->FactorySuggestShop();
			if( FactoryShop[i]->getTrainingQueue().size() == 0 ){
				FactoryShop[i]->train( getSug );
			}
			if( FactoryShop[i]->getAddon() != NULL){
				if( MacroMan->CurrentStrat == "2 rax FE"){
				  if( MacroMan->Squads[0].Tanks.size() > 0 && !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }
				} else if( MacroMan->CurrentStrat == "Goliath M&M"){
				  if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Charon_Boosters ) == 0){
					FactoryShop[i]->getAddon()->upgrade( BWAPI::UpgradeTypes::Charon_Boosters);
				  } else if(  !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
				 	FactoryShop[i]->getAddon()->research(BWAPI::TechTypes::Tank_Siege_Mode);
				  }
				}

			}
		}

		for(int i=0; i<StarportNoAddon.size(); i++){
			//StarportNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Control_Tower );
			BWAPI::UnitType getSug = MacroMan->StarPortNoTSuggest();
			if( StarportNoAddon[i]->getTrainingQueue().size() == 0){
			  if( getSug == BWAPI::UnitTypes::Terran_Control_Tower ){
                 StarportNoAddon[i]->buildAddon( BWAPI::UnitTypes::Terran_Control_Tower );
			  } else if ( getSug == BWAPI::UnitTypes::Terran_Wraith ){
				  StarportNoAddon[i]->train( getSug );
			  }

			}
			
		}

		for(int i=0; i<StarportCtower.size(); i++){
			if( StarportCtower[i]->getTrainingQueue().size() == 0){
				BWAPI::UnitType getSug = MacroMan->StarPortCTSuggest();
				StarportCtower[i]->train( getSug);
			}
		}

		if( Academy != NULL){

				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Stim_Packs) ){
					Academy->research( BWAPI::TechTypes::Stim_Packs);
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::U_238_Shells ) == 0){
					Academy->upgrade( BWAPI::UpgradeTypes::U_238_Shells );
				}
			

		}

		if(EngineeringBay != NULL){

				if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Weapons) == 0 ){
					EngineeringBay->upgrade( BWAPI::UpgradeTypes::Terran_Infantry_Weapons );
				} else if( Broodwar->self()->getUpgradeLevel( BWAPI::UpgradeTypes::Terran_Infantry_Armor) == 0){
					EngineeringBay->upgrade(  BWAPI::UpgradeTypes::Terran_Infantry_Armor );
				}
			
		}
		
		if(ScienceFacility != NULL){

				if( !Broodwar->self()->hasResearched( BWAPI::TechTypes::Irradiate)){
					if(  TotalUnitInProduction( BWAPI::UnitTypes::Terran_Science_Vessel) > 0
						|| Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Science_Vessel) > 0){
					  ScienceFacility->research( BWAPI::TechTypes::Irradiate);
					}
				} else if( ScienceFacility->getAddon() == NULL ){
					ScienceFacility->buildAddon( BWAPI::UnitTypes::Terran_Physics_Lab);
				}
				if( ScienceFacility->getAddon() != NULL){
					//ScienceFacility->getAddon()->research( BWAPI::TechTypes::Yamato_Gun);
				}


		}

}


void ProductionManager::onFrameMacroRandom(){

	checkSCVbuild();

	
	if(  BuildingSupplyOrder.size() > 0 ){
			 if( BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Barracks
				  && Broodwar->self()->supplyUsed()/2 == 10 ){
		      addToQueue( BuildingSupplyOrder[0].type );
		      BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			 }
	}
	


	//if( BuildingSupplyOrder.size() > 0 && BuildingSupplyOrder[0].supply <=  Broodwar->self()->supplyUsed()/2 ){ // && bManager->WallCalculated
	if( BuildingSupplyOrder.size() > 0 
		&& BuildingSupplyOrder[0].type.mineralPrice() <= getAvailableMinerals()
		&& BuildingSupplyOrder[0].type.gasPrice() <= getAvailableGas() ){ 
		
			if(  BuildingSupplyOrder[0].type == BWAPI::UnitTypes::Terran_Supply_Depot
				&& Broodwar->getFrameCount() < 1500
				&& Broodwar->self()->supplyUsed()/2 < 9 ){
              //do nothing, fixes the waaay tooo early supply depot
			} else {
		    addToQueue( BuildingSupplyOrder[0].type );
		    BuildingSupplyOrder.erase( BuildingSupplyOrder.begin() );
			}


	}

	//future macro build
	if( BuildingSupplyOrder.size() == 0 ){ 

		bool buildingDepot = false;
		if( TotalTypeInQueue( BWAPI::UnitTypes::Terran_Supply_Depot ) > 0 ){
			buildingDepot = true;
		}

		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 10 && !buildingDepot
			 && Broodwar->self()->supplyUsed() < 140 ){
		//if( !buildingDepot ){
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	
		if( (Broodwar->self()->supplyTotal() -  Broodwar->self()->supplyUsed()) < 15 && !buildingDepot
			&& Broodwar->self()->supplyUsed() >= 140
			&&  Broodwar->self()->supplyTotal() != 400 ){
		//if( !buildingDepot ){
				Broodwar->printf("Double supply");
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		  addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot );
		}	

	}



		//&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV) < CCmanager[0]->SCVsaturation
		for(int i=0; i<CCmanager.size(); i++){
			
			bool earlyRaxNeeded = false;
			if( Broodwar->getFrameCount() < 3000 && TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0){
				earlyRaxNeeded = true;
			}
			//prioritise building a barracks
			if(  earlyRaxNeeded && getAvailableMinerals() < 50){
				continue;
			}

			//if( (CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation )
			//	&& (getAvailableMinerals() >= 50 || MacroMan->ZState == Z_Pool_Defend )){
			if( CCmanager[i]->MineralSCV.size() < CCmanager[i]->SCVsaturation){
		   CCmanager[i]->buildSCV();
	      }

		}

		for(int i=0; i<Barracks.size(); i++){

		  if( i == 0 
			&&  (GMan->stillLifting == true || GMan->BarracksLift == true )
			&& bManager->WallSound ){
			if( Barracks[i]->getTrainingQueue().size() != 0){
			Barracks[i]->cancelTrain();
			}
			continue;
		  }


			BWAPI::UnitType toTrain = MacroMan->BarrackSuggest();
			if( toTrain != BWAPI::UnitTypes::None && Barracks[i]->getTrainingQueue().size() == 0 ){
				Barracks[i]->train( toTrain );
			}
		}


}


void ProductionManager::onFrame(){


	if(  reservedMinerals < 0 ){
		reservedMinerals = 0;
	}
	if(  reservedGas < 0 ){
		reservedGas = 0;
	}

	Broodwar->drawTextScreen(300,60,"Reserved minerals:%d",reservedMinerals);
	Broodwar->drawTextScreen(300,70,"Reserved gas:%d",reservedGas);

	Broodwar->drawTextScreen(300,80,"Current SCV:%d",CCmanager[0]->MineralSCV.size());
	Broodwar->drawTextScreen(300,90,"Saturation SCV:%d",CCmanager[0]->SCVsaturation);


    if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }


	if( CurrentStrategy == Macro_Strat  ){
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		onFrameMacroZerg();
		} else if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss) {
			onFrameMacroProtoss();
		} else if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran){
			onFrameMacroTerran();
		} else if( Broodwar->enemy()->getRace() == BWAPI::Races::Random
			|| Broodwar->enemy()->getRace() == BWAPI::Races::Unknown ){
			onFrameMacroRandom();
		} else {
			onFrameMacro();
		}

	}
	if( CurrentStrategy == Worker_Rush ){
		onFrameWorkerRush();
	}

	if( CurrentStrategy == BBS_Rush ){
		onFrameMarineRush();
	}



}

void ProductionManager::onUnitDestroy(BWAPI::Unit * unit){

	if( unit->getPlayer() != Broodwar->self() || !unit->getType().isBuilding() ){
		return;
	}

	if( unit->getType().isAddon()  ){
		return; 
	}

	//rebuild the building
	//addToQueue( unit->getType() );

	if( unit->isCompleted() &&  unit->getType() == BWAPI::UnitTypes::Terran_Barracks ){
		addToQueue( unit->getType() );
	}


}


void ProductionManager::PlaceTurrets(){

	//place turrets in natural

	//check if the natural exists
	if( CCmanager.size() > 1 ){

		/*
		BWAPI::TilePosition Turret1( CCmanager[1]->LandLocation.x() + 4, CCmanager[1]->LandLocation.y() - 1 );//above comsat
		BWAPI::TilePosition Turret2( CCmanager[1]->LandLocation.x() + 1, CCmanager[1]->LandLocation.y() + 3 );//below CC
		BWAPI::TilePosition Turret3( CCmanager[1]->LandLocation.x() - 2, CCmanager[1]->LandLocation.y()  );//to the left of CC
		ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret1);
		ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret2);
		ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret3);
		*/
		
		if( CCmanager[1]->Geysers.size() > 0){
			BWAPI::TilePosition GeyserTile = CCmanager[1]->Geysers[0]->getTilePosition();

		       BWAPI::TilePosition Turret1;//above comsat
		       BWAPI::TilePosition Turret2;//below CC

			  if( CCmanager[1]->LandLocation.y() < GeyserTile.y() -2 ){ //ref is down
		       Turret1 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() + 1, CCmanager[1]->LandLocation.y() + 4 );
		       Turret2 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() - 1, CCmanager[1]->LandLocation.y() + 4 );
			  }
			  if( CCmanager[1]->LandLocation.y() > GeyserTile.y() + 3 ){ //ref is up
		       Turret1 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() + 1, CCmanager[1]->LandLocation.y() -2 );
		       Turret2 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() - 1, CCmanager[1]->LandLocation.y() -2 );
			  }
			  if( CCmanager[1]->LandLocation.x() > GeyserTile.x() + 2 ){ //ref is left
		       Turret1 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() - 3, CCmanager[1]->LandLocation.y()  );
		       Turret2 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() - 3, CCmanager[1]->LandLocation.y() -2 );
			  }
			  if( CCmanager[1]->LandLocation.x() < GeyserTile.x() - 4 ){ //ref is right
		       Turret1 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() + 6, CCmanager[1]->LandLocation.y()  );
		       Turret2 = BWAPI::TilePosition ( CCmanager[1]->LandLocation.x() + 6, CCmanager[1]->LandLocation.y() -2 );
			  }

			  //2 turrets at the natural gas
	     	//ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret1);
	     	//ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret2);

			BWAPI::TilePosition Turret3( CCmanager[0]->LandLocation.x() + 4, CCmanager[0]->LandLocation.y() - 1 );//1 turret in the main base
	     	ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret3);

			BWAPI::TilePosition Turret4( CCmanager[1]->LandLocation.x() + 4, CCmanager[1]->LandLocation.y() - 1 );//1 turret in the main base
	     	ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret4);



		       BWAPI::TilePosition Turret5;//above comsat
		       BWAPI::TilePosition Turret6;//below CC

			  if( CCmanager[0]->LandLocation.y() < GeyserTile.y() -2 ){ //ref is down
		       Turret5 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() + 1, CCmanager[0]->LandLocation.y() + 4 );
		       Turret6 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() - 1, CCmanager[0]->LandLocation.y() + 4 );
			  }
			  if( CCmanager[0]->LandLocation.y() > GeyserTile.y() + 3 ){ //ref is up
		       Turret5 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() + 1, CCmanager[0]->LandLocation.y() -2 );
		       Turret6 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() - 1, CCmanager[0]->LandLocation.y() -2 );
			  }
			  if( CCmanager[0]->LandLocation.x() > GeyserTile.x() + 2 ){ //ref is left
		       Turret5 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() - 3, CCmanager[0]->LandLocation.y()  );
		       Turret6= BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() - 3, CCmanager[0]->LandLocation.y() -2 );
			  }
			  if( CCmanager[0]->LandLocation.x() < GeyserTile.x() - 4 ){ //ref is right
		       Turret5 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() + 6, CCmanager[0]->LandLocation.y()  );
		       Turret6 = BWAPI::TilePosition ( CCmanager[0]->LandLocation.x() + 6, CCmanager[0]->LandLocation.y() -2 );
			  }

	     	//ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret5);
	     	//ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, Turret6);



		}
		

	}

}

void ProductionManager::onUnitComplete(BWAPI::Unit *unit){

	if( unit->getPlayer() == Broodwar->enemy() ){
		return;
	}

	// && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss
	if(  ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss  )
		|| ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Zerg  )
		&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Supply_Depot ) == 2 ){
			if( GMan->alreadySupply != true ){
		     GMan->BarracksLift = true;
		     GMan->alreadySupply = true;
			}
	}

	/*
	if( unit->getType().isBuilding() ){
		reservedMinerals -= unit->getType().mineralPrice();
		reservedGas -= unit->getType().gasPrice();
	}
	*/


	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_Refinery ){
		Broodwar->printf("Refinery done");
		CCmanager[0]->Refinerys.push_back(unit);
		CCmanager[0]->toGas();
	}
	*/

	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_Refinery ){

		int closestCC = 999;
		int closeID = 0;
		for( unsigned int j=0; j<CCmanager.size(); j++ ){
			if(  CCmanager[j]->CommandCenter->getPosition().getDistance(unit->getPosition()) < closestCC ){
					closestCC = CCmanager[j]->CommandCenter->getPosition().getDistance(unit->getPosition());
					closeID = j;
			}
		}

		Broodwar->printf("Refinery done");
		CCmanager[closeID]->Refinerys.push_back( unit );
		CCmanager[closeID]->toGas();

	}
	*/

	if( unit->getType() == BWAPI::UnitTypes::Terran_Armory ){
		Armory.push_back(unit);
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Science_Facility ){
		ScienceFacility = unit;
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Command_Center ){
		bool alreadyExists = false;
	    for( unsigned int i=0; i<CCmanager.size(); i++ ){
		  //no double entry
		  if( CCmanager[i]->CommandCenter->getID() == unit->getID() ){
			alreadyExists = true;
		  }
	    }
		if( !alreadyExists ){
		  //float if needed
			bool alreadBaseloc = false;
			BOOST_FOREACH( BWTA::BaseLocation* baseLoc ,  BWTA::getBaseLocations() ){
				if( unit->getTilePosition() == baseLoc->getTilePosition() ){
					alreadBaseloc = true;
					break;
				}
			}
		  if( alreadBaseloc == false ){
			BaseManager* newBase = new BaseManager( unit, InfoMan->OurNat );
		    CCmanager.push_back( newBase );
		  } else {
		    BaseManager* newBase = new BaseManager( unit, unit->getTilePosition() );
		    CCmanager.push_back( newBase );
		  }
			/*
		  if( Broodwar->self()->completedUnitCount(  BWAPI::UnitTypes::Terran_Command_Center ) == 2 ){
			BaseManager* newBase = new BaseManager( unit, InfoMan->OurNat );
		    CCmanager.push_back( newBase );
		  } else {
		    BaseManager* newBase = new BaseManager( unit, unit->getTilePosition() );
		    CCmanager.push_back( newBase );
		  }
		  */

		}

	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Engineering_Bay ){
		Broodwar->printf("Added engineering bay");
		EngineeringBay = unit;
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
			addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition(InfoMan->PosMainChoke) );
			addToQueue( BWAPI::UnitTypes::Terran_Missile_Turret);
		}
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Starport ){
		Broodwar->printf("Added starport");
		StarportNoAddon.push_back(unit);
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Control_Tower ){
		Broodwar->printf("Control tower done");
		for(unsigned int i=0; i<StarportNoAddon.size(); i++){
			if(  StarportNoAddon[i]->getAddon() != NULL ){
				StarportCtower.push_back( StarportNoAddon[i] );
				StarportNoAddon.erase( StarportNoAddon.begin() + i );
				break;
			}
		}
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Factory ){
		FactoryNoAddon.push_back(unit);
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop ){
		for(unsigned int i=0; i<FactoryNoAddon.size(); i++){
			if(  FactoryNoAddon[i]->getAddon() != NULL ){
				FactoryShop.push_back( FactoryNoAddon[i] );
				FactoryNoAddon.erase( FactoryNoAddon.begin() + i );
				break;
			}
		}
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Barracks ){
		Barracks.push_back(unit);
	}

	if( unit->getType() == BWAPI::UnitTypes::Terran_Academy ){
		Academy = unit;
		/*
		if( CurrentStrategy == Bio_8_Rax ){
		CCmanager[0]->toGas(2);//put two more on gas
		}
		*/

		if( MacroMan->CurrentStrat == "2 rax FE"){
			//addToQueue( BWAPI::UnitTypes::Terran_Missile_Turret);
			//addToQueue( BWAPI::UnitTypes::Terran_Missile_Turret);
			//get tile position between CC and mineral field
			//BWAPI::TilePosition Tur1( InfoMan->OurNat.x() + 1, InfoMan->OurNat.y() - 2);
			//BWAPI::TilePosition Tur2( InfoMan->OurNat.x() + 1, InfoMan->OurNat.y() + 4);
			//addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret,Tur1);
			//addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret,Tur2);
		}

	}

	if (unit->getType().isWorker()){
		bool alreadyAssigned = false;
		if( ScoutMan != NULL ){
		  for(unsigned int i=0; i<ScoutMan->workerScouts.size(); i++ ){
			if( ScoutMan->workerScouts[i].scv->getID() == unit->getID() ){
				alreadyAssigned = true;
			  }
		  }
		}
		for(unsigned int j=0; j<ProdMan->BuildingsQueue.size(); j++){
			if( ProdMan->BuildingsQueue[j].scv != NULL ){
				if( ProdMan->BuildingsQueue[j].scv->getID() == unit->getID() ){
				  alreadyAssigned = true;
			  }
			}
		}
		if( !alreadyAssigned ){
			//find the command center that this unit belongs to
			int closestCC = 999;
			int closeID = 0;
			for( unsigned int j=0; j<CCmanager.size(); j++ ){
				if(  CCmanager[j]->CommandCenter->getPosition().getDistance(unit->getPosition()) < closestCC ){
					closestCC = CCmanager[j]->CommandCenter->getPosition().getDistance(unit->getPosition());
					closeID = j;
				}
			}
		  CCmanager[closeID]->addSCV( unit );
		}

	}

}