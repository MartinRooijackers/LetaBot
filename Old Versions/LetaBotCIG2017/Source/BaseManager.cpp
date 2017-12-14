#include "BaseManager.h"


#include <boost/foreach.hpp>


bool isLeft(BWAPI::Position a, BWAPI::Position b, BWAPI::Position c){
     return ((b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x())) >= 0;
}




void BaseManager::CalCulateTrickChoice(){

	if( LanLatency == true){
	  CalCulateTrick();
	}

}

BaseManager::BaseManager( Unit* CC , BWAPI::TilePosition land ){


	Broodwar->printf("added command center");

	InitialMinerals = 0; //will be added on addMineralField()
	SCVsonGas = 3;
	cycleMineral = 0;
	MineralSCV.clear();
	CommandCenter = CC;
	LandLocation = land;
	buildComsat = false;

	if( LandLocation == CC->getTilePosition() ){
		BaseReady = true;
	} else {
		BaseReady = false;
	}

	BWAPI::Position landPos = BWAPI::Position( land );

	if( BaseReady == true ){

		/*
      for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
      {
          if ( CommandCenter->getDistance(*m)<  11*32 ){
             //Minerals.push_back( *m);
			 addMineralField(*m);
		  }
      }
	  */

	  std::set<Unit*> GetMinerals = CommandCenter->getUnitsInRadius( 15*32 );
		BOOST_FOREACH( Unit* mineral, GetMinerals )
        {
			if(  mineral->getType() == BWAPI::UnitTypes::Resource_Mineral_Field   ){
				addMineralField( mineral );
			}
		}

	  CalCulateTrickChoice();
	  for(std::set<Unit*>::iterator g=Broodwar->getGeysers().begin();g!=Broodwar->getGeysers().end();g++)
      {
          if ( CommandCenter->getDistance(*g) < 11*32 ){
            Geysers.push_back(*g);
			//Broodwar->printf("added geyser");
		  }
      }
	  //SCVsaturation = Minerals.size() * 2 + 2; //force gather trick is almost saturated at 2 times minerals
	}

    /*
	for(unsigned int i=0; i<InfoMan->Minerals.size(); i++){
		if ( landPos.getDistance( InfoMan->Minerals[i]->getPosition() )<  300 ){
            Minerals.push_back( InfoMan->Minerals[i] );
		  }
	}
	for(unsigned int i=0; i<InfoMan->Geysers.size(); i++){
		if ( landPos.getDistance( InfoMan->Geysers[i]->getPosition() )<  300 ){
			Geysers.push_back( InfoMan->Geysers[i] );
			Broodwar->printf("added geyser");
		  }
	}

	*/
	/*
    for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
    {
          if ( CommandCenter->getDistance(*m)<  300 ){
            Minerals.push_back( *m);
		  }
    }
	for(std::set<Unit*>::iterator g=Broodwar->getGeysers().begin();g!=Broodwar->getGeysers().end();g++)
    {
          if ( CommandCenter->getDistance(*g)<  300 ){
            Geysers.push_back(*g);
			Broodwar->printf("added geyser");
		  }
    }
	*/


	//SCVsaturation = Minerals.size() * 2.5; //build in is (almost) saturated at 2.5 times minerals
	//SCVsaturation = Minerals.size() * 2 + 2; //force gather trick is almost saturated at 2 times minerals


	//maynarding
	if( BaseReady == true && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) == 2 ){
	  for(int i=0; i<6; i++){
			addSCV( CCmanager[0]->getBuilder() );
	  }
	  //add a refinery
	  if( Geysers.size() > 0){
		  // ||  (Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && CCmanager.size() >= 2 )
			if( Broodwar->enemy()->getRace() != BWAPI::Races::Zerg    ){
		      ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Refinery, Geysers[0]->getTilePosition() );
			}		  
	    //ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Refinery, Geysers[0]->getTilePosition() );
	  }
	}

	if( BaseReady == true && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) > 2 ){
	  if( Geysers.size() > 0){
			//if( Broodwar->enemy()->getRace() != BWAPI::Races::Zerg ){
		      ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Refinery, Geysers[0]->getTilePosition() );
			//}
	    //ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Refinery, Geysers[0]->getTilePosition() );
	  }
	}

}


