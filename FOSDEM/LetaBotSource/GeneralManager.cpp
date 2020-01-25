#include "GeneralManager.h"

#include "Common.h"

#include <boost/foreach.hpp>

GeneralManager::GeneralManager(){

	BarracksLift = false;
	putBdown = false;
	stillLifting = false;
	onlyOnce = false;
	alreadySupply = false;
}

void GeneralManager::GateManage()
{


	if(  BarracksLift == true ){
		BarracksLift = false;
		StartLift = Broodwar->getFrameCount();
		ProdMan->Barracks[0]->lift();
		putBdown = true;
	}
	if( Broodwar->getFrameCount() - StartLift > 500 && putBdown == true){
		putBdown = false;
		ProdMan->Barracks[0]->land( bManager->BarracksWall ); 
	}

}

void GeneralManager::GateSCV(){

	if( !bManager->WallSound ){
		return;
	}
	if(  ProdMan->Barracks.size() == 0){
		return;
	}

	//Broodwar->printf("Gate SCV");

	if(  BarracksLift == true ){
	    if( ProdMan->Barracks[0]->getTrainingQueue().size() != 0){
		ProdMan->Barracks[0]->cancelTrain();
	    }
		//BarracksLift = false;
		//StartLift = Broodwar->getFrameCount();
		ProdMan->Barracks[0]->lift();
		//putBdown = true;
		Broodwar->printf("Lifting barracks");
	}
	if(  BarracksLift == true && ProdMan->Barracks[0]->isLifted() ){
		BarracksLift = false;
		StartLift = Broodwar->getFrameCount();
		putBdown = true;
	}
	if( Broodwar->getFrameCount() - StartLift > 170 && putBdown == true){
		putBdown = false;
		if( bManager->WallSound == true ){
		  ProdMan->Barracks[0]->land( bManager->BarracksWall ); 
		} else {
			BWAPI::TilePosition tempMove = bManager->getBuildLocationNear( InfoMan->OurBase, BWAPI::UnitTypes::Terran_Barracks, BWTA::getRegion(BWAPI::TilePosition(InfoMan->PosOurBase) ) );
	      bManager->mapArea( bManager->getBuildRectangle(tempMove, BWAPI::UnitTypes::Terran_Barracks) , 0,0 );
		  ProdMan->Barracks[0]->land( tempMove ); 
		}
	}


}

void GeneralManager::StartLiftBarracks(){

	if( !bManager->WallSound ){
		return;
	}

	if( onlyOnce ){
		return;
	}

	onlyOnce = true;
	stillLifting = true;
	//if( bManager->WallSound ){ //if wall calculated, get location near choke
	toMove = bManager->getBuildLocationNear( bManager->BarracksWall, BWAPI::UnitTypes::Terran_Barracks, BWTA::getRegion(BWAPI::TilePosition(InfoMan->PosOurBase )) );
	//} else { //otherwise, get location near current location

	//}
	bManager->mapArea( bManager->getBuildRectangle(toMove, BWAPI::UnitTypes::Terran_Barracks) , 0,0 );

}

