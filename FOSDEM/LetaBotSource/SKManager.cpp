#include "SKManager.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <boost/foreach.hpp>

#include "Common.h"

SKManager::SKManager(){
	Squads.reserve(200);
	SKSquad* Defence = new SKSquad;
	Defence->currentState = SK_Defend_Base;
	Defence->AttackBuilding = NULL;
	Defence->AttackTarget = BWAPI::Positions::None;
	Squads.push_back( Defence );
	SKSquad* Attack = new SKSquad;
	Attack->currentState = SK_Attack_Base;
	Attack->AttackBuilding = NULL;
	Attack->AttackTarget = BWAPI::Positions::None;
	Squads.push_back( Attack );

}

//add unit to the defence at first
void SKManager::AddUnit(Unit unit){

	if( unit->getType() == BWAPI::UnitTypes::Terran_Marine ){
		Squads[0]->Marines.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Medic ){
		Squads[0]->Medics.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel ){
		Squads[0]->ScienceVessels.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Firebat){
		Squads[0]->FireBats.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Goliath){
		Squads[0]->Goliaths.push_back(unit);
	}

}


void SKManager::onDefend(SKSquad* SQuad){

	BWAPI::Unit overLord = NULL;//is there a scouting overlord in our base?
	BOOST_FOREACH(Unit enemyO, Broodwar->enemy()->getUnits() ){
		if( enemyO->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
			BWTA::Region* OverReg = BWTA::getRegion( enemyO->getTilePosition() );
			if( OverReg != NULL ) {
			  if( InfoMan->OurMainregion == OverReg  ){
				  overLord = enemyO;
			  }
			}
		}
	}
	
	//Broodwar->printf("Defenduu");
	BWAPI::Position MarinePos = InfoMan->PosMainChoke;
	for(int i=0; i<SQuad->Marines.size(); i++){
		//attack scouting overlord
		  if( i > 3 && overLord != NULL ){
			  if( SQuad->Marines[i]->getTarget() == NULL ){
				  SQuad->Marines[i]->attack(overLord);
			  } else if(SQuad->Marines[i]->getTarget()->getID() != overLord->getID() ){
				  SQuad->Marines[i]->attack(overLord);
			  }
			continue;
		  }
		   if( SQuad->Marines[i]->getDistance( MarinePos ) > 5*32 ){
				//prevent spam
				if( SQuad->Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
					SQuad->Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
					SQuad->Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){
			     SQuad->Marines[i]->attack(MarinePos);
			     continue;
				}
		   } 
	}
	for(int i=0; i<SQuad->Medics.size(); i++){
		   if( SQuad->Medics[i]->getDistance( MarinePos ) > 5*32 ){
			   SQuad->Medics[i]->attack(MarinePos);
			   continue;
		   } 
	}
	for(int i=0; i<SQuad->FireBats.size(); i++){
		   if( SQuad->FireBats[i]->getDistance( MarinePos ) > 5*32 ){
				//prevent spam
				if( SQuad->FireBats[i]->getOrder() != BWAPI::Orders::AttackMove &&
					SQuad->FireBats[i]->getOrder() != BWAPI::Orders::AttackTile && 
					SQuad->FireBats[i]->getOrder() != BWAPI::Orders::AttackUnit ){
			     SQuad->FireBats[i]->attack(MarinePos);
			     continue;
				}
		   } 
	}


}

void SKManager::NextBuilding( SKSquad* SQuad ){

	//dont call if not needed
	if( SQuad->AttackBuilding != NULL &&
		SQuad->AttackTarget != BWAPI::Positions::None ){

	    return;
	}

	  if( InfoMan->EnemyBuildings.size() > 0 ){
	    SQuad->AttackBuilding = InfoMan->EnemyBuildings[0].building;
		SQuad->AttackTarget = InfoMan->EnemyBuildings[0].position;
		Broodwar->printf("Next building to attack");
	  } else {
		  SQuad->AttackTarget = BWAPI::Positions::None;
	  }

}

void SKManager::onBuildingDestroyed(Unit unit)
{
	for(int i=0; i<Squads.size(); i++){
		if( Squads[i]->AttackBuilding == NULL ){
			NextBuilding( Squads[i] );
		  return;
	    }

	  if(  unit->getID() == Squads[i]->AttackBuilding->getID()  ){
		  Broodwar->printf("Building destroyed, attacking new building");
		  Squads[i]->AttackBuilding = NULL;
	     NextBuilding( Squads[i] );
	  }
	}

}