void BaseManager::addSCV( Unit* scv ){

	    if( scv == NULL ){
			return;
		}

	    /*
	    scv->rightClick( Minerals[cycleMineral] );
		cycleMineral++;
		cycleMineral = cycleMineral % Minerals.size();
		MineralSCV.push_back( scv );
		*/

	    //check if the scv is already assigned
		for(unsigned int i=0; i<MineralSCV.size(); i++){
			if( MineralSCV[i].scv->getID() == scv->getID() ){
				//Broodwar->printf("scv already stored");
				return;
			}
		}

	  SCVmineral newSCV;
	  newSCV.scv = scv;
	  newSCV.mineralPatch = NULL;
	  newSCV.useTrick = false;
	  int minMin = 9999;
      int closMin = -1;
	  for(unsigned int j=0; j<Minerals.size(); j++){
		int curSCV = 0 ;
		for(unsigned int i=0; i<MineralSCV.size(); i++){
			if( MineralSCV[i].mineralPatch == NULL){
				Broodwar->printf("assignment problems mineral scv");
				continue;
			}
			if( MineralSCV[i].mineralPatch->getID() == Minerals[j].mineralPatch->getID() ){
				curSCV++;
			}
		}
		if( curSCV < minMin ){
	      minMin = curSCV;
          closMin = j;
		}
	  }

	  if( closMin != -1 ){
	    newSCV.mineralPatch = Minerals[closMin].mineralPatch;
		if( Minerals[closMin].mineralTrick != NULL  ){
			newSCV.useTrick = true;
			newSCV.mineralTrick = Minerals[closMin].mineralTrick;
		}
	  }
	  if( newSCV.mineralPatch != NULL  ){
	    newSCV.scv->gather( newSCV.mineralPatch );
	  }
	  if( newSCV.mineralPatch == NULL  ){
		  Broodwar->printf("SCV not assigned to mineral patch");
	  }
	  MineralSCV.push_back( newSCV );
}

void BaseManager::buildSCV(){

	if( BaseReady == false ){
		return;
	}

	if( MineralSCV.size() >= SCVsaturation ){
		return;
	}

	if( ProdMan->Academy != NULL && CommandCenter->getAddon() == NULL
		 && buildComsat == true){
		return;//build comsat station first
	}

	if( CommandCenter->getTrainingQueue().size() == 0 ){
		CommandCenter->train(BWAPI::UnitTypes::Terran_SCV);
	}

}


void BaseManager::addMineralField(Unit* mineral){

	InitialMinerals++;

	//Minerals.push_back(mineral);
	bool isPathTrick = false;
	
	/*
	for(int i=0; i<InfoMan->FSmineral.MineralID.size(); i++){
		if( InfoMan->FSmineral.MineralID[i] == mineral->getID() ){
			isPathTrick = true;
		}
	}
	*/
	isPathTrick = false;

	MineralPatch newpatch;
	newpatch.trickPossible = isPathTrick;
	newpatch.mineralPatch = mineral;
	newpatch.mineralTrick = NULL; //still has to be determined at calcualte trick
	Minerals.push_back(newpatch);

}


