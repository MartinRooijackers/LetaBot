#include "MicroManagerZerg.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <boost/foreach.hpp>

#include "Common.h"





MicroManagerZerg::MicroManagerZerg(){


	MMpush = false;
	RetreatIpush = false;

	
	DefenceBases.clear();
	
	Graph.clear();

	BOOST_FOREACH( BWTA::Region* reg, BWTA::getRegions() ){

		MoveGraphZ newG;
		newG.reg = reg;
		newG.Center = reg->getCenter();
		Graph.push_back( newG );
	}


	/*
	InfluenceMapPos.clear();

	//InfoMan->OurMainregion->getPolygon();
	for(int x=0; x<Broodwar->mapWidth(); x++){
		for(int y=0; y<Broodwar->mapHeight(); y++){
			if( BWTA::getRegion(  x , y ) == InfoMan->OurMainregion
				||   BWTA::getRegion(  x , y ) == BWTA::getRegion(  InfoMan->PosOurNat ) ){
                XY newXY;
				newXY.x = x;
				newXY.y = y;
				InfluenceMapPos.push_back( newXY );
			}
	  }
	}

	*/

}


void MicroManagerZerg::onFrame()
{



	for(int i=0; i<ProdMan->BuildingsQueue.size(); i++){

		if( ProdMan->BuildingsQueue[i].type == BWAPI::UnitTypes::Terran_Command_Center
			&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) >= 2 
						){
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
				InfluenceBaseZ newInfluenceBase;
				newInfluenceBase.CClocation = ProdMan->BuildingsQueue[i].buildLocation;
				newInfluenceBase.DefenceScoreNeeded = 0;
				newInfluenceBase.reg = BWTA::getRegion( ProdMan->BuildingsQueue[i].buildLocation  );
				MicroSquadZerg newDsquad;
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

		int MarinesNeeded = 0 + ( defScore / 50.0 );
		int MedicsNeeded = ( defScore / 200.0 );

		//double check to make sure we always have at least 1 marine defending if needed
		if (MarinesNeeded == 0 && defScore > 0) {
			MarinesNeeded = 1;
		}

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

		OnFrameZergSquadDefendExpand( DefenceBases[j].defenceSquad );

	}


	
   	for( int i=0; i<Graph.size(); i++){

		Broodwar->drawCircleMap( Graph[i].Center.x, Graph[i].Center.y ,  4 , BWAPI::Colors::White , true );

		BOOST_FOREACH( BWTA::Chokepoint* choke , Graph[i].reg->getChokepoints() ){

			Broodwar->drawLineMap(choke->getRegions().first->getCenter().x, choke->getRegions().first->getCenter().y , choke->getRegions().second->getCenter().x, choke->getRegions().second->getCenter().y , BWAPI::Colors::White );
		
		}

	}
	

	/*
	if( MMpush == true ){
		InitialPush();
	}
	*/


}


void MicroManagerZerg::StartIPush()
{


	Broodwar->printf( "Staring push" );

	MMpush = true;
	MicroSquadZerg newSquad;

	while( MacroMan->Squads[0].Marines.size() > 4){
		newSquad.Marines.push_back( MacroMan->Squads[0].Marines.back() );
		MacroMan->Squads[0].Marines.pop_back();
	}
	while( MacroMan->Squads[0].Medics.size() > 0){
		newSquad.Medics.push_back( MacroMan->Squads[0].Marines.back() );
		MacroMan->Squads[0].Medics.pop_back();
	}
	
	//Squads.push_back( newSquad );


}


