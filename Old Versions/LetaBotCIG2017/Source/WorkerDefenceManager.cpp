#include "Common.h"
#include "WorkerDefenceManager.h"


WorkerDefenceManager::WorkerDefenceManager()
{
	DefenceSCVS.clear();

	//get retreat position
	if( CCmanager.size() > 0 && CCmanager[0]->CommandCenter != NULL){
	  ToRetreat = CCmanager[0]->CommandCenter->getPosition();
	}
	int furthest = 0;
	for(int i=0; i<CCmanager[0]->Minerals.size(); i++){
		if( CCmanager[0]->Minerals[i].mineralPatch->getDistance( InfoMan->PosMainChoke ) > furthest){
		  furthest = CCmanager[0]->Minerals[i].mineralPatch->getDistance( InfoMan->PosMainChoke );
		  ToRetreat = CCmanager[0]->Minerals[i].mineralPatch->getPosition();
		  ToRetreatM = CCmanager[0]->Minerals[i].mineralPatch;
		}
	}

}


void WorkerDefenceManager::OnFrame()
{

	if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }

	BWAPI::Position CCpos = BWAPI::Position( InfoMan->OurBase );
	unsigned int getAWorker = 0;//Get the total amount of workers attacking us
	Unit* toAttack = NULL;
	BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
		//if not wall in
		if( enemy->getDistance( CCpos ) < 40*32 
			//&& !enemy->getType().isFlyer()
			&& enemy->getType().isWorker()
			&& BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion){
			getAWorker++;
			toAttack = enemy;
		}
		if( enemy->getDistance( CCpos ) < 14*32 
			&& !enemy->getType().isFlyer()
			&& !enemy->getType().isWorker()
			&&  enemy->getType() == BWAPI::UnitTypes::Zerg_Zergling
			&& BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion){
			getAWorker += 2;
			//getAWorker++;
			//if( MacroMan->ZState == Z_Pool_Defend){
 			  //getAWorker++;//get another worker
			//}
			toAttack = enemy;
		}

		//pull 3 workers for zealot
		if( enemy->getDistance( CCpos ) < 100*32 
			&&  enemy->getType() == BWAPI::UnitTypes::Protoss_Zealot
			&& bManager->WallSound == false
			&& BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion){
			getAWorker += 3;
			//getAWorker++;
			//if( MacroMan->ZState == Z_Pool_Defend){
 			  //getAWorker++;//get another worker
			//}
			toAttack = enemy;
		}


		//pull 1 workers for reaver
		if( enemy->getDistance( CCpos ) < 100*32 
			&&  enemy->getType() == BWAPI::UnitTypes::Protoss_Reaver
			&& bManager->WallSound == false
			&& BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion){
			getAWorker += 1;
			//getAWorker++;
			//if( MacroMan->ZState == Z_Pool_Defend){
 			  //getAWorker++;//get another worker
			//}
			toAttack = enemy;
		}



		//pull 3 workers for zealot non wall
		if( enemy->getDistance( CCpos ) < 15*32 
			&& !enemy->getType().isFlyer()
			&& !enemy->getType().isWorker()
			&&  enemy->getType() == BWAPI::UnitTypes::Protoss_Zealot
			&& enemy->getDistance( InfoMan->PosMainChoke ) > 5*32 
			&& bManager->WallSound == true){
			getAWorker += 3;
			//getAWorker++;
			//if( MacroMan->ZState == Z_Pool_Defend){
 			  //getAWorker++;//get another worker
			//}
			toAttack = enemy;
		}


		//pull 2 workers for marine
		//  && BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion
		if( (enemy->getDistance( CCpos ) < 80*32)
			&& !enemy->getType().isFlyer()
			&& !enemy->getType().isWorker()
			&&  enemy->getType() == BWAPI::UnitTypes::Terran_Marine){
				if( MacroMan->Squads[0].Bunkers.size() == 0 ){
					getAWorker += 2;
				} else if( BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion ){
			       getAWorker += 2;
				}
			//getAWorker++;
			//if( MacroMan->ZState == Z_Pool_Defend){
 			  //getAWorker++;//get another worker
			//}
			toAttack = enemy;
		}


		//pull 5 workers for building
		if( enemy->getType().isBuilding()
			&& enemy->getDistance( CCpos ) < 100*32
			&& 
			(BWTA::getRegion( enemy->getPosition() ) == InfoMan->OurMainregion 
			|| BWTA::getRegion( enemy->getPosition() ) == InfoMan->NatRegion  )
                            ){
			getAWorker += 5;
			//getAWorker++;
			//if( MacroMan->ZState == Z_Pool_Defend){
 			  //getAWorker++;//get another worker
			//}
			toAttack = enemy;
		}


	}


	if( getAWorker > DefenceSCVS.size() && CCmanager[0]->MineralSCV.size() > 2  ){
		//Broodwar->printf("Adding scv to defence");
		Unit* Dscvs = CCmanager[0]->getFullHPSCV();
		if( Dscvs != NULL ){
			/*
		  if(  toAttack != NULL ){
			  Dscvs->attack( toAttack->getPosition() );
		  }
		  */
		  AddSCV( Dscvs );
		}
	}

	if( getAWorker < DefenceSCVS.size() ){//once the defence is done, return scv to base
		while( getAWorker < DefenceSCVS.size() ){
			CCmanager[0]->addSCV( DefenceSCVS.back() );
			DefenceSCVS.pop_back();
			//Broodwar->printf("Returning scv to mining");
		}
	}

	for(unsigned int i=0; i<DefenceSCVS.size(); i++){
		if( !DefenceSCVS[i]->exists() ){ //dead scv
			DefenceSCVS.erase( DefenceSCVS.begin() + i );
			i--;
			continue;
		}
		
		if( DefenceSCVS[i]->getHitPoints() < 26 && LanLatency == true ){
			RepairSCVS.push_back( DefenceSCVS[i] );
			DefenceSCVS.erase( DefenceSCVS.begin() + i );
			i--;
			continue;
		}
		
		if( toAttack != NULL ){
			/*
			//prevent spam
			if( DefenceSCVS[i]->getOrder() != BWAPI::Orders::AttackUnit
				&& DefenceSCVS[i]->getOrder() != BWAPI::Orders::AttackTile
				&& DefenceSCVS[i]->getOrder() != BWAPI::Orders::AttackMove ){
				DefenceSCVS[i]->attack(toAttack->getPosition() );
			}
			*/
			

		  std::set<Unit*> EnemyClose = DefenceSCVS[i]->getUnitsInRadius( 30*32 );
		  Unit* closest = NULL;
		  int closeDist = 9999999;
	  	  BOOST_FOREACH( Unit* close, EnemyClose )
          {
			if(  close->getPlayer() != Broodwar->enemy() ){
				continue;
			}
			if(  close->getType().isBuilding() 
				&& 
				!(
				close->getType() == BWAPI::UnitTypes::Protoss_Pylon
				||  close->getType() == BWAPI::UnitTypes::Protoss_Assimilator
				)

				){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > DefenceSCVS[i]->getDistance( close->getPosition() )  ){
				closeDist = DefenceSCVS[i]->getDistance( close->getPosition() );
				closest = close;
			}
		  }

		  if(  closest != NULL ){

			if( DefenceSCVS[i]->getTarget() != NULL){
				//prevent spam attacking
				if( DefenceSCVS[i]->getTarget()->getID() != closest->getID()    ){
                  DefenceSCVS[i]->attack(closest);
				}
			} else {
		     DefenceSCVS[i]->attack( closest );
			}
		  }

			//if( DefenceSCVS[i]->getTarget() != NULL){
				//prevent spam attacking
				//if( DefenceSCVS[i]->getTarget()->getID() != toAttack->getID()    ){
                  //DefenceSCVS[i]->attack(toAttack);
				//}
			//} else {
		    // DefenceSCVS[i]->attack(toAttack->getPosition() );
			//}
			
		}
	}

	for(unsigned int i=0; i<RepairSCVS.size(); i++){
		if( RepairSCVS[0]->getHitPoints() >= 59 || getAWorker == 0
			 || getAWorker < RepairSCVS.size()  ){
			//DefenceSCVS.push_back( RepairSCVS[i] );
			CCmanager[0]->addSCV( RepairSCVS[i] );
			RepairSCVS.erase( RepairSCVS.begin() + i );
			i--;
			continue;
		}

	    if( RepairSCVS[i]->getOrder() == BWAPI::Orders::Repair  ){
         continue;
	    }
		if( CCmanager[0]->Minerals.size() > 0  ){
			if(   CCmanager[0]->Minerals[0].mineralPatch->getDistance( RepairSCVS[i] ) > 2*32  ){
				if( ToRetreatM != NULL){
					if( !RepairSCVS[i]->isGatheringMinerals() ){
				       RepairSCVS[i]->gather( ToRetreatM );
					}
				}
				//RepairSCVS[i]->move( ToRetreat );
				continue;
			} else {
				//assign repair
			  for(unsigned int j=0; j<RepairSCVS.size(); j++){
				  if( i != j &&  RepairSCVS[i]->getOrder() != BWAPI::Orders::Repair  ){ //&&  RepairSCVS[j]->getOrder() != BWAPI::Orders::Repair
					  RepairSCVS[i]->repair( RepairSCVS[j] );
				      break;
				  }
			  }
			}

		}
	}

}

void WorkerDefenceManager::AddSCV( BWAPI::Unit* scv )
{
	DefenceSCVS.push_back(scv);
}