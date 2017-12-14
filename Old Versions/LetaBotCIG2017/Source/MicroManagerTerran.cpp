#include "MicroManagerTerran.h"

#include <BWAPI.h>
//#include <BWTA.h>
#include <math.h>
#include <boost/foreach.hpp>

#include "Common.h"





MicroManagerTerran::MicroManagerTerran(){




	Graph.clear();

	BOOST_FOREACH( BWTA::Region* reg, BWTA::getRegions() ){

		MoveGraphT newG;
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


void MicroManagerTerran::drawInfo()
{


   	for( int i=0; i<Graph.size(); i++){

		Broodwar->drawCircleMap( Graph[i].Center.x(), Graph[i].Center.y() ,  4 , BWAPI::Colors::White , true );

		BOOST_FOREACH( BWTA::Chokepoint* choke , Graph[i].reg->getChokepoints() ){

			Broodwar->drawLineMap(choke->getRegions().first->getCenter().x(), choke->getRegions().first->getCenter().y() , choke->getRegions().second->getCenter().x(), choke->getRegions().second->getCenter().y() , BWAPI::Colors::White );
		
		}
		/*
		BOOST_FOREACH( BWTA::Region* reg , Graph[i].reg->getReachableRegions() ){
			Broodwar->drawLineMap( Graph[i].Center.x(), Graph[i].Center.y() , reg->getCenter().x(), reg->getCenter().y() , BWAPI::Colors::White );
		}
		*/

	}

}


void MicroManagerTerran::onFrame()
{



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
				InfluenceBaseT newInfluenceBase;
				newInfluenceBase.CClocation = ProdMan->BuildingsQueue[i].buildLocation;
				newInfluenceBase.DefenceScoreNeeded = 0;
				newInfluenceBase.reg = BWTA::getRegion( ProdMan->BuildingsQueue[i].buildLocation  );
				MicroSquadTerran newDsquad;
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
			BOOST_FOREACH( BWAPI::Unit* enemy, Broodwar->enemy()->getUnits() ){
				if( cent.getDistance( enemy->getPosition() ) < 20*32 ){
					defScore += enemy->getType().buildScore();
				}
			}
			/*
			BOOST_FOREACH( BWAPI::Unit* enemy, Broodwar->getUnitsInRadius(   DefenceBases[j].reg->getCenter(), 20*32 ) ){
				if( enemy->getPlayer() == Broodwar->enemy() ){
					defScore += enemy->getType().buildScore();
				}
			}
			*/
		}

		int VulturesNeeded = 0 + ( defScore / 75 );

		if( VulturesNeeded > 7 ){
			VulturesNeeded = 7;
		}
		int GoliathsNeeded = 0;
		if (defScore > 0 ){
			GoliathsNeeded = 1;
		}

		for(int m=0; m<DefenceBases[j].defenceSquad.Vultures.size(); m++){
			if(  !DefenceBases[j].defenceSquad.Vultures[m]->exists() ){
				DefenceBases[j].defenceSquad.Vultures.erase( DefenceBases[j].defenceSquad.Vultures.begin() + m);
				m--;
				continue;
			}
		}


		for(int m=0; m<DefenceBases[j].defenceSquad.Goliaths.size(); m++){
			if(  !DefenceBases[j].defenceSquad.Goliaths[m]->exists() ){
				DefenceBases[j].defenceSquad.Goliaths.erase( DefenceBases[j].defenceSquad.Goliaths.begin() + m);
				m--;
				continue;
			}
		}


		if( DefenceBases[j].defenceSquad.Vultures.size() < VulturesNeeded ){
			if( MacroMan->Squads[0].Vultures.size() > 0 ){
				DefenceBases[j].defenceSquad.Vultures.push_back( MacroMan->Squads[0].Vultures[ MacroMan->Squads[0].Vultures.size() - 1 ]  );
				MacroMan->Squads[0].Vultures.pop_back();
			}
		}

		if( DefenceBases[j].defenceSquad.Vultures.size() > VulturesNeeded ){
			if(  DefenceBases[j].defenceSquad.Vultures.size() > 0 ){
				MacroMan->AddUnit( DefenceBases[j].defenceSquad.Vultures[ DefenceBases[j].defenceSquad.Vultures.size() - 1 ] );
				DefenceBases[j].defenceSquad.Vultures.pop_back();
			}
		}


		if( DefenceBases[j].defenceSquad.Goliaths.size() < GoliathsNeeded ){
			if( MacroMan->Squads[0].Goliaths.size() > 0 ){
				DefenceBases[j].defenceSquad.Goliaths.push_back( MacroMan->Squads[0].Goliaths[ MacroMan->Squads[0].Goliaths.size() - 1 ]  );
				MacroMan->Squads[0].Goliaths.pop_back();
			}
		}

		if( DefenceBases[j].defenceSquad.Goliaths.size() > GoliathsNeeded ){
			if(  DefenceBases[j].defenceSquad.Goliaths.size() > 0 ){
				MacroMan->AddUnit( DefenceBases[j].defenceSquad.Goliaths[ DefenceBases[j].defenceSquad.Goliaths.size() - 1 ] );
				DefenceBases[j].defenceSquad.Goliaths.pop_back();
			}
		}


		OnFrameTerranSquadDefendExpand( DefenceBases[j].defenceSquad );

	}





	//drawInfo();

}



