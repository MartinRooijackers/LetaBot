#include "Common.h"
#include "ScoutManager.h"
#include "InformationManager.h"

ScoutManager::ScoutManager( int totalSCV)
{


	directionScoutMain = 0;//no direction set

	for(int i=0; i<totalSCV; i++){
		Scout newScout;
		newScout.scv = CCmanager[0]->getBuilder();
		if(  newScout.scv == NULL){
			continue; 
		}

		addSCV( newScout.scv );

		/*
		//send scout scv to nearest base
		int bestBase = -1;
		double lowestDist = 9999.0;
		for(int j=0; j<InfoMan->PossibleBase.size(); j++){
			double curDist = InfoMan->PossibleBase[j].getDistance(  newScout.scv->getTilePosition() ) ;
			bool alreadyTaken = false;
			for(int k=0; k<workerScouts.size(); k++){
				if( workerScouts[k].TileTarget ==  InfoMan->PossibleBase[j] ){
					alreadyTaken = true;
				}
			}
			if( alreadyTaken ){
				continue;
			}
			if( lowestDist > curDist ){
				lowestDist = curDist;
				bestBase = j;
			}
		}
		//newScout.TileTarget = InfoMan->PossibleBase[i%InfoMan->PossibleBase.size()];
		if( bestBase == -1 ){
			continue;
		}
		newScout.TileTarget = InfoMan->PossibleBase[bestBase];
		newScout.Target = BWAPI::Position( newScout.TileTarget );
		newScout.scv->move( newScout.Target );

		workerScouts.push_back( newScout );
		Broodwar->printf("scout added to %d %d", newScout.Target.x(), newScout.Target.y() );
		*/


	}

	/*
	if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown && WAMan == NULL ){
		Broodwar->printf( "Base found" );
		WAMan = new WorkerAttackManager();
	}
	*/

}


BWAPI::Position ScoutManager::GetNaturalLocScout( BWAPI::Position Target )
{


	//BWTA::BaseLocation* MainSloc = BWTA::getNearestBaseLocation( Target );
	BWTA::BaseLocation* MainSloc;

	int ClosestDist = 99999;
	BOOST_FOREACH( BWTA::BaseLocation* baseloc , BWTA::getBaseLocations() ){
		if( Target.getDistance( baseloc->getPosition() ) < ClosestDist
			     ){
			ClosestDist = Target.getDistance( baseloc->getPosition() ) ;
			MainSloc = baseloc;
		}
	}

	//TODO: change to walking distance  for maps like Fortress

	//int ClosestDist = 99999;
	ClosestDist = 99999;
	BWAPI::Position toScout;
	BOOST_FOREACH( BWTA::BaseLocation* baseloc , BWTA::getBaseLocations() ){
		if( MainSloc->getPosition() != baseloc->getPosition()
			&& baseloc->getGeysers().size() > 0
			&&  !baseloc->isIsland() ){
			if( MainSloc->getGroundDistance( baseloc ) < ClosestDist){
				ClosestDist = MainSloc->getGroundDistance( baseloc );
				toScout = baseloc->getPosition();
			}
		}
	  //MainSloc->getGroundDistance
	}
	return toScout;

}

void ScoutManager::addSCV(BWAPI::Unit *unit){

	if(unit == NULL){
		return;
	}
	if( !unit->getType().isWorker() ){
		return;
	}

	Scout newScout;
	newScout.scv = unit;

	//send scout scv to nearest base
	int bestBase = -1;
	double lowestDist = 9999.0;
	for(int j=0; j<InfoMan->PossibleBase.size(); j++){
		bool alreadyAssigned = false;
		for( int i=0; i<workerScouts.size(); i++ ){
			if( workerScouts[i].TileTarget == InfoMan->PossibleBase[j] ){
				alreadyAssigned = true;
			}
		}
		if( alreadyAssigned ){
			continue;
		}
		double curDist = InfoMan->PossibleBase[j].getDistance( newScout.scv->getTilePosition() ) ;
		if( lowestDist > curDist ){
			 lowestDist = curDist;
			 bestBase = j;
		}
	}
	if( bestBase == -1 ){
		return;
	}
		//workerScouts[i].TileTarget = InfoMan->PossibleBase[ workerScouts.size() % InfoMan->PossibleBase.size()];
		newScout.TileTarget = InfoMan->PossibleBase[ bestBase ];
		 newScout.Target = BWAPI::Position( newScout.TileTarget );
		 newScout.CurrentNatural = GetNaturalLocScout( newScout.scv->getPosition() );
		 newScout.TargetNatural = GetNaturalLocScout(  newScout.Target );
		 newScout.CurrentNatScouted = false;
		 newScout.TargetNatScouted = false;

		 workerScouts.push_back( newScout );
		 Broodwar->printf( "Added SCV scout" );
}




