#include "MicroManagerProtoss.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <boost/foreach.hpp>

#include "Common.h"





MicroManagerProtoss::MicroManagerProtoss(){


	Graph.clear();

	BOOST_FOREACH( BWTA::Region* reg, BWTA::getRegions() ){

		MoveGraph newG;
		newG.reg = reg;
		newG.Center = reg->getCenter();
		Graph.push_back( newG );
	}


	for( int i=0; i<Graph.size(); i++){

		/*
		for(int j=0; j<Graph[i].reg->getReachableRegions().size(); j++){

			for(int k=0; k<Graph.size(); k++){
              
			}
		}
		*/

	}



}



void MicroManagerProtoss::drawInfo()
{


   	for( int i=0; i<Graph.size(); i++){

		Broodwar->drawCircleMap( Graph[i].Center.x, Graph[i].Center.y ,  4 , BWAPI::Colors::White , true );

		BOOST_FOREACH( BWTA::Chokepoint* choke , Graph[i].reg->getChokepoints() ){

			Broodwar->drawLineMap(choke->getRegions().first->getCenter().x, choke->getRegions().first->getCenter().y , choke->getRegions().second->getCenter().x, choke->getRegions().second->getCenter().y , BWAPI::Colors::White );
		
		}
		/*
		BOOST_FOREACH( BWTA::Region* reg , Graph[i].reg->getReachableRegions() ){
			Broodwar->drawLineMap( Graph[i].Center.x(), Graph[i].Center.y() , reg->getCenter().x(), reg->getCenter().y() , BWAPI::Colors::White );
		}
		*/

	}

}


void MicroManagerProtoss::EarlyDefend()
{

	BWAPI::Position DefaultPos;
	//BWAPI::Position DefaultPos =  CCmanager[CCmanager.size() - 1]->CommandCenter->getPosition();
	if( CCmanager.size() > 0 ){
		DefaultPos =  CCmanager[CCmanager.size() - 1]->CommandCenter->getPosition();
	}
	for(int i=0; i<Squads[0].Marines.size(); i++){
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
	//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 )  )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
		Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }
		//by default move to the CC
		if( closest == NULL ){
			Squads[0].Marines[i]->move( DefaultPos );
			continue;
		}
		if( closest != NULL ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
			if( (closest->getType() == BWAPI::UnitTypes::Protoss_Zealot 
				|| BWTA::getRegion( closest->getTilePosition() ) == InfoMan->OurMainregion  ) ){
				if( Squads[0].Marines[i]->getTarget() != NULL){
					if( Squads[0].Marines[i]->getTarget()->getID() != closest->getID() ){
						Squads[0].Marines[i]->attack( closest);
						continue;
					}
				} else {
					Squads[0].Marines[i]->attack( closest);
			        continue;
				}
			} else {
			  Squads[0].Marines[i]->move( DefaultPos );
			  continue;
			}
		}

	}

	for(int i=0; i<Squads[0].Tanks.size(); i++){
		Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
		//by default move to the CC
		if( closest == NULL ){

			if(  Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
				&& Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
					Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
					continue;
			}

			Squads[0].Tanks[i]->move( DefaultPos );
			continue;
		}
		if( closest != NULL ){
			if(  closeDist > 12*32 && Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
					Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
					continue;
			}

			if( closeDist > 6*32 &&  closeDist < 12*32 
				&& Squads[0].Tanks[i]->getType() != BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
				&& Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
					Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
					continue;
			}
			if( closeDist > 6*32 && Squads[0].Tanks[i]->getType() != BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode  ){
				if( Squads[0].Tanks[i]->getTarget() != NULL){
					if( Squads[0].Tanks[i]->getTarget()->getID() != closest->getID() ){
						Squads[0].Tanks[i]->attack( closest);
						continue;
					}
				} else {
					Squads[0].Tanks[i]->attack( closest);
			        continue;
				}
			} 
			//if enemy gets too close, fall back
			if( closeDist <= 6*32 && !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode )  ){
			  Squads[0].Tanks[i]->move( DefaultPos );
			  continue;
			}
		}
	}



	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );	
		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

	  // if we can't shoot, run away  if(kite)
	  if (  ( Squads[0].Medics[i]->getHitPoints() < 25 ) || PhotonCan == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Medics[i]->move( fleePosition );
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
			if( close->getPlayer() == Broodwar->self() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed() ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL ){
			if( Squads[0].Medics[i]->getOrderTarget() == NULL ){
				Squads[0].Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			else if( Squads[0].Medics[i]->getOrderTarget()->getID() != needsHeal->getID() ){
				Squads[0].Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			continue;
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
			  Unit closestUnit = NULL;
			  int closeDist = 999999;
			  BOOST_FOREACH( Unit close, Squads[0].Marines )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }

				if( Squads[0].Medics[i]->getPosition().getDistance( DefaultPos ) > 6*32 ){
			      Squads[0].Medics[i]->move(  DefaultPos );//regular move
				}

			}

			continue;
		}
  
	}




}

