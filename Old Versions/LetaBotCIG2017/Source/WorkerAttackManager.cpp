#include "Common.h"
#include "WorkerAttackManager.h"
#include "InformationManager.h"


WorkerAttackManager::WorkerAttackManager()
{

	StopWorkerRush = false;

	for(unsigned int i=0; i<ScoutMan->workerScouts.size(); i++ ){
		AddSCV( ScoutMan->workerScouts[i].scv );
	}
	ScoutMan->workerScouts.clear();//No more need for scouting
	InfoMan->GetNaturalEnemy();
	//Broodwar->printf(" Mineral workers %d ", CCmanager[0]->MineralSCV.size());
	//Broodwar->printf(" Attacking worekrs %d ",  AttackSCVS.size() );
    for(unsigned int j=0; j<AttackSCVS.size(); j++){
      for( unsigned int i=0; i<CCmanager[0]->MineralSCV.size(); i++ ){
		  if( CCmanager[0]->MineralSCV[i].scv->getID() == AttackSCVS[j].scv->getID() ){
			  Broodwar->printf("Error: SCV assigned to multiple managers");
		  }
	  }
	}
	

	EnemyPos = BWAPI::Position( InfoMan->EnemyBase );
	EnemyNat = BWAPI::Position( InfoMan->EnemyNatural );

	BWTA::Region* natRegion = BWTA::getRegion( EnemyNat );
	bool NatHasGeyser = false; //check for maps like andromeda
	//natRegion->getb
	BOOST_FOREACH( BWTA::BaseLocation* base , natRegion->getBaseLocations() ){
		if( base->getGeysers().size() > 0 ){
			NatHasGeyser = true;
		}

	}

	if( NatHasGeyser == false ){
		BWAPI::Position enemyChoke;
		BOOST_FOREACH( BWTA::Chokepoint* choke , natRegion->getChokepoints() ){
			if( (
				choke->getRegions().first == natRegion 
				&& choke->getRegions().second == BWTA::getRegion( InfoMan->PosEnemyBase )
				) ||
				(
				choke->getRegions().second == natRegion 
				&& choke->getRegions().first == BWTA::getRegion( InfoMan->PosEnemyBase )
				)
				){
					enemyChoke = choke->getCenter();
			}
		}

		EnemyNat = BWAPI::Position(  (EnemyNat.x() + enemyChoke.x() )/2 , (EnemyNat.y() + enemyChoke.y()  )/ 2  );

	}

}


