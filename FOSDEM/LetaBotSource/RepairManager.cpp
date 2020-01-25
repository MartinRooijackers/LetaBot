
#include "Common.h"
#include "RepairManager.h"
#include <boost/foreach.hpp>


RepairManager::RepairManager(){


}



BWAPI::Unit RepairManager::getRepairSCV( BWAPI::TilePosition locRepair )
{

	//get nearest base
	int closesBaseDist = 9999;
	int closestID = 0;
	for( int i=0; i< CCmanager.size(); i++ ){

		if( CCmanager[i]->BaseReady == true 
			&&  CCmanager[i]->CommandCenter->getTilePosition().getDistance( locRepair ) < closesBaseDist
			&&  CCmanager[i]->MineralSCV.size() > 0
			){
			closesBaseDist =  CCmanager[i]->CommandCenter->getTilePosition().getDistance( locRepair );
			closestID = i;
		}

	}

	BWAPI::Unit ClosestRepairSCV = NULL;
	int SCVid = -1;
	int distClosest = 99999;
	for( int i=0; i< CCmanager[closestID]->MineralSCV.size(); i++ ){
		//get full hp SCV closest to what needs repairing
		if(  CCmanager[closestID]->MineralSCV[i].scv->getHitPoints() == 60 &&
			CCmanager[closestID]->MineralSCV[i].scv->getDistance( BWAPI::Position( locRepair ) ) < distClosest){
			distClosest =  CCmanager[closestID]->MineralSCV[i].scv->getDistance( BWAPI::Position( locRepair ) );
			SCVid = i;
			ClosestRepairSCV = CCmanager[closestID]->MineralSCV[i].scv;
		}
	}


	if( SCVid != -1 ){
		CCmanager[closestID]->MineralSCV.erase( CCmanager[closestID]->MineralSCV.begin() + SCVid);
		return ClosestRepairSCV;
	}


	return CCmanager[closestID]->getBuilder();

}


void RepairManager::returnRepairSCV( BWAPI::Unit scv )
{

	if( scv == NULL ){
		return;
	}

	//get nearest base
	int closesBaseDist = 9999;
	int closestID = -1;
	for( int i=0; i< CCmanager.size(); i++ ){

		if(  CCmanager[i]->CommandCenter->getPosition().getDistance( scv->getPosition() ) < closesBaseDist
			&& CCmanager[i]->BaseReady == true  ){
			closesBaseDist =  CCmanager[i]->CommandCenter->getPosition().getDistance( scv->getPosition() );
			closestID = i;
		}

	}

	

	if( closestID != -1 ){
		CCmanager[closestID]->addSCV( scv );
	}



}