void MicroManagerTerran::OnFrameTerranSquadDefendExpand(  MicroSquadTerran squad  )
{




	for(unsigned int i=0; i<squad.Vultures.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		std::set<Unit*> EnemyClose = squad.Vultures[i]->getUnitsInRadius( 9*32 );
		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Bunker = false;
		BOOST_FOREACH( Unit* close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Terran_Bunker){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Bunker 
				&& close->getPlayer() == Broodwar->enemy()
				&& squad.Vultures[i]->getDistance( close->getPosition() ) < 8*32  ){
				Bunker = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > squad.Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = squad.Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}




	  // if we can't shoot, run away  if(kite)
		//  Squads[0].Vultures[i]->getHitPoints() < 25 &&
		//( closeDist < 4*32 &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 ) 
		 // || ( closest != NULL &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 )
	  if (   false )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x(), Marines[i]->getPosition().y(), 
			//fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);

		squad.Vultures[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  squad.Vultures[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( squad.Vultures[i]->getPosition().getDistance( squad.AttackTarget ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( squad.Vultures[i]->getOrderTarget() == NULL ){
                        squad.Vultures[i]->attack(  closest );
					  }
					  else if( squad.Vultures[i]->getOrderTarget()->getID() != closest->getID() ){
		                squad.Vultures[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( squad.Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  squad.Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  squad.Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  squad.Vultures[i]->attack( squad.AttackTarget );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}
	  
	}





	for(unsigned int i=0; i<squad.Goliaths.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		std::set<Unit*> EnemyClose = squad.Goliaths[i]->getUnitsInRadius( 15*32 );
		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Bunker = false;
		BOOST_FOREACH( Unit* close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Terran_Bunker){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Bunker 
				&& close->getPlayer() == Broodwar->enemy()
				&& squad.Goliaths[i]->getDistance( close->getPosition() ) < 8*32  ){
				Bunker = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > squad.Goliaths[i]->getDistance( close->getPosition() )  ){
				closeDist = squad.Goliaths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}





	  // if we can't shoot, run away  if(kite)
		//  || (Squads[0].Goliaths[i]->getHitPoints() < 25 && closest != NULL &&  Squads[0].Goliaths[i]->getGroundWeaponCooldown() != 0 )
	  if ( ( closeDist < 2*32 &&  squad.Goliaths[i]->getGroundWeaponCooldown() != 0 )  )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x(), Marines[i]->getPosition().y(), 
			//fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);

		squad.Goliaths[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  squad.Goliaths[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( squad.Goliaths[i]->getPosition().getDistance( squad.AttackTarget ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( squad.Goliaths[i]->getOrderTarget() == NULL ){
                        squad.Goliaths[i]->attack(  closest );
					  }
					  else if( squad.Goliaths[i]->getOrderTarget()->getID() != closest->getID() ){
		                squad.Goliaths[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( squad.Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  squad.Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  squad.Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  squad.Goliaths[i]->attack(  squad.AttackTarget );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}




}