void WorkerAttackManager::OnFrame()
{

	if( StopWorkerRush == true ){
		return;
	}

	//Broodwar->drawBox(CoordinateType::Map,InfoMan->EnemyNatural.x()*32,InfoMan->EnemyNatural.y()*32,InfoMan->EnemyNatural.x()*32+3*32,InfoMan->EnemyNatural.y()*32+3*32,Colors::Orange,false);

	//TODO:  change back to  >  3

	while( CCmanager[0]->MineralSCV.size() > 3 ){ //grab all but 2 scvs to attack
		AddSCV(   CCmanager[0]->getBuilder() );
	}

	//BWAPI::Position EnemyPos = BWAPI::Position( InfoMan->EnemyBase );
	//BWAPI::Position EnemyNat = BWAPI::Position( InfoMan->EnemyNatural );

	//BWTA::Region*


	//Remove dead scvs
	for(unsigned int i=0; i<AttackSCVS.size(); i++){
		if( !AttackSCVS[i].scv->exists() ){
			AttackSCVS.erase( AttackSCVS.begin() + i );
			i--;
			continue;
		}
	}


	//Send all scvs to attack

	for(unsigned int i=0; i<AttackSCVS.size(); i++){
		if(  AttackSCVS[i].state == Moving_Base && AttackSCVS[i].scv->getDistance( EnemyPos ) >= 32*50  ){
			AttackSCVS[i].scv->move( EnemyPos );
		}
		//&&  BWTA::getRegion( AttackSCVS[i].scv->getPosition() ) ==  BWTA::getRegion(  InfoMan->PosEnemyBase )
		//&& AttackSCVS[i].scv->getDistance( EnemyPos ) < 32*50 
		if(  AttackSCVS[i].state == Moving_Base &&  AttackSCVS[i].scv->getDistance( EnemyPos ) < 32*50   ){
			AttackSCVS[i].state = Attacking_Base;
		}
		if( AttackSCVS[i].state  == Attacking_Base && AttackSCVS[i].scv->getHitPoints() < 23){
			AttackSCVS[i].state = Drill_Flee;
			if( BWTA::getRegion(InfoMan->PosEnemyBase) ==  BWTA::getRegion(AttackSCVS[i].scv->getPosition() ) ){
			  if( CCmanager[0]->Minerals.size() > 0 ){
			      AttackSCVS[i].scv->gather( CCmanager[0]->Minerals[0].mineralPatch ); //CCmanager[0]->Minerals[0]
			    } else {
                  AttackSCVS[i].scv->move( EnemyNat );
			    }
			}
			//CCmanager[0]->Minerals[0];
		}

		//probably chasing a scouting worker
		if( AttackSCVS[i].state  == Attacking_Base 
			&& BWTA::getRegion(  AttackSCVS[i].scv->getPosition() ) !=  BWTA::getRegion( InfoMan->PosEnemyBase )
			&& BWTA::getRegion(  AttackSCVS[i].scv->getPosition() ) !=  BWTA::getRegion( InfoMan->EnemyNatural )			
			){
			AttackSCVS[i].state = Fleeing_Nat;
			AttackSCVS[i].scv->move( EnemyNat );           
		}

		//if( AttackSCVS[i].state  == Drill_Flee &&  AttackSCVS[i].scv->getDistance( InfoMan->PosEnemyBase ) > 300){
		if( AttackSCVS[i].state  == Drill_Flee && BWTA::getRegion(InfoMan->PosEnemyBase) !=  BWTA::getRegion(AttackSCVS[i].scv->getPosition() ) ){
			AttackSCVS[i].state = Fleeing_Nat;
			AttackSCVS[i].scv->move( EnemyNat );
		}
		//arrived at fleeing position
		if(  AttackSCVS[i].state == Fleeing_Nat && AttackSCVS[i].scv->getDistance(EnemyNat) < 130 ){
			AttackSCVS[i].state = Repairing;
			AttackSCVS[i].LastUnstuckFrame = Broodwar->getFrameCount();
		}
		//Fully healed
		if(  AttackSCVS[i].state == Repairing && AttackSCVS[i].scv->getHitPoints() == 60 ){
			AttackSCVS[i].state = Attacking_Base;
		}

		//defend yourself versus SCVs that are trying to take you down in the natural
		if(  AttackSCVS[i].state == Repairing ){
		  BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
			if( enemy->getType().isWorker()
				&&  enemy->getDistance( AttackSCVS[i].scv ) < 2*32
				&&  BWTA::getRegion(  enemy->getPosition() ) == BWTA::getRegion( InfoMan->EnemyNatural ) ){
					AttackSCVS[i].state = Defend_Repair;
			}
		  }
		}


		if(  AttackSCVS[i].state == Defend_Repair  ){
			bool EnemiesLeft = false;
			// &&  BWTA::getRegion(  enemy->getPosition() ) == BWTA::getRegion( InfoMan->EnemyNatural )
		  BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
			if( enemy->getType().isWorker()
				&&  enemy->getDistance( AttackSCVS[i].scv ) < 3*32 ){
					//AttackSCVS[i].scv->attack( enemy->getPosition() );
					AttackSCVS[i].scv->move( InfoMan->PosOurNat );
					EnemiesLeft = true;
					break;
			}
		  }
		  if( EnemiesLeft == false ){
			   AttackSCVS[i].state = Fleeing_Nat;
			   AttackSCVS[i].scv->move( EnemyNat );
		  }
		}


		/*
					if( AttackSCVS[i].state == Attacking_Base && !(   AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackUnit ||  AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackTile
			||  AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackMove ||  AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AtkMoveEP) ){
			AttackSCVS[i].scv->attack( EnemyPos );
		} 
		*/
	}

	 //Manage attack scvs

	  //first check if any of the scvs are under attack
	  bool Attacked = false;
	  Unit* workerAttacking = NULL;
	  for(unsigned int i=0; i<AttackSCVS.size(); i++){
			BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
				//get the nearest worker unit inside the enemy base
				//Getregion not good enough?
				if( enemy->getType().isWorker() && enemy->getTarget() != NULL ){
					if( enemy->getTarget()->getID() == AttackSCVS[i].scv->getID() ){
						Attacked = true;
						workerAttacking = enemy;
					}
				}	
				if( enemy->getType().isWorker() && enemy->getOrderTarget() != NULL ){
					if( enemy->getOrderTarget()->getID() == AttackSCVS[i].scv->getID() ){
						Attacked = true;
						workerAttacking = enemy;
					}
				}	

			 }
	  }

	  //decide what to attack
	  for(unsigned int i=0; i<AttackSCVS.size(); i++){
		   if( AttackSCVS[i].state != Attacking_Base){
			   continue;
		   }

		   //if under attack, attack the worker unit attacking you
		   if(  Attacked == true ){
			  if( workerAttacking != NULL ){
				//check if the worker unit is at the natural
				if( BWTA::getRegion(workerAttacking->getPosition() ) ==  BWTA::getRegion( EnemyNat ) ){
			      AttackSCVS[i].scv->attack( workerAttacking  );
				  continue;
			    }
			  }
		   }

		   bool notAttackingWorker = false;
		   if( AttackSCVS[i].scv->getTarget() != NULL){
			   if( !AttackSCVS[i].scv->getTarget()->getType().isWorker() ){
				   notAttackingWorker = true;
			   }
		   }
		   if( AttackSCVS[i].scv->getTarget() == NULL){
			   notAttackingWorker = true;
		   }
		   //extra check to prevent oscilation
		   if( AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackUnit ){
			   notAttackingWorker = false;
		   }
		   bool alreadyAssigned = false;
		   //SCVs should attack the closest worker when inside of the base
			int closestA = 99999;
			BWAPI::Unit* toAttack = NULL;
			BWAPI::Position enemybase = BWAPI::Position(InfoMan->EnemyBase);
			BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
				//get the nearest worker unit inside the enemy base
				//Getregion not good enough?
				if( enemy->getType().isWorker() &&  enemy->getDistance(enemybase) < 800 
					&&  enemy->getDistance( AttackSCVS[i].scv->getPosition() ) < closestA ){
					closestA = enemy->getDistance( AttackSCVS[i].scv->getPosition() ); 
					toAttack = enemy;
				}
			        
			 }

			if(  toAttack != NULL && AttackSCVS[i].scv->getTarget() == NULL   ){
				        AttackSCVS[i].scv->attack(toAttack);
					    alreadyAssigned = true;
			}

			if(  toAttack != NULL && AttackSCVS[i].scv->getTarget() != NULL   ){
				if( AttackSCVS[i].scv->getTarget() != NULL ){
					if( !AttackSCVS[i].scv->getTarget()->getType().isWorker() 
						|| AttackSCVS[i].scv->getTarget()->getID() != toAttack->getID() ){
					   //Broodwar->printf( "attacking worker");
				        AttackSCVS[i].scv->attack(toAttack);
					    alreadyAssigned = true;
					   }
					 }
					 else {
					   //Broodwar->printf( "attacking worker");
				        AttackSCVS[i].scv->attack(toAttack);
					    alreadyAssigned = true;
					 }
					 //make sure that the scvs don't attack a building
					 if( AttackSCVS[i].scv->getOrderTarget() != NULL ){
					   if( !AttackSCVS[i].scv->getOrderTarget()->getType().isWorker() ){
					   //Broodwar->printf( "attacking worker");
				        AttackSCVS[i].scv->attack(toAttack);
					    alreadyAssigned = true;		
					   }
					 }
				 }
			   

		   if( !alreadyAssigned &&  !(   AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackUnit ||  AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackTile
			||  AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AttackMove ||  AttackSCVS[i].scv->getOrder() == BWAPI::Orders::AtkMoveEP) ){
			AttackSCVS[i].scv->attack( EnemyPos );
		  } 
	}

	//assign scvs to repair each other
    for(unsigned int i=0; i<AttackSCVS.size(); i++){
		if( AttackSCVS[i].state != Repairing  ){
		  continue;
	  }
	  if( AttackSCVS[i].LastUnstuckFrame <  Broodwar->getFrameCount() - 120 ){
		  AttackSCVS[i].LastUnstuckFrame = Broodwar->getFrameCount();
		  AttackSCVS[i].scv->stop();
	  }
	 if( AttackSCVS[i].scv->getOrder() == BWAPI::Orders::Repair  ){
       continue;
	 }

	  for(unsigned int j=0; j<AttackSCVS.size(); j++){
        if( i == j ||  AttackSCVS[j].state != Repairing ){
		  continue;
	    }
		else {
			AttackSCVS[i].scv->repair( AttackSCVS[j].scv);
			break;
		}
	  }
	}

	/*
	unsigned int WorkersAttacking = 0;
	BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
		if( enemy->getDistance( EnemyPos ) ){
		  if(  enemy->getOrder() == BWAPI::Orders::AttackUnit || enemy->getOrder() == BWAPI::Orders::AttackTile
			|| enemy->getOrder() == BWAPI::Orders::AttackMove || enemy->getOrder() == BWAPI::Orders::AtkMoveEP){
			WorkersAttacking++;
		  }
		}
	}
	*/


	/*
	if( WorkersAttacking > 0 ){
		Broodwar->printf("Under attack");
	}
	*/


	bool DepotAlive = true;
	if( Broodwar->isVisible(InfoMan->EnemyBase) ){
	  DepotAlive = false; //check if the depot is still in place
	  BOOST_FOREACH( Unit* enemy,  Broodwar->getUnitsOnTile( InfoMan->EnemyBase.x(),InfoMan->EnemyBase.y() ) ){
		  if( enemy->getType().isResourceDepot() ){
			  DepotAlive = true;
		  }
	  }
	}

	//rush succesful, or enemy lifted of command center, or enough time has passed
	if( DepotAlive == false ||  Broodwar->getFrameCount() > 13000 ){
		//CurrentStrategy = Normal_Wraith;//switch to wraits to finish enemy off
		ProdMan->BuildingSupplyOrder.clear();
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot,10);
		//ProdMan->addToQueue( BWAPI::UnitTypes::Terran_Supply_Depot);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot,10);
		ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot,10);
		StopWorkerRush = true;
		Broodwar->printf("Enemy depot destoryed, switching to Normal strategy");
	}

}