void MicroManagerProtoss::onFrame()
{

	//drawInfo();
  



	for(int i=0; i<ProdMan->BuildingsQueue.size(); i++){

		if( ProdMan->BuildingsQueue[i].type == BWAPI::UnitTypes::Terran_Command_Center
			&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) >= 2 ){
			bool already = false;
			for(int j=0; j<DefenceBases.size(); j++){
				if(  ProdMan->BuildingsQueue[i].buildLocation == DefenceBases[j].CClocation ){
					already = true;
				}
			}

			if( already == false 
				&& BWTA::getRegion( ProdMan->BuildingsQueue[i].buildLocation  ) != InfoMan->NatRegion 
				&& BWTA::getRegion( ProdMan->BuildingsQueue[i].buildLocation  ) != InfoMan->OurMainregion
				){
				InfluenceBaseP newInfluenceBase;
				newInfluenceBase.CClocation = ProdMan->BuildingsQueue[i].buildLocation;
				newInfluenceBase.DefenceScoreNeeded = 0;
				newInfluenceBase.reg = BWTA::getRegion( ProdMan->BuildingsQueue[i].buildLocation  );
				MicroSquadProtoss newDsquad;
				BWAPI::Position bestPos = BWAPI::Positions::Unknown;
				int closestEbase = 99999;
				int furthestOurbase = 0;
				BOOST_FOREACH( BWTA::Chokepoint * choke, newInfluenceBase.reg->getChokepoints() ){
					if( choke->getCenter().getDistance( InfoMan->PosOurBase ) > furthestOurbase ){
						furthestOurbase = choke->getCenter().getDistance( InfoMan->PosOurBase );
						bestPos = choke->getCenter();
					}
					/*
					if( choke->getCenter().getDistance( MacroMan->attackPos ) < closestEbase ){
						closestEbase = choke->getCenter().getDistance( MacroMan->attackPos );
						bestPos = choke->getCenter();
					}
					*/
				}
				newDsquad.AttackTarget = bestPos;
				newInfluenceBase.defenceSquad = newDsquad;
				DefenceBases.push_back( newInfluenceBase );
			}

		}

	}


	for(int j=0; j<DefenceBases.size(); j++){
		int defScore = 0;
		if(  DefenceBases[j].reg != NULL ){
			BWAPI::Position cent = DefenceBases[j].reg->getCenter();
			BOOST_FOREACH( BWAPI::Unit enemy, Broodwar->enemy()->getUnits() ){
				if( cent.getDistance( enemy->getPosition() ) < 20*32 ){
					defScore += enemy->getType().buildScore();
				}
			}
			/*
			BOOST_FOREACH( BWAPI::Unit enemy, Broodwar->getUnitsInRadius(   DefenceBases[j].reg->getCenter(), 20*32 ) ){
				if( enemy->getPlayer() == Broodwar->enemy() ){
					defScore += enemy->getType().buildScore();
				}
			}
			*/
		}

		int MarinesNeeded = ( defScore / 50.0 );
		int MedicsNeeded = ( defScore / 100.0 );

		if( MarinesNeeded > 7 ){
			MarinesNeeded = 7;
		}
		if( MedicsNeeded > 2 ){
			MedicsNeeded = 2;
		}

		for(int m=0; m<DefenceBases[j].defenceSquad.Marines.size(); m++){
			if(  !DefenceBases[j].defenceSquad.Marines[m]->exists() ){
				DefenceBases[j].defenceSquad.Marines.erase( DefenceBases[j].defenceSquad.Marines.begin() + m);
				m--;
				continue;
			}
		}
		for(int m=0; m<DefenceBases[j].defenceSquad.Medics.size(); m++){
			if(  !DefenceBases[j].defenceSquad.Medics[m]->exists() ){
				DefenceBases[j].defenceSquad.Medics.erase( DefenceBases[j].defenceSquad.Medics.begin() + m);
				m--;
				continue;
			}
		}


		if( DefenceBases[j].defenceSquad.Marines.size() < MarinesNeeded ){
			if( MacroMan->Squads[0].Marines.size() > 0 ){
				DefenceBases[j].defenceSquad.Marines.push_back( MacroMan->Squads[0].Marines[ MacroMan->Squads[0].Marines.size() - 1 ]  );
				MacroMan->Squads[0].Marines.pop_back();
			}
		}

		if( DefenceBases[j].defenceSquad.Medics.size() < MedicsNeeded ){
			if( MacroMan->Squads[0].Medics.size() > 0 ){
				DefenceBases[j].defenceSquad.Medics.push_back( MacroMan->Squads[0].Medics[ MacroMan->Squads[0].Medics.size() - 1 ]  );
				MacroMan->Squads[0].Medics.pop_back();
			}
		}


		if( DefenceBases[j].defenceSquad.Marines.size() > MarinesNeeded ){
			if(  DefenceBases[j].defenceSquad.Marines.size() > 0 ){
				MacroMan->AddUnit( DefenceBases[j].defenceSquad.Marines[ DefenceBases[j].defenceSquad.Marines.size() - 1 ] );
				DefenceBases[j].defenceSquad.Marines.pop_back();
			}
		}

		if( DefenceBases[j].defenceSquad.Medics.size() > MedicsNeeded ){
			if(  DefenceBases[j].defenceSquad.Medics.size() > 0 ){
				MacroMan->AddUnit( DefenceBases[j].defenceSquad.Medics[ DefenceBases[j].defenceSquad.Medics.size() - 1 ] );
				DefenceBases[j].defenceSquad.Medics.pop_back();
			}
		}


		OnFrameProtossSquadDefenceExpand( DefenceBases[j].defenceSquad );

	}




	/*
	if( MacroMan->PState == P_Wall ){
		EarlyDefend();
	}
	*/

}




