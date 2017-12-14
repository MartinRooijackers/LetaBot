#include "Common.h"
#include "MarineRushManager.h"
#include "InformationManager.h"


MarineRushManager::MarineRushManager()
{

	StopMarineRush = false;

	AttackMarines.clear();
}


void MarineRushManager::OnFrame()
{
	/*
	if( StopWorkerRush == true ){
		return;
	}
	*/




	//Remove dead marines
	for(unsigned int i=0; i<AttackMarines.size(); i++){
		if( !AttackMarines[i]->exists() ){
			AttackMarines.erase( AttackMarines.begin() + i );
			i--;
			continue;
		}
	}


	//Send all marines to attack
	for(unsigned int i=0; i<AttackMarines.size(); i++){

		//move to the middle of the map if location is unknown yet
		if( InfoMan->PosEnemyBase == BWAPI::Positions::Unknown  ){
			BWAPI::Position MiddleMap( Broodwar->mapWidth()/2, Broodwar->mapHeight()/2);
		    AttackMarines[i]->attack(MiddleMap);
			continue;
		}







		
		int MaxDistAttack =  30*32;
		std::set<Unit*> EnemyClose = AttackMarines[i]->getUnitsInRadius( MaxDistAttack );
		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		int TotalLurker = 0;
		bool OverrunLurker = false;
		//BOOST_FOREACH( Unit* close, EnemyClose )
		BOOST_FOREACH( Unit* close, EnemyClose )
        {
			if( AttackMarines[i]->getDistance( close->getPosition() ) > MaxDistAttack){
				continue;
			}

			if(  close->getType().isBuilding() 
				&&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony
				&&  close->getType() != BWAPI::UnitTypes::Zerg_Creep_Colony		
				&&  close->getType() != BWAPI::UnitTypes::Terran_Bunker	
				){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Larva){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Egg){
				continue;
			}

			if(  close->getType().isWorker()
				&& InfoMan->PosEnemyBase != BWAPI::Positions::Unknown ){
					//dont chase drones outside the enemy natural or main
					if(  BWTA::getRegion( close->getPosition()  ) != BWTA::getRegion( InfoMan->PosEnemyBase )
						&&  BWTA::getRegion( close->getPosition()  ) != BWTA::getRegion( InfoMan->EnemyNatural )
						&& close->getDistance( InfoMan->PosEnemyBase ) > 50*32  ){
				        continue;
					}
			}

			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > AttackMarines[i]->getDistance( close->getPosition() )  ){
				closeDist = AttackMarines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}




	  // if we can't shoot, run away  if(kite)
		//    || (Squads[0].Marines[i]->getHitPoints() < 20 && closest != NULL && LanLatency == true )
		//   ||  ClosestBurrowedLurker < 8*32
		if (    ( closeDist < 4*32 &&  AttackMarines[i]->getGroundWeaponCooldown() != 0 )
			   )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x(), Marines[i]->getPosition().y(), 
			//fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);
		  if( AttackMarines[i]->getOrder() != BWAPI::Orders::Move){
		      AttackMarines[i]->move( fleePosition );
		  }
		  //Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		BWAPI::Position attackPos = InfoMan->PosEnemyBase;

		//if main depot destroyed, hunt remaining buildings
		if(  StopMarineRush == true ){
			if( InfoMan->EnemyBuildings.size() > 0 ){
				attackPos = InfoMan->EnemyBuildings[0].position;
			}
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  AttackMarines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( AttackMarines[i]->getPosition().getDistance( attackPos ) > 5*32 ){
                  if( closest != NULL ){

					    if( AttackMarines[i]->getOrderTarget() == NULL ){
                         AttackMarines[i]->attack(  closest );
						 continue;
					    }
					    else if( AttackMarines[i]->getOrderTarget()->getID() != closest->getID() ){
		                  AttackMarines[i]->attack(  closest );
							continue;
					    }



					  //prevent spam
					  if( AttackMarines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  AttackMarines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  AttackMarines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

							  AttackMarines[i]->attack(  closest->getPosition() );

					  }

				  } else {

							  AttackMarines[i]->move(  attackPos );


				  }
				  //Broodwar->printf("Attack");
				}			
			continue;
		}

	}


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
	if( (DepotAlive == false ||  Broodwar->getFrameCount() > 13000)
		&& StopMarineRush == false
		){
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
		StopMarineRush = true;
		Broodwar->printf("Enemy depot destoryed, switching to Normal strategy");
	}

}

void MarineRushManager::AddMarine( BWAPI::Unit* marine )
{

	if(  marine->getType() == BWAPI::UnitTypes::Terran_Marine ){
	  AttackMarines.push_back(marine);
	}
}

