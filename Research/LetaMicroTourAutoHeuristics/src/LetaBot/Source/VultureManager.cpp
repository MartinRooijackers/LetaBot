#include "VultureManager.h"
#include "Common.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <algorithm>
#include <boost/foreach.hpp>


VultureManager::VultureManager()
{

	
	BOOST_FOREACH( Unit* my, Broodwar->self()->getUnits()){
		AddVulture( my );
	}
	




}


void VultureManager::OnFrame(){


	BOOST_FOREACH( Unit* my, Broodwar->self()->getUnits()){
		AddVulture( my );
	}

	for(unsigned int i=0; i<Vultures.size(); i++){
		//remove dead marines
		if( !Vultures[i].vulture->exists() ){
			Vultures.erase(Vultures.begin() + i );
			i--;
			continue;
		}
	}


	////////////////////////////////////////////////////////////////
    // Spider mine trick map									////
	////////////////////////////////////////////////////////////////

	/*
	//drop mines
	for(int i=0; i<Vultures.size(); i++){
		if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine ){
		  Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, Vultures[i].vulture->getPosition() );
		}

	}
	*/
	


	
	/*
	//drop mines
	for(int i=0; i<Vultures.size(); i++){
		if( Vultures[i].vulture->getSpiderMineCount() != Vultures[i].mineCount ){
			Vultures[i].mineCount = Vultures[i].vulture->getSpiderMineCount();
			Vultures[i].mineFrame = Broodwar->getFrameCount();
		}

		if( Vultures[i].vulture->getSpiderMineCount() == 3){
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine ){
		    Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, Vultures[i].FirstMine );
		  }
		} else {
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine
			   &&  Broodwar->getFrameCount() > (Vultures[i].mineFrame + 5) ){
		    Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, Vultures[i].vulture->getPosition() );
		  }
		}
	}
	*/

	/*
	//drop mines random
	for(unsigned int i=0; i<Vultures.size(); i++){
		if( Vultures[i].vulture->getSpiderMineCount() != Vultures[i].mineCount ){
			Vultures[i].mineCount = Vultures[i].vulture->getSpiderMineCount();
			Vultures[i].mineFrame = Broodwar->getFrameCount();
		}

		if( Vultures[i].vulture->getSpiderMineCount() == 3){
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine ){
		    Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, Vultures[i].FirstMine );
		  }
		} else {
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine 
			  && Broodwar->getFrameCount() > (Vultures[i].mineFrame + 2) ){
			int disX = (rand() % 10) - 5;
			int disY = (rand() % 10) - 5;
			BWAPI::Position randM( Vultures[i].vulture->getPosition().x() + disX , Vultures[i].vulture->getPosition().y() + disY );
		    Vultures[i].curMine = randM;
			Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, randM );
			Vultures[i].mineFrame = Broodwar->getFrameCount();
		  }
		}
		Broodwar->drawCircleMap( Vultures[i].curMine.x() , Vultures[i].curMine.y(), 2 , BWAPI::Colors::Purple, true);
	}
	*/

	/*
	//drop mines random
	for(unsigned int i=0; i<Vultures.size(); i++){
		if( Vultures[i].vulture->getSpiderMineCount() != Vultures[i].mineCount ){
			Vultures[i].mineCount = Vultures[i].vulture->getSpiderMineCount();
			Vultures[i].mineFrame = Broodwar->getFrameCount();
			Vultures[i].orderGiven = false;
		}

		if( Vultures[i].vulture->getSpiderMineCount() == 3){
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine ){
		    Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, Vultures[i].FirstMine );
			continue;
		  }
		} else {
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine 
			  && Broodwar->getFrameCount() > (Vultures[i].mineFrame + 6) ){
				  Unit* closestMine = NULL;
				  double minDist = 99999.0;
				  BOOST_FOREACH( Unit* mine, Broodwar->self()->getUnits() ){
					  if( mine->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine
						  && mine->getDistance( Vultures[i].vulture ) < minDist){
						  minDist = mine->getDistance( Vultures[i].vulture );
						  closestMine = mine;
					  }
				  }
			int disX = 9;
			int disY = 9;
			if( closestMine->getPosition().x() == Vultures[i].vulture->getPosition().x() ){
				disX = 0;
			}
			if( closestMine->getPosition().y() == Vultures[i].vulture->getPosition().y() ){
				disY = 0;
			}
			if( closestMine->getPosition().x() > Vultures[i].vulture->getPosition().x() ){
				disX = -9;
			}
			if( closestMine->getPosition().y() > Vultures[i].vulture->getPosition().y() ){
				disY = -9;
			}
			//BWAPI::Position randM( Vultures[i].vulture->getPosition().x() + disX , Vultures[i].vulture->getPosition().y() + disY );
			BWAPI::Position randM( closestMine->getPosition().x() + disX , closestMine->getPosition().y() + disY );
		    Vultures[i].curMine = randM;
			Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, randM );
			Vultures[i].mineFrame = Broodwar->getFrameCount();
			Vultures[i].orderGiven = true;
		  }




		  //location of next mine drop known
		  if( Vultures[i].curMine != BWAPI::Positions::None && Broodwar->getFrameCount() > (Vultures[i].mineFrame + 10) ){


				  Unit* closestMine = NULL;
				  double minDist = 99999.0;
				  BOOST_FOREACH( Unit* mine, Broodwar->self()->getUnits() ){
					  if( mine->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine
						  && mine->getDistance( Vultures[i].vulture ) < minDist){
						  minDist = mine->getDistance( Vultures[i].vulture );
						  closestMine = mine;
					  }
				  }
			int disX = 9;
			int disY = 9;
			if( closestMine->getPosition().x() == Vultures[i].vulture->getPosition().x() ){
				disX = 0;
			}
			if( closestMine->getPosition().y() == Vultures[i].vulture->getPosition().y() ){
				disY = 0;
			}
			if( closestMine->getPosition().x() > Vultures[i].vulture->getPosition().x() ){
				disX = -9;
			}
			if( closestMine->getPosition().y() > Vultures[i].vulture->getPosition().y() ){
				disY = -9;
			}
			//BWAPI::Position randM( Vultures[i].vulture->getPosition().x() + disX , Vultures[i].vulture->getPosition().y() + disY );
			BWAPI::Position randM( closestMine->getPosition().x() + disX , closestMine->getPosition().y() + disY );
			if( Vultures[i].curMine.x() != randM.x() ||  Vultures[i].curMine.y() != randM.y() ){
		      Vultures[i].curMine = randM;
			  Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, randM );
			  Vultures[i].mineFrame = Broodwar->getFrameCount();
			  Vultures[i].orderGiven = true;
			  Broodwar->printf("repositioning mine pos");
			}
			  
		  }




		}
		Broodwar->drawCircleMap( Vultures[i].curMine.x() , Vultures[i].curMine.y(), 2 , BWAPI::Colors::Yellow, true);
	}
	*/


	////////////////////////////////////////////////////////////////
    // End of Spider mine trick map code						////
	////////////////////////////////////////////////////////////////




	////////////////////////////////////////////////////////////////
    // Become SkyHigh									        ////
	////////////////////////////////////////////////////////////////


	

	Unit* DL = NULL;
	Unit* DR = NULL;
	Unit* DU = NULL;
	Unit* DD = NULL;

	//wait for new dragoons to spawn
	if(  Vultures.size() == 0 ){
		return;
	}


	BOOST_FOREACH( Unit* dragoon, Broodwar->enemy()->getUnits() ){
		if( dragoon->getType() != BWAPI::UnitTypes::Protoss_Dragoon){
			continue;
		}
		if( Vultures[0].vulture->getDistance( dragoon) > 11*32){
			continue;
		}
	  if( DL == NULL){
		  DL = dragoon;
	  } else if( DL->getPosition().x() > dragoon->getPosition().x() ){
		  DL = dragoon;
	  }
	  if( DR == NULL){
		  DR = dragoon;
	  } else if( DR->getPosition().x() < dragoon->getPosition().x() ){
		  DR = dragoon;
	  }
	  if( DU == NULL){
		  DU = dragoon;
	  } else if( DU->getPosition().y() > dragoon->getPosition().y() ){
		  DU = dragoon;
	  }
	  if( DD == NULL){
		  DD = dragoon;
	  } else if( DD->getPosition().y() < dragoon->getPosition().y() ){
		  DD = dragoon;
	  }


	}

	if( DL == NULL || DR == NULL || DU == NULL || DD == NULL){
	    for(unsigned int i=0; i<Vultures.size(); i++){
			Vultures[i].vulture->move( BWAPI::Position( Vultures[i].vulture->getPosition().x() + 64, Vultures[i].vulture->getPosition().y() ) );
	    }
		return;
	}


		int Lcount = 0;
		int UCount = 0;
		int Dcount = 0;
		int Rcount = 0;

	//drop mines random
	for(unsigned int i=0; i<Vultures.size(); i++){
		if( Vultures[i].vulture->getSpiderMineCount() != Vultures[i].mineCount ){
			Vultures[i].mineCount = Vultures[i].vulture->getSpiderMineCount();
			Vultures[i].mineFrame = Broodwar->getFrameCount();
			Vultures[i].orderGiven = false;
		}


		//move out of the way of spider mines
		if( Vultures[i].vulture->getSpiderMineCount() == 0){
			if( Vultures[i].DragoonAttack == 0 && Vultures[i].vulture->getDistance(DL) < 4*32 ){
				BWAPI::Position moveTo( DL->getPosition().x() - 33*4, DL->getPosition().y()); 
				Vultures[i].vulture->move(moveTo);
				continue;
			}
			if( Vultures[i].DragoonAttack == 3 && Vultures[i].vulture->getDistance(DR) < 4*32 ){
				BWAPI::Position moveTo( DR->getPosition().x() + 33*4, DR->getPosition().y()); 
				Vultures[i].vulture->move(moveTo);
				continue;
			}
		}


		if( Vultures[i].vulture->getSpiderMineCount() == 3
			|| ( Vultures[i].vulture->getSpiderMineCount() == 1 &&  Vultures[i].DragoonAttack == 3 ) ){
			//calculate first mine location
			BWAPI::Position newFM;
			//int sep = 64;
			int DSep = 75;
			int Startint = 0;
			if( Vultures[i].DragoonAttack == 0){
				int sep = 64;
				if( Vultures[i].CodeNummer % 2 == 0 ){ //even
					sep = 64 * ( Vultures[i].CodeNummer/2 );
				} else {
					sep = 64 * -( (Vultures[i].CodeNummer+1)/2 );
				}

				newFM = BWAPI::Position( DL->getPosition().x() - DSep, DL->getPosition().y() + Startint - sep ); 
				Lcount++;
			}
			if( Vultures[i].DragoonAttack == 1){
				int sep = 64;
				if( Vultures[i].CodeNummer % 2 == 0 ){ //even
					sep = 64 * ( Vultures[i].CodeNummer/2 );
				} else {
					sep = 64 * -( (Vultures[i].CodeNummer+1)/2 );
				}

				newFM = BWAPI::Position( DU->getPosition().x() + Startint - sep, DU->getPosition().y() - DSep ); 
				UCount++;
			}
			if( Vultures[i].DragoonAttack == 2){
				int sep = 64;
				if( Vultures[i].CodeNummer % 2 == 0 ){ //even
					sep = 64 * ( Vultures[i].CodeNummer/2 );
				} else {
					sep = 64 * -( (Vultures[i].CodeNummer+1)/2 );
				}

				newFM = BWAPI::Position( DD->getPosition().x() + Startint - sep, DD->getPosition().y() + DSep ); 
				Dcount++;
			}
			if( Vultures[i].DragoonAttack == 3){
				int sep = 64;
				if( Vultures[i].CodeNummer % 2 == 0 ){ //even
					sep = 64 * ( Vultures[i].CodeNummer/2 );
				} else {
					sep = 64 * -( (Vultures[i].CodeNummer+1)/2 );
				}

				newFM = BWAPI::Position( DR->getPosition().x() + DSep, DR->getPosition().y() + Startint - sep ); 
				Rcount++;
			}

		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine ||  Vultures[i].FirstMine != newFM  ){
			  Vultures[i].FirstMine = newFM;
		      Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, Vultures[i].FirstMine );
			  continue;
		  }
		  Broodwar->drawCircleMap( Vultures[i].FirstMine.x() , Vultures[i].FirstMine.y(), 3 , BWAPI::Colors::Red, true);
		} else if(  Vultures[i].vulture->getSpiderMineCount() == 1 &&  
			         ( Vultures[i].DragoonAttack == 1 || Vultures[i].DragoonAttack == 2 )  ){ //switch to right side
		   Vultures[i].CodeNummer = 0;
	       for(unsigned int i=0; i<Vultures.size(); i++){
		     if( Vultures[i].DragoonAttack == 3 ){
			   Vultures[i].CodeNummer++;
		     }
	       }
		   Vultures[i].DragoonAttack = 3;

		} else {
		  if( Vultures[i].vulture->getOrder() != BWAPI::Orders::PlaceMine 
			  && Broodwar->getFrameCount() > (Vultures[i].mineFrame + 6) ){
				  Unit* closestMine = NULL;
				  double minDist = 99999.0;
				  BOOST_FOREACH( Unit* mine, Broodwar->self()->getUnits() ){
					  if( mine->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine
						  && mine->getDistance( Vultures[i].vulture ) < minDist){
						  minDist = mine->getDistance( Vultures[i].vulture );
						  closestMine = mine;
					  }
				  }
			int disX = 9;
			int disY = 9;
			if( closestMine == NULL ){
				continue;
			}
			if( closestMine->getPosition().x() == Vultures[i].vulture->getPosition().x() ){
				disX = 0;
			}
			if( closestMine->getPosition().y() == Vultures[i].vulture->getPosition().y() ){
				disY = 0;
			}
			if( closestMine->getPosition().x() > Vultures[i].vulture->getPosition().x() ){
				disX = -9;
			}
			if( closestMine->getPosition().y() > Vultures[i].vulture->getPosition().y() ){
				disY = -9;
			}
			//BWAPI::Position randM( Vultures[i].vulture->getPosition().x() + disX , Vultures[i].vulture->getPosition().y() + disY );
			BWAPI::Position randM( closestMine->getPosition().x() + disX , closestMine->getPosition().y() + disY );
		    Vultures[i].curMine = randM;
			Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, randM );
			Vultures[i].mineFrame = Broodwar->getFrameCount();
			Vultures[i].orderGiven = true;
		  }




		  //location of next mine drop known
		  if( Vultures[i].curMine != BWAPI::Positions::None && Broodwar->getFrameCount() > (Vultures[i].mineFrame + 10) ){


				  Unit* closestMine = NULL;
				  double minDist = 99999.0;
				  BOOST_FOREACH( Unit* mine, Broodwar->self()->getUnits() ){
					  if( mine->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine
						  && mine->getDistance( Vultures[i].vulture ) < minDist){
						  minDist = mine->getDistance( Vultures[i].vulture );
						  closestMine = mine;
					  }
				  }
			int disX = 9;
			int disY = 9;
			if( closestMine == NULL ){
				continue;
			}
			if( closestMine->getPosition().x() == Vultures[i].vulture->getPosition().x() ){
				disX = 0;
			}
			if( closestMine->getPosition().y() == Vultures[i].vulture->getPosition().y() ){
				disY = 0;
			}
			if( closestMine->getPosition().x() > Vultures[i].vulture->getPosition().x() ){
				disX = -9;
			}
			if( closestMine->getPosition().y() > Vultures[i].vulture->getPosition().y() ){
				disY = -9;
			}
			//BWAPI::Position randM( Vultures[i].vulture->getPosition().x() + disX , Vultures[i].vulture->getPosition().y() + disY );
			BWAPI::Position randM( closestMine->getPosition().x() + disX , closestMine->getPosition().y() + disY );
			if( Vultures[i].curMine.x() != randM.x() ||  Vultures[i].curMine.y() != randM.y() ){
		      Vultures[i].curMine = randM;
			  Vultures[i].vulture->useTech( BWAPI::TechTypes::Spider_Mines, randM );
			  Vultures[i].mineFrame = Broodwar->getFrameCount();
			  Vultures[i].orderGiven = true;
			  Broodwar->printf("repositioning mine pos");
			}
			  
		  }




		}
		Broodwar->drawCircleMap( Vultures[i].curMine.x() , Vultures[i].curMine.y(), 3 , BWAPI::Colors::Yellow, true);
	}

	

	////////////////////////////////////////////////////////////////
    // No longer SkyHigh									    ////
	////////////////////////////////////////////////////////////////




}


void VultureManager::AddVulture( Unit* unit){

	//extra check
	for(unsigned int i=0; i<Vultures.size(); i++){

		if( unit->getID() == Vultures[i].vulture->getID() ){
			return;
		}
	}


	if( unit->getType() == BWAPI::UnitTypes::Terran_Vulture ){

		Vulture newV;
		newV.vulture = unit;
		newV.FirstMine = BWAPI::Position( unit->getPosition().x() + 64 , unit->getPosition().y() );
		newV.mineCount = 3;
		newV.mineFrame = 0;
		newV.curMine = BWAPI::Positions::None;
		//if( Vultures.size() % 3 == 0 ){
		newV.DragoonAttack = Vultures.size() % 3;
		//}
		newV.CodeNummer = 0;
	    for(unsigned int i=0; i<Vultures.size(); i++){
		  if( newV.DragoonAttack == Vultures[i].DragoonAttack ){
			newV.CodeNummer++;
		  }
	    }


	    Vultures.push_back( newV );
	}


}