void BaseManager::CalCulateTrick(){
	/*
		for(int i=0; i<Minerals.size(); i++){
		  for(int j=0; j<Minerals.size(); j++){
			  if( i != j && Minerals[j].trickPossible && !Minerals[i].trickPossible
				  && Minerals[i].mineralPatch->getDistance( Minerals[j].mineralPatch ) < 1*32 ){
					  Minerals[i].mineralTrick = Minerals[j].mineralPatch;
			  }
		  }
		}
	*/

	int GetMineralPos[5] = {0,0,0,0,0};

	for(int i=0; i<Minerals.size(); i++){

		//determine orientation for mineral field, 1=up,2=bottom,3=left,4=right
		int orientation = 0;
	    BWAPI::Position TopLeft( CommandCenter->getPosition().x() - 32*10, CommandCenter->getPosition().y() - 32*10 );
	    BWAPI::Position BottomRight( CommandCenter->getPosition().x() + 32*10, CommandCenter->getPosition().y() + 32*10 );
	    BWAPI::Position BottomLeft( CommandCenter->getPosition().x() - 32*10, CommandCenter->getPosition().y() + 32*10 );
	    BWAPI::Position TopRight( CommandCenter->getPosition().x() + 32*10, CommandCenter->getPosition().y() - 32*10 );
		bool isUpRight = false;
		bool isBottomLeft = false;
		if( isLeft( TopLeft, BottomRight, Minerals[i].mineralPatch->getPosition() ) ){
			isBottomLeft = true;
		}
		if( isLeft( BottomLeft, TopRight, Minerals[i].mineralPatch->getPosition() ) ){
			isUpRight = true;
		}
		if( isBottomLeft == true && isUpRight == true){
          orientation = 2;
		}
		if( isBottomLeft == true && isUpRight == false){
          orientation = 3;
		}
		if( isBottomLeft == false && isUpRight == true){
          orientation = 4;
		}
		if( isBottomLeft == false && isUpRight == false){
          orientation = 1;
		}
		GetMineralPos[ orientation ]++;
	}

	int MineralOrient = 0;
	int bestCount = 0;
	for(int i=1; i<=4; i++){
		if( GetMineralPos[i] > bestCount){
			bestCount = GetMineralPos[i];
			MineralOrient = i;
		}
	}

	for(int i=0; i<Minerals.size(); i++){

		//determine orientation for mineral field, 1=up,2=bottom,3=left,4=right
		int orientation = 0;
	    BWAPI::Position TopLeft( CommandCenter->getPosition().x() - 32*10, CommandCenter->getPosition().y() - 32*10 );
	    BWAPI::Position BottomRight( CommandCenter->getPosition().x() + 32*10, CommandCenter->getPosition().y() + 32*10 );
	    BWAPI::Position BottomLeft( CommandCenter->getPosition().x() - 32*10, CommandCenter->getPosition().y() + 32*10 );
	    BWAPI::Position TopRight( CommandCenter->getPosition().x() + 32*10, CommandCenter->getPosition().y() - 32*10 );
		bool isUpRight = false;
		bool isBottomLeft = false;
		if( isLeft( TopLeft, BottomRight, Minerals[i].mineralPatch->getPosition() ) ){
			isBottomLeft = true;
		}
		if( isLeft( BottomLeft, TopRight, Minerals[i].mineralPatch->getPosition() ) ){
			isUpRight = true;
		}
		if( isBottomLeft == true && isUpRight == true){
          orientation = 2;
		}
		if( isBottomLeft == true && isUpRight == false){
          orientation = 3;
		}
		if( isBottomLeft == false && isUpRight == true){
          orientation = 4;
		}
		if( isBottomLeft == false && isUpRight == false){
          orientation = 1;
		}


		int TotalTrick = 0;
		  for(int j=0; j<Minerals.size(); j++){
			  if( i != j && Minerals[i].mineralPatch->getDistance( Minerals[j].mineralPatch ) <= 1*16 ){
				  if(  orientation == 1 && MineralOrient == 1 ){
					  TotalTrick++;
				  }
				  if(  orientation == 2 && MineralOrient == 2 ){
					  //Broodwar->printf("Added trick bottom");
					  TotalTrick++;
				  }
				  if(  orientation == 3 && MineralOrient == 3 ){
					  //Broodwar->printf("Added trick left");
					  TotalTrick++;
				  }
				  if(  orientation == 4 && MineralOrient == 4 ){
					  //Broodwar->printf("Added trick right");
					  TotalTrick++;
				  }
				  //Minerals[i].mineralTrick = Minerals[j].mineralPatch;
			  }
		  }
		  /*
		  if( TotalTrick != 2){
			  continue;
		  }
		  */
		  for(int j=0; j<Minerals.size(); j++){
			  if( i != j && Minerals[i].mineralPatch->getDistance( Minerals[j].mineralPatch ) <= 1*16 ){
				  if(  orientation == 1 && MineralOrient == 1 ){
					  //Broodwar->printf("Added trick up");
					  if(  Minerals[i].mineralPatch->getPosition().y() < Minerals[j].mineralPatch->getPosition().y()){
						  Minerals[j].trickPossible = true;
						  Minerals[j].mineralTrick = Minerals[i].mineralPatch;
					  }
				  }
				  if(  orientation == 2 && MineralOrient == 2 ){
					  //Broodwar->printf("Added trick bottom");
					  if(  Minerals[i].mineralPatch->getPosition().y() > Minerals[j].mineralPatch->getPosition().y()){
						  Minerals[j].trickPossible = true;
						  Minerals[j].mineralTrick = Minerals[i].mineralPatch;
					  }
				  }
				  if(  orientation == 3 && MineralOrient == 3 ){
					  //Broodwar->printf("Added trick left");
					  if(  Minerals[i].mineralPatch->getPosition().x() < Minerals[j].mineralPatch->getPosition().x()){
						  Minerals[j].trickPossible = true;
						  Minerals[j].mineralTrick = Minerals[i].mineralPatch;
					  }
				  }
				  if(  orientation == 4 && MineralOrient == 4 ){
					  //Broodwar->printf("Added trick right");
					  if(  Minerals[i].mineralPatch->getPosition().x() > Minerals[j].mineralPatch->getPosition().x()){
						  Minerals[j].trickPossible = true;
						  Minerals[j].mineralTrick = Minerals[i].mineralPatch;
					  }
				  }
				  //Minerals[i].mineralTrick = Minerals[j].mineralPatch;
			  }
		  }




	}

}