void GeneralManager::onFrame(){



	//check to see if the buildings are still there
	for(int i=0; i<InfoMan->EnemyBuildings.size(); i++){
		BWAPI::TilePosition BuildingTile( InfoMan->EnemyBuildings[i].position );
		if( Broodwar->isVisible( BWAPI::TilePosition( InfoMan->EnemyBuildings[i].position ) ) ){
			bool buildingonTile = false;
			//			BOOST_FOREACH( BWAPI::Unit unit, Broodwar->getUnitsOnTile( BuildingTile.x() , BuildingTile.y() ) ){
			BOOST_FOREACH( BWAPI::Unit unit, Broodwar->getUnitsOnTile( BuildingTile.x , BuildingTile.y ) ){
				if( unit->getType() == InfoMan->EnemyBuildings[i].type ){
					buildingonTile = true;
					break;
				}
			}
			if( buildingonTile == false ){
				InfoMan->EnemyBuildings.erase( InfoMan->EnemyBuildings.begin() + i );
				i--;
				continue;
			}
			//Broodwar->getUnitsOnTile
		}
	}

	//if( CurrentStrategy == Wall_in ){
	  //GateManage();
	//}

    /*

	if( ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss )
		|| ( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && MacroMan->CurrentStrat == "2 Port Wraith" )  ){ //&& Broodwar->enemy()->getRace() == BWAPI::Races::Protoss 
		GateSCV();
	}

	*/

	//scout at 13 scvs
	if(  ScoutMan == NULL 
		&& (Broodwar->enemy()->getRace() == BWAPI::Races::Unknown || Broodwar->enemy()->getRace() == BWAPI::Races::Random)
		&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV ) >= 9 ){
		ScoutMan = new ScoutManager(1);
	}
	
	
	//scout at 13 scvs
	if(  ScoutMan == NULL && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV ) >= 12 ){
		ScoutMan = new ScoutManager(1);
	}
	

	if( ( ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss )
		    || ( CurrentStrategy == Macro_Strat && Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && MacroMan->CurrentStrat == "2 Port Wraith" ) )
		&& stillLifting ){ //lift barracks when needed
		if(  ProdMan->Barracks[0]->getTilePosition() == toMove
			 && !ProdMan->Barracks[0]->isLifted()
			 && ProdMan->Barracks[0]->getOrder() != BWAPI::Orders::BuildingLand  ){
			stillLifting = false;
			//return;
		}
		if( !ProdMan->Barracks[0]->isLifted() && stillLifting ){
			ProdMan->Barracks[0]->lift();
		}
		if( ProdMan->Barracks[0]->isLifted() && ProdMan->Barracks[0]->getOrder() != BWAPI::Orders::BuildingLand 
			 &&  stillLifting ){
			ProdMan->Barracks[0]->land(  toMove );
		}
	}


	if( CurrentStrategy == Macro_Strat && MacroMan->PState == P_Wall && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss){

		int TanksNeeded = 3;
		int TotalGateWays = 0;
		int FramesTillForceLift = 11000;
		for( int b=0; b<InfoMan->EnemyBuildings.size(); b++ ){
			if( InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Protoss_Gateway  ){
				TotalGateWays++;
			}
		}
		if( TotalGateWays >= 2 ){ //defend against 2 gate rush build
			TanksNeeded = 5;
			FramesTillForceLift = 13000;
		}
		if( TotalGateWays >= 3 ){ //defend against 2 gate rush build
			TanksNeeded = 7;
			FramesTillForceLift = 14000;
		}
		

		bool OneFact3RaxReady = false;
		if(  MacroMan->CurrentStrat == "OneFact3Rax"
			 &&  Broodwar->self()->supplyUsed() > 70 ) {
				 OneFact3RaxReady = true;
		}

		if( (MacroMan->Squads[0].Tanks.size() >= TanksNeeded
			&&   Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) >= 2
			)  || Broodwar->getFrameCount() > FramesTillForceLift
			||  OneFact3RaxReady == true
			){
			int totUnitClose = 0;
			/*
			BOOST_FOREACH( Unit close, Broodwar->enemy()->getUnits() ){
				if( close->getDistance( InfoMan->PosOurBase ) < 30*32){
					totUnitClose++;
				}
		    }
			*/
			BOOST_FOREACH( Unit close, CCmanager[0]->CommandCenter->getUnitsInRadius( 30*32 ) ){
				if( close->getPlayer() == Broodwar->enemy()
					&& !close->getType().isBuilding()
					&&  close->getDistance( InfoMan->PosOurBase ) < 30*32){ //double check
					totUnitClose++;
				}
		    }

			//no units close and enough to push out
			if( totUnitClose == 0){
		      StartLiftBarracks();
			  MacroMan->OnFramePushOut();
			}
		}
		//StartLiftBarracks();
	}

	if(   Broodwar->enemy()->getRace() == BWAPI::Races::Zerg 
		&& MacroMan->CurrentStrat == "2 Port Wraith" 
		&& ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center) > 0 ){
        
				//Broodwar->printf("Moving Rax");
		      StartLiftBarracks();
			  MacroMan->OnFramePushOut();


	}

}