void ScoutManager::ScoutMain( Unit* scv )
{


	if(  directionScoutMain == 0  ){
		directionScoutMain = 1;
	}


	if( BWTA::getRegion( scv->getPosition() ) != BWTA::getRegion( InfoMan->PosEnemyBase ) ){

		scv->move( InfoMan->PosEnemyBase  );
		//Broodwar->printf("Going to enemy base");

		return;
	}

	int BaseSize = 10*32;

	int Xlocation = 0;
	int Ylocation = 0;
	if(  directionScoutMain == 1  ){
		Xlocation = InfoMan->PosEnemyBase.x() + BaseSize;
		Ylocation = InfoMan->PosEnemyBase.y() + BaseSize;
	}
	if(  directionScoutMain == 2  ){
		Xlocation = InfoMan->PosEnemyBase.x() - BaseSize;
		Ylocation = InfoMan->PosEnemyBase.y() + BaseSize;
	}
	if(  directionScoutMain == 3  ){
		Xlocation = InfoMan->PosEnemyBase.x() - BaseSize;
		Ylocation = InfoMan->PosEnemyBase.y() - BaseSize;
	}
	if(  directionScoutMain == 4  ){
		Xlocation = InfoMan->PosEnemyBase.x() + BaseSize;
		Ylocation = InfoMan->PosEnemyBase.y() - BaseSize;
	}

	BWAPI::Position scoutMainPos( Xlocation, Ylocation );

	Broodwar->drawCircleMap( scoutMainPos.x() , scoutMainPos.y() ,3,Colors::Cyan ,true);

	if( scv->getDistance(  scoutMainPos ) > 2*32 ){

		scv->move( scoutMainPos  );
		return;

	} else {

		directionScoutMain++;
		if( directionScoutMain == 5 ){
			directionScoutMain = 1;
		}
	}
	/*
	int MoveSize = 9;

	if( scv->getPosition().y() >  InfoMan->PosEnemyBase.y() + 7*32 ){
		BWAPI::Position ToMove( scv->getPosition().x() , scv->getPosition().y() - MoveSize*32);
		scv->move(  ToMove  );
		return;
	}

	if( scv->getPosition().y() <  InfoMan->PosEnemyBase.y() - 4*32 ){
		BWAPI::Position ToMove( scv->getPosition().x() , scv->getPosition().y() + MoveSize*32);
		scv->move(  ToMove  );
		return;
	}

	if( scv->getPosition().x() <  InfoMan->PosEnemyBase.x() - 4*32 ){
		BWAPI::Position ToMove( scv->getPosition().x() + MoveSize*32  , scv->getPosition().y() );
		scv->move(  ToMove  );
		return;
	}

	if( scv->getPosition().x() >  InfoMan->PosEnemyBase.x() + 8*32 ){
		BWAPI::Position ToMove( scv->getPosition().x() - MoveSize*32  , scv->getPosition().y() );
		scv->move(  ToMove  );
		return;
	}
	*/



	return;
}