void BaseManager::MineralGather(){

	for(unsigned int i=0; i<MineralSCV.size(); i++){
		if( !MineralSCV[i].scv->exists() ){//remove destroyed scvs
			MineralSCV.erase( MineralSCV.begin() + i );
			i--;
			continue;
		}
		//check if scv is not used in constructing buildings
		bool alreadyBuilding = false;
		for(unsigned int j=0; j<ProdMan->BuildingsQueue.size(); j++){
			if( ProdMan->BuildingsQueue[j].scv != NULL ){
				if( ProdMan->BuildingsQueue[j].scv->getID() == MineralSCV[i].scv->getID() ){
					alreadyBuilding = true;
			  }
			}
		}
		if( alreadyBuilding ){
			Broodwar->printf("scv already assigned to building");
			MineralSCV.erase( MineralSCV.begin() + i );
			i--;
			continue;
		}


		//no mineral patches availabe anymore
		if(  MineralSCV[i].mineralPatch == NULL  ){
			//Broodwar->printf("No mineral patch assigned to scv");
          //TODO: give this scv another task
			continue;
		}

		//reassign SCV when current mineral patch has run out
		if(  MineralSCV[i].mineralPatch != NULL  ){
			if( !MineralSCV[i].mineralPatch->exists() ){
			  Broodwar->printf("Mineral patch used up. Reassigning SCV");
			  BWAPI::Unit* thisSCV = MineralSCV[i].scv;
			  MineralSCV.erase( MineralSCV.begin() + i);
			  addSCV( thisSCV );
			  break; //TODO: make this a continue that doesn't mess up the MineralSCV loop
			}
		}


		if( MineralSCV[i].useTrick == false){
		  if( !MineralSCV[i].scv->isGatheringMinerals() ){
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
		  }

		  //scv currently working at the correct mineral patch
		  if(  MineralSCV[i].scv->isCarryingMinerals()   ){
			  if(  MineralSCV[i].scv->getOrder() != BWAPI::Orders::ReturnMinerals){
				  MineralSCV[i].scv->returnCargo();
			    continue;
			  }
			continue;
		  }
		  if(  MineralSCV[i].scv->getOrderTarget() == NULL  ){
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
			continue;
		  }

		  //scv going to the wrong mineral patch
		  if( MineralSCV[i].scv->getOrderTarget()->getID()  !=  MineralSCV[i].mineralPatch->getID()){
			//Broodwar->printf("reassinging scv");
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
		  }
		} else if( MineralSCV[i].mineralTrick != NULL ) { //always gotta check for null pointers

		  //scv currently working at the correct mineral patch
		  if(  MineralSCV[i].scv->isCarryingMinerals()   ){
			  if(  MineralSCV[i].scv->getOrder() != BWAPI::Orders::ReturnMinerals){
				  MineralSCV[i].scv->returnCargo();
			    continue;
			  }
			  continue;
		  }

		  //use path trick as long as the SCV isn't close to the actual mineral field
		  if( MineralSCV[i].mineralPatch->getDistance( MineralSCV[i].scv->getPosition() ) > 40 ){
			  //don't spam
			  if( MineralSCV[i].scv->getTarget() != NULL ){
				  if( MineralSCV[i].scv->getTarget()->getID() == MineralSCV[i].mineralTrick->getID() ){
					  continue;
			    }
			  }
			  MineralSCV[i].scv->gather( MineralSCV[i].mineralTrick );
			  continue;
		  }

		  //use path trick as long as the SCV isn't close to the actual mineral field
		  if( MineralSCV[i].mineralPatch->getDistance( MineralSCV[i].scv->getPosition() ) <= 40 ){
			  if( MineralSCV[i].scv->getTarget() != NULL ){
				  if( MineralSCV[i].scv->getTarget()->getID() == MineralSCV[i].mineralPatch->getID() ){
					  continue;
			    }
			  }
			  MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
			  continue;
		  }

		  /*
		  if( !MineralSCV[i].scv->isGatheringMinerals() ){
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
		  }

		  if(  MineralSCV[i].scv->getOrderTarget() == NULL  ){
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
			continue;
		  }

		  //scv going to the wrong mineral patch
		  if( MineralSCV[i].scv->getOrderTarget()->getID()  !=  MineralSCV[i].mineralPatch->getID()){
			//Broodwar->printf("reassinging scv");
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
		  }
		  */

		}
		/*
		//set idle scvs to mine
		if(  !MineralSCV[i].scv->isGatheringMinerals() ){

	      MineralSCV[i]->rightClick( Minerals[cycleMineral] );
		  cycleMineral++;
		  cycleMineral = cycleMineral % Minerals.size();

		}
		*/

	}



}