//somewhat like Biomanager
void SKManager::onAttack(SKSquad* SQuad){

	//Broodwar->printf("Currently Attacking");

	if( SQuad->AttackTarget == BWAPI::Positions::None  ){
		NextBuilding( SQuad );
	}

	for(int i=0; i<SQuad->Goliaths.size(); i++){

		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = SQuad->Goliaths[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		int SunkenCount = 0;
		bool Sunken = false;
		int MedicClose = 0;
		int MarineClose = 0;
		int GoliathClose = 0;
		int totalE = 0;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType() == BWAPI::UnitTypes::Terran_Medic ){
				MedicClose++;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Marine ){
				MarineClose++;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Goliath ){
				GoliathClose++;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				Sunken= true;
				SunkenCount++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > SQuad->Goliaths[i]->getDistance( close->getPosition() )  ){
				closeDist = SQuad->Goliaths[i]->getDistance( close->getPosition() );
				closest = close;
				totalE++;
			}
		}

		/*
		//use stim before engaging enemy
		if( closest != NULL && !SQuad.Marines[i]->isStimmed() && SQuad.Marines[i]->getHitPoints() == 40 ){
			 SQuad.Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}
		*/
		
		//do we have enough to break a sunken
		//MarineClose >= SunkenCount*4
		if( MedicClose >= SunkenCount && MarineClose >= SunkenCount*3   ){
			Sunken = false;
		}
		//retreat when facing overwhelming odds
		if( totalE > ( MedicClose+MarineClose+GoliathClose ) ){
			Sunken = true;
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 4*32 &&  SQuad->Goliaths[i]->getGroundWeaponCooldown() != 0 ) 
	        ||  Sunken == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
		SQuad->Goliaths[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  SQuad->Goliaths[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( SQuad->Goliaths[i]->getPosition().getDistance( SQuad->AttackTarget ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( SQuad->Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  SQuad->Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  SQuad->Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  SQuad->Goliaths[i]->attack(  SQuad->AttackTarget );
					  }				  
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}
	}



	for(int i=0; i<SQuad->Marines.size(); i++){

		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = SQuad->Marines[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		int SunkenCount = 0;
		bool Sunken = false;
		int MedicClose = 0;
		int MarineClose = 0;
		int GoliathClose = 0;
		int totalE = 0;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType() == BWAPI::UnitTypes::Terran_Medic ){
				MedicClose++;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Marine ){
				MarineClose++;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Goliath ){
				GoliathClose++;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				Sunken= true;
				SunkenCount++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > SQuad->Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = SQuad->Marines[i]->getDistance( close->getPosition() );
				closest = close;
				totalE++;
			}
		}

		/*
		//use stim before engaging enemy
		if( closest != NULL && !SQuad.Marines[i]->isStimmed() && SQuad.Marines[i]->getHitPoints() == 40 ){
			 SQuad.Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}
		*/
		
		//do we have enough to break a sunken
		//MarineClose >= SunkenCount*4
		if( MedicClose >= SunkenCount && MarineClose >= SunkenCount*3   ){
			Sunken = false;
		}
		//retreat when facing overwhelming odds
		if( totalE > ( MedicClose+MarineClose+GoliathClose ) ){
			Sunken = true;
		}
		Sunken = false;//go for it

		//use stim before engaging enemy
		if( closest != NULL && !SQuad->Marines[i]->isStimmed() && SQuad->Marines[i]->getHitPoints() == 40
			&& closeDist < 5*32 ){
			 SQuad->Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 4*32 &&  SQuad->Marines[i]->getGroundWeaponCooldown() != 0 ) 
		  || (SQuad->Marines[i]->getHitPoints() < 15 && closest != NULL)
	        ||  Sunken == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		SQuad->Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  SQuad->Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( SQuad->Marines[i]->getPosition().getDistance( SQuad->AttackTarget ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( SQuad->Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  SQuad->Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  SQuad->Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  SQuad->Marines[i]->attack(  SQuad->AttackTarget );

					  }
				  
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}
	}


	for(unsigned int i=0; i<SQuad->FireBats.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = SQuad->FireBats[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > SQuad->FireBats[i]->getDistance( close->getPosition() )  ){
				closeDist = SQuad->FireBats[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !SQuad->FireBats[i]->isStimmed() && SQuad->FireBats[i]->getHitPoints() == 50
			&& closeDist < 3*32 ){
			 SQuad->FireBats[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

	  // if we can't shoot, run away  if(kite)
	  if (  (closest  != NULL && SQuad->FireBats[i]->getHitPoints() < 25) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Firebats[i]->getPosition().x, Firebats[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		SQuad->FireBats[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  SQuad->FireBats[i]->getGroundWeaponCooldown() == 0 ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( SQuad->FireBats[i]->getPosition().getDistance( SQuad->AttackTarget ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( SQuad->FireBats[i]->getOrderTarget() == NULL ){
                        SQuad->FireBats[i]->attack(  closest );
					  }
					  else if( SQuad->FireBats[i]->getOrderTarget()->getID() != closest->getID() ){
		                SQuad->FireBats[i]->attack(  closest );
					  }
				  } else {
					  //prevent spam
					  if( SQuad->FireBats[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  SQuad->FireBats[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  SQuad->FireBats[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  SQuad->FireBats[i]->attack(  SQuad->AttackTarget );
					  }

				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}
  
	}



	for(int i=0; i<SQuad->Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset UnitsClose = SQuad->Medics[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int MedicClose = 0;
		int MarineClose = 0;
		int SunkenCount = 0;
		int totalE = 0;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType() == BWAPI::UnitTypes::Terran_Medic ){
				MedicClose++;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Marine ){
				MarineClose++;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				Sunken = true;
				SunkenCount++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > SQuad->Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = SQuad->Medics[i]->getDistance( close->getPosition() );
				closest = close;
				totalE++;
			}
		}


		//do we have enough to break a sunken
		//MarineCount() MarineClose >= SunkenCount*4
		if( MedicClose >= SunkenCount && MarineClose >= SunkenCount*4  ){
			Sunken = false;
		}
		//retreat when facing overwhelming odds
		if( totalE > ( MedicClose+MarineClose )*2 ){
			Sunken = true;
		}

		Sunken = false;//go for it

	  // if we can't shoot, run away  if(kite)
	  if (  ( SQuad->Medics[i]->getHitPoints() < 25 && closest != NULL ) || Sunken == true
		    || ( MarineClose == 0 && closest != NULL ))
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		SQuad->Medics[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

	  //check if a unit nearby needs healing
	  Unit needsHeal = NULL;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->self() && closeDist > SQuad->Medics[i]->getDistance( close->getPosition() )
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed() ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL ){
			if( SQuad->Medics[i]->getOrderTarget() == NULL ){
				SQuad->Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			else if( SQuad->Medics[i]->getOrderTarget()->getID() != needsHeal->getID() ){
				SQuad->Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			continue;
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
			  Unit closestUnit = NULL;
			  int closeDist = 999999;
/*
		      BOOST_FOREACH( Unit close, Marines )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  */
			  BOOST_FOREACH( Unit close, SQuad->Marines )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  /*
			  BOOST_FOREACH( Unit close, Firebats )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  */
			  /*
			  if(  closestUnit != NULL){
				  //Medics[i]->attack(  closestUnit->getPosition() );//heal move
				  Medics[i]->move(  closestUnit->getPosition() );//regular move
			  }
			  */

				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			  if( SQuad->Medics[i]->getPosition().getDistance( SQuad->AttackTarget ) > 6*32 ){
				  SQuad->Medics[i]->move( SQuad->AttackTarget );//regular move
				}

			}
			/*
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Medics[i]->getPosition().getDistance( attackPos ) > 6*32 ){
		            Medics[i]->attack(  attackPos );//heal move
				}
			}
			*/
			continue;
		}
  
	}


}

//SK terran should use mobility
void SKManager::onFrame(){

	/*
	if(  Squads.size() > 1 ){
		Broodwar->printf("More than 1");
	}
	*/


	//switch to aggresive/defencive
	int totaltroops = MedicCount() + MarineCount() + FireBatCount();
	if( totaltroops > 6  &&  MedicCount() >= 4  &&  FireBatCount() >= 3
		&& Squads[0]->currentState == SK_Defend_Base ){
		Squads[0]->currentState = SK_Attack_Base;
	}
	
	//if( totaltroops < 5 && Squads[0]->currentState == SK_Attack_Base ){
		//Squads[0]->currentState = SK_Defend_Base;
	//}

	for(int i=0; i<Squads.size(); i++){

	//remove dead marines
	for(unsigned int j=0; j<Squads[i]->Marines.size(); j++){
		//remove dead marines
		if( !Squads[i]->Marines[j]->exists() ){
			Squads[i]->Marines.erase(Squads[i]->Marines.begin() + j );
			j--;
			continue;
		}
	}
    //remove dead medics
	for(unsigned int j=0; j<Squads[i]->Medics.size(); j++){
		//remove dead marines
		if( !Squads[i]->Medics[j]->exists() ){
			Squads[i]->Medics.erase(Squads[i]->Medics.begin() + j );
			j--;
			continue;
		}
	}
    //remove dead fireBats
	for(unsigned int j=0; j<Squads[i]->FireBats.size(); j++){
		//remove dead marines
		if( !Squads[i]->FireBats[j]->exists() ){
			Squads[i]->FireBats.erase(Squads[i]->FireBats.begin() + j );
			j--;
			continue;
		}
	}
    //remove dead Goliaths
	for(unsigned int j=0; j<Squads[i]->Goliaths.size(); j++){
		//remove dead marines
		if( !Squads[i]->Goliaths[j]->exists() ){
			Squads[i]->Goliaths.erase(Squads[i]->Goliaths.begin() + j );
			j--;
			continue;
		}
	}

	/*
	if( i == 1 ){
		//Broodwar->printf("curState: %d",Squads[i]->currentState);
		Broodwar->drawTextScreen(300,100,"Marines:%d",Squads[i]->Marines.size());
		Broodwar->drawTextScreen(300,110,"Medics:%d",Squads[i]->Medics.size());
		Broodwar->drawTextScreen(300,120,"FireBats:%d",Squads[i]->FireBats.size());
		Broodwar->drawTextScreen(300,140,"State %d:",Squads[i]->currentState);
	}
	*/

		if( Squads[i]->currentState == SK_Defend_Base ){
			/*
			//push out with these parameters
			if( MarineCount() > 2 && MedicCount() > 0 ){
				Squads[i].currentState = SK_Attack_Base;
				Broodwar->printf("Squad set to attack");
				break;
			}
			*/
			onDefend( Squads[i]);
		}
		if( Squads[i]->currentState == SK_Attack_Base ){
			//Broodwar->printf("Squad attacking");
			onAttack( Squads[i]);
		}

	}

}

void SKManager::InitialPush(){

	//create a new squad
	SKSquad* InitialAttack = new SKSquad;
	InitialAttack->currentState = SK_Attack_Base;
	InitialAttack->AttackTarget = BWAPI::Positions::None;
	InitialAttack->AttackBuilding = NULL;

	//grab all the firebats, medics and all but 1 Marine
	for( int i=0; i<Squads[0]->FireBats.size(); i++ ){
		InitialAttack->FireBats.push_back( Squads[0]->FireBats[i] );
	}
	Squads[0]->FireBats.clear();
	//grab all the medics
	for( int i=0; i<Squads[0]->Medics.size(); i++ ){
		InitialAttack->Medics.push_back( Squads[0]->Medics[i] );
	}
	Squads[0]->Medics.clear();
	//grab all but 1 Marine
	int totMarines = Squads[0]->Marines.size()-1;
	for( int i=0; i<totMarines; i++ ){
		InitialAttack->Marines.push_back( Squads[0]->Marines.back() );
		Squads[0]->Marines.pop_back();
	}

	Squads.push_back( InitialAttack );


}

int SKManager::MarineCount(){
	int sumM = 0;
	for(int i=0; i<Squads.size(); i++){
		sumM += Squads[i]->Marines.size();		
	}
	return sumM;
}

int SKManager::MedicCount(){
	int sumM = 0;
	for(int i=0; i<Squads.size(); i++){
		sumM += Squads[i]->Medics.size();		
	}
	return sumM;
}

int SKManager::FireBatCount(){
	int sumM = 0;
	for(int i=0; i<Squads.size(); i++){
		sumM += Squads[i]->FireBats.size();		
	}
	return sumM;
}

int SKManager::GoliathCount(){
	int sumM = 0;
	for(int i=0; i<Squads.size(); i++){
		sumM += Squads[i]->Goliaths.size();		
	}
	return sumM;
}


int SKManager::VesselCount(){
	int sumV = 0;
	for(int i=0; i<Squads.size(); i++){
		sumV += Squads[i]->ScienceVessels.size();		
	}
	return sumV;
}


BWAPI::UnitType SKManager::RecommendBio(){

	//if there is no academy, just build a marine
	if(  ProdMan->Academy == NULL ){
		return BWAPI::UnitTypes::Terran_Marine;
	}

	if(  ( SKMan->Squads[0]->currentState != SK_Attack_Base )  ){
	  if( ProdMan->getAvailableGas() > 50 ){
		  if(  Squads[0]->Medics.size() < 4 ){
			//Broodwar->printf("Medic count: %d ",MedicCount() );
		  return BWAPI::UnitTypes::Terran_Medic;
		  } else if( Squads[0]->FireBats.size() < 3 ){
			  return BWAPI::UnitTypes::Terran_Firebat;
		  } else {
		    return BWAPI::UnitTypes::Terran_Medic;
		  }
		  /*
		  if(  Squads[0]->FireBats.size() < 1 ){
		  return BWAPI::UnitTypes::Terran_Firebat;
		}	
		*/
	  } else {
		return BWAPI::UnitTypes::Terran_Marine;
	  }
	}

	if( SKMan->Squads[0]->currentState == SK_Attack_Base ){ //EightRaxMan->echtstaat == SK_Wraith
	  if( ProdMan->getAvailableGas() > 25 ){
		  if(  MedicCount()*4 < MarineCount() ){
			//Broodwar->printf("Medic count: %d ",MedicCount() );
		  return BWAPI::UnitTypes::Terran_Medic;
		}
		  if( FireBatCount() < 2 ){
			  return BWAPI::UnitTypes::Terran_Firebat;
		  }
	  } else {
		return BWAPI::UnitTypes::Terran_Marine;
	  }
	}

	return BWAPI::UnitTypes::Terran_Marine;
}