void MicroManagerProtoss::OnFrameProtossSquadDefenceExpand( MicroSquadProtoss squad ){

	/*
	Broodwar->drawTextScreen( 300,110, " Enemy buildings: %d", InfoMan->EnemyBuildings.size());
	Broodwar->drawTextScreen( 300,120, " HuntP: %d", HuntP);
	Broodwar->drawTextScreen( 300,130, " HuntAllowedP: %d", HuntAllowedP);		
	*/


	BWAPI::Position defencePosP = squad.AttackTarget;

	Broodwar->drawCircleMap( squad.AttackTarget.x, squad.AttackTarget.y, 5 , BWAPI::Colors::Orange,true);

	/*
	if( attackPosP != BWAPI::Positions::Invalid &&  attackPosP != BWAPI::Positions::None &&  attackPosP != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( attackPosP.x(), attackPosP.y(), 5 , BWAPI::Colors::Purple,true);
	}
	*/


	for(unsigned int i=0; i<squad.Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = squad.Marines[i]->getUnitsInRadius( 20*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		int totalPhoton = 0;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			
			if(  !close->isDetected() ){
				continue;
			}			
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}

			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
				totalPhoton++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > squad.Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = squad.Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !squad.Marines[i]->isStimmed() 
			&& squad.Marines[i]->getHitPoints() == 40  && closeDist < 5*32 ){
			 squad.Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( squad.Marines[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}

		/*
		if( Squads[0].Marines[i]->isUnderStorm() ){
			CloseStorm = true;
		}
		*/

	  // if we can't shoot, run away  if(kite)
		//  || (Squads[0].Marines[i]->getHitPoints() < 25 && closest != NULL )
		// && !Squads[0].Marines[i]->isAttackFrame()
		if ( ( closeDist < 4*32 &&  squad.Marines[i]->getGroundWeaponCooldown() != 0  && !squad.Marines[i]->isStartingAttack()  )
			||  CloseStorm == true
			|| (squad.Marines[i]->getHitPoints() < 25 && closest != NULL &&  squad.Marines[i]->getGroundWeaponCooldown() != 0 ) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		squad.Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  squad.Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( squad.Marines[i]->getPosition().getDistance( defencePosP ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( squad.Marines[i]->getOrderTarget() == NULL ){
                        squad.Marines[i]->attack(  closest );
					  }
					  else if( squad.Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                squad.Marines[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( squad.Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  squad.Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  squad.Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  squad.Marines[i]->attack(  defencePosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}


	  
	}





	for(unsigned int i=0; i< squad.Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = squad.Medics[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		int totalPhoton = 0;
		int FriendlyUnitCount = 0;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getPlayer() == Broodwar->self() && close->getType() != BWAPI::UnitTypes::Terran_Medic ){
				FriendlyUnitCount++;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
				totalPhoton++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > squad.Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = squad.Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( squad.Medics[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}





	  // if we can't shoot, run away  if(kite)

		// ||  ( FriendlyUnitCount == 0 &&  Squads[id].Medics[i]->getDistance( InfoMan->PosOurBase ) > 30*32 )

		if (  ( squad.Medics[i]->getHitPoints() < 25 && closest != NULL ) 
			|| CloseStorm == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		squad.Medics[i]->move( fleePosition );
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
			if( close->getPlayer() == Broodwar->self() && closeDist > squad.Medics[i]->getDistance( close->getPosition() )
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed() ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL ){
			if( squad.Medics[i]->getOrderTarget() == NULL ){
				squad.Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			else if( squad.Medics[i]->getOrderTarget()->getID() != needsHeal->getID() ){
				squad.Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			continue;
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ){ //|| closeDist > 3*32
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
			  BOOST_FOREACH( Unit close, squad.Marines )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  /*
			  if(  closestUnit != NULL){
				  //Medics[i]->attack(  closestUnit->getPosition() );//heal move
				  Medics[i]->move(  closestUnit->getPosition() );//regular move
			  }
			  */

				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( squad.Medics[i]->getPosition().getDistance( defencePosP ) > 6*32 ){
			      squad.Medics[i]->move(  defencePosP );//regular move
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







void MicroManagerProtoss::AddUnit( Unit unit){

	if( unit == NULL){
		return;
	}

	if( unit->getPlayer() == Broodwar->enemy() ){
		return;
	}


	//Command center complete
	if( unit->getType() == BWAPI::UnitTypes::Terran_Command_Center
		&& unit->getDistance( InfoMan->PosOurNat ) < 5*32  ){

		//PlaceNatBunker();
	}


	if( unit->getType() == BWAPI::UnitTypes::Terran_Marine){
		Squads[0].Marines.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Medic){
		Squads[0].Medics.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode){
		Squads[0].Tanks.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Goliath){
		Squads[0].Goliaths.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Vulture){
		Squads[0].Vultures.push_back(unit);
	}
	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_Wraith){
		Squads[0].Wraiths.push_back(unit);
	}
	*/

	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_Valkyrie){
		Squads[0].Valkryies.push_back(unit);
	}
	*/
	if( unit->getType() == BWAPI::UnitTypes::Terran_Bunker){

		Bunkers.push_back(unit);

	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel){
		Squads[0].ScienceVessels.push_back(unit);
	}
	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser){
		Squads[0].BattleCruisers.push_back(unit);
	}
	*/

	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_SCV && ZState == Z_Plug_Ramp
		&& Squads[0].SCVs.size() < 4  ){
		Squads[0].SCVs.push_back(unit);
	}
	*/

}