void BaseManager::MineralGatherLock(){

	for(unsigned int i=0; i<MineralSCV.size(); i++){
		if( !MineralSCV[i].scv->exists() ){//remove destroyed scvs
			MineralSCV.erase( MineralSCV.begin() + i );
			i--;
			continue;
		}
		//check if scv is not used in constructing buildings
		bool alreadyBuilding = false;
		for(unsigned int j=0; j<ProdMan->BuildingsQueue.size(); j++){
			if( ProdMan->BuildingsQueue[j].scv != NULL ){
				if( ProdMan->BuildingsQueue[j].scv->getID() == MineralSCV[i].scv->getID() ){
					alreadyBuilding = true;
			  }
			}
		}
		if( alreadyBuilding ){
			Broodwar->printf("scv already assigned to building");
			MineralSCV.erase( MineralSCV.begin() + i );
			i--;
			continue;
		}


		//no mineral patches availabe anymore
		if(  MineralSCV[i].mineralPatch == NULL  ){
			//Broodwar->printf("No mineral patch assigned to scv");
          //TODO: give this scv another task
			continue;
		}

		//reassign SCV when current mineral patch has run out
		if(  MineralSCV[i].mineralPatch != NULL  ){
			if( !MineralSCV[i].mineralPatch->exists() ){
			  Broodwar->printf("Mineral patch used up. Reassigning SCV");
			  BWAPI::Unit* thisSCV = MineralSCV[i].scv;
			  MineralSCV.erase( MineralSCV.begin() + i);
			  addSCV( thisSCV );
			  break; //TODO: make this a continue that doesn't mess up the MineralSCV loop
			}
		}


		  if( !MineralSCV[i].scv->isGatheringMinerals() ){
			 MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
			 //continue;
		  }

		  //scv currently working at the correct mineral patch
		  if(  MineralSCV[i].scv->isCarryingMinerals()   ){
			  if(  MineralSCV[i].scv->getOrder() != BWAPI::Orders::ReturnMinerals){
				  MineralSCV[i].scv->returnCargo();
			    continue;
			  }
			continue;
		  }
		  if(  MineralSCV[i].scv->getOrderTarget() == NULL  ){
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
			continue;
		  }

		  //scv going to the wrong mineral patch
		  if( MineralSCV[i].scv->getOrderTarget()->getID()  !=  MineralSCV[i].mineralPatch->getID()){
			//Broodwar->printf("reassinging scv");
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
		  }
		

		/*
		//set idle scvs to mine
		if(  !MineralSCV[i].scv->isGatheringMinerals() ){

	      MineralSCV[i]->rightClick( Minerals[cycleMineral] );
		  cycleMineral++;
		  cycleMineral = cycleMineral % Minerals.size();

		}
		*/

	}



}


void BaseManager::MineralGatherNoTrick(){


	for(unsigned int i=0; i<MineralSCV.size(); i++){
		if( !MineralSCV[i].scv->exists() ){//remove destroyed scvs
			MineralSCV.erase( MineralSCV.begin() + i );
			i--;
			continue;
		}
		//check if scv is not used in constructing buildings
		bool alreadyBuilding = false;
		for(unsigned int j=0; j<ProdMan->BuildingsQueue.size(); j++){
			if( ProdMan->BuildingsQueue[j].scv != NULL ){
				if( ProdMan->BuildingsQueue[j].scv->getID() == MineralSCV[i].scv->getID() ){
					alreadyBuilding = true;
			  }
			}
		}
		if( alreadyBuilding ){
			Broodwar->printf("scv already assigned to building");
			MineralSCV.erase( MineralSCV.begin() + i );
			i--;
			continue;
		}

		//no mineral patches availabe anymore
		if(  MineralSCV[i].mineralPatch == NULL  ){
			//Broodwar->printf("No mineral patch assigned to scv");
          //TODO: give this scv another task
			continue;
		}

		//reassign SCV when current mineral patch has run out
		if(  MineralSCV[i].mineralPatch != NULL  ){
			if( !MineralSCV[i].mineralPatch->exists() ){
			  Broodwar->printf("Mineral patch used up. Reassigning SCV");
			  BWAPI::Unit* thisSCV = MineralSCV[i].scv;
			  MineralSCV.erase( MineralSCV.begin() + i);
			  addSCV( thisSCV );
			  break; //TODO: make this a continue that doesn't mess up the MineralSCV loop
			}
		}

		  if( !MineralSCV[i].scv->isGatheringMinerals() ){
			MineralSCV[i].scv->gather( MineralSCV[i].mineralPatch );
		  }

		  if(  MineralSCV[i].scv->isCarryingMinerals()   ){
			  if(  MineralSCV[i].scv->getOrder() != BWAPI::Orders::ReturnMinerals){
				  MineralSCV[i].scv->returnCargo();
			    continue;
			  }
			continue;
		  }
	}
}