/*
void MicroManagerZerg::InitialPush()
{

	BWAPI::Position attackPos = InfoMan->PosEnemyBase;

	int SunkMax = 2;

	if( Broodwar->getFrameCount() > 13000){
		RetreatIpush = true;
	}

	if( RetreatIpush == false ){

		attackPos = InfoMan->PosEnemyBase;

	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				if( ClosestBurrowedLurker > Squads[0].Marines[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = Squads[0].Marines[i]->getDistance( close->getPosition() );
				}
			}

			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				//Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40
			&& closeDist < 5*32 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3;

		
		if( totSunk > SunkMax ){
			RetreatIpush = true;
		}
		
	  // if we can't shoot, run away  if(kite)
		if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
			||  ClosestBurrowedLurker < 8*32  )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::Move){
		    Squads[0].Marines[i]->move( fleePosition );
		  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].Marines[i]->getPosition().getDistance( attackPos ) > 6*32 ){
                  if( closest != NULL ){
					  if( closest->getType() == BWAPI::UnitTypes::Zerg_Scourge  ){
					    if( Squads[0].Marines[i]->getOrderTarget() == NULL ){
                         Squads[0].Marines[i]->attack(  closest );
					    }
					    else if( Squads[0].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                  Squads[0].Marines[i]->attack(  closest );
					    }
					  } else {


					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

							  Squads[0].Marines[i]->attack(  closest->getPosition() );

					  }

					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Marines[i]->attack(  attackPos );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}


	  
	}


	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		//bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {

			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				if( ClosestBurrowedLurker > Squads[0].Marines[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = Squads[0].Marines[i]->getDistance( close->getPosition() );
				}
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				//Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3 + Squads[0].Goliaths.size()*2;
		
		if( totSunk > SunkMax ){
			RetreatIpush = true;
		}
		


	  // if we can't shoot, run away  if(kite)
	  if ( ( ( Squads[0].Medics[i]->getHitPoints() < 25 ) 
		  || ClosestBurrowedLurker < 8*32 ) &&  Squads[0].Medics[i]->getDistance( InfoMan->PosOurBase ) > 3*32 )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		 if( Squads[0].Medics[i]->getOrder() != BWAPI::Orders::Move){
		       Squads[0].Medics[i]->move( fleePosition );
		  }
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
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed()
				&& close->getType() == BWAPI::UnitTypes::Terran_Marine ){
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


				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].Medics[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].Medics[i]->move(  attackPos );//regular move
				}

			}

			continue;
		}
  
	}



	} else { //get back to base





		attackPos = InfoMan->PosOurBase;

	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){	

		if( BWTA::getRegion( Squads[0].Marines[i]->getPosition() ) == InfoMan->NatRegion ){
			MacroMan->AddUnit( Squads[0].Marines[i] );
			Squads[0].Marines.erase( Squads[0].Marines.begin() + i );
			i--;
			continue;
		}

		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		int totSunk = 0;
		bool Sunken = false;
		int ClosestBurrowedLurker = 99999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				if( ClosestBurrowedLurker > Squads[0].Marines[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = Squads[0].Marines[i]->getDistance( close->getPosition() );
				}
			}

			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40
			&& closeDist < 5*32 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3;

		
		if( totSunk > SunkMax ){
			RetreatIpush = true;
		}
		
	  // if we can't shoot, run away  if(kite)
		if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
		  || (Squads[0].Marines[i]->getHitPoints() < 20 && closest != NULL && LanLatency == true )
			||  ClosestBurrowedLurker < 8*32  )
	  {
		BWAPI::Position fleePosition( Squads[0].Marines[i]->getPosition() - closest->getPosition() + Squads[0].Marines[i]->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::Move){
		    Squads[0].Marines[i]->move( fleePosition );
		  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].Marines[i]->getPosition().getDistance( attackPos ) > 6*32 ){
                  if( closest != NULL ){
					  if( closest->getType() == BWAPI::UnitTypes::Zerg_Scourge  ){
					    if( Squads[0].Marines[i]->getOrderTarget() == NULL ){
                         Squads[0].Marines[i]->attack(  closest );
					    }
					    else if( Squads[0].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                  Squads[0].Marines[i]->attack(  closest );
					    }
					  } else {


					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

							  Squads[0].Marines[i]->attack(  closest->getPosition() );

					  }

					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Marines[i]->attack(  attackPos );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}


	  
	}


	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		

		if( BWTA::getRegion( Squads[0].Medics[i]->getPosition() ) == InfoMan->NatRegion ){
			MacroMan->AddUnit( Squads[0].Medics[i] );
			Squads[0].Medics.erase( Squads[0].Medics.begin() + i );
			i--;
			continue;
		}

		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {

			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				if( ClosestBurrowedLurker > Squads[0].Marines[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = Squads[0].Marines[i]->getDistance( close->getPosition() );
				}
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				//Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3 + Squads[0].Goliaths.size()*2;
		
	
		


	  // if we can't shoot, run away  if(kite)
	  if ( ( ( Squads[0].Medics[i]->getHitPoints() < 25 ) 
		  || ClosestBurrowedLurker < 8*32 ) &&  Squads[0].Medics[i]->getDistance( InfoMan->PosOurBase ) > 3*32 )
	  {
		BWAPI::Position fleePosition(Squads[0].Medics[i]->getPosition() - closest->getPosition() + Squads[0].Medics[i]->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		 if( Squads[0].Medics[i]->getOrder() != BWAPI::Orders::Move){
		       Squads[0].Medics[i]->move( fleePosition );
		  }
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
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed()
				&& close->getType() == BWAPI::UnitTypes::Terran_Marine ){
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


				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].Medics[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].Medics[i]->move(  attackPos );//regular move
				}

			}

			continue;
		}



	  }



	}

}

*/