void ScoutManager::OnFrame(){

	if( CurrentStrategy == Worker_Rush &&  InfoMan->EnemyBase != BWAPI::TilePositions::Unknown && WAMan == NULL && Broodwar->getFrameCount() > 4 ){
		Broodwar->printf( "Base found" );
		WAMan = new WorkerAttackManager();
		return;
	}

	// InfoMan->PossibleBase.size() == 1
	if(   InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){//Base found by process of elimination

	    for(unsigned int i=0; i<workerScouts.size(); i++){

			if( i == 0){
			  ScoutMain( workerScouts[i].scv );
			} else {

			//if( !workerScouts[i].scv->isAttacking() ){
			  if( workerScouts[i].scv->getOrder() != BWAPI::Orders::AttackMove
			  	  && workerScouts[i].scv->getOrder() != BWAPI::Orders::AttackUnit
				  && workerScouts[i].scv->getOrder() != BWAPI::Orders::AttackTile ){
			      workerScouts[i].scv->attack( InfoMan->PosEnemyBase );
			  }
			}

		}
		return;

	}
	
	for(unsigned int i=0; i<workerScouts.size(); i++){

		Broodwar->drawCircleMap( workerScouts[i].CurrentNatural.x() ,workerScouts[i].CurrentNatural.y(), 6,BWAPI::Colors::Cyan, true );
		Broodwar->drawCircleMap( workerScouts[i].TargetNatural.x() ,workerScouts[i].TargetNatural.y(), 6,BWAPI::Colors::Orange, true  );
		Broodwar->drawCircleMap( workerScouts[i].Target.x() ,workerScouts[i].Target.y(), 6,BWAPI::Colors::Yellow, true  );


		if( workerScouts[i].CurrentNatScouted == false){
			if( workerScouts[i].scv->getDistance( workerScouts[i].CurrentNatural ) > 3*32 ){
					workerScouts[i].scv->move( workerScouts[i].CurrentNatural );
			}
			if( workerScouts[i].scv->getDistance( workerScouts[i].CurrentNatural ) < 3*32 ){
					workerScouts[i].CurrentNatScouted = true;
			}
		}
		if( workerScouts[i].CurrentNatScouted == true &&  workerScouts[i].TargetNatScouted == false ){
			if( workerScouts[i].scv->getDistance( workerScouts[i].TargetNatural ) > 5*32 ){
					workerScouts[i].scv->move( workerScouts[i].TargetNatural );
			}
			if( workerScouts[i].scv->getDistance( workerScouts[i].TargetNatural ) < 5*32 ){
					workerScouts[i].TargetNatScouted = true;
			}
		}

		if( workerScouts[i].CurrentNatScouted == true &&  workerScouts[i].TargetNatScouted == true ){
		    workerScouts[i].scv->move(workerScouts[i].Target);
		}

		//workerScouts[i].scv->move(workerScouts[i].Target);
		//Broodwar->drawBox(CoordinateType::Map,workerScouts[i].Target.x(),workerScouts[i].Target.y(),workerScouts[i].Target.x()+4*32,workerScouts[i].Target.y()+4*32,Colors::Orange,true);
		//Broodwar->drawBox(CoordinateType::Map,100,100,110,110,Colors::Orange,true);

		//if the SCV is destroyed, its target is the target location
		if( !workerScouts[i].scv->exists() ){
			InfoMan->setLocationEnemyBase( workerScouts[i].TileTarget );
			break;
		}

		if( Broodwar->isVisible( workerScouts[i].TileTarget ) ){
			bool depot = false;
			//Broodwar->enemy()->getUnits()
			BOOST_FOREACH(BWAPI::Unit*  enemy,  Broodwar->enemy()->getUnits() ) {
				if( enemy->getType().isResourceDepot() && enemy->getTilePosition() ==  workerScouts[i].TileTarget ){
					depot = true;
				}
			}
            if( !depot ){
				Broodwar->printf( "No base here" );
				InfoMan->NoBaseHere( workerScouts[i].TileTarget );
				if(  InfoMan->PossibleBase.size() > 0 ){

				  //send scout scv to nearest base
				  int bestBase = -1;
				  double lowestDist = 9999.0;
				  for(int j=0; j<InfoMan->PossibleBase.size(); j++){
					  double curDist = InfoMan->PossibleBase[j].getDistance(  workerScouts[i].scv->getTilePosition() ) ;
					  if( lowestDist > curDist ){
					  	  lowestDist = curDist;
				 		  bestBase = j;
					  }
				  }

				  //workerScouts[i].TileTarget = InfoMan->PossibleBase[ workerScouts.size() % InfoMan->PossibleBase.size()];
				  workerScouts[i].TileTarget = InfoMan->PossibleBase[ bestBase ];
				  workerScouts[i].Target = BWAPI::Position( workerScouts[i].TileTarget );

		          workerScouts[i].CurrentNatural = GetNaturalLocScout( workerScouts[i].scv->getPosition() );
		          workerScouts[i].TargetNatural = GetNaturalLocScout(  workerScouts[i].Target );
		          workerScouts[i].CurrentNatScouted = false;
		          workerScouts[i].TargetNatScouted = false;

				  
				  //on 4 player maps, add an extra scout scv after the first base scout vs Zerg
				  if(  InfoMan->PossibleBase.size() > 1 && workerScouts.size() < 2 
					  && (Broodwar->enemy()->getRace() == BWAPI::Races::Zerg 
					  || Broodwar->enemy()->getRace() == BWAPI::Races::Unknown 
					  || Broodwar->enemy()->getRace() == BWAPI::Races::Random)
					  
					  ){
					  addSCV( CCmanager[0]->getBuilder() );
				  }
				  

			    }
			} 
			else {
			  //InfoMan->EnemyBase = workerScouts[i].TileTarget;
			  InfoMan->setLocationEnemyBase( workerScouts[i].TileTarget );
			}
			if(  InfoMan->PossibleBase.size() == 1){//Base found by process of elimination
				//InfoMan->EnemyBase =  InfoMan->PossibleBase[0];
				InfoMan->setLocationEnemyBase( InfoMan->PossibleBase[0] );
			}
		}
	}

	
}