void BaseManager::MineralGatherChoice(){


    if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }

	if( LanLatency == true){
	  //MineralGather(); //TODO: compensate for latency
	  MineralGatherLock();
	} else {
	  MineralGatherNoTrick();
	}

}


void BaseManager::LiftToNat(){

    if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }


			if( !CommandCenter->isLifted() ){
				CommandCenter->lift();
			}

			if( CommandCenter->isLifted() && !Broodwar->isVisible( LandLocation ) ){
				CommandCenter->move( BWAPI::Position( LandLocation ) );
			}
			else if( CommandCenter->isLifted() && CommandCenter->getOrder() != BWAPI::Orders::BuildingLand ){
				CommandCenter->land( LandLocation );
			}

}

void BaseManager::onFrame(){

	for(int i=0; i<Minerals.size(); i++){
		if( Minerals[i].mineralTrick != NULL ){
			Broodwar->drawCircleMap( Minerals[i].mineralPatch->getPosition().x(),Minerals[i].mineralPatch->getPosition().y(),4,BWAPI::Colors::Purple,true);		   
		}
	}
	
	for(int i=0; i<MineralSCV.size(); i++){
		if( MineralSCV[i].useTrick == true ){
			//Broodwar->drawCircleMap( Minerals[i].mineralPatch->getPosition().x(),Minerals[i].mineralPatch->getPosition().y(),2,BWAPI::Colors::Purple,true);
			BWAPI::Position scvpos = MineralSCV[i].scv->getPosition();
			BWAPI::Position minpos = MineralSCV[i].mineralPatch->getPosition();
			Broodwar->drawLineMap(scvpos.x(),scvpos.y(),minpos.x(),minpos.y(),BWAPI::Colors::Red);
		}
	}
	

	//Broodwar->drawTextMap( CommandCenter->getPosition().x() , CommandCenter->getPosition().y(), "%d" , BaseReady );
	//BWAPI::Position LanLocPos = BWAPI::Position( LandLocation );
	//Broodwar->drawCircleMap( LanLocPos.x(),  LanLocPos.y(), 5 , BWAPI::Colors::Orange , true );

	if( BaseReady == false){
		if( LandLocation == CommandCenter->getTilePosition() 
			&& !CommandCenter->isLifted()
			&&  CommandCenter->getOrder() != BWAPI::Orders::BuildingLand){
		BaseReady = true;
		Broodwar->printf("CC has landed");

		if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){

			BWAPI::TilePosition TurretLoc = bManager->getBuildLocationNear(LandLocation,BWAPI::UnitTypes::Terran_Missile_Turret,0);
			BWAPI::TilePosition TurretLoc2 = BWAPI::TilePosition( LandLocation.x() + 4, LandLocation.y() - 2);




	  int ClosestChokeDist = 99999;
	  BWAPI::Position BunkerPos = BWAPI::Positions::None;
	  BWAPI::Position natPos = BWAPI::Position(InfoMan->OurNat);
	  BWAPI::Position mainPos = InfoMan->PosOurBase; //BWAPI::Position(InfoMan->PosOurBase);
	  BWTA::Region* natRegion = BWTA::getRegion( natPos );
	  BWTA::Region* mainRegion = BWTA::getRegion( mainPos  );

       BOOST_FOREACH( BWTA::Chokepoint*  choke, BWTA::getChokepoints() ){
		   if( (choke->getRegions().first == natRegion && choke->getRegions().second != mainRegion ) ||
		   (choke->getRegions().second == natRegion && choke->getRegions().first != mainRegion ) ){
			 if( choke->getCenter().getDistance( InfoMan->PosEnemyBase ) < ClosestChokeDist ){
			   ClosestChokeDist = choke->getCenter().getDistance( InfoMan->PosEnemyBase );
			   BunkerPos = choke->getCenter();
			 }
	     }
       }




	   if( TurretLoc.getDistance( BWAPI::TilePosition( BunkerPos ) ) < TurretLoc2.getDistance( BWAPI::TilePosition( BunkerPos ) ) ){
		   ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, TurretLoc );
	        } else{
			ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Missile_Turret, TurretLoc2 );
	        }

		}

		if(  InfoMan->PosOurNat.getDistance( BWAPI::Position( LandLocation ) )  < 5*32  ){
		  //MacroMan->PlaceNatBunker();
		}

		/*
        for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
        {
          if ( CommandCenter->getDistance(*m) <  11*32 ){
            //Minerals.push_back( *m);
			  addMineralField(*m);
		  }
        }
		*/
	    std::set<Unit*> GetMinerals = CommandCenter->getUnitsInRadius( 15*32 );
		BOOST_FOREACH( Unit* mineral, GetMinerals )
        {
			if(  mineral->getType() == BWAPI::UnitTypes::Resource_Mineral_Field   ){
				addMineralField( mineral );
			}
		}

		CalCulateTrickChoice();
	    for(std::set<Unit*>::iterator g=Broodwar->getGeysers().begin();g!=Broodwar->getGeysers().end();g++)
        {
          if ( CommandCenter->getDistance(*g)<  11*32 ){
            Geysers.push_back(*g);
			Broodwar->printf("added geyser");
		  }
        }

		//Maynarding
		//CCmanager[0]->SCVsaturation -= 18;
		for(int i=0; i<8; i++){
			addSCV( CCmanager[0]->getBuilder() );
		}

		//add a refinery
		if( Geysers.size() > 0){
			if( Broodwar->enemy()->getRace() != BWAPI::Races::Zerg ){
		      ProdMan->addToQueueTile( BWAPI::UnitTypes::Terran_Refinery, Geysers[0]->getTilePosition() );
			}
		}

		/*
		//remove initial defence
		IDMan->stopDefence = true;
		BOOST_FOREACH( Unit* unit, IDMan->DFunits ){
			AMan->AddUnit(unit);
		}
		BOOST_FOREACH( Unit* unit, IDMan->Vultures ){
			AMan->AddUnit(unit);
		}
		IDMan->DFunits.clear();
		IDMan->Vultures.clear();
		*/

		return;
	  } //if ( lanlocation == CC.landposition

	  if( MacroMan->PState == P_Push || MacroMan->PState == P_Defend_Nat ){
		  LiftToNat();
		  /*
			if( !CommandCenter->isLifted() ){
				CommandCenter->lift();
			}

			if( CommandCenter->isLifted() && !Broodwar->isVisible( LandLocation ) ){
				CommandCenter->move( BWAPI::Position( LandLocation ) );
			}
			else if( CommandCenter->isLifted() && CommandCenter->getOrder() != BWAPI::Orders::BuildingLand ){
				CommandCenter->land( LandLocation );
			}
			*/
		}
	  if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){ //MacroMan->ZState == Z_Expand_Defend
		   LiftToNat();
		   /*
			if( !CommandCenter->isLifted() ){
				CommandCenter->lift();
			}

			if( CommandCenter->isLifted() && !Broodwar->isVisible( LandLocation ) ){
				CommandCenter->move( BWAPI::Position( LandLocation ) );
			}
			else if( CommandCenter->isLifted() && CommandCenter->getOrder() != BWAPI::Orders::BuildingLand ){
				CommandCenter->land( LandLocation );
			}
			*/
	  }
	}

	if( LanLatency == true ){
      SCVsaturation = Minerals.size()*2 + 1;//Update saturation
	} else {
	  SCVsaturation = Minerals.size()*2.5 + 1;//Update saturation
	}
	//build a comsat station after saturation is complete
	if( MineralSCV.size() >= (SCVsaturation - 2 )
		 && buildComsat == false){
		buildComsat = true;
		Broodwar->printf("Enough SCV, now building comsat");
	}

	//don't build a comsat station when saturation isn't complete
	if( MineralSCV.size() < (SCVsaturation - 2 )
		 && buildComsat == true){
		buildComsat = false;
		//Broodwar->printf("Enough SCV, now building comsat");
	}

	//check if there are to many SCVs here, if so let them join the fight
	if( MineralSCV.size() > SCVsaturation + 3 ){
		//in case of terran, just add SCVs to other CC
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
			bool addedToCC = false;
			for(int i=0; i<CCmanager.size(); i++ ){
				if( CCmanager[i]->SCVsaturation > CCmanager[0]->MineralSCV.size() ){
					if( CCmanager[i]->CommandCenter != NULL ){
						if( CCmanager[i]->CommandCenter->exists() ){
							addedToCC = true;
							CCmanager[i]->addSCV( getFullHPSCV() );
							break;
						}
					}
				}
			}
			//if not possible just add it to the fight
			if( addedToCC == false){
				MacroMan->AddSCV( getFullHPSCV() );
			}
		} else {
		  MacroMan->AddSCV( getFullHPSCV() );
		}
	}


	if( BaseReady == true && buildComsat ){
		//build a comsat station when it is complete
		if( ProdMan->Academy != NULL && CommandCenter->getAddon() == NULL ){
			CommandCenter->buildAddon( BWAPI::UnitTypes::Terran_Comsat_Station );
		}
	}


	for(unsigned int i=0; i<Minerals.size(); i++){
		if( !Minerals[i].mineralPatch->exists() ){//remove empty mineral field
			Minerals.erase( Minerals.begin() + i );
			i--;
			continue;
		}
		//draw ID data
		//Broodwar->drawTextMap( Minerals[i].mineralPatch->getPosition().x(),Minerals[i].mineralPatch->getPosition().y(),"%d",Minerals[i].mineralPatch->getID() );
	}

	//MineralGather();
	MineralGatherChoice(); //functon used to get SCVs to gather minerals


	//remove dead SCVs on gas
	for(unsigned int i=0; i<GasSCV.size(); i++){
		if( !GasSCV[i]->exists() ){//remove dead scv
			GasSCV.erase( GasSCV.begin() + i );
			i--;
			continue;
		}
	}


	if( Refinerys.size() > 0 && GasSCV.size() < SCVsonGas ){
		toGas(  3 - GasSCV.size() );
	}

	if( GasSCV.size() > SCVsonGas && GasSCV.size() > 0 ){
		addSCV( GasSCV.back() );
		GasSCV.pop_back();
	}

	for(unsigned int i=0; i<GasSCV.size(); i++){
		if( !GasSCV[i]->isGatheringGas() && Refinerys.size() > 0  ){
			GasSCV[i]->gather( Refinerys[0] );
		}
	}

}


