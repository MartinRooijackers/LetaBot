#include "WraithManager.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <boost/foreach.hpp>

#include "Common.h"

WraithManager::WraithManager(){
	Wraiths.clear();
	Wraiths.reserve(200);

	AttackTarget = BWAPI::Positions::None;
	AttackBuilding = NULL;
	RepairSCV = NULL;

}

void WraithManager::AddWraith(Unit unit){
	if(  unit->getType()  == BWAPI::UnitTypes::Terran_Wraith ){
		  WraithAgent newWraith;
		  newWraith.currentState = W_Attack_Base;
		  newWraith.Wraith = unit;
		  newWraith.stopped_frames = 0;
		  Wraiths.push_back( newWraith );
	}
}


void WraithManager::NextBuilding(){

	//dont call if not needed
	if( AttackBuilding != NULL &&
		AttackTarget != BWAPI::Positions::None ){
	    return;
	}

	  if( InfoMan->EnemyBuildings.size() > 0 ){
	    AttackBuilding = InfoMan->EnemyBuildings[0].building;
		AttackTarget = InfoMan->EnemyBuildings[0].position;
		Broodwar->printf("Wraith: Next building to attack");
	  } else {
		  AttackTarget = BWAPI::Positions::None;
		  AttackTarget = BWAPI::Position( rand() % Broodwar->mapWidth() ,  rand() % Broodwar->mapHeight() );
	  }

}

void WraithManager::onBuildingDestroyed(Unit unit)
{
	if( unit == NULL ){
		return;
	}
		if( AttackBuilding == NULL ){
			NextBuilding();
		  return;
	    }

	  if(  unit->getID() == AttackBuilding->getID()  ){
		  //Broodwar->printf("Wriath building destroyed, attacking new building");
		  AttackBuilding = NULL;
	     NextBuilding();
	  }
	

}


