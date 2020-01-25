
#include "Common.h"
#include "ScannerManager.h"
#include <boost/foreach.hpp>


ScannerManager::ScannerManager(){

	LastScanFrame = 0;
}


void ScannerManager::CheckBaseScan(){

	if( ScoutMan->workerScouts.size() == 0
		&& InfoMan->PosEnemyBase == BWAPI::Positions::Unknown ){
			if( Scanners.size() > 0 &&  InfoMan->PossibleBase.size() > 0 ){
				if( Scanners[0]->getEnergy() >= 50 ){
					Scanners[0]->useTech( BWAPI::TechTypes::Scanner_Sweep, BWAPI::Position( InfoMan->PossibleBase[0] ) );
				}
			}
	}

}

void ScannerManager::AutoScanCloack(){


	BOOST_FOREACH( Unit cloackedE, Broodwar->enemy()->getUnits() ){
		//if( cloackedE->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar
			//|| cloackedE->getType() == BWAPI::UnitTypes::Unknown ){

		//out of bounds check
		if( !cloackedE->isDetected() && cloackedE->exists()
			&& cloackedE->getPosition().x > 0 && cloackedE->getPosition().x < Broodwar->mapWidth()*32
			&& cloackedE->getPosition().y > 0 && cloackedE->getPosition().y < Broodwar->mapHeight()*32){

				bool CombatUnitClose = false;
				int radius = 4 * 32;
				if (Broodwar->enemy()->getRace() == BWAPI::Races::Zerg) {
					radius = 7 * 32; //lurker range
				}
				Unitset UnitsClose = cloackedE->getUnitsInRadius(radius );
		        BOOST_FOREACH( Unit close, UnitsClose )
                {
			      if(  close->getType().isBuilding()  ){
				    continue;
			      }
			      if(  close->getType().isWorker()  ){
				    continue;
			      }
				  CombatUnitClose = true;
		        }

				//if( cloackedE->getUnitsInRadius( 4*32).size() > 0 ){
				if( CombatUnitClose == true){

	             for(unsigned int i=0; i<Scanners.size(); i++){
			       if( Scanners[i]->getEnergy() > 50){
			         Scanners[i]->useTech( BWAPI::TechTypes::Scanner_Sweep, cloackedE->getPosition() );
			         LastScanFrame = Broodwar->getFrameCount();
			         break;
			       }
	             }
		         break;

		       }
		}

	}



}

void ScannerManager::OnFrame(){

	if( Scanners.size() == 0 ){
		return;
	}

	for(unsigned int i=0; i<Scanners.size(); i++){
		if( !Scanners[i]->exists() ){
			Scanners.erase( Scanners.begin() + i );
			i--;
			continue;
		}
	}

	//prevent scan spam
	if( LastScanFrame + 100 > Broodwar->getFrameCount() ){
		return;
	}

	//early stage only scan close to bunker
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg &&
		MacroMan->ZState == Z_Expand_Defend ){



	BOOST_FOREACH( Unit cloackedE, Broodwar->enemy()->getUnits() ){
		//if( cloackedE->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar
			//|| cloackedE->getType() == BWAPI::UnitTypes::Unknown ){

		//out of bounds check
		if( !cloackedE->isDetected() && cloackedE->exists()
			&& cloackedE->getPosition().x > 0 && cloackedE->getPosition().x < Broodwar->mapWidth()*32
			&& cloackedE->getPosition().y > 0 && cloackedE->getPosition().y < Broodwar->mapHeight()*32){

				bool BunkerClose = false;
				Unitset UnitsClose = cloackedE->getUnitsInRadius( 6*32);
		        BOOST_FOREACH( Unit close, UnitsClose )
                {
				  if(  close->getType() == BWAPI::UnitTypes::Terran_Bunker  ){
				    BunkerClose = true;
			      }
			      
		        }

				//if( cloackedE->getUnitsInRadius( 4*32).size() > 0 ){
				if( BunkerClose == true){

	             for(unsigned int i=0; i<Scanners.size(); i++){
			       if( Scanners[i]->getEnergy() > 50){
			         Scanners[i]->useTech( BWAPI::TechTypes::Scanner_Sweep, cloackedE->getPosition() );
			         LastScanFrame = Broodwar->getFrameCount();
			         break;
			       }
	             }
		         break;

		       }
		}

	}

	} else {
		//otherwise, just scan for cloacked units
    	AutoScanCloack();
	}


	if( MacroMan->HuntT2 == true || MacroMan->HuntP == true ||  MacroMan->Hunt == true ){

	    for(unsigned int i=0; i<Scanners.size(); i++){
			if( Scanners[i]->getEnergy() >= 149){

		std::vector< Position > PossiblePos;
		BOOST_FOREACH( BWTA::BaseLocation* close, BWTA::getBaseLocations() )
        {
			PossiblePos.push_back( BWAPI::Position( close->getTilePosition() ) );
		}
		//attackPosT2 = PossiblePos[ rand() %  PossiblePos.size()  ];
		Scanners[i]->useTech( BWAPI::TechTypes::Scanner_Sweep, PossiblePos[ rand() %  PossiblePos.size()  ] );
		Broodwar->printf("Scanning random pos");

			}
	  }
	}
	//scan to make sure there are no spider mines blocking the expansion
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){

	    for(unsigned int i=0; i<Scanners.size(); i++){
			if( Scanners[i]->getEnergy() >= 199){
		     for(unsigned int j=0; j<ProdMan->BuildingsQueue.size(); j++){
			   if( ProdMan->BuildingsQueue[j].type == BWAPI::UnitTypes::Terran_Command_Center ){
				   if( ProdMan->BuildingsQueue[j].scv != NULL){
					   if( ProdMan->BuildingsQueue[j].scv->getDistance( BWAPI::Position( ProdMan->BuildingsQueue[j].buildLocation ) ) < 5*32 ){
				        Scanners[i]->useTech( BWAPI::TechTypes::Scanner_Sweep, BWAPI::Position( ProdMan->BuildingsQueue[j].buildLocation ) );
			           //LastScanFrame = Broodwar->getFrameCount();
			           break;
					   }
				   }
			  }
		     }
		   }
	    }
	}

}

void ScannerManager::AddScanner( BWAPI::Unit scanner){
	if(  scanner->getPlayer() == Broodwar->self() && scanner->getType() == BWAPI::UnitTypes::Terran_Comsat_Station ){
		Scanners.push_back( scanner );
	}
}