Unit* BaseManager::getBuilder(){

	Unit* builder = NULL;
	for(int i=0; i<MineralSCV.size(); i++){
		//check if the SCV is not to far away
		if(  MineralSCV[i].scv->getDistance( CommandCenter ) < 4*32 ){
			builder = MineralSCV[i].scv;
			MineralSCV.erase( MineralSCV.begin() + i);
			break;
		}

	}
	/*
	if(  MineralSCV.size() > 0 ){
	  SCVmineral getBuilder = MineralSCV.back();
	  MineralSCV.pop_back();
	  builder = getBuilder.scv;
	  //builder = MineralSCV[0];
	  //MineralSCV.erase( MineralSCV.begin() );
	}
	*/
	return builder;
}

Unit* BaseManager::getFullHPSCV(){

	Unit* scv = NULL;
	for(int i=0; i<MineralSCV.size(); i++){
		//check if the SCV is not to far away and full hp
		if(  MineralSCV[i].scv->getDistance( CommandCenter ) < 4*32
			&& MineralSCV[i].scv->getHitPoints() == 60 ){
			scv = MineralSCV[i].scv;
			MineralSCV.erase( MineralSCV.begin() + i);
			break;
		}

	}
	//grab the highest hp possible
	if( scv == NULL ){
		int highestHP = 0;
		int choice = -1;
	  for(int i=0; i<MineralSCV.size(); i++){
		//check if the SCV is not to far away and full hp
		if(  MineralSCV[i].scv->getDistance( CommandCenter ) < 4*32
			&& MineralSCV[i].scv->getHitPoints() > highestHP ){
			choice = i;
			highestHP = MineralSCV[i].scv->getHitPoints();
		}
		
	  }
	  if( choice != -1 ){

		scv = MineralSCV[choice].scv;
		MineralSCV.erase( MineralSCV.begin() + choice);

	  }
	}
	/*
	if(  MineralSCV.size() > 0 ){
	  SCVmineral getBuilder = MineralSCV.back();
	  MineralSCV.pop_back();
	  builder = getBuilder.scv;
	  //builder = MineralSCV[0];
	  //MineralSCV.erase( MineralSCV.begin() );
	}
	*/
	return scv;
}


//transfer 3 scvs to gas
void BaseManager::toGas(  int ToSend ){
	//int ToSend = 3;
	//Broodwar->printf("to gas");
  while( MineralSCV.size() > 0 ){
	  if( Refinerys.size() == 1 && GasSCV.size() < 3 ){
		 ToSend--;
		 Unit* gasSCV = getBuilder();
		 if( gasSCV == NULL ){
			 //return;
			 //continue;
			 break;
		 }
	    gasSCV->gather(Refinerys[0]);
	    GasSCV.push_back( gasSCV );
		Broodwar->printf("added 1 to gas");
		if( ToSend == 0 ){
			break;
		}
	  } else {
		  break;
	  }
  }
}