//Wraiths should be constantly on the move
void WraithManager::onFrame(){

	if(  AttackTarget == BWAPI::Positions::None ){
		/*
		if( InfoMan->EnemyBuildings.size() > 0 ){
			AttackPos = InfoMan->EnemyBuildings[ rand()%InfoMan->EnemyBuildings.size() ].position;
        } else { //if no building knowledge is available, scout at random
			AttackPos = BWAPI::Position( rand() % Broodwar->mapWidth() ,  rand() % Broodwar->mapHeight() );
		}
		*/
		NextBuilding();
	}

	//repairs are needed

	if( RepairSCV != NULL){
	
		 bool haveToRepair = false;
		  Unitset WraithClose = RepairSCV->getUnitsInRadius( 20*32 );
		  Unit closest = NULL;
		  Unit EnemyBuild = NULL;
		  int closeDist = 9999999;
		  bool ToClose = false;
		  bool DetectClose = false;//check if overlord is close by
		  BOOST_FOREACH( Unit close, WraithClose )
          {

			if( close->getType() == BWAPI::UnitTypes::Terran_Wraith
				&& close->getHitPoints() < 120 ){
					haveToRepair = true;
				if( RepairSCV->getOrder() != BWAPI::Orders::Repair){
					RepairSCV->repair( close);
				}
			}
		  }

		  if( !RepairSCV->exists() ){
			  RepairSCV = NULL;
		  }
		  if( haveToRepair == false && RepairSCV != NULL ){
			  CCmanager[0]->addSCV( RepairSCV );
			  RepairSCV = NULL;
		  }
	}

	for(int i=0; i<Wraiths.size(); i++){

		//random scouting
		if(   Wraiths[i].Wraith->getDistance(AttackTarget) < 2*32
			&& AttackBuilding == NULL){
				NextBuilding();
		}

		if( Wraiths[i].currentState == W_Repair && Wraiths[i].Wraith->getHitPoints() > 119 ){
			Wraiths[i].currentState = W_Attack_Base;
		}

		if( Wraiths[i].Wraith->getHitPoints() < 51 ){
			Wraiths[i].currentState = W_Repair;
			if( RepairSCV == NULL){
				BWAPI::Unit Rscv = CCmanager[0]->getBuilder();
				if( Rscv != NULL){
					RepairSCV  = Rscv;
				}
			}
			if( Wraiths[i].Wraith->getDistance( InfoMan->PosOurBase) > 10*32 ){
				Wraiths[i].Wraith->move( InfoMan->PosOurBase);
			} else {
				//Wraiths[i].Wraith->stop();
				if( RepairSCV != NULL ){
					Wraiths[i].Wraith->move( RepairSCV->getPosition()  );
				}
			}
			continue;
		}

		if( Wraiths[i].currentState == W_Repair && Wraiths[i].Wraith->getHitPoints() < 119 ){
			continue;
		}


		if( Wraiths[i].currentState == W_Attack_Base ){
			//Wraiths[i].Wraith->attack( InfoMan->PosEnemyBase );

			//decide who to engage
		
			if(  Wraiths[i].Wraith->getEnergy() > 60 && !Wraiths[i].Wraith->isCloaked()  ){
			Wraiths[i].Wraith->useTech( BWAPI::TechTypes::Cloaking_Field);
		  }


		  Unitset EnemyClose = Wraiths[i].Wraith->getUnitsInRadius( 15*32 );
		  Unit closest = NULL;
		  Unit EnemyBuild = NULL;
		  int closeDist = 9999999;
		  bool ToClose = false;
		  bool DetectClose = false;//check if overlord is close by
		  BOOST_FOREACH( Unit close, EnemyClose )
          {

			if( close->getPlayer() != Broodwar->enemy()){
				continue;
			}
			if(  close->getType().isBuilding()  ){
				EnemyBuild = close;
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Larva 
				|| close->getType() == BWAPI::UnitTypes::Zerg_Cocoon 
				|| close->getType() == BWAPI::UnitTypes::Zerg_Egg 
				|| close->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony ){
				continue;
			}
			//check if we are getting dangerusly close to a spore
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony
				&&  close->getDistance( Wraiths[i].Wraith->getPosition() ) < 8*32 ){
				ToClose = true;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony
				&&  close->getDistance( Wraiths[i].Wraith->getPosition() ) < 10*32 ){
				DetectClose = true;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Overlord
				&&  close->getDistance( Wraiths[i].Wraith->getPosition() ) < 11*32 ){
				DetectClose = true;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Hydralisk
				&&  close->getDistance( Wraiths[i].Wraith->getPosition() ) < 5*31 ){
				//ToClose = true;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk
				&&  close->getDistance( Wraiths[i].Wraith->getPosition() ) < 5*31 ){
				ToClose = true;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Scourge
				&&  close->getDistance( Wraiths[i].Wraith->getPosition() ) < 2*32 ){
				ToClose = true;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Wraiths[i].Wraith->getDistance( close->getPosition() )  ){
				closeDist = Wraiths[i].Wraith->getDistance( close->getPosition() );
				closest = close;
			}
		  }


		  for( int b=0; b<InfoMan->EnemyBuildings.size(); b++){
			  if(  InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Zerg_Spore_Colony
				  &&  InfoMan->EnemyBuildings[b].position.getDistance( Wraiths[i].Wraith->getPosition() ) < 8*32){
				  ToClose = true;
			  }
		  }

		  //if there is nothing else, then attack buildings
		  if( closest == NULL && EnemyBuild != NULL ){
			  closest = EnemyBuild;
		  }

		  //bool canEngage = ToClose;
		  if( ToClose == true && DetectClose == false && Wraiths[i].Wraith->isCloaked() ){
            ToClose  = false;
		  }

		  if( Wraiths.size() > 6){
            ToClose  = false;
		  }

		  //check if we need to move away from danger
		  if( (ToClose == true 
			  || ( Wraiths[i].Wraith->getGroundWeaponCooldown() != 0 || Wraiths[i].Wraith->getAirWeaponCooldown() != 0 ) ) 
			  && closest != NULL ){
			  //BWAPI::Position fleePosition( (Wraiths[i].Wraith->getPosition() - closest->getPosition())+(Wraiths[i].Wraith->getPosition() - closest->getPosition()) + Wraiths[i].Wraith->getPosition());
			  BWAPI::Position fleePosition( InfoMan->PosOurBase  );
		  		    
			  BWAPI::Broodwar->drawLineMap(Wraiths[i].Wraith->getPosition().x, Wraiths[i].Wraith->getPosition().y, 
			fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
			  Wraiths[i].Wraith->move(fleePosition);
			  continue;
		  }

		  //check if we can engage
		  if( ToClose == false && closest != NULL ){
			  //wraiths slow down when approaching enemy if given attack when not in range
			  if( closest->getDistance( Wraiths[i].Wraith ) > 5*32){
				  Wraiths[i].Wraith->move( closest->getPosition() );
				  continue;
			  }
			  if( Wraiths[i].Wraith->getGroundWeaponCooldown() == 0 && Wraiths[i].Wraith->getAirWeaponCooldown() == 0 ){
  
					  if( Wraiths[i].Wraith->getOrderTarget() == NULL ){
                        Wraiths[i].Wraith->attack(  closest );
						continue;
					  }
					  else if( Wraiths[i].Wraith->getOrderTarget()->getID() != closest->getID() ){
		                Wraiths[i].Wraith->attack(  closest );
						continue;
					  }
					  //prevent spam
					  if( Wraiths[i].Wraith->getOrder() != BWAPI::Orders::AttackMove &&
						  Wraiths[i].Wraith->getOrder() != BWAPI::Orders::AttackTile && 
						  Wraiths[i].Wraith->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Wraiths[i].Wraith->attack(  closest );
					  }
			  } else { //weapon on cooldown


				  //Wraiths[i].Wraith->move(  closest->getPosition() );
			  //BWAPI::Position fleePosition( (Wraiths[i].Wraith->getPosition() - closest->getPosition())+(Wraiths[i].Wraith->getPosition() - closest->getPosition()) + Wraiths[i].Wraith->getPosition());
			  BWAPI::Position fleePosition( InfoMan->PosOurBase  );	    
			  BWAPI::Broodwar->drawLineMap(Wraiths[i].Wraith->getPosition().x, Wraiths[i].Wraith->getPosition().y, 
			fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
			  Wraiths[i].Wraith->move(fleePosition);

			  }
			  continue;
		  }

		  //if there are no targets, then move to the target location
		  if( closest == NULL ){ //ToClose == false && 
			  if( AttackTarget != BWAPI::Positions::None ){
			    Wraiths[i].Wraith->move( AttackTarget );
			  }
			  continue;
		  }


		}

	}

}