void MicroManagerZerg::OnFrameZergSquadDefendExpand( MicroSquadZerg squad ){

	
	//int SunkMulti = 1;
	//squad.AttackTarget
	if( squad.AttackTarget != BWAPI::Positions::Invalid &&  squad.AttackTarget != BWAPI::Positions::None &&  squad.AttackTarget != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( squad.AttackTarget.x, squad.AttackTarget.y, 5 , BWAPI::Colors::Purple,true);
	}

	/*
	if( Squads[0].SCVs.size() == 0 && CCmanager[0]->MineralSCV.size() > (CCmanager[0]->SCVsaturation - 5) ){
		AddSCV( CCmanager[0]->getFullHPSCV() );
	}
	*/


	/*
	//get a list of enemy units
	std::vector< BWAPI::Unit > EnemyUnits;
	BOOST_FOREACH( Unit close, Broodwar->enemy()->getUnits() ){

		if(  close->getType().isBuilding() && close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
			continue;
		}
		EnemyUnits.push_back( close );
	}
	*/


	for(unsigned int i=0; i<squad.Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistAttack =  30*32;
		Unitset EnemyClose = squad.Marines[i]->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		int TotalLurker = 0;
		bool OverrunLurker = false;
		//BOOST_FOREACH( Unit close, EnemyClose )
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( squad.Marines[i]->getDistance( close->getPosition() ) > MaxDistAttack){
				continue;
			}
			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				//&&  close->isBurrowed()
				TotalLurker++;
				if( ClosestBurrowedLurker > squad.Marines[i]->getDistance( close->getPosition() )
				){
					ClosestBurrowedLurker = squad.Marines[i]->getDistance( close->getPosition() );
				}
			}

			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
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
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > squad.Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = squad.Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}



		//use stim before engaging enemy
		if( closest != NULL && !squad.Marines[i]->isStimmed() && squad.Marines[i]->getHitPoints() == 40
			&& closeDist < 6*32 ){
			 squad.Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		int ourScore = squad.Marines.size() + squad.Medics.size() * 3;


		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

		if( squad.Marines.size() > TotalLurker*4 ){
			OverrunLurker = true;
		}

		/*
		if( Squads[0].Marines[i]->getDistance( BWAPI::Position( InfoMan->EnemyNatural ) ) > 20*32 ){
			if( closeDist > 5*32 ){
				Squads[0].Marines[i]->move( attackPos );
				continue;
			}
			if( Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 ){
				Squads[0].Marines[i]->move( attackPos );
				continue;
			}
		}
		*/

	  // if we can't shoot, run away  if(kite)
		//    || (Squads[0].Marines[i]->getHitPoints() < 20 && closest != NULL && LanLatency == true )
		//   ||  ClosestBurrowedLurker < 8*32
		if ( ( LanLatency == true ||  isUnderDS ) && (   ( closeDist < 4*32 &&  squad.Marines[i]->getGroundWeaponCooldown() != 0 && LanLatency == true )
			||  isUnderDS  )  )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  if( squad.Marines[i]->getOrder() != BWAPI::Orders::Move){
		      squad.Marines[i]->move( fleePosition );
		  }
		  //Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		/*
		//make sure that marines are not attacking larva
		if( closest == NULL ){
		  if( Squads[0].Marines[i]->getOrderTarget() != NULL
			   && Squads[0].Marines[i]->getPosition().getDistance( attackPos ) > 5*32  ){
			  if( Squads[0].Marines[i]->getOrderTarget()->getType() == BWAPI::UnitTypes::Zerg_Larva
				  || Squads[0].Marines[i]->getOrderTarget()->getType() == BWAPI::UnitTypes::Zerg_Egg){
					  Squads[0].Marines[i]->move( attackPos );
			  }
			}

		}
		*/

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  squad.Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( squad.Marines[i]->getPosition().getDistance(squad.AttackTarget ) > 5*32 ){
                  if( closest != NULL ){
					  if(  closeDist > 6*32  ){
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

							  squad.Marines[i]->attack(  closest->getPosition() );

					  }

					  }
				  } else {

					  //prevent spam
					  if( squad.Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  squad.Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  squad.Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  squad.Marines[i]->attack( squad.AttackTarget );

					  }
				  }
				  //Broodwar->printf("Attack");
				}			
			continue;
		}
	  
	}


	BWAPI::Unit closestMarine = NULL;
	int closestMarineDist = 99999;

	BOOST_FOREACH( Unit close, squad.Marines )
              {
				  if( close->getPosition().getDistance( squad.AttackTarget ) < closestMarineDist ){
			         closestMarine = close;
			         closestMarineDist = close->getPosition().getDistance(squad.AttackTarget );
				  }
		}

	for(unsigned int i=0; i< squad.Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = squad.Medics[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		int TotalLurker = 0;
		bool OverrunLurker = false;
		BOOST_FOREACH( Unit close, UnitsClose )
        {

			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				TotalLurker++;
				if( ClosestBurrowedLurker > squad.Medics[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = squad.Medics[i]->getDistance( close->getPosition() );
				}
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > squad.Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = squad.Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}


	  // if we can't shoot, run away  if(kite)
		// LanLatency == true &&
	  if (   ( ( ( squad.Medics[i]->getHitPoints() < 20 )
		  ||  isUnderDS ) 
		  
		  &&  squad.Medics[i]->getDistance( InfoMan->PosOurBase ) > 3*32 ) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		 if( squad.Medics[i]->getOrder() != BWAPI::Orders::Move){
		       squad.Medics[i]->move( fleePosition );
		  }
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
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed()
				&& close->getType() == BWAPI::UnitTypes::Terran_Marine ){
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
			  /*
			  BOOST_FOREACH( Unit close, Squads[0].Marines )
              {
				  if( close->getPosition().getDistance( attackPos ) < closeDist ){
			         closestUnit = close;
			         closeDist = close->getPosition().getDistance( attackPos );
				  }
		      }
			  
			  if(  closestUnit != NULL){
				  //Squads[0].Medics[i]->attack(  closestUnit->getPosition() );//heal move
				  Squads[0].Medics[i]->move(  closestUnit->getPosition() );//regular move
				  continue;
			  }
			  */
			  
			  if(  closestMarine != NULL){
				  //Squads[0].Medics[i]->attack(  closestUnit->getPosition() );//heal move
				  squad.Medics[i]->move(  closestMarine->getPosition() );//regular move
				  continue;
			  }
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( squad.Medics[i]->getPosition().getDistance( squad.AttackTarget ) > 6*32 ){
			      squad.Medics[i]->move(   squad.AttackTarget );//regular move
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