void RepairManager::OnFrame(){

	if( MacroMan->AllowSDestroy == true){

	for(unsigned int i=0; i<RB.size(); i++){
		if( true ){ //good old    if(true)  trick
			for(unsigned int j=0; j<RB[i].RepairSCVS.size(); j++){
				CCmanager[0]->addSCV( RB[i].RepairSCVS[j] );
			}
			RB[i].RepairSCVS.empty();
			RB.erase( RB.begin() + i );
			Broodwar->printf("Repair ended");
			i--;
			continue;
		}
	}


		return;
	}

	BOOST_FOREACH( Unit myB, Broodwar->self()->getUnits() ){
		if( !myB->getType().isBuilding() || !myB->isCompleted() ){
			continue;
		}
		if( myB->getHitPoints() < myB->getType().maxHitPoints() ){
          bool exists = false;
	      BOOST_FOREACH(RBuilding rBuild, RB ){
			  if( rBuild.toRepair->getID() == myB->getID() ){
				  exists = true;
				  break;
			  }
	      }
		  //building needs repair
		  if( exists == false  
			  &&
			  (CCmanager[0]->MineralSCV.size() > 8
				  || CCmanager[0]->MineralSCV.size() == CCmanager[0]->SCVsaturation
				  )  ){
			  RBuilding newRB;
			  newRB.toRepair = myB;
			  //Unit getSCV = CCmanager[0]->getBuilder();
			  Unit getSCV = getRepairSCV( myB->getTilePosition() );
			  if(  getSCV != NULL ){
			  newRB.RepairSCVS.push_back( getSCV );
			  }
			  //getSCV = CCmanager[0]->getBuilder();
			  getSCV = getRepairSCV( myB->getTilePosition() );
			  if(  getSCV != NULL ){
			  newRB.RepairSCVS.push_back( getSCV );
			  }

			  RB.push_back( newRB );
			  Broodwar->printf("Repair started");
		  }
		}
	}

	for(unsigned int i=0; i<RB.size(); i++){
		if( RB[i].toRepair->getHitPoints() == RB[i].toRepair->getType().maxHitPoints() ){
			for(unsigned int j=0; j<RB[i].RepairSCVS.size(); j++){
				//CCmanager[0]->addSCV( RB[i].RepairSCVS[j] );
				returnRepairSCV( RB[i].RepairSCVS[j] );
			}
			RB[i].RepairSCVS.empty();
			RB.erase( RB.begin() + i );
			Broodwar->printf("Repair ended");
			i--;
			continue;
		}
	}

	//make sure that the repair SCVs are actually repairing
	for(unsigned int i=0; i<RB.size(); i++){
		if( RB[i].toRepair->getHitPoints() != RB[i].toRepair->getType().maxHitPoints() ){
			for(unsigned int j=0; j<RB[i].RepairSCVS.size(); j++){
				if( RB[i].RepairSCVS[j]->getOrder() != BWAPI::Orders::Repair ){
				RB[i].RepairSCVS[j]->repair(  RB[i].toRepair );
				}
				if( !RB[i].RepairSCVS[j]->exists() ){
					RB[i].RepairSCVS.erase( RB[i].RepairSCVS.begin() + j );
					j--;
					continue;
				}

				//remove SCVs that are low on HP
				if( RB[i].RepairSCVS[j]->getHitPoints() < 19 ){
					//CCmanager[0]->addSCV( RB[i].RepairSCVS[j] );
					returnRepairSCV( RB[i].RepairSCVS[j] );
					RB[i].RepairSCVS.erase( RB[i].RepairSCVS.begin() + j );
					j--;
					continue;
				}

			}
		}
	}

	for(unsigned int i=0; i<RB.size(); i++){
		if( RB[i].toRepair->getHitPoints() != RB[i].toRepair->getType().maxHitPoints() ){

			if(  Broodwar->enemy()->getRace() == BWAPI::Races::Protoss 
				&& MacroMan->PState == P_Wall
				 &&  RB[i].RepairSCVS.size() < 3){
					 int TotalRepairNeeded = 2;
					 if( RB[i].toRepair->getHitPoints() <  (int)( RB[i].toRepair->getType().maxHitPoints()/2 ) ){
						 TotalRepairNeeded = 4;
					 }
					 if( RB[i].RepairSCVS.size() < TotalRepairNeeded ){

				       //Unit extraSCV = CCmanager[0]->getFullHPSCV();
					   Unit extraSCV = getRepairSCV( RB[i].toRepair->getTilePosition() );
				       if( extraSCV != NULL ){
				          RB[i].RepairSCVS.push_back( extraSCV );
				       }
				     }
			}

			if(  RB[i].RepairSCVS.size() < 2 ){
				//Unit extraSCV = CCmanager[0]->getBuilder();
				Unit extraSCV = getRepairSCV( RB[i].toRepair->getTilePosition() );
				if( extraSCV != NULL ){
				  RB[i].RepairSCVS.push_back( extraSCV );
				}
			}

		}

		if( RB[i].toRepair->getType() == BWAPI::UnitTypes::Terran_Bunker
			 &&   RB[i].toRepair->getHitPoints()*1.5 < RB[i].toRepair->getType().maxHitPoints() ){

			if(  RB[i].RepairSCVS.size() < 4 ){
				//Unit extraSCV = CCmanager[0]->getBuilder();
				Unit extraSCV = getRepairSCV( RB[i].toRepair->getTilePosition() );
				if( extraSCV != NULL ){
				  RB[i].RepairSCVS.push_back( extraSCV );
				}
			}

		}

	}



}

void RepairManager::AddSCV( BWAPI::Unit scv){

}