void WorkerAttackManager::AddSCV( BWAPI::Unit* scv )
{
	SCvAttack newSCV;
	newSCV.scv = scv;
	newSCV.state = Moving_Base;
	AttackSCVS.push_back(newSCV);
}



/*

	//Broodwar->drawBox(CoordinateType::Map,InfoMan->EnemyNatural.x()*32,InfoMan->EnemyNatural.y()*32,InfoMan->EnemyNatural.x()*32+3*32,InfoMan->EnemyNatural.y()*32+3*32,Colors::Orange,false);


	//Remove dead scvs
	for(unsigned int i=0; i<AttackSCVS.size(); i++){
		if( !AttackSCVS[i]->exists() ){
			AttackSCVS.erase( AttackSCVS.begin() + i );
			i--;
			continue;
		}
	}

	while( CCmanager[0]->MineralSCV.size() > 3 ){ //grab all but 2 scvs to attack
		AddSCV(   CCmanager[0]->getBuilder() );
	}

	BWAPI::Position EnemyPos = BWAPI::Position( InfoMan->EnemyBase );
	BWAPI::Position EnemyNat = BWAPI::Position( InfoMan->EnemyNatural );

	//Send all scvs to attack
	std::vector<Unit*> SCVinBase;
	SCVinBase.clear();
	for(unsigned int i=0; i<AttackSCVS.size(); i++){
		if( AttackSCVS[i]->getDistance( EnemyPos ) > 32*50 ){
			AttackSCVS[i]->attack( EnemyPos );
		} else {
			SCVinBase.push_back( AttackSCVS[i] );
		}
	}

	unsigned int WorkersAttacking = 0;
	BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
		if( enemy->getDistance( EnemyPos ) ){
		  if(  enemy->getOrder() == BWAPI::Orders::AttackUnit || enemy->getOrder() == BWAPI::Orders::AttackTile
			|| enemy->getOrder() == BWAPI::Orders::AttackMove || enemy->getOrder() == BWAPI::Orders::AtkMoveEP){
			WorkersAttacking++;
		  }
		}
	}

	/*
	if( WorkersAttacking > 0 ){
		Broodwar->printf("Under attack");
	}
	*/

/*
	if( SCVinBase.size() >= WorkersAttacking ){
		BOOST_FOREACH(BWAPI::Unit*  scv,  SCVinBase ){
			bool retreating = false;
			if(  scv->getHitPoints() < 20 ){
				//BWAPI::Broodwar->drawLineMap(scv->getPosition().x(), scv->getPosition().y(),EnemyNat.x(), EnemyNat.y(), BWAPI::Colors::Cyan);
				//Broodwar->printf("Retreat to natural");
			    retreating = true;
				scv->move( EnemyNat );
				continue;
			}

		  if(  scv->getOrder() == BWAPI::Orders::AttackUnit || scv->getOrder() == BWAPI::Orders::AttackTile
			|| scv->getOrder() == BWAPI::Orders::AttackMove || scv->getOrder() == BWAPI::Orders::AtkMoveEP){
			//Nothing
		  } else {
			  scv->attack( EnemyPos );
		  }
		}
	}

	*/