#include "MacroManager.h"
#include "Common.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <algorithm>
#include <boost/foreach.hpp>




MacroManager::MacroManager(){


	BWAPI::Position ClosestTankProtoss = InfoMan->PosOurBase;
	DistTankProtoss = 999999999;
	AllowSDestroy = false;

	PState = P_Wall;
	ZState = Z_Init;
	TState = T_Choke_Defend;

	EarlyPool = false;
	NatBunkerChoke = NULL;
	NatBunkerPlace = false;
	NatLurkerTurretPlace = false;
	MutaBuild = false;

	ZealotRushExpected = false;
	ZealotRushBunker = false; // build early bunker in case of zealot rush

	TurretPlaced = false;

	WallBunkerPlaced = false;

	FloatingBarracks = NULL;
	ETankPositions.clear();

	ELurkerPositions.clear();

	MacroSquad defenceSquad;
	defenceSquad.AttackBuilding = NULL;
	defenceSquad.AttackTarget = BWAPI::Positions::None;
	defenceSquad.SQstate = SQ_Defend_Base;
	Squads.push_back( defenceSquad );

	currentBuilding = NULL;
	attackPos == BWAPI::Positions::None;
	Hunt = false;
	HuntAllowed = false;

	currentBuildingP = NULL;
	attackPosP == BWAPI::Positions::None;
	HuntP = false;
	HuntAllowedP = false;

	NoNeedTank = false;
	EnemyTerranHasBunker = false;
	
	//TODO: fix the nrrow choke pathfinding in blue storm
	if( 
		Broodwar->mapHash() == "aab66dbf9c85f85c47c219277e1e36181fe5f9fc"  ){
		NoNeedTank = true;
	}
	

	currentBuildingT2 = NULL;
	attackPosT2 == BWAPI::Positions::None;
	HuntT2 = false;
	HuntAllowedT2 = false;

	TankClose = false;
	StartAttackFrame = 999999;
	/*
	//FollowupP.push_back( BWAPI::UnitTypes::Terran_Academy);
	FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory);
	FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
	FollowupP.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay);
	FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
	FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
	FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
	*/

	//FollowupZ.push_back(BWAPI::UnitTypes::Terran_Supply_Depot);
	//FollowupZ.push_back(BWAPI::UnitTypes::Terran_Refinery);
	//FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory);
	//FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory);
	//FollowupZ.push_back(BWAPI::UnitTypes::Terran_Command_Center);
	//FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory);
    //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory);
	//FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);


	BWAPI::Position natPos = BWAPI::Position(InfoMan->OurNat);
	BWAPI::Position mainPos = BWAPI::Position(InfoMan->PosOurBase);

	BWTA::Region* natRegion = BWTA::getRegion(  BWAPI::TilePosition (natPos )  ); 
	BWTA::Region* mainRegion = BWTA::getRegion(BWAPI::TilePosition( mainPos )  );

	BWTA::Chokepoint* BunkerChoke = NULL;
   BOOST_FOREACH( BWTA::Chokepoint*  choke, BWTA::getChokepoints() ){
	   if( (choke->getRegions().first == natRegion && choke->getRegions().second == mainRegion) ||
		   (choke->getRegions().second == natRegion && choke->getRegions().first == mainRegion) ){
           BunkerChoke = choke;
	   }
   }
   //chokePos = BunkerChoke->getCenter();
   chokePos = BWAPI::Position( bManager->getBuildLocationNear( BWAPI::TilePosition( InfoMan->PosMainChoke ), BWAPI::UnitTypes::Terran_Supply_Depot, InfoMan->OurMainregion ) );


   CurrentStrat = "Siege Expand";
   if( InitialStrategy == Macro_Strat) {
     ProdMan->BuildingSupplyOrder.clear();

	 if( Broodwar->enemy()->getRace() == BWAPI::Races::Random
		 || Broodwar->enemy()->getRace() == BWAPI::Races::Unknown ){
      CurrentStrat = "Init Random";
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 12);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	 }

     //build orders
	 if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){

		 CurrentStrat = "Standard Flowchart";



		 if (CurrentStrat == "Standard Flowchart") {
			 ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 9);
			 ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks, 10);
			 ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 13);
			 ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Command_Center, 20);


		 }


      //CurrentStrat = "2 rax FE";

		if(  CurrentStrat == "1 Rax Gas"){	
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 12);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);

	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 17);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 20);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Armory, 22);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 24);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 24);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 28);

		}

		if(  CurrentStrat == "Sparks Terran"){	
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 13);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 13);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 20);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 24);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 27);


		}


		/*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 13);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 13);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Bunker, 15);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 16);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 20);
		  
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 21);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 23);		
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 23);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */

		if(  CurrentStrat == "2 rax FE"){	
			ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 13);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 13);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Bunker, 15);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 16);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 20);
		  
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 23);
		  /*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 23);		
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */

		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Science_Facility, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);

		  /*
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Science_Facility, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */



		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Academy);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Refinery);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Engineering_Bay);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Supply_Depot);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Academy);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory); //earlier to stop lurkers
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Starport);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Armory);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Starport);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Science_Facility);

		}
		if(  CurrentStrat == "Goliath M&M"){	
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 12);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 17);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 20);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Armory, 22);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 24);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 28);


		  FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Starport);
		  //FollowupZ.push_back(BWAPI::UnitTypes::Terran_Science_Facility);

		}
		if(  CurrentStrat == "2 Port Wraith"){		
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 12);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 13);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 22);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 22);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 30);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 31);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 32);
		  /*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 19);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 21);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */
		}

	  }
	  if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){

		  // tscmooP,  Bereaver 

		  CurrentStrat = "Siege Expand";
		  //CurrentStrat = "OneFact3Rax";

	     //boost::algorithm::to_lower(str);
	     std::string lowerCaseName = Broodwar->enemy()->getName();
	     std::transform(lowerCaseName.begin(), lowerCaseName.end(), lowerCaseName.begin(), ::tolower);

/*
	if( lowerCaseName == "tscmoop"
		|| lowerCaseName == "bereaver"
		|| lowerCaseName == "tomas vajda"
		|| lowerCaseName == "tomasvajda"

		|| Broodwar->enemy()->getName() == "Bereaver" 
        || Broodwar->enemy()->getName() == "tscmoop"
		|| Broodwar->enemy()->getName() == "Tomas Vajda"
		|| Broodwar->enemy()->getName() == "TomasVajda"



		){
	  CurrentStrat = "OneFact3Rax";


	}
	*/



		  if(  CurrentStrat == "Siege Expand"){
			  Broodwar->printf("Using Siege expand");
			  //use the same build order as the SSCAI 2015 version. Ceteris paribus
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 20);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 21);

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery );

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );

		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Academy);
		  //skip turret ring
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);

		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Starport );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Starport );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Starport );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );

			  /*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 21);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 20);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 21);

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery );

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Academy);
		  //skip turret ring
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Starport );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  */
		  /*
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  */

		  }


		  //FollowupP.push_back(BWAPI::UnitTypes::Terran_Academy);


	  //}



		  if(  CurrentStrat == "OneFact3Rax"){

			  Broodwar->printf("Using 1 fact 3 rax");

			  /*
	  addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	  addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	  addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 12);	
	  addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
	  addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	  addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 21);
	  addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
	  */
			  //use the same build order as the SSCAI 2015 version. Ceteris paribus
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 20);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 21);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);


		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);

			  /*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 21);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 20);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 21);

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery );

		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Academy);
		  //skip turret ring
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Starport );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Factory );
		  */
		  /*
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  */

		  }


		  //FollowupP.push_back(BWAPI::UnitTypes::Terran_Academy);


	  }



	  if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
		  CurrentStrat = "3 fact vult";
		  //CurrentStrat = "2 port wraith";
		  if(  CurrentStrat == "3 fact vult"){
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 18);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 20);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 22);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Armory, 36);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 30);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 32);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
			//ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Armory, 36);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
		    //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 32);
			FollowupT.push_back(BWAPI::UnitTypes::Terran_Factory);
			FollowupT.push_back(BWAPI::UnitTypes::Terran_Factory);
	        //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
		    //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 32);
			FollowupT.push_back(BWAPI::UnitTypes::Terran_Command_Center);

			//FollowupT.push_back( BWAPI::UnitTypes::t );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Armory );
			FollowupT.push_back(BWAPI::UnitTypes::Terran_Factory);
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Academy );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Science_Facility );


		  }



		  if(  CurrentStrat == "2 rax Tank Push"){
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 22);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 15);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 18);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 22);
			
			//ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 32);
			//ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 32);
	        //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
		    //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 32);
	        //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
			//ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Science_Facility, 36);


			//FollowupT.push_back( BWAPI::UnitTypes::t );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Armory );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );

			FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Science_Facility );


		  }


		  if(  CurrentStrat == "2 port wraith"){
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 18);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 22);
			ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 32);
			ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Starport, 32);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 32);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
			ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Science_Facility, 36);


			//FollowupT.push_back( BWAPI::UnitTypes::t );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Armory );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Academy );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Barracks );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );
			FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );


		  }



	  }
   }




}


void MacroManager::OnFramePushOut(){

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss){
	  //PState = P_Push;
	  //if( CCmanager.size() > 1){
	  
	  //}
	  //Squads[0].SQstate = SQ_Attack_Base;
		PState = P_Defend_Nat;
		//currentBuilding = NULL;
		//attackPos == BWAPI::Positions::None;

	}
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		ZState = Z_Expand_Defend;
	}
}



//find the next building to attack
void MacroManager::NextBuildingMM()
{
  if( currentBuilding == NULL ){

	  attackPos = InfoMan->PosEnemyBase;
	  //Broodwar->enemy()->getUnits();
	  
	  BOOST_FOREACH(  Unit enemyB, Broodwar->enemy()->getUnits() ){
		  if(  enemyB->getType().isResourceDepot() 
			  &&  enemyB->getPosition().getDistance( InfoMan->PosEnemyBase ) < 100 ){
			  currentBuilding = enemyB;
			  attackPos = enemyB->getPosition();
			  Broodwar->printf("Enemy base set");
		  }
	  }
	  
  } else {
	  if( InfoMan->EnemyBuildings.size() > 0 ){
	    currentBuilding = InfoMan->EnemyBuildings[0].building;
		attackPos = InfoMan->EnemyBuildings[0].position;
		Broodwar->printf("Next building to attack");
	  } else {
		  attackPos = BWAPI::Positions::None;
	  }
  }

}

//find the next building to attack
void MacroManager::NextBuildingMMp()
{
 
	  if( InfoMan->EnemyBuildings.size() > 0 ){
	    currentBuildingP = InfoMan->EnemyBuildings[0].building;
		attackPosP = InfoMan->EnemyBuildings[0].position;
		Broodwar->printf("Next building to attack");
	  } else {
		  attackPosP = BWAPI::Positions::None;
	  }
  

}

//find the next building to attack
void MacroManager::NextBuildingMMt()
{
 /*
	for( int i=0; i<InfoMan->EnemyBuildings.size(); i++ ){
		if( !InfoMan->EnemyBuildings[i].type.isAddon() ){
	    currentBuildingT2 = InfoMan->EnemyBuildings[i].building;
		attackPosT2 = InfoMan->EnemyBuildings[i].position;
		Broodwar->printf("Next building to attack");
		return;
		}
	}
	attackPosT2 = BWAPI::Positions::None;
	return;
	*/

	
	  if( InfoMan->EnemyBuildings.size() > 0 ){
	    currentBuildingT2 = InfoMan->EnemyBuildings[0].building;
		attackPosT2 = InfoMan->EnemyBuildings[0].position;
		Broodwar->printf("Next building to attack");
	  } else {
		  attackPosT2 = BWAPI::Positions::None;
	  }
  


}




void MacroManager::OnFrameProtossSquadDefenceExpansion(int id){

	/*
	Broodwar->drawTextScreen( 300,110, " Enemy buildings: %d", InfoMan->EnemyBuildings.size());
	Broodwar->drawTextScreen( 300,120, " HuntP: %d", HuntP);
	Broodwar->drawTextScreen( 300,130, " HuntAllowedP: %d", HuntAllowedP);		
	*/


	BWAPI::Position defencePosP = BWAPI::Positions::Unknown;

	BWAPI::TilePosition CCtoDef = BWAPI::TilePositions::Unknown;

	//defend an expansion building
	if( ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) ){

		for( int i=0; i<ProdMan->BuildingsQueue.size(); i++){
			if( ProdMan->BuildingsQueue[i].type == BWAPI::UnitTypes::Terran_Command_Center ){
				CCtoDef = ProdMan->BuildingsQueue[i].buildLocation;
			}

		}

	} else {
		if( CCmanager.size() > 0 ){
		  CCtoDef = CCmanager[ CCmanager.size() - 1 ]->LandLocation;
		}

	}

	BWTA::Region* toDefRegion = BWTA::getRegion( CCtoDef );
		
	int furthest = 0;
	BWTA::Chokepoint* furthersChoke = NULL;
	BOOST_FOREACH( BWTA::Chokepoint* choke , toDefRegion->getChokepoints() ){
		if( choke->getCenter().getDistance( InfoMan->PosOurBase ) > furthest ){
			furthest = choke->getCenter().getDistance( InfoMan->PosOurBase );
			furthersChoke = choke;
		}
	}

	if( furthersChoke != NULL ){
		defencePosP = furthersChoke->getCenter();
	}

	Broodwar->drawCircleMap( defencePosP.x, defencePosP.y, 5 , BWAPI::Colors::Orange,true);

	/*
	if( attackPosP != BWAPI::Positions::Invalid &&  attackPosP != BWAPI::Positions::None &&  attackPosP != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( attackPosP.x(), attackPosP.y(), 5 , BWAPI::Colors::Purple,true);
	}
	*/


	for(unsigned int i=0; i<Squads[id].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[id].Marines[i]->getUnitsInRadius( 20*32 );
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
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !Squads[id].Marines[i]->isStimmed() 
			&& Squads[id].Marines[i]->getHitPoints() == 40  && closeDist < 5*32 ){
			 Squads[id].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[id].Marines[i]->getPosition() ) < 6*32 ){
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
		if ( ( closeDist < 4*32 &&  Squads[id].Marines[i]->getGroundWeaponCooldown() != 0  && !Squads[id].Marines[i]->isStartingAttack()  )
			||  CloseStorm == true
			|| (Squads[id].Marines[i]->getHitPoints() < 25 && closest != NULL &&  Squads[id].Marines[i]->getGroundWeaponCooldown() != 0 ) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[id].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[id].Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[id].Marines[i]->getPosition().getDistance( defencePosP ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[id].Marines[i]->getOrderTarget() == NULL ){
                        Squads[id].Marines[i]->attack(  closest );
					  }
					  else if( Squads[id].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[id].Marines[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[id].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[id].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[id].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[id].Marines[i]->attack(  defencePosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}


	  
	}



	for(unsigned int i=0; i<Squads[id].FireBats.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[id].FireBats[i]->getUnitsInRadius( 20*32 );
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
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].FireBats[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].FireBats[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !Squads[id].FireBats[i]->isStimmed() 
			&& Squads[id].FireBats[i]->getHitPoints() == 50 && closeDist < 3*32 ){
			 Squads[id].FireBats[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[id].FireBats[i]->getPosition() ) < 6*32 ){
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
		if ( (  Squads[id].FireBats[i]->getGroundWeaponCooldown() != 0  && !Squads[id].FireBats[i]->isStartingAttack()  )
			||  CloseStorm == true
			|| (Squads[id].FireBats[i]->getHitPoints() < 20 && closest != NULL && Squads[id].FireBats[i]->getGroundWeaponCooldown() != 0 ) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[id].FireBats[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[id].FireBats[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32

				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[id].FireBats[i]->getPosition().getDistance( defencePosP ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[id].FireBats[i]->getOrderTarget() == NULL ){
                        Squads[id].FireBats[i]->attack(  closest );
					  }
					  else if( Squads[id].FireBats[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[id].FireBats[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[id].FireBats[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[id].FireBats[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[id].FireBats[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[id].FireBats[i]->attack(  defencePosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}


	  
	}



	for(unsigned int i=0; i< Squads[id].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = Squads[id].Medics[i]->getUnitsInRadius( 9*32 );
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
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[id].Medics[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}





	  // if we can't shoot, run away  if(kite)

		// ||  ( FriendlyUnitCount == 0 &&  Squads[id].Medics[i]->getDistance( InfoMan->PosOurBase ) > 30*32 )

		if (  ( Squads[id].Medics[i]->getHitPoints() < 25 && closest != NULL ) 
			|| CloseStorm == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[id].Medics[i]->move( fleePosition );
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
			if( close->getPlayer() == Broodwar->self() && closeDist > Squads[id].Medics[i]->getDistance( close->getPosition() )
				&& close->getHitPoints() != close->getType().maxHitPoints()  && !close->isBeingHealed() ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL ){
			if( Squads[id].Medics[i]->getOrderTarget() == NULL ){
				Squads[id].Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			else if( Squads[id].Medics[i]->getOrderTarget()->getID() != needsHeal->getID() ){
				Squads[id].Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
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
			  BOOST_FOREACH( Unit close, Squads[id].Marines )
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
				if( Squads[id].Medics[i]->getPosition().getDistance( defencePosP ) > 6*32 ){
			      Squads[id].Medics[i]->move(  defencePosP );//regular move
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
	
	if( AllowSDestroy == false && Broodwar->getFrameCount() > StartAttackFrame + 1000
		&& ClosestTankProtoss.getDistance( InfoMan->PosOurBase ) < 30*32 ){
         AllowSDestroy = true;
	}
	

	// 	for(unsigned int i=0; i<Squads[0].Tanks.size(); i++){
	for(unsigned int i=0; i<0; i++){


		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		/*
		if( Squads[0].Tanks[i]->getPosition().getDistance( InfoMan->PosOurBase ) > 30*32 
			||  BWTA::getRegion( Squads[0].Tanks[i]->getPosition() ) != InfoMan->OurMainregion ){
			TankClose = true;
		}
		*/

		
		if( InfoMan->PosEnemyBase.getDistance( Squads[0].Tanks[i]->getPosition() ) < DistTankProtoss ){
			DistTankProtoss = InfoMan->PosEnemyBase.getDistance( Squads[0].Tanks[i]->getPosition() );
			ClosestTankProtoss = Squads[0].Tanks[i]->getPosition();
		}
		

		Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		Unit closestDragoon = NULL;
		Unit closestHT = NULL;
		Unit closestReavu = NULL;


		Unit SupplyDepot = NULL;

		/*
		if(   bManager->WallSound == true
				&& AllowSDestroy == true){
		         BOOST_FOREACH( Unit close, Broodwar->getUnitsOnTile( bManager->SupplyWall1.x(),bManager->SupplyWall1.y() ) ){
					 if( close->getType() == BWAPI::UnitTypes::Terran_Supply_Depot ){
						 SupplyDepot = close;
						 if( Squads[0].Tanks[i]->getOrderTarget() == NULL){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 } else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != SupplyDepot->getID() ){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 }
						 continue;
					 }
				 }
			}
			*/

		int closeDist = 9999999;
		int totalZealot = 0;
		BOOST_FOREACH( Unit close, EnemyClose )
        {

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}
			if(  !close->isDetected() ){
				continue;
			}
			if( close->getType().isFlyer()  ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_Zealot  ){
				totalZealot++;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_Dragoon  ){
				closestDragoon = close;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_High_Templar  ){
				closestHT = close;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_Reaver  ){
				closestReavu = close;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		/*
		if( closest == NULL ){
		  if(   bManager->WallSound == true
				&& AllowSDestroy == true){
		         BOOST_FOREACH( Unit close, Broodwar->getUnitsOnTile( bManager->SupplyWall1.x(),bManager->SupplyWall1.y() ) ){
					 if( close->getType() == BWAPI::UnitTypes::Terran_Supply_Depot ){
						 SupplyDepot = close;
						 if( Squads[0].Tanks[i]->getOrderTarget() == NULL){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 } else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != SupplyDepot->getID() ){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 }
						 continue;
					 }
				 }
			}
		}
		*/


		if( closest == NULL ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		} else if( closeDist >= 12*32 ){
		//unsiege if there are no attacking buildings
        if( closest->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony &&
			closest->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon &&
			closest->getType() != BWAPI::UnitTypes::Terran_Bunker ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		}
	  }

		if( closest == NULL ){
			Squads[0].Tanks[i]->attack(  attackPosP );
		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].Tanks[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}


	  // if we can't shoot, run away  if(kite)
	  if ( ( ( closeDist < 5*32 
		      &&  Squads[0].Tanks[i]->getGroundWeaponCooldown() != 0
			  &&  !Squads[0].Tanks[i]->isAttackFrame() ) 
			  || CloseStorm == true )

		  &&  Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Tanks[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest != NULL  ){ //|| closeDist > 3*32
			if(  closeDist >= 12*32 ){
					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }
				continue;
			}
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );

					  //siege up for sunken, bunker and cannon
					  if( closest->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ||
						  closest->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
						  closest->getType() == BWAPI::UnitTypes::Terran_Bunker ){
						if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
		                 }
					  }

					  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
						  && Squads[0].Tanks.size() > totalZealot ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
					  }
					  
					  //prioiritize
					  if( closestDragoon != NULL){
						  if(  closestDragoon->getDistance( Squads[0].Tanks[i] ) < 10*32){
							   closest = closestDragoon;
						  }
					  }
					  //prioiritize
					  if( closestHT != NULL){
						  if(  closestHT->getDistance( Squads[0].Tanks[i] ) < 10*32){
							   closest = closestHT;
						  }
					  }
					  //prioiritize
					  if( closestReavu != NULL){
						  if(  closestReavu->getDistance( Squads[0].Tanks[i] ) < 10*32){
							   closest = closestReavu;
						  }
					  }

					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }

				  //Broodwar->printf("Attack");
				
			
			continue;
		}


	  
	}
	/*
	if( AllowSDestroy == false && Broodwar->getFrameCount() > 10000
		&& ClosestTankProtoss.getDistance( InfoMan->PosOurBase ) < 30*32 ){
         AllowSDestroy = true;
	}
	*/

}



void MacroManager::OnFrameProtossSquadAttack(int id){

	/*
	Broodwar->drawTextScreen( 300,110, " Enemy buildings: %d", InfoMan->EnemyBuildings.size());
	Broodwar->drawTextScreen( 300,120, " HuntP: %d", HuntP);
	Broodwar->drawTextScreen( 300,130, " HuntAllowedP: %d", HuntAllowedP);		
	*/

	if( (currentBuildingP == NULL || attackPosP == BWAPI::Positions::None) && InfoMan->EnemyBuildings.size() > 0 ){
		NextBuildingMMp();
	}
	//Broodwar->drawCircleMap( attackPos.x() , attackPos.y() ,5,Colors::Purple,true);
	if( ( currentBuildingP == NULL || attackPosP == BWAPI::Positions::None ) && HuntP == false && HuntAllowedP == true
		 && InfoMan->EnemyBuildings.size() == 0 ){
		HuntP = true;
		std::vector< Position > PossiblePos;
		BOOST_FOREACH( BWTA::BaseLocation* close, BWTA::getBaseLocations() )
        {
			if( !close->isIsland()  ){
			  PossiblePos.push_back( BWAPI::Position( close->getTilePosition() ) );
			}
		}
		attackPosP = PossiblePos[ rand() %  PossiblePos.size()  ];
		Broodwar->printf("New location to look");
	}
	if( InfoMan->EnemyBuildings.size() > 0 && HuntP == true){
		HuntP = false;
		NextBuildingMMp();
	}
	if(  HuntP == true && HuntAllowedP == true ){
		if( Broodwar->isVisible(  BWAPI::TilePosition( attackPosP ) ) ){
			HuntP = false;
			attackPosP = BWAPI::Positions::None;
		}
	}

	/*
	if( attackPosP != BWAPI::Positions::Invalid &&  attackPosP != BWAPI::Positions::None &&  attackPosP != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( attackPosP.x(), attackPosP.y(), 5 , BWAPI::Colors::Purple,true);
	}
	*/


	//unload all bunkers
	for( int b=0; b<Squads[0].Bunkers.size(); b++){
		if(  Squads[0].Bunkers[b]->getLoadedUnits().size() > 0 ){
			Squads[0].Bunkers[b]->unloadAll();
		}
	  }


	//get a list of enemy units
	std::vector< BWAPI::Unit > EnemyUnits;
	BOOST_FOREACH( Unit close, Broodwar->enemy()->getUnits() ){

		if(  close->getType().isBuilding() && close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
			continue;
		}
		EnemyUnits.push_back( close );
	}


	//get tank closest to attackposP
	int tankAttackCloseDist = 99999;
	BWAPI::Unit tankClosestAttack = NULL;
	BOOST_FOREACH( BWAPI::Unit tank, Squads[0].Tanks ){
		if(  tank->getDistance( attackPosP) < tankAttackCloseDist ){
			tankAttackCloseDist =  tank->getDistance( attackPosP);
			tankClosestAttack = tank;
		}
	}


	  BOOST_FOREACH( Unit scienceVessel, Squads[0].ScienceVessels ){
	  //for(unsigned int i=0; i< Squads[0].ScienceVessels.size(); i++){

		int MaxDistAttack =  15*32;
		//Unitset UnitsClose = scienceVessel->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		Unit closestRanged = NULL;
		int closeDistRanged = 9999999;
		Unit closestEMP = NULL; //who to irradiate?
		int closeDistEMP = 9999999; //who to irradiate?
		int totSunk = 0;

		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if( scienceVessel->getDistance( close->getPosition() ) > MaxDistAttack){
				continue;
			}
			if(  close->getType().isBuilding() && close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon  ){
				continue;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > scienceVessel->getDistance( close->getPosition() )  ){
				closeDist = scienceVessel->getDistance( close->getPosition() );
				closest = close;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDistRanged > scienceVessel->getDistance( close->getPosition() )  
				&&  
				(close->getType() == BWAPI::UnitTypes::Protoss_Dragoon
				||  close->getType() == BWAPI::UnitTypes::Protoss_Corsair
				||  close->getType() == BWAPI::UnitTypes::Protoss_Scout
				||  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon 
				)
				){
				closeDistRanged = scienceVessel->getDistance( close->getPosition() );
				closestRanged = close;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDistEMP > scienceVessel->getDistance( close->getPosition() )
				&& ( close->getType() == BWAPI::UnitTypes::Protoss_High_Templar
				||  close->getType() == BWAPI::UnitTypes::Protoss_Archon )  ){
				closeDistEMP = scienceVessel->getDistance( close->getPosition() );
				closestEMP = close;
			}

		}



		if( closestEMP != NULL && scienceVessel->getEnergy() > 100
			&& Broodwar->self()->hasResearched( BWAPI::TechTypes::EMP_Shockwave ) ){
				//  && !closestEMP->getShields() == 0
			if( scienceVessel->getOrder() != BWAPI::Orders::CastEMPShockwave
				 ){
					scienceVessel->useTech( BWAPI::TechTypes::EMP_Shockwave, closestEMP->getPosition() );
				continue;
			}
		}

		if( closestRanged == NULL && closeDistRanged > 5*32){
			if( Squads[0].Marines.size() > 0){
				int MarineDistClosest = 99999;
				BWAPI::Unit ClosestMarine = NULL; 
				for( int j=0; j<Squads[0].Marines.size(); j++){
					if( Squads[0].Marines[j]->getDistance( attackPosP ) < MarineDistClosest){
						MarineDistClosest = Squads[0].Marines[j]->getDistance( attackPosP );
						ClosestMarine = Squads[0].Marines[j];
					}
				}
				if( ClosestMarine != NULL){
					if( scienceVessel->getPosition().getDistance( ClosestMarine->getPosition() ) > 3*32 ){
				      scienceVessel->move(  ClosestMarine->getPosition() );
					}

				} else {
					if( scienceVessel->getOrder() != BWAPI::Orders::Move ){
                  scienceVessel->move( attackPosP );
					}
				}
			} else {
				if( scienceVessel->getOrder() != BWAPI::Orders::Move ){
			      scienceVessel->move( attackPosP );
				}
			}

			continue;
		}

		if(  closeDistRanged < 6*33 && closestRanged != NULL ){

			//try to move to a tank first
			if( tankClosestAttack != NULL ){
				//prevent spam
				if(  scienceVessel->getTargetPosition() !=   tankClosestAttack->getPosition() ){
				  BWAPI::Position fleePosition( tankClosestAttack->getPosition()  );    
		          scienceVessel->move( fleePosition );
				}
		        continue;
			} else { //otherwise just move to the base
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );    
		  scienceVessel->move( fleePosition );
		  continue;

			}
		}

	}


	  /*
	for(unsigned int i=0; i<Squads[0].Ghosts.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Ghosts[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		Unit closestLD = NULL;
		int closeDistLD = 9999999;
		bool PhotonCan = false;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Ghosts[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Ghosts[i]->getDistance( close->getPosition() );
				closest = close;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDistLD > Squads[0].Ghosts[i]->getDistance( close->getPosition() )
				&& ( close->getType() == BWAPI::UnitTypes::Protoss_Dragoon
				||  close->getType() == BWAPI::UnitTypes::Protoss_Reaver
				||  close->getType() == BWAPI::UnitTypes::Protoss_Shuttle 
				||  close->getType() == BWAPI::UnitTypes::Protoss_Arbiter
				||  close->getType() == BWAPI::UnitTypes::Protoss_Carrier )  ){
				closeDistLD = Squads[0].Ghosts[i]->getDistance( close->getPosition() );
				closestLD = close;
			}

		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].Ghosts[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}

	  // if we can't shoot, run away  if(kite)
		if ( ( closeDist < 6*32 &&  Squads[0].Ghosts[i]->getGroundWeaponCooldown() != 0 && !Squads[0].Ghosts[i]->isAttackFrame()  ) 
	        ||  PhotonCan == true
			||  CloseStorm == true)
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Ghosts[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }


		if( closestLD != NULL && Squads[0].Ghosts[i]->getEnergy() > 100
			&& Broodwar->self()->hasResearched( BWAPI::TechTypes::Lockdown ) ){
				if( Squads[0].Ghosts[i]->getOrder() != BWAPI::Orders::CastLockdown
				&& !closestLD->isIrradiated() ){
					Squads[0].Ghosts[i]->useTech( BWAPI::TechTypes::Lockdown, closestLD );
				continue;
			}
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Ghosts[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Ghosts[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[0].Ghosts[i]->getOrderTarget() == NULL ){
						  Squads[0].Ghosts[i]->attack(  closest ); 
						  //Squads[0].Vultures[i]->patrol(  closest->getPosition() ); //use patrol micro
					  }
					  else if( Squads[0].Ghosts[i]->getOrderTarget()->getID() != closest->getID() ){
						  Squads[0].Ghosts[i]->attack(  closest ); 
		                 //Squads[0].Vultures[i]->patrol(  closest->getPosition() ); //use patrol micro
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Ghosts[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Ghosts[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Ghosts[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Ghosts[i]->attack(  attackPosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}
	*/



	for(unsigned int i=0; i<Squads[0].Vultures.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Vultures[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].Vultures[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}

	  // if we can't shoot, run away  if(kite)
		if ( ( closeDist < 4*32 &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 && !Squads[0].Vultures[i]->isAttackFrame()  ) 
	        ||  PhotonCan == true
			||  CloseStorm == true)
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Vultures[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Vultures[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Vultures[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[0].Vultures[i]->getOrderTarget() == NULL ){
						  Squads[0].Vultures[i]->attack(  closest ); 
						  //Squads[0].Vultures[i]->patrol(  closest->getPosition() ); //use patrol micro
					  }
					  else if( Squads[0].Vultures[i]->getOrderTarget()->getID() != closest->getID() ){
						  Squads[0].Vultures[i]->attack(  closest ); 
		                 //Squads[0].Vultures[i]->patrol(  closest->getPosition() ); //use patrol micro
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Vultures[i]->attack(  attackPosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}


	int CloseToTankDist = 10*32;

	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistAttack = 20*32;
		//Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 20*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		int totalPhoton = 0;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  Squads[0].Marines[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}

			
			if(  !close->isDetected() ){
				continue;
			}			
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}

			//dont chase shuttle you can't shoot
			//  || !Broodwar->isWalkable( close->getPosition().x/ 32 ,  close->getPosition().y / 32 ) 
			if(  close->getType().isFlyer()
				&&  (Squads[0].Marines[i]->getDistance( close->getPosition() ) > 4*32
				)

				){
				continue;
			}

			if(  close->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				PhotonCan = true;
				totalPhoton++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() 
			&& Squads[0].Marines[i]->getHitPoints() == 40  && closeDist < 5*32 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].Marines[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}

		/*
		if( Squads[0].Marines[i]->isUnderStorm() ){
			CloseStorm = true;
		}
		*/


		if( totalPhoton*9 < Squads[0].Marines.size() + Squads[0].Medics.size() ){
			PhotonCan = false;
		}

		bool TankNearyBy = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Marines[i] ) < 6*32
				 ||  tankClosestAttack->getDistance( attackPosP ) < Squads[0].Marines[i]->getDistance( attackPosP )  ){
				TankNearyBy = true;
			}
		}

		bool TooFarFromTank = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Marines[i]->getPosition() ) > CloseToTankDist
				&&  tankClosestAttack->getDistance( Squads[0].Marines[i]->getPosition() ) < (CloseToTankDist + 5*32) ){
				 TooFarFromTank = true;
			}
		}
		if( TooFarFromTank == true ){

		 if( tankClosestAttack != NULL ){
			 Squads[0].Marines[i]->move( tankClosestAttack->getPosition() );
	  	  continue;
		 }
		}
		
		/*
		//check how far awat the marine is from tanks
		for(int t=0; t<Squads[0].Tanks.size(); t++ ){
			if( Squads[0].Tanks[t]->getDistance( Squads[0].Marines[i] ) < 6*32
				 ||  Squads[0].Tanks[t]->getDistance( attackPosP ) < Squads[0].Marines[i]->getDistance( attackPosP )  ){
				TankNearyBy = true;
			}
		}
		*/

		if( Squads[0].Tanks.size() < 3  || BWTA::getRegion( BWAPI::TilePosition( Squads[0].Marines[i]->getPosition() ) ) == InfoMan->OurMainregion ){
			TankNearyBy = true;
		}

		if( NoNeedTank == true ){
			TankNearyBy = true;
		}

	  // if we can't shoot, run away  if(kite)
		//  || (Squads[0].Marines[i]->getHitPoints() < 25 && closest != NULL )
		// && !Squads[0].Marines[i]->isAttackFrame()
		if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0  && !Squads[0].Marines[i]->isStartingAttack()  )
	        ||  PhotonCan == true
			||  CloseStorm == true
			||  TankNearyBy == false
			|| (Squads[0].Marines[i]->getHitPoints() < 25 && closest != NULL &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 ) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Marines[i]->getPosition().getDistance( attackPosP ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[0].Marines[i]->getOrderTarget() == NULL ){
                        Squads[0].Marines[i]->attack(  closest );
					  }
					  else if( Squads[0].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Marines[i]->attack(  closest );
					  }
				  } else {


					  //prevent stoppage at the ramp by attacking the new location once the main base is gone
					  if (Hunt == true) {
						  if (Squads[0].Marines[i]->getOrderTargetPosition() != attackPos) {
							  Squads[0].Marines[i]->attack(attackPos);
						  }
					  }

					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Marines[i]->attack(  attackPosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}



	for(unsigned int i=0; i<Squads[0].FireBats.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistAttack = 20*32;
		//Unitset EnemyClose = Squads[0].FireBats[i]->getUnitsInRadius( 20*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool PhotonCan = false;
		int totalPhoton = 0;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if( Squads[0].FireBats[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				continue;
			}
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
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].FireBats[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].FireBats[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].FireBats[i]->isStimmed() 
			&& Squads[0].FireBats[i]->getHitPoints() == 50 && closeDist < 3*32 ){
			 Squads[0].FireBats[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].FireBats[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}

		/*
		if( Squads[0].Marines[i]->isUnderStorm() ){
			CloseStorm = true;
		}
		*/


		if( totalPhoton*3 < Squads[0].Marines.size() + Squads[0].Medics.size() ){
			PhotonCan = false;
		}




		bool TooFarFromTank = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].FireBats[i] ) > CloseToTankDist
				&&  tankClosestAttack->getDistance( Squads[0].FireBats[i]->getPosition() ) < (CloseToTankDist + 5*32)){
				 TooFarFromTank = true;
			}
		}
		if( TooFarFromTank == true ){

		 if( tankClosestAttack != NULL ){
			 Squads[0].FireBats[i]->move( tankClosestAttack->getPosition() );
	  	  continue;
		 }
		}





		bool TankNearyBy = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].FireBats[i] ) < 6*32
				 ||  tankClosestAttack->getDistance( attackPosP ) < Squads[0].FireBats[i]->getDistance( attackPosP )  ){
				TankNearyBy = true;
			}
		}

		if( NoNeedTank == true ){
			TankNearyBy = true;
		}

		/*
		//check how far awat the marine is from tanks
		for(int t=0; t<Squads[0].Tanks.size(); t++ ){
			if( Squads[0].Tanks[t]->getDistance( Squads[0].FireBats[i] ) < 7*32
				 ||  Squads[0].Tanks[t]->getDistance( attackPosP ) < Squads[0].FireBats[i]->getDistance( attackPosP ) ){
				TankNearyBy = true;
			}
		}
		*/


		if( Squads[0].Tanks.size() < 3 ||  closeDist < 2*32
			|| BWTA::getRegion(BWAPI::TilePosition(Squads[0].FireBats[i]->getPosition()) ) == InfoMan->OurMainregion  ){
			TankNearyBy = true;
		}


	  // if we can't shoot, run away  if(kite)
		//  || (Squads[0].Marines[i]->getHitPoints() < 25 && closest != NULL )
		// && !Squads[0].Marines[i]->isAttackFrame()
		if ( (  Squads[0].FireBats[i]->getGroundWeaponCooldown() != 0  && !Squads[0].FireBats[i]->isStartingAttack()  )
	        ||  PhotonCan == true
			||  CloseStorm == true
			|| TankNearyBy == false
			|| (Squads[0].FireBats[i]->getHitPoints() < 20 && closest != NULL && Squads[0].FireBats[i]->getGroundWeaponCooldown() != 0 ) )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].FireBats[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].FireBats[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].FireBats[i]->getPosition().getDistance( attackPosP ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[0].FireBats[i]->getOrderTarget() == NULL ){
                        Squads[0].FireBats[i]->attack(  closest );
					  }
					  else if( Squads[0].FireBats[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].FireBats[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].FireBats[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].FireBats[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].FireBats[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].FireBats[i]->attack(  attackPosP );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}



	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 15*32 );
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
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		if( totalPhoton*3 < Squads[0].Marines.size() + Squads[0].Medics.size() ){
			PhotonCan = false;
		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].Medics[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}






		bool TooFarFromTank = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Medics[i] ) > CloseToTankDist
				&&  tankClosestAttack->getDistance( Squads[0].Medics[i]->getPosition() ) < (CloseToTankDist + 5*32)  ){
				 TooFarFromTank = true;
			}
		}
		if( TooFarFromTank == true ){

		 if( tankClosestAttack != NULL ){
			 Squads[0].Medics[i]->move( tankClosestAttack->getPosition() );
	  	  continue;
		 }
		}




		bool TankNearyBy = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Medics[i] ) < 6*32
				 ||  tankClosestAttack->getDistance( attackPosP ) < Squads[0].Medics[i]->getDistance( attackPosP )  ){
				TankNearyBy = true;
			}
		}

		if( NoNeedTank == true ){
			TankNearyBy = true;
		}

		/*
		//check how far awat the marine is from tanks
		for(int t=0; t<Squads[0].Tanks.size(); t++ ){
			if( Squads[0].Tanks[t]->getDistance( Squads[0].Medics[i] ) < 6*32
				||  Squads[0].Tanks[t]->getDistance( attackPosP ) < Squads[0].Medics[i]->getDistance( attackPosP ) ){
				TankNearyBy = true;
			}
		}
		*/

		if( Squads[0].Tanks.size() < 3 || BWTA::getRegion(BWAPI::TilePosition(Squads[0].Medics[i]->getPosition() ) ) == InfoMan->OurMainregion ){
			TankNearyBy = true;
		}
		

	  // if we can't shoot, run away  if(kite)

		//  ||  ( FriendlyUnitCount == 0 &&  Squads[0].Medics[i]->getDistance( InfoMan->PosOurBase ) > 30*32 )
		if (  ( Squads[0].Medics[i]->getHitPoints() < 25 && closest != NULL ) 
			|| PhotonCan == true
			|| CloseStorm == true
			||  TankNearyBy == false
			)
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
              /*
		      BOOST_FOREACH( Unit close, Marines )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  */
			  BOOST_FOREACH( Unit close, Squads[0].Marines )
              {
				  if( close->getPosition().getDistance( attackPosP ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( attackPosP );
				  }
		      }
			  
			  if(  closestUnit != NULL){
				  //Medics[i]->attack(  closestUnit->getPosition() );//heal move
				  Squads[0].Medics[i]->move(  closestUnit->getPosition() );//regular move
				  continue;
			  }
			  

				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].Medics[i]->getPosition().getDistance( attackPosP ) > 6*32 ){
			      Squads[0].Medics[i]->move(  attackPosP );//regular move
				  continue;
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




		for(unsigned int i=0; i<Squads[0].SCVs.size(); i++){


		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		int MaxDistAttack = 10*32;
		Unitset UnitsClose = Squads[0].SCVs[i]->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			
			//if(  Squads[0].SCVs[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				//continue;
			//}
			
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].SCVs[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].SCVs[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		/*
	  // if we can't shoot, run away  if(kite)
		if (   Sunken == true || closeDist < 6*32
		    ||  isUnderDS )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  		  if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::Move){
		   Squads[0].SCVs[i]->move( fleePosition );
				  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }
	  */


	  //check if a unit nearby needs healing
	  Unit needsHeal = NULL;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->self() 
				&& close->getHitPoints() != close->getType().maxHitPoints()
				&& ( close->getType() == BWAPI::UnitTypes::Terran_Goliath 
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode) ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL &&  !Squads[0].SCVs[i]->isRepairing() ){
			Squads[0].SCVs[i]->repair( needsHeal );
			continue;
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
			  Unit closestUnit = NULL;
			  int closeDist = 999999;


					  //prevent spam
					  if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].SCVs[i]->attack(  attackPosP );

					  }
                /*
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].SCVs[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].SCVs[i]->move(  attackPos );//regular move
				}
				*/

			}
			continue;
		}


		}


	
	if( AllowSDestroy == false && Broodwar->getFrameCount() > StartAttackFrame + 1000
		&& ClosestTankProtoss.getDistance( InfoMan->PosOurBase ) < 30*32 ){
         AllowSDestroy = true;
	}
	


	for(unsigned int i=0; i<Squads[0].Tanks.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		/*
		if( Squads[0].Tanks[i]->getPosition().getDistance( InfoMan->PosOurBase ) > 30*32 
			||  BWTA::getRegion( Squads[0].Tanks[i]->getPosition() ) != InfoMan->OurMainregion ){
			TankClose = true;
		}
		*/

		
		if( InfoMan->PosEnemyBase.getDistance( Squads[0].Tanks[i]->getPosition() ) < DistTankProtoss ){
			DistTankProtoss = InfoMan->PosEnemyBase.getDistance( Squads[0].Tanks[i]->getPosition() );
			ClosestTankProtoss = Squads[0].Tanks[i]->getPosition();
		}
		
		int MaxDistAttack = 15*32;
		//Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		Unit closestDragoon = NULL;
		Unit closestHT = NULL;
		Unit closestReavu = NULL;


		Unit SupplyDepot = NULL;

		/*
		if(   bManager->WallSound == true
				&& AllowSDestroy == true){
		         BOOST_FOREACH( Unit close, Broodwar->getUnitsOnTile( bManager->SupplyWall1.x(),bManager->SupplyWall1.y() ) ){
					 if( close->getType() == BWAPI::UnitTypes::Terran_Supply_Depot ){
						 SupplyDepot = close;
						 if( Squads[0].Tanks[i]->getOrderTarget() == NULL){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 } else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != SupplyDepot->getID() ){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 }
						 continue;
					 }
				 }
			}
			*/

		int closeDist = 9999999;
		int totalZealot = 0;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  Squads[0].Tanks[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}
			if(  !close->isDetected() ){
				continue;
			}
			if( close->getType().isFlyer()  ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_Zealot  ){
				totalZealot++;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_Dragoon  ){
				closestDragoon = close;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_High_Templar  ){
				closestHT = close;
			}
			if( close->getType() == BWAPI::UnitTypes::Protoss_Reaver  ){
				closestReavu = close;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		/*
		if( closest == NULL ){
		  if(   bManager->WallSound == true
				&& AllowSDestroy == true){
		         BOOST_FOREACH( Unit close, Broodwar->getUnitsOnTile( bManager->SupplyWall1.x(),bManager->SupplyWall1.y() ) ){
					 if( close->getType() == BWAPI::UnitTypes::Terran_Supply_Depot ){
						 SupplyDepot = close;
						 if( Squads[0].Tanks[i]->getOrderTarget() == NULL){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 } else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != SupplyDepot->getID() ){
						   Squads[0].Tanks[i]->attack( SupplyDepot );
						 }
						 continue;
					 }
				 }
			}
		}
		*/


		if( closest == NULL ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		} else if( closeDist >= 12*31 ){
		//unsiege if there are no attacking buildings
        if( closest->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony &&
			closest->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon &&
			closest->getType() != BWAPI::UnitTypes::Terran_Bunker ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		}
	  }

		if( closest == NULL ){


			if( Broodwar->mapHash() == "aab66dbf9c85f85c47c219277e1e36181fe5f9fc" ){
				if( BWTA::getRegion(BWAPI::TilePosition(Squads[0].Tanks[i]->getPosition() ) ) == InfoMan->NatRegion ){
				    //BWTA::Region* NatReg = BWTA::getRegion( InfoMan->PosOurNat);
					BOOST_FOREACH( BWTA::Chokepoint* choke , InfoMan->NatRegion->getChokepoints() ){
						if( choke->getRegions().first != InfoMan->NatRegion 
							&& choke->getRegions().first != InfoMan->OurMainregion
							&& choke->getRegions().first->getBaseLocations().size() > 0
							){
								Squads[0].Tanks[i]->attack(  choke->getRegions().first->getCenter() );
								break;
						}
						if( choke->getRegions().second != InfoMan->NatRegion 
							&& choke->getRegions().second != InfoMan->OurMainregion
							&& choke->getRegions().second->getBaseLocations().size() > 0
							){
								Squads[0].Tanks[i]->attack(  choke->getRegions().second->getCenter() );
								break;
						}
					}
					continue;
				}
			}

			Squads[0].Tanks[i]->attack(  attackPosP );
			continue;


		}


		bool CloseStorm = false;
		BOOST_FOREACH( Bullet storm, Broodwar->getBullets() ){
			if( storm->getType() == BWAPI::BulletTypes::Psionic_Storm ){
				if( storm->getPosition().getDistance( Squads[0].Tanks[i]->getPosition() ) < 6*32 ){
					CloseStorm = true;
				}
			}
		}


	  // if we can't shoot, run away  if(kite)
	  if ( ( ( closeDist < 5*32 
		      &&  Squads[0].Tanks[i]->getGroundWeaponCooldown() != 0
			  &&  !Squads[0].Tanks[i]->isAttackFrame() ) 
			  || CloseStorm == true )

		  &&  Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Tanks[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest != NULL  ){ //|| closeDist > 3*32
			if(  closeDist >= 12*31 ){
					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }
				continue;
			}
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );

					  //siege up for sunken, bunker and cannon
					  if( closest->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ||
						  closest->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
						  closest->getType() == BWAPI::UnitTypes::Terran_Bunker ){
						if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
		                 }
					  }

					  // && Squads[0].Tanks.size() > totalZealot
					  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
						  && ( closestDragoon != NULL || closestHT != NULL || closestReavu != NULL  )						  
						  && Squads[0].Tanks.size() > totalZealot ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
					  }
					  

					  //prioiritize
					  if( closestDragoon != NULL){
						  if(  closestDragoon->getDistance( Squads[0].Tanks[i] ) < 10*32){
							   closest = closestDragoon;
						  }
					  }
					  //prioiritize
					  if( closestHT != NULL){
						  if(  closestHT->getDistance( Squads[0].Tanks[i] ) < 10*32){
							   closest = closestHT;
						  }
					  }
					  //prioiritize
					  if( closestReavu != NULL){
						  if(  closestReavu->getDistance( Squads[0].Tanks[i] ) < 10*32){
							   closest = closestReavu;
						  }
					  }

					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( closest != NULL && Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }

				  //Broodwar->printf("Attack");
				
			
			continue;
		}


	  
	}
	/*
	if( AllowSDestroy == false && Broodwar->getFrameCount() > 10000
		&& ClosestTankProtoss.getDistance( InfoMan->PosOurBase ) < 30*32 ){
         AllowSDestroy = true;
	}
	*/

}


void MacroManager::OnFrameProtossSquadDefence(int id){

	BWAPI::Position DefaultPos =  CCmanager[CCmanager.size() - 1]->CommandCenter->getPosition();

	//Broodwar->drawTextScreen( 200, 200 , "CCmanagersize:  %d" ,  CCmanager.size() );

	if( PState == P_Defend_Nat ){


		if( NatBunkerPlace == true && CCmanager.size() > 1 &&  CCmanager[1]->BaseReady == true
			&&  !CCmanager[1]->CommandCenter->isLifted()
			&& ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0 ){
              DefaultPos = InfoMan->PosOurNat;
		} else {

		 //DefaultPos =  CCmanager[CCmanager.size() - 1]->CommandCenter->getPosition();
			//BWTA::computeDistanceTransform
			int FurthestChoke = -1;
			BWAPI::Position FurthestPos = BWAPI::Positions::Unknown; 
		BWTA::Region* NatReg = BWTA::getRegion(BWAPI::TilePosition(InfoMan->PosOurNat ));
		BOOST_FOREACH( BWTA::Chokepoint* choke, NatReg->getChokepoints() ){
			if( choke->getRegions().first != InfoMan->OurMainregion
				&& choke->getRegions().second != InfoMan->OurMainregion){
				//TODO: BWTA::getGroundDistance
					if(  BWAPI::TilePosition( choke->getCenter() ).getApproxDistance (InfoMan->OurBase)   > FurthestChoke ){
					    FurthestChoke = BWAPI::TilePosition(choke->getCenter()).getApproxDistance(InfoMan->OurBase);
						DefaultPos = choke->getCenter();
					}
			}
		  }
		}
	}

	/*
	if( ZState == Z_Expand_Defend){
		if( NatBunkerPlace == true && CCmanager.size() > 1 &&  CCmanager[1]->BaseReady == true ){
          DefaultPos = InfoMan->PosOurNat;
		} else {
		  BWTA::Region* NatReg = BWTA::getRegion( InfoMan->PosOurNat);
		  BOOST_FOREACH( BWTA::Chokepoint* choke, NatReg->getChokepoints() ){
			if( choke->getRegions().first != InfoMan->OurMainregion
				&& choke->getRegions().second != InfoMan->OurMainregion){
					DefaultPos = choke->getCenter();
			}
		  }
		}
	}
	*/

	if( PState == P_Wall && Squads[0].Bunkers.size() > 0 && WallBunkerPlaced == true ){
		DefaultPos = Squads[0].Bunkers[0]->getPosition();
	}



	std::vector< BWAPI::Unit > EnemyUnits;
	BOOST_FOREACH( Unit close, Broodwar->enemy()->getUnits() ){

		if(  close->getType().isBuilding() && close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
			continue;
		}
		for(int j=0; j<CCmanager.size(); j++ ){
			if( close->getDistance( BWAPI::Position( CCmanager[j]->LandLocation ) ) < 30*32 ){
				EnemyUnits.push_back( close );
				break;
			}

		}

	}

	//  BOOST_FOREACH( Unit close, UnitsClose )

	for(unsigned int i=0; i< Squads[0].ScienceVessels.size(); i++){

		//Unitset UnitsClose = Squads[0].ScienceVessels[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		Unit AMuta = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			
			if(  close->getType().isBuilding() ){
				continue;
			}
			
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].ScienceVessels[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].ScienceVessels[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


			Squads[0].ScienceVessels[i]->move(DefaultPos);


	}

	

	for(int i=0; i<Squads[id].Vultures.size(); i++){


		//Unitset EnemyClose = Squads[id].Vultures[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 4*32 &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 )
		      )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
		Squads[0].Vultures[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }


		//by default move to the CC
		if( closest == NULL ){
			Squads[id].Vultures[i]->move( DefaultPos );
			continue;
		}
		if( closest != NULL ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
			if( (closest->getType() == BWAPI::UnitTypes::Protoss_Zealot 
				|| BWTA::getRegion( BWAPI::TilePosition( closest->getPosition() )) == InfoMan->OurMainregion )   ){
				if( Squads[id].Vultures[i]->getTarget() != NULL){
					if( Squads[id].Vultures[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].Vultures[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].Vultures[i]->attack( closest);
			        continue;
				}
			} else {
			  Squads[id].Vultures[i]->move( DefaultPos );
			  continue;
			}
		}

	}

   for(int i=0; i<Squads[id].Ghosts.size(); i++){


		Unitset EnemyClose = Squads[id].Ghosts[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].Ghosts[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].Ghosts[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
 
	  // if we can't shoot, run away  if(kite)
		if ( ( closeDist < 5*32 &&  Squads[0].Ghosts[i]->getGroundWeaponCooldown() != 0 &&  !Squads[0].Ghosts[i]->isAttackFrame() )  )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
		Squads[0].Ghosts[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }


		//by default move to the CC
		if( closest == NULL ){
			if( Squads[id].Ghosts[i]->getOrder() != BWAPI::Orders::Move ){
			Squads[id].Ghosts[i]->move( DefaultPos );
			}
			continue;
			
		}

		if( closest != NULL &&  PState != P_Wall ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
				if( Squads[id].Ghosts[i]->getTarget() != NULL){
					if( Squads[id].Ghosts[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].Ghosts[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].Ghosts[i]->attack( closest);
			        continue;
				}
		}


	}

   

	for(int i=0; i<Squads[id].FireBats.size(); i++){


		//Unitset EnemyClose = Squads[id].FireBats[i]->getUnitsInRadius( 40*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			
			if(  !close->isDetected()  ){
				continue;
			}
			
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].FireBats[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].FireBats[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
 

	//use stim before engaging enemy
		if( closest != NULL && !Squads[0].FireBats[i]->isStimmed() && Squads[0].FireBats[i]->getHitPoints() == 50 ){
			 Squads[0].FireBats[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		//check if too far away from the natural
		bool FarAwayNat = false;
		if( CCmanager.size() > 1 ){
		  if( Squads[0].FireBats[i]->getDistance( CCmanager[1]->CommandCenter ) > 12*32
			  &&  BWTA::getRegion(BWAPI::TilePosition(Squads[0].FireBats[i]->getPosition() ) ) != InfoMan->OurMainregion
			  && Squads[0].FireBats[i]->getDistance( InfoMan->PosMainChoke ) > 12*32  ){
			  FarAwayNat = true;
		  }
		}

	  // if we can't shoot, run away  if(kite)
		if ( (  Squads[0].FireBats[i]->getGroundWeaponCooldown() != 0 &&  !Squads[0].FireBats[i]->isAttackFrame() )
			|| FarAwayNat   )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
		Squads[0].FireBats[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }


		//by default move to the CC
		if( closest == NULL ){
			if( Squads[id].FireBats[i]->getOrder() != BWAPI::Orders::Move ){
			Squads[id].FireBats[i]->move( DefaultPos );
			}
			continue;
			
		}
		if( closest != NULL &&  PState == P_Wall ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
			if( (closest->getType() == BWAPI::UnitTypes::Protoss_Zealot 
				|| BWTA::getRegion( BWAPI::TilePosition(closest->getPosition() )) == InfoMan->OurMainregion )  ){
				if( Squads[id].FireBats[i]->getTarget() != NULL){
					if( Squads[id].FireBats[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].FireBats[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].FireBats[i]->attack( closest);
			        continue;
				}
			} else {
			  Squads[id].FireBats[i]->move( DefaultPos );
			  continue;
			}
		}

		if( closest != NULL &&  PState != P_Wall ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
				if( Squads[id].FireBats[i]->getTarget() != NULL){
					if( Squads[id].FireBats[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].FireBats[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].FireBats[i]->attack( closest);
			        continue;
				}
		}


	}



	for(int i=0; i<Squads[id].Marines.size(); i++){


		//Unitset EnemyClose = Squads[id].Marines[i]->getUnitsInRadius( 40*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			
			if(  !close->isDetected()  ){
				continue;
			}
			
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
 
	  if( PState == P_Wall && Squads[0].Bunkers.size() > 0 && WallBunkerPlaced == true
		   && i < 4 ){
			   Squads[id].Marines[i]->rightClick( Squads[0].Bunkers[0] );
	  }

	  if( PState != P_Wall && Squads[0].Bunkers.size() > 0 && WallBunkerPlaced == true
		   && i < 4 ){
			   if( Squads[0].Bunkers[0]->getLoadedUnits().size() > 0 ){
				   Squads[0].Bunkers[0]->unloadAll();
			   }
	  }

	  if( PState == P_Defend_Nat && Squads[0].Bunkers.size() > 0
		   && i < 4 ){
			   for( int b=0; b<Squads[0].Bunkers.size(); b++){
				   if(  BWTA::getRegion(BWAPI::TilePosition(Squads[0].Bunkers[b]->getPosition()) ) != InfoMan->OurMainregion ){
			             Squads[id].Marines[i]->rightClick( Squads[0].Bunkers[b] );
						 break;
				   }
			   }
	  }



	//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		//check if too far away from the natural
		bool FarAwayNat = false;
		if( CCmanager.size() > 1 ){
		  if( Squads[0].Marines[i]->getDistance( CCmanager[1]->CommandCenter ) > 12*32
			  &&  BWTA::getRegion(BWAPI::TilePosition(Squads[0].Marines[i]->getPosition()) ) != InfoMan->OurMainregion
			  && Squads[0].Marines[i]->getDistance( InfoMan->PosMainChoke ) > 12*32 ){
			  FarAwayNat = true;
		  }
		}

		if( PState == P_Wall &&  BWTA::getRegion(BWAPI::TilePosition(Squads[0].Marines[i]->getPosition()) ) != InfoMan->OurMainregion ){
			FarAwayNat = true;
		}

	  // if we can't shoot, run away  if(kite)
		if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 &&  !Squads[0].Marines[i]->isAttackFrame() )
			|| FarAwayNat   )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
		Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }


		//by default move to the CC
		if( closest == NULL ){
			if( Squads[id].Marines[i]->getOrder() != BWAPI::Orders::Move ){
			Squads[id].Marines[i]->move( DefaultPos );
			}
			continue;
			
		}
		if( closest != NULL &&  PState == P_Wall ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
			if( closest->getType() == BWAPI::UnitTypes::Protoss_Zealot 
				|| BWTA::getRegion(BWAPI::TilePosition(closest->getPosition() )) == InfoMan->OurMainregion
				|| closest->getPosition().getDistance( InfoMan->PosMainChoke ) < 3*32    ){
				if( Squads[id].Marines[i]->getTarget() != NULL){
					if( Squads[id].Marines[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].Marines[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].Marines[i]->attack( closest);
			        continue;
				}
			} else {
			  Squads[id].Marines[i]->move( DefaultPos );
			  continue;
			}
		}

		if( closest != NULL &&  PState != P_Wall ){
			// && Squads[id].Marines[i]->getHitPoints() >= 20 
				if( Squads[id].Marines[i]->getTarget() != NULL){
					if( Squads[id].Marines[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].Marines[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].Marines[i]->attack( closest);
			        continue;
				}
		}


	}

	for(int i=0; i<Squads[id].Tanks.size(); i++){

		//Unitset EnemyClose = Squads[id].Tanks[i]->getUnitsInRadius( 40*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  close->getType().isBuilding()  ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}
			if(  !close->isDetected() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[id].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[id].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}



		//check if too far away from the natural
		bool FarAwayNat = false;
		if( CCmanager.size() > 1 ){
		  if( Squads[0].Tanks[i]->getDistance( CCmanager[1]->CommandCenter ) > 12*32
			  &&  BWTA::getRegion(BWAPI::TilePosition(Squads[0].Tanks[i]->getPosition() )) != InfoMan->OurMainregion  ){
			  FarAwayNat = true;
		  }
		}

		//by default move to the CC
		if( closest == NULL ){

			if(  Squads[id].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
				&& Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
					Squads[id].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
					continue;
			}


			if( Squads[id].Tanks[i]->getOrder() != BWAPI::Orders::Move ){
			  Squads[id].Tanks[i]->move( DefaultPos );
			}
			continue;
		}
		if( closest != NULL ){

			//if to far away from nat, move back
			if( FarAwayNat  && Squads[id].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode  ){
			  Squads[id].Tanks[i]->move( DefaultPos );
			  continue;
			}

			if(  closeDist > 12*32 && Squads[id].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
					Squads[id].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
					continue;
			}

			if( closeDist > 6*32 &&  closeDist < 12*32 
				&& Squads[id].Tanks[i]->getType() != BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
				&& Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode ) ){
					Squads[id].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
					continue;
			}
			if( closeDist > 6*32 && Squads[id].Tanks[i]->getType() != BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode  ){
				if( Squads[id].Tanks[i]->getTarget() != NULL){
					if( Squads[id].Tanks[i]->getTarget()->getID() != closest->getID() ){
						Squads[id].Tanks[i]->attack( closest);
						continue;
					}
				} else {
					Squads[id].Tanks[i]->attack( closest);
			        continue;
				}
			} 

			//if enemy gets too close, fall back
			if( closeDist <= 6*32 && !Broodwar->self()->hasResearched( BWAPI::TechTypes::Tank_Siege_Mode )  ){
			  Squads[id].Tanks[i]->move( DefaultPos );
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
			  BOOST_FOREACH( Unit close, Squads[0].Marines )
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
			  /*
				if( Squads[0].Medics[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].Medics[i]->move(  attackPos );//regular move
				}
				*/

				if( Squads[0].Medics[i]->getPosition().getDistance( DefaultPos ) > 6*32 ){
			      Squads[0].Medics[i]->move(  DefaultPos );//regular move
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

void MacroManager::OnFrameProtossSquad(){

	/*
	//make sure that there are units that can defend the expansions
	if( Squads.size() > 1  && PState == P_Push ){

		if( Squads[1].Marines.size() < 2 ){
			if( Squads[0].Marines.size() > 5 ){
				BWAPI::Unit getUnit = Squads[0].Marines.back();
				Squads[1].Marines.push_back( getUnit );
				Squads[0].Marines.pop_back();
			}
		}
		if( Squads[1].Medics.size() < 2 ){
			if( Squads[0].Medics.size() > 5 ){
				BWAPI::Unit getUnit = Squads[0].Medics.back();
				Squads[1].Medics.push_back( getUnit );
				Squads[0].Medics.pop_back();
			}
		}
		if( Squads[1].FireBats.size() < 1 ){
			if( Squads[0].FireBats.size() > 3 ){
				BWAPI::Unit getUnit = Squads[0].FireBats.back();
				Squads[1].FireBats.push_back( getUnit );
				Squads[0].FireBats.pop_back();
			}
		}
	}
	*/

	for(int i=0; i<Squads.size(); i++){
		if( Squads[i].SQstate == SQ_Defend_Base && PState == P_Push){
			OnFrameProtossSquadDefenceExpansion(i);
			continue;
		}
		if( Squads[i].SQstate == SQ_Defend_Base){
			OnFrameProtossSquadDefence(i);
			continue;
		}
		if( Squads[i].SQstate == SQ_Attack_Base){
			OnFrameProtossSquadAttack(i);
			continue;
		}
	}

}

void MacroManager::OnFrameProtoss(){


	if( attackPosP != BWAPI::Positions::Invalid &&  attackPosP != BWAPI::Positions::None &&  attackPosP != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( attackPosP.x, attackPosP.y, 5 , BWAPI::Colors::Purple,true);
	}

	if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }



	int TotalZealotCount = 0;
	BOOST_FOREACH(BWAPI::Unit unit, Broodwar->enemy()->getUnits()) {
		if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot) {
			TotalZealotCount++;
		}
	}
	//protoss is building up zealots. Defend against this
	if ( PState == P_Wall && TotalZealotCount > 1) {
		ZealotRushExpected = true;
	}

	/*
	if( PState == P_Wall 
		&& (Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Protoss_Zealot ) > 0
		|| Broodwar->enemy()->allUnitCount( BWAPI::UnitTypes::Protoss_Pylon ) > 0 )
		&& Broodwar->self()->allUnitCount( BWAPI::UnitTypes::Terran_Supply_Depot ) < 2
		&& ZealotRushBunker == false ){

			ZealotRushBunker = true;
			ProdMan->addToQueueBegin( BWAPI::UnitTypes::Terran_Bunker );

	}
	*/


	//scout at 12 scvs
	if(  ScoutMan == NULL && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_SCV ) >= 12 ){
		ScoutMan = new ScoutManager(1);
	}

	int PhotonCannonCount = 0;
	for( int i=0; i<InfoMan->EnemyBuildings.size(); i++){
		if( InfoMan->EnemyBuildings[i].type == BWAPI::UnitTypes::Protoss_Photon_Cannon ){
			PhotonCannonCount++;
		}
	}

	// ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Barracks ) > 0
	if( WallBunkerPlaced == false && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Barracks ) > 0
		&& bManager->WallSound == false ){
			  ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition( InfoMan->PosMainChoke) , true);
			  WallBunkerPlaced = true;
	}


	if( CCmanager.size() > 1 ){
		if( CCmanager[1]->BaseReady == true){
			MacroMan->PlaceNatBunker();
		}
	}




	// Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Barracks) >= 4

	bool pushOut = false;
	/*
	if( Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Barracks ) >= 4
		 || PhotonCannonCount > 0  ){
			 pushOut = true;
	}
	*/

	if( Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Science_Vessel ) > 0
		|| PhotonCannonCount > 0
		|| Broodwar->getFrameCount() > 18000 ){
		 pushOut = true;
	}

	//in the case of 1 fact 3 rax, push out immediately
	if( MacroMan->CurrentStrat == "OneFact3Rax" 
		&&  pushOut == false 
		&& PState == P_Defend_Nat  ){
	  PState = P_Push;
	  Squads[0].SQstate = SQ_Attack_Base;
	  StartAttackFrame = Broodwar->getFrameCount();
	}

	if( PState == P_Defend_Nat 
		&& 
		pushOut == true ){
	  PState = P_Push;
	  Squads[0].SQstate = SQ_Attack_Base;
	  StartAttackFrame = Broodwar->getFrameCount();
	   
	}


	if( Squads.size() == 1 && PState == P_Push ){

		if( CCmanager.size() >= 3 ||
			CCmanager.size() >= 2 && ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) >= 1 ){

	      MacroSquad defenceSquad;
	      defenceSquad.AttackBuilding = NULL;
	      defenceSquad.AttackTarget = BWAPI::Positions::None;
	      defenceSquad.SQstate = SQ_Defend_Base;
	      Squads.push_back( defenceSquad );
		  Broodwar->printf( "defence squad added" );
		}
	}

	OnFrameProtossSquad();

}


void MacroManager::OnFrameZergSquadAttack(int id){

	
	int SunkMulti = 1;

	if( currentBuilding == NULL || attackPos == BWAPI::Positions::None ){
		NextBuildingMM();
	}

	if( currentBuilding != NULL && Broodwar->isVisible(  currentBuilding->getTilePosition() )
		&& Broodwar->getUnitsOnTile(  currentBuilding->getTilePosition().x ,  currentBuilding->getTilePosition().y ).size() == 0  ){
        NextBuildingMM();
	}

	//Broodwar->drawCircleMap( attackPos.x() , attackPos.y() ,5,Colors::Purple,true);
	if(  Hunt == false && HuntAllowed == true && InfoMan->EnemyBuildings.size() == 0 ){ //currentBuilding == NULL &&
		Hunt = true;
		std::vector< Position > PossiblePos;
		BOOST_FOREACH( BWTA::BaseLocation* close, BWTA::getBaseLocations() )
        {
			if( !close->isIsland() ){
			  PossiblePos.push_back( BWAPI::Position( close->getTilePosition() ) );
			}
		}
		attackPos = PossiblePos[ rand() %  PossiblePos.size()  ];
	}
	if( InfoMan->EnemyBuildings.size() > 0 && Hunt == true){
		Hunt = false;
		NextBuildingMM();
	}
	if(  Hunt == true && HuntAllowed == true ){
		if( Broodwar->isVisible(  BWAPI::TilePosition( attackPos ) ) ){
			Hunt = false;
		}
	}

	if( attackPos != BWAPI::Positions::Invalid &&  attackPos != BWAPI::Positions::None &&  attackPos != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( attackPos.x, attackPos.y, 5 , BWAPI::Colors::Purple,true);
	}

	/*
	if( Squads[0].SCVs.size() == 0 && CCmanager[0]->MineralSCV.size() > (CCmanager[0]->SCVsaturation - 5) ){
		AddSCV( CCmanager[0]->getFullHPSCV() );
	}
	*/

	for(unsigned int b=0; b<Squads[0].Bunkers.size(); b++){
		if( Squads[0].Bunkers[b]->getLoadedUnits().size() > 0){
		  Squads[0].Bunkers[b]->unloadAll();
		}
	}

	/*
	if( NatBunkerChoke != NULL){
		if( NatBunkerChoke->getLoadedUnits().size() > 0){
		  NatBunkerChoke->unloadAll();
		}
	}
	*/



	//get a list of enemy units
	std::vector< BWAPI::Unit > EnemyUnits;
	BOOST_FOREACH( Unit close, Broodwar->enemy()->getUnits() ){

		if(  close->getType().isBuilding() && close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
			continue;
		}
		EnemyUnits.push_back( close );
	}




	bool LurkerCloseToTank = false;



	//get tank closest to the attack position

	//get tank closest to attackposP
	int tankAttackCloseDist = 99999;
	BWAPI::Unit tankClosestAttack = NULL;
	BOOST_FOREACH( BWAPI::Unit tank, Squads[0].Tanks ){
		if(  tank->getDistance( attackPos ) < tankAttackCloseDist ){
			tankAttackCloseDist =  tank->getDistance( attackPos );
			tankClosestAttack = tank;
		}
	}




	for(unsigned int i=0; i<Squads[0].Tanks.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistAttack = 15*32;
		//Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		//BOOST_FOREACH( Unit close, EnemyClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if(  Squads[0].Tanks[i]->getDistance( close->getPosition() ) > MaxDistAttack  ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if (close->getType() == BWAPI::UnitTypes::Zerg_Larva) {
				continue;
			}

			if( close->getType().isFlyer()  ){
				continue;
			}

			if( LurkerCloseToTank == false
				&& (close->getType() == BWAPI::UnitTypes::Zerg_Lurker || close->getType() == BWAPI::UnitTypes::Unknown )
				 &&  Squads[0].Tanks[i]->getDistance( close->getPosition() ) < 6*32 ){
					 LurkerCloseToTank = true;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		if( LurkerCloseToTank == false ){
		  for(int j=0; j<ELurkerPositions.size(); j++){
			if(  Squads[0].Tanks[i]->getDistance( ELurkerPositions[j].position ) < 6*32 ){
               LurkerCloseToTank = true;
			}
		  }
		}


		if( closest == NULL ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		} else if( closeDist > 12*31 ){  // 12*32
		//unsiege if there are no attacking buildings
        //if( closest->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony &&
		//	closest->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon &&
		//	closest->getType() != BWAPI::UnitTypes::Terran_Bunker ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		//}
	  }

		if( closest == NULL ){
			Squads[0].Tanks[i]->attack(  attackPos );
			continue;
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 5*32 &&  Squads[0].Tanks[i]->getGroundWeaponCooldown() != 0 )
		  &&  Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  if( Squads[0].Tanks[i]->getOrder() != BWAPI::Orders::Move ){
		     Squads[0].Tanks[i]->move( fleePosition );
		  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest != NULL  ){ //|| closeDist > 3*32
			if(  closeDist > 12*32 ){
					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }
				continue;
			}
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );

					  //siege up for sunken, bunker and cannon
					  if( closest->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ||
						  closest->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
						  closest->getType() == BWAPI::UnitTypes::Terran_Bunker ){
						if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
		                 }
					  }

					  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
					  }
					  

					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }

				  //Broodwar->printf("Attack");
				
			
			continue;
		}


	  
	}





	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistAttack =  30*32;
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		int TotalLurker = 0;
		bool OverrunLurker = false;
		//BOOST_FOREACH( Unit close, EnemyClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {

			if( Squads[0].Marines[i]->getDistance( close->getPosition() ) > MaxDistAttack){
				continue;
			}
			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				//&&  close->isBurrowed()
				TotalLurker++;
				if( ClosestBurrowedLurker > Squads[0].Marines[i]->getDistance( close->getPosition() )
				){
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
			if( close->getType() == BWAPI::UnitTypes::Zerg_Larva){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Egg){
				continue;
			}

			if( close->getType() == BWAPI::UnitTypes::Zerg_Queen && Squads[0].Marines[i]->getDistance( close->getPosition() ) > 4*32 ){
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


		for(int j=0; j<ELurkerPositions.size(); j++){
			if( ELurkerPositions[j].position.getDistance( Squads[0].Marines[i]->getPosition() ) < 8*32 ){
				TotalLurker++;
				if( ClosestBurrowedLurker > Squads[0].Marines[i]->getDistance( ELurkerPositions[j].position )
				){
					ClosestBurrowedLurker = Squads[0].Marines[i]->getDistance( ELurkerPositions[j].position );
				}
			}
		}


		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40
			&& closeDist < 6*32 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3;

		
		if( totSunk * SunkMulti < ourScore){
			Sunken = false;
		}
		

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

		if( (Squads[0].Marines.size() > TotalLurker*3 && TotalLurker < 4)
			
			||  LurkerCloseToTank == true
			
			){
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
		if ( ( LanLatency == true ||  isUnderDS ) 
			&& (   ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 && LanLatency == true )
			||  (ClosestBurrowedLurker < 9*32  && OverrunLurker == false  )
	        ||  Sunken == true 
			||  isUnderDS  )  )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::Move){
		      Squads[0].Marines[i]->move( fleePosition );
		  }
		  //Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }



		int CloseToTankDist = 10*32;

		//if the marine is to far away from a tank, move back to the tank
		bool TooFarFromTank = false;

		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Marines[i]->getPosition() ) > CloseToTankDist
				&&  tankClosestAttack->getDistance( Squads[0].Marines[i]->getPosition() ) < (CloseToTankDist + 5*32) ){
				 TooFarFromTank = true;
			}
		}
		if( TooFarFromTank == true ){

		 if( tankClosestAttack != NULL ){
			 Squads[0].Marines[i]->move( tankClosestAttack->getPosition() );
	  	  continue;
		  }
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
		if( closest == NULL ||  Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].Marines[i]->getPosition().getDistance( attackPos ) > 5*32 ){
                  if( closest != NULL ){
					  if(  closeDist > 6*32  ){
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

					  //prevent stoppage at the ramp by attacking the new location once the main base is gone
					  if (Hunt == true) {
						  if (Squads[0].Marines[i]->getOrderTargetPosition() != attackPos) {
							  Squads[0].Marines[i]->attack(attackPos);
						  }
					  }

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


	BWAPI::Unit closestMarine = NULL;
	int closestMarineDist = 99999;

	BOOST_FOREACH( Unit close, Squads[0].Marines )
              {
				  if( close->getPosition().getDistance( attackPos ) < closestMarineDist ){
			         closestMarine = close;
			         closestMarineDist = close->getPosition().getDistance( attackPos );
				  }
		}

	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 10*32 );
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
				if( ClosestBurrowedLurker > Squads[0].Medics[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = Squads[0].Medics[i]->getDistance( close->getPosition() );
				}
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3 + Squads[0].Goliaths.size()*2;
		
		if( totSunk * SunkMulti < ourScore){
			Sunken = false;
		}
		

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

		if( Squads[0].Marines.size() > TotalLurker*4 ){
			OverrunLurker = true;
		}

	  // if we can't shoot, run away  if(kite)
		// LanLatency == true &&
	  if (   ( ( ( Squads[0].Medics[i]->getHitPoints() < 20 ) || Sunken == true
		  ||  isUnderDS
		  || (ClosestBurrowedLurker < 8*32  && OverrunLurker == false  ) ) 
		  
		  &&  Squads[0].Medics[i]->getDistance( InfoMan->PosOurBase ) > 3*32 ) )
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
				  Squads[0].Medics[i]->move(  closestMarine->getPosition() );//regular move
				  continue;
			  }
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].Medics[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].Medics[i]->move(  attackPos );//regular move
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





	for(unsigned int i=0; i<Squads[0].Goliaths.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Goliaths[i]->getUnitsInRadius( 11*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord
				&& Squads[0].Goliaths[i]->getDistance( close->getPosition() ) > 5*32 ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Goliaths[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Goliaths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
		
		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3 + Squads[0].Goliaths.size()*2;

		if( totSunk * SunkMulti < ourScore){
			Sunken = false;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 5*32 &&  Squads[0].Goliaths[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
	        ||  Sunken == true
			||  isUnderDS )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		 if( Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::Move){
		  Squads[0].Goliaths[i]->move( fleePosition );
		}
		continue;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Goliaths[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].Goliaths[i]->getPosition().getDistance( attackPos ) > 6*32 ){
                  if( closest != NULL ){
					  if( Squads[0].Goliaths[i]->getOrderTarget() == NULL ){
                        Squads[0].Goliaths[i]->attack(  closest );
					  }
					  else if( Squads[0].Goliaths[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Goliaths[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Goliaths[i]->attack(  attackPos );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}


	  
	}


		for(unsigned int i=0; i<Squads[0].SCVs.size(); i++){


		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		int MaxDistAttack = 10*32;
		Unitset UnitsClose = Squads[0].SCVs[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		//BOOST_FOREACH( Unit close, UnitsClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if( Squads[0].SCVs[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].SCVs[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].SCVs[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3 + Squads[0].Goliaths.size()*2;

		if( totSunk * SunkMulti < ourScore){
			Sunken = false;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		/*
	  // if we can't shoot, run away  if(kite)
		if (   Sunken == true || closeDist < 6*32
		    ||  isUnderDS )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  		  if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::Move){
		   Squads[0].SCVs[i]->move( fleePosition );
				  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }
	  */


	  //check if a unit nearby needs healing
	  Unit needsHeal = NULL;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->self() 
				&& close->getHitPoints() != close->getType().maxHitPoints()
				&& ( close->getType() == BWAPI::UnitTypes::Terran_Goliath 
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode) ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL &&  !Squads[0].SCVs[i]->isRepairing() ){
			Squads[0].SCVs[i]->repair( needsHeal );
			continue;
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
			  Unit closestUnit = NULL;
			  int closeDist = 999999;


					  //prevent spam
					  if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].SCVs[i]->attack(  attackPos );

					  }
                /*
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].SCVs[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].SCVs[i]->move(  attackPos );//regular move
				}
				*/

			}
			continue;
		}


		}


	  for(unsigned int i=0; i< Squads[0].ScienceVessels.size(); i++){

		  int MaxDistAttack = 15*32;
		Unitset UnitsClose = Squads[0].ScienceVessels[i]->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		Unit closestRangeAttack = NULL;
		int closeDistRangeAttack = 9999999;
		Unit closestIr = NULL; //who to irradiate?
		int closeDistIr = 9999999; //who to irradiate?
		int totSunk = 0;

		//BOOST_FOREACH( Unit close, UnitsClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if( Squads[0].ScienceVessels[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				continue;
			}

			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].ScienceVessels[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].ScienceVessels[i]->getDistance( close->getPosition() );
				closest = close;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDistRangeAttack > Squads[0].ScienceVessels[i]->getDistance( close->getPosition() ) 
				&& ( close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk
				||  close->getType() == BWAPI::UnitTypes::Zerg_Hydralisk
				||  close->getType() == BWAPI::UnitTypes::Zerg_Scourge
				||  close->getType() == BWAPI::UnitTypes::Zerg_Devourer )
				
				){
				closeDistRangeAttack = Squads[0].ScienceVessels[i]->getDistance( close->getPosition() );
				closestRangeAttack = close;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDistIr > Squads[0].ScienceVessels[i]->getDistance( close->getPosition() )
				&& ( close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk
				||  close->getType() == BWAPI::UnitTypes::Zerg_Lurker
				||  close->getType() == BWAPI::UnitTypes::Zerg_Ultralisk
				||  close->getType() == BWAPI::UnitTypes::Zerg_Defiler )  ){
				closeDistIr = Squads[0].ScienceVessels[i]->getDistance( close->getPosition() );
				closestIr = close;
			}

		}


		/*
		for(int j=0; j<ELurkerPositions.size(); j++){
			if( ELurkerPositions[j].position.getDistance( Squads[0].ScienceVessels[i]->getPosition() ) < 10*32 ){
				if( ELurkerPositions[j].position !
			}
		}
		*/


		if( closestIr != NULL && Squads[0].ScienceVessels[i]->getEnergy() > 75 ){
			if( Squads[0].ScienceVessels[i]->getOrder() != BWAPI::Orders::CastIrradiate
				&& !closestIr->isIrradiated() ){
				Squads[0].ScienceVessels[i]->useTech( BWAPI::TechTypes::Irradiate, closestIr);
				continue;
			}
		}

		//might as well use it if no lurker is available
		if( closest != NULL && Squads[0].ScienceVessels[i]->getEnergy() > 150 ){
			if( Squads[0].ScienceVessels[i]->getOrder() != BWAPI::Orders::CastIrradiate
				&& !closest->isIrradiated() ){
				Squads[0].ScienceVessels[i]->useTech( BWAPI::TechTypes::Irradiate, closest);
				continue;
			}
		}




		if( closestRangeAttack == NULL && closeDistRangeAttack > 5*32){
			if( Squads[0].Marines.size() > 0){
				int MarineDistClosest = 99999;
				BWAPI::Unit ClosestMarine = NULL; 
				for( int j=0; j<Squads[0].Marines.size(); j++){
					if( Squads[0].Marines[j]->getDistance( attackPos ) < MarineDistClosest){
						MarineDistClosest = Squads[0].Marines[j]->getDistance( attackPos );
						ClosestMarine = Squads[0].Marines[j];
					}
				}
				if( ClosestMarine != NULL ){
						//if( Squads[0].ScienceVessels[i]->getOrderTargetPosition() != ClosestMarine->getPosition() ){
					//prevent spamming the move command
					if (MarineDistClosest > 2 * 32) {
						Squads[0].ScienceVessels[i]->move(ClosestMarine->getPosition());
					}
						//}
				} else {
					if( Squads[0].ScienceVessels[i]->getOrderTargetPosition() != attackPos ){
                        Squads[0].ScienceVessels[i]->move( attackPos );
					}
				}
			} else {

				if( Squads[0].ScienceVessels[i]->getOrderTargetPosition() != attackPos ){
			       Squads[0].ScienceVessels[i]->move( attackPos );
				}
			}

			continue;
		}

		if(  closeDistRangeAttack < 5*32 && closestRangeAttack != NULL ){
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
          if( Squads[0].ScienceVessels[i]->getOrderTargetPosition() != fleePosition ){
		    Squads[0].ScienceVessels[i]->move( fleePosition );
		  }
		  continue;
		}
		/*
		if(  closeDist < 5*32 && closest != NULL ){
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
          if( Squads[0].ScienceVessels[i]->getOrderTargetPosition() != fleePosition ){
		    Squads[0].ScienceVessels[i]->move( fleePosition );
		  }
		  continue;
		}
		*/
	}


	for(unsigned int i=0; i<Squads[0].BattleCruisers.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].BattleCruisers[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() || !close->isDetected() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].BattleCruisers[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].BattleCruisers[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
		
		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].BattleCruisers[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].BattleCruisers[i]->getPosition().getDistance( attackPos ) > 6*32 ){
                  if( closest != NULL ){
					  if( Squads[0].BattleCruisers[i]->getOrderTarget() == NULL ){
                        Squads[0].BattleCruisers[i]->attack(  closest );
					  }
					  else if( Squads[0].BattleCruisers[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].BattleCruisers[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackUnit &&
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AtkMoveEP ){

		                  Squads[0].BattleCruisers[i]->attack(  attackPos );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}
	  
	  // if we can't shoot, run away  if(kite)

		//if ( ( closeDist < 5*32 && closest != NULL  && LanLatency == true &&  Squads[0].BattleCruisers[i]->getGroundWeaponCooldown() != 0 ) 
		//	||  isUnderDS )
	  //{
		if (   isUnderDS )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  if(  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::Move ){
		     Squads[0].BattleCruisers[i]->move( fleePosition );
		  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }

	}




	for(unsigned int i=0; i<Squads[0].Vultures.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Vultures[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		int ClosestBurrowedLurker = 99999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {

			//a lurker
			if(  !close->isDetected() || close->getType() == BWAPI::UnitTypes::Zerg_Lurker ){
				if( ClosestBurrowedLurker > Squads[0].Vultures[i]->getDistance( close->getPosition() ) ){
					ClosestBurrowedLurker = Squads[0].Vultures[i]->getDistance( close->getPosition() );
				}
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord
				&& Squads[0].Vultures[i]->getDistance( close->getPosition() ) > 5*32 ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
		
		int ourScore = Squads[0].Marines.size() + Squads[0].Medics.size() * 3 + Squads[0].Goliaths.size()*2;

		if( totSunk * 20 < ourScore){
			Sunken = false;
		}
		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}
	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 5*32 &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 ) 
	        ||  Sunken == true
			||  isUnderDS
			|| ClosestBurrowedLurker < 8*32)
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );      
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		Squads[0].Vultures[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }
		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Vultures[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].Vultures[i]->getPosition().getDistance( attackPos ) > 6*32 ){
                  if( closest != NULL ){
					  if( Squads[0].Vultures[i]->getOrderTarget() == NULL ){
                        Squads[0].Vultures[i]->attack(  closest );
					  }
					  else if( Squads[0].Vultures[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Vultures[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Vultures[i]->attack(  attackPos );

					  }
				  }
				  //Broodwar->printf("Attack");
				}			
			continue;
		}	  
	}






}


void MacroManager::OnFrameZergSquadDefenceChoke(int id){

	int TimeToSplit = 9000;


	if (MutaBuild == false) {

		BOOST_FOREACH(Unit close, Broodwar->enemy()->getUnits())
		{
			if (close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk ) {
				MutaBuild = true;
			}
		}

	}

	//BWAPI::Position DefaultPos =  chokePos;
	BWAPI::Position DefaultPos =  InfoMan->PosMainChoke;
	if(
		(ZState == Z_Expand_Defend || ZState == Z_Expand_Defend_2_hatch || ZState == Z_Expand_Defend_3_hatch)
		
		){
		if( NatBunkerPlace == true && CCmanager.size() > 1 &&  CCmanager[1]->BaseReady == true ){
          DefaultPos = InfoMan->PosOurNat;
		} else {


			int FurthestChoke = -1;
			BWAPI::Position FurthestPos = BWAPI::Positions::Unknown; 
		BWTA::Region* NatReg = BWTA::getRegion(BWAPI::TilePosition(InfoMan->PosOurNat));
		BOOST_FOREACH( BWTA::Chokepoint* choke, NatReg->getChokepoints() ){
			if( choke->getRegions().first != InfoMan->OurMainregion
				&& choke->getRegions().second != InfoMan->OurMainregion){
				// TODO:  get ground distance
					if(   BWAPI::TilePosition( choke->getCenter() ).getApproxDistance(InfoMan->OurBase)   > FurthestChoke ){
					    FurthestChoke = BWAPI::TilePosition(choke->getCenter()).getApproxDistance(InfoMan->OurBase);
						DefaultPos = choke->getCenter();
					}
			}
		  }

			/*
		  BWTA::Region* NatReg = BWTA::getRegion( InfoMan->PosOurNat);
		  BOOST_FOREACH( BWTA::Chokepoint* choke, NatReg->getChokepoints() ){
			if( choke->getRegions().first != InfoMan->OurMainregion
				&& choke->getRegions().second != InfoMan->OurMainregion){
					DefaultPos = choke->getCenter();
			}
		  }
		  */


		}
	}
	//BWAPI::Position DefaultPos =  WDMan->ToRetreat;

	/*
	if( ZState == Z_Pool_Defend){
		if( Squads[0].SCVs.size() < 6){
		  //Broodwar->printf("Plugging ramp");
		  AddSCV( CCmanager[0]->getFullHPSCV() );
		}
	}
	*/



	if( ZState == Z_Plug_Ramp ){

		//get 4 SCvs to plug the ramp
		if (Squads[0].SCVs.size() < 4) {
			//Squads[0].SCVs.push_back(CCmanager[0]->getFullHPSCV());
				//CCmanager[0]->getFullHPSCV();
		}

	  for(unsigned int i=0; i< Squads[0].SCVs.size(); i++){

		Unitset UnitsClose = Squads[0].SCVs[i]->getUnitsInRadius( 10*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].SCVs[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].SCVs[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		if( closest  != NULL && closeDist < 2 * 32 && Squads[0].SCVs[i]->getDistance( InfoMan->PosMainChoke ) < 2*32 ){
			if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackMove
				&& Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackTile
				&& Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackUnit){
					Squads[0].SCVs[i]->attack( closest );
			}
		} else {
			Squads[0].SCVs[i]->move( InfoMan->PosMainChoke );
		}

	  }
	}



	for(unsigned int i=0; i<Squads[0].Goliaths.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = Squads[0].Goliaths[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Goliaths[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Goliaths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( ( closeDist < 4*32
			&& Squads[0].Goliaths[i]->getDistance( WDMan->ToRetreat ) > 1*32 ) )
	  {

		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Goliaths[i]->move( fleePosition );
		     continue;

		//Marines[i].state = M_Fleeing;
	  }

		if( Squads[0].Goliaths[i]->getGroundWeaponCooldown() == 0  ){
                  if( closest != NULL ){
					  if( Squads[0].Goliaths[i]->getOrderTarget() == NULL ){
                        Squads[0].Goliaths[i]->attack(  closest );
					  }
					  else if( Squads[0].Goliaths[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Goliaths[i]->attack(  closest );
					  }
					  continue;
				  }
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			//get to the bunkaa!!!
			BWAPI::Position BaseMove = DefaultPos;
			if( Broodwar->getFrameCount() > 8000 && i % 2 == 1 ){
				BaseMove = InfoMan->PosOurBase;
			}

				if( Squads[0].Goliaths[i]->getPosition().getDistance( BaseMove ) > 4*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Squads[0].Goliaths[i]->attack(  BaseMove );
					  }
				  //Broodwar->printf("Attack");
				}			
			continue;
		}	  
	}



	for(unsigned int i=0; i<Squads[0].Vultures.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = Squads[0].Vultures[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( ( closeDist < 4*32
			&& Squads[0].Vultures[i]->getDistance( WDMan->ToRetreat ) > 1*32 ) )
	  {

		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Vultures[i]->move( fleePosition );
		     continue;

		//Marines[i].state = M_Fleeing;
	  }

		if( Squads[0].Vultures[i]->getGroundWeaponCooldown() == 0  ){
                  if( closest != NULL ){
					  if( Squads[0].Vultures[i]->getOrderTarget() == NULL ){
                        Squads[0].Vultures[i]->attack(  closest );
					  }
					  else if( Squads[0].Vultures[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Vultures[i]->attack(  closest );
					  }
					  continue;
				  }
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
			BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			//get to the bunkaa!!!

				if( Squads[0].Vultures[i]->getPosition().getDistance( attackPos ) > 4*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Squads[0].Vultures[i]->attack(  attackPos );
					  }
				  //Broodwar->printf("Attack");
				}			
			continue;
		}	  
	}





	//get maximum distance from natural base to bunker
	//dont go further than this in case of lurkers
	int MaxDistToBunker = 0;
	for(unsigned int i=0; i<Squads[0].Bunkers.size(); i++){

		if(CCmanager.size() > 1 ){
			//check if it is a bunker close to the natural
			if(  Squads[0].Bunkers[i]->getDistance( CCmanager[1]->CommandCenter->getPosition()  ) < 15*32 ){
				if( Squads[0].Bunkers[i]->getDistance( CCmanager[1]->CommandCenter->getPosition()  ) > MaxDistToBunker ){
					MaxDistToBunker = Squads[0].Bunkers[i]->getDistance( CCmanager[1]->CommandCenter->getPosition()  );
				}
			}
		}

	}

	if( MaxDistToBunker  > 0 ){
		MaxDistToBunker -= 32;
	}

	bool BunkerInDanger = false;
	for (unsigned int i = 0; i < Squads[0].Bunkers.size(); i++) {
		Unitset EnemyClose = Squads[0].Bunkers[i]->getUnitsInRadius(20 * 32);
		BOOST_FOREACH(Unit close, EnemyClose)
		{

			if (close->getPlayer() != Broodwar->enemy()) {
				continue;
			}

			if (close->getDistance(Squads[0].Bunkers[i]) < 5 * 32) {
				BunkerInDanger = true;
			}
		}


	}


	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {

			if (close->getType() == BWAPI::UnitTypes::Zerg_Drone) {
				continue;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		int closestBurrowedLurker = 99999;
		BOOST_FOREACH(LurkerPos close, ELurkerPositions)
		{
			int lurkerDist = Squads[0].Marines[i]->getDistance(close.position);
			if (lurkerDist  < closestBurrowedLurker) {
				closestBurrowedLurker = lurkerDist;
			}
		}

		//move away from lurkers that are burrowed
		if (closestBurrowedLurker < 10*32 && BunkerInDanger == false)
		{
			BWAPI::Position fleePosition(WDMan->ToRetreat);
			Squads[0].Marines[i]->move(fleePosition);
			continue;
		}


		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40
			&& closeDist < 5*32 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}




		// make space for a bunker if no zergs are nearby
		// && (ZState == Z_Plug_Ramp || ZState ==  Z_Choke_Defend)
		// DefaultPos
        // Squads[0].Marines[i]->getDistance(InfoMan->PosMainChoke) < 6 * 32
		if ( (closest == NULL  || closeDist > 15*32 ) 
			&& ProdMan->TotalTypeInQueue(BWAPI::UnitTypes::Terran_Bunker) > 0
			&& bManager->BuildMap[Squads[0].Marines[i]->getTilePosition().x][Squads[0].Marines[i]->getTilePosition().y] == BLOCKED 
			&&   Squads[0].Marines[i]->getDistance(DefaultPos ) < 6 * 32
			)
		{
			BWAPI::Position fleePosition(WDMan->ToRetreat);
			Squads[0].Marines[i]->move(fleePosition);
			continue;
		}



		int totalBunkNat = 0;
		bool moveToBunk = false;
		BWAPI::Unit ChokeBunker = NULL;
		for(int b=0; b<Squads[0].Bunkers.size(); b++){

			if( BWTA::getRegion(BWAPI::TilePosition(Squads[0].Bunkers[b]->getPosition()) ) == InfoMan->OurMainregion
				&& ZState != Z_Pool_Defend
				&&  Squads[0].Bunkers[b]->getPosition().getDistance( CCmanager[0]->CommandCenter->getPosition() ) < 7*32  ){
					if( Squads[0].Bunkers[b]->getLoadedUnits().size() > 0 ){
					  Squads[0].Bunkers[b]->unload( Squads[0].Marines[i] );
					}
			}

			if (BWTA::getRegion(BWAPI::TilePosition(Squads[0].Bunkers[b]->getPosition())) == InfoMan->OurMainregion
				&& (ZState == Z_Plug_Ramp || ZState == Z_Choke_Defend )
				&&  Squads[0].Bunkers[b]->getPosition().getDistance( InfoMan->PosMainChoke) < 7 * 32) {
				if (i < 4 ) {
					Squads[0].Marines[i]->rightClick(Squads[0].Bunkers[b]);
					moveToBunk = true;
					ChokeBunker = Squads[0].Bunkers[b];
					//continue;
				}
			}



			if(  BWTA::getRegion(BWAPI::TilePosition(Squads[0].Bunkers[b]->getPosition() )) != InfoMan->OurMainregion ){
				totalBunkNat++;
				if(  i < 4  &&  totalBunkNat == 1 ){
			      Squads[0].Marines[i]->rightClick( Squads[0].Bunkers[b] );
				  moveToBunk = true;
			      //continue;
				}
				if(  i >= 4 && i < 8  &&  totalBunkNat == 2 && MutaBuild == false ){
			      Squads[0].Marines[i]->rightClick( Squads[0].Bunkers[b] );
				  moveToBunk = true;
			      //continue;
				}
				if (i >= 4 && i < 8 && totalBunkNat == 2 && MutaBuild == true) {
					BOOST_FOREACH(BWAPI::Unit marines, Squads[0].Bunkers[b]->getLoadedUnits()) {
						if (Squads[0].Marines[i]->getID() == marines->getID()) {
							Squads[0].Bunkers[b]->unloadAll();

						}
					}
				}

			}

		}

		if( moveToBunk ){
			continue;
		}
		
		/*
		//get out of the bunker
		if( closest != NULL && closeDist > 4*32 
			&& ZState == Z_Pool_Defend  ){
			if( Squads[0].Bunkers.size() > 0){
			  Squads[0].Bunkers[0]->unload( Squads[0].Marines[i] );
			}
		}


		//get to the bunkaa!!!
		if( NatBunkerChoke != NULL && i < 4 ){
			//Squads[0].Bunkers[0]->load( Squads[0].Marines[i] );
			Squads[0].Marines[i]->rightClick( NatBunkerChoke );
			continue;
		} 
		*/


	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0
			&& Squads[0].Marines[i]->getDistance( WDMan->ToRetreat ) > 2*32 ) )
	  {
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 
		     Squads[0].Marines[i]->move( fleePosition );
		     continue;
	  }

		BWTA::Region* theRegion =  BWTA::getRegion(BWAPI::TilePosition(Squads[0].Marines[i]->getPosition() ));


		//while defending 1 base, don't go beyond
		if ((ZState == Z_Plug_Ramp || ZState ==  Z_Choke_Defend)
			&& theRegion != InfoMan->OurMainregion)
		{

			BWAPI::Position fleePosition(WDMan->ToRetreat);
			Squads[0].Marines[i]->move(fleePosition);
			continue;
		}

		if (
			(ZState == Z_Plug_Ramp || ZState == Z_Choke_Defend)
			&&
			ChokeBunker != NULL && closest != NULL
			) {
			if (BWTA::getRegion(BWAPI::TilePosition(closest->getPosition()))  != InfoMan->OurMainregion
				 && closeDist > 4*32  ) {
				Broodwar->printf("Move back to choke bunker");
				Squads[0].Marines[i]->move(ChokeBunker->getPosition() );
				continue;
			}
			
		}

		//&& BunkerInDanger == false
		if ( ( Squads[0].Marines[i]->getDistance( WDMan->ToRetreat ) > 20*32 )
			&& ( theRegion != InfoMan->OurMainregion && theRegion != BWTA::getRegion(BWAPI::TilePosition(InfoMan->PosOurNat) )  )
			&&  Squads[0].Marines[i]->getPosition().getDistance( InfoMan->PosMainChoke ) > 15*32
			&& BunkerInDanger == false
			)
	  {

 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 
		     Squads[0].Marines[i]->move( fleePosition );
		     continue;
	  }





		if( CCmanager.size() > 1 ){

			//dont let marine get outside of the bunker
			if(  MaxDistToBunker != 0 ){
				//Broodwar->printf("Max dist bunker not 0   it is  %d", MaxDistToBunker);
		     if (  theRegion != InfoMan->OurMainregion  
				   && theRegion != InfoMan->NatRegion
			       &&  Squads[0].Marines[i]->getPosition().getDistance(  CCmanager[1]->CommandCenter->getPosition()  ) > MaxDistToBunker 
				   &&  Squads[0].Marines[i]->getPosition().getDistance( InfoMan->PosMainChoke ) > 6*32 
				   &&  BunkerInDanger == false
				   )
	          {
 		        BWAPI::Position fleePosition( WDMan->ToRetreat ); 
		        Squads[0].Marines[i]->move( fleePosition );
		        continue;
	          }
			}
		}




		if( Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){
                  if( closest != NULL ){


					  //prevent spam
					  
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
					  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile &&
					  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

					    Squads[0].Marines[i]->attack(closest->getPosition() );

					  }
					  

					  /*
					  if( Squads[0].Marines[i]->getOrderTarget() == NULL ){
                        Squads[0].Marines[i]->attack(  closest );
					  }
					  else if( Squads[0].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Marines[i]->attack(  closest );
					  }
					  continue;
					  */

				  }
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			//get to the bunkaa!!!

			BWAPI::Position BaseMove = DefaultPos;
			if( Broodwar->getFrameCount() > TimeToSplit  ){
				if( i % 4 == 1 ){
				BaseMove = InfoMan->PosOurBase;
			} else {
				if( CCmanager.size() > 1  ){
					if( CCmanager[1]->Minerals.size() > 0){
						BaseMove =  CCmanager[1]->Minerals[ rand() % CCmanager[1]->Minerals.size() ].mineralPatch->getPosition();
					} else {
						BaseMove = DefaultPos;
					}
				}
				else if((ZState == Z_Plug_Ramp || ZState ==  Z_Choke_Defend)) {

					for (int b = 0; b<Squads[0].Bunkers.size(); b++) {
						if (BWTA::getRegion(BWAPI::TilePosition(Squads[0].Bunkers[b]->getPosition())) == InfoMan->OurMainregion
							&& (ZState == Z_Plug_Ramp || ZState ==  Z_Choke_Defend)
							&& Squads[0].Bunkers[b]->getPosition().getDistance(InfoMan->PosMainChoke) < 7 * 32) {
								BaseMove = Squads[0].Bunkers[b]->getPosition();
								//continue;
						 }
						}
					}
						//BaseMove = InfoMan->PosOurBase;

				}
			
			}

				if( Squads[0].Marines[i]->getPosition().getDistance( BaseMove ) > 4*32 ){

					if (Squads[0].Marines[i]->getOrder() != BWAPI::Orders::Move) {
						Squads[0].Marines[i]->move(BaseMove);

					}

			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  /*
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Marines[i]->attack(  BaseMove );

					  }
					  */
				  //Broodwar->printf("Attack");
				}			
			continue;
		}	  
	}

	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}



		int closestBurrowedLurker = 99999;
		BOOST_FOREACH(LurkerPos close, ELurkerPositions)
		{
			int lurkerDist = Squads[0].Medics[i]->getDistance(close.position);
			if (lurkerDist  < closestBurrowedLurker) {
				closestBurrowedLurker = lurkerDist;
			}
		}

		//move away from lurkers that are burrowed
		if (closestBurrowedLurker < 10 * 32)
		{
			BWAPI::Position fleePosition(WDMan->ToRetreat);
			Squads[0].Medics[i]->move(fleePosition);
			continue;
		}


	  // if we can't shoot, run away  if(kite)
	  if (  ( Squads[0].Medics[i]->getHitPoints() < 15 )  )
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

			BWAPI::Position BaseMove = DefaultPos;
			if( Broodwar->getFrameCount() > TimeToSplit  ){
				if(  i % 3 == 1 ){
				BaseMove = InfoMan->PosOurBase;
			} else {
				if( CCmanager.size() > 1  ){
					if( CCmanager[1]->Minerals.size() > 0){
						BaseMove =  CCmanager[1]->Minerals[ rand() % CCmanager[1]->Minerals.size() ].mineralPatch->getPosition();
					} else {
						BaseMove = DefaultPos;
					}
				}
			}
			}
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].Medics[i]->getPosition().getDistance( BaseMove ) > 6*32 ){
			      Squads[0].Medics[i]->attack(  BaseMove );//regular move
				  continue;
				}
			}
			continue;
		}
 
	}










	for(unsigned int i=0; i< Squads[0].ScienceVessels.size(); i++){

		Unitset UnitsClose = Squads[0].ScienceVessels[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		Unit AMuta = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk ){
				AMuta = close;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].ScienceVessels[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].ScienceVessels[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		if( AMuta  != NULL && Squads[0].ScienceVessels[i]->getEnergy() > 75){
			if( Squads[0].ScienceVessels[i]->getOrder() != BWAPI::Orders::CastIrradiate
				&& !AMuta->isIrradiated() ){
			  Squads[0].ScienceVessels[i]->useTech( BWAPI::TechTypes::Irradiate, AMuta);
			}
		} else {
			Squads[0].ScienceVessels[i]->move(DefaultPos);
		}

	}

	
	for(unsigned int i=0; i<Squads[0].Tanks.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType().isFlyer()  ){
				continue;
			}

			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		BWTA::Region* theRegion =  BWTA::getRegion( Squads[0].Tanks[i]->getTilePosition() );

		if ( ( Squads[0].Tanks[i]->getDistance( WDMan->ToRetreat ) > 20*32 )
			&& ( theRegion != InfoMan->OurMainregion && theRegion != BWTA::getRegion(BWAPI::TilePosition(InfoMan->PosOurNat ) )  ) )
	  {

 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 
		     Squads[0].Tanks[i]->move( fleePosition );
		     continue;
	  }



		if( closest == NULL ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		} else if( closeDist > 12*31 && closest != NULL ){  // 12*32
		//unsiege if there are no attacking buildings
        if( closest->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony &&
			closest->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon &&
			closest->getType() != BWAPI::UnitTypes::Terran_Bunker ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		}
	  }

		if( closest == NULL ){
			Squads[0].Tanks[i]->attack( DefaultPos );
			continue;
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 5*32 &&  Squads[0].Tanks[i]->getGroundWeaponCooldown() != 0 )
		  &&  Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Tanks[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest != NULL  ){ //|| closeDist > 3*32
			if(  closeDist > 12*32 ){
					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }
				continue;
			}
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );

					  //siege up for sunken, bunker and cannon
					  if( closest->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ||
						  closest->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
						  closest->getType() == BWAPI::UnitTypes::Terran_Bunker ){
						if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
		                 }
					  }

					  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
					  }
					  

					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }

				  //Broodwar->printf("Attack");
				
			
			continue;
		}




		BWTA::Region* theRegionTank = BWTA::getRegion(Squads[0].Tanks[i]->getTilePosition());


		//while defending 1 base, don't go beyond
		if (ZState == Z_Choke_Defend
			&& theRegionTank != InfoMan->OurMainregion)
		{

			BWAPI::Position fleePosition(WDMan->ToRetreat);
			Squads[0].Tanks[i]->move(fleePosition);
			continue;
		}


	  
	}







	for(unsigned int i=0; i<Squads[0].Wraiths.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = Squads[0].Wraiths[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Wraiths[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Wraiths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( ( closeDist < 4*(32 + 5)
			&& Squads[0].Wraiths[i]->getDistance( WDMan->ToRetreat ) > 1*32 )
			 || Squads[0].Wraiths[i]->getGroundWeaponCooldown() != 0 )
	  {

		  if( closest != NULL ){

			  BWAPI::Position fleePosition( (Squads[0].Wraiths[i]->getPosition() - closest->getPosition())+(Squads[0].Wraiths[i]->getPosition() - closest->getPosition()) + Squads[0].Wraiths[i]->getPosition());
		     Squads[0].Wraiths[i]->move( fleePosition );
			 continue;
		  } else {

		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Wraiths[i]->move( fleePosition );
		     continue;

		  }

		//Marines[i].state = M_Fleeing;
	  }

		if( Squads[0].Wraiths[i]->getGroundWeaponCooldown() == 0  ){
                  if( closest != NULL ){
					  if( Squads[0].Wraiths[i]->getOrderTarget() == NULL ){
                        Squads[0].Wraiths[i]->attack(  closest );
					  }
					  else if( Squads[0].Wraiths[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Wraiths[i]->attack(  closest );
					  }
					  continue;
				  }
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			//get to the bunkaa!!!
			/*
			BWAPI::Position BaseMove = DefaultPos;
			if( Broodwar->getFrameCount() > 8000 && i % 2 == 1 ){
				BaseMove = InfoMan->PosOurBase;
			}
			*/

				if( Squads[0].Wraiths[i]->getPosition().getDistance( DefaultPos ) > 4*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					 Squads[0].Wraiths[i]->move(  DefaultPos );

				  //Broodwar->printf("Attack");
				}	
				if( Squads[0].Wraiths[i]->getPosition().getDistance( DefaultPos ) < 3*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					Squads[0].Wraiths[i]->move(  InfoMan->PosOurBase );
				  //Broodwar->printf("Attack");
				}	
			continue;
		}	  
	}




	for(unsigned int i=0; i<Squads[0].Valkryies.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );		
		Unitset EnemyClose = Squads[0].Valkryies[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Valkryies[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Valkryies[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( ( closeDist < 4*32
			&& Squads[0].Valkryies[i]->getDistance( WDMan->ToRetreat ) > 1*32 ) )
	  {

		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Valkryies[i]->move( fleePosition );
		     continue;

		//Marines[i].state = M_Fleeing;
	  }

		if( Squads[0].Valkryies[i]->getGroundWeaponCooldown() == 0  ){
                  if( closest != NULL ){
					  if( Squads[0].Valkryies[i]->getOrderTarget() == NULL ){
                        Squads[0].Valkryies[i]->attack(  closest );
					  }
					  else if( Squads[0].Valkryies[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Valkryies[i]->attack(  closest );
					  }
					  continue;
				  }
		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			//get to the bunkaa!!!

				if( Squads[0].Valkryies[i]->getPosition().getDistance( DefaultPos ) > 4*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( Squads[0].Valkryies[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Valkryies[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Valkryies[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Squads[0].Valkryies[i]->attack(  DefaultPos );
					  }
				  //Broodwar->printf("Attack");
				}			
			continue;
		}	  
	}




}

void MacroManager::OnFrameZergSquadDefence(int id){

	//BWAPI::Position DefaultPos =  chokePos;
	BWAPI::Position DefaultPos =  WDMan->ToRetreat;

	/*
	if( ZState == Z_Pool_Defend){
		if( Squads[0].SCVs.size() < 6){
		  //Broodwar->printf("Plugging ramp");
		  AddSCV( CCmanager[0]->getFullHPSCV() );
		}
	}
	*/

	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !Squads[0].Marines[i]->isStimmed() && Squads[0].Marines[i]->getHitPoints() == 40 ){
			 Squads[0].Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}


		//use stim before engaging enemy
		if( closest != NULL && closeDist > 4*32 ){
			if(  Squads[0].Bunkers.size() > 0 ){
			  Squads[0].Bunkers[0]->unload( Squads[0].Marines[i] );
			}
		}

	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( 
			(
			(closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0) 
			|| (closeDist < 3*32 && Squads[0].Bunkers.size() > 0 ) )
			&& Squads[0].Marines[i]->getDistance( WDMan->ToRetreat ) > 1*32  )
	  {

			//get to the bunkaa!!!
			if( Squads[0].Bunkers.size() > 0 ){
				//Squads[0].Bunkers[0]->load( Squads[0].Marines[i] );
				Squads[0].Marines[i]->rightClick( Squads[0].Bunkers[0]);
				continue;
			} else {

		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Marines[i]->move( fleePosition );
		     continue;
			}
		//Marines[i].state = M_Fleeing;
	  }

      if( Squads[0].Bunkers.size() > 0 ){
		  if(  Squads[0].Marines[i]->getDistance( CCmanager[0]->CommandCenter ) > 6*32 ){
			  Squads[0].Marines[i]->move( Squads[0].Bunkers[0]->getPosition() );
			  continue;
		  }
	  }


		if( Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){

                  if( closest != NULL ){
					  if( Squads[0].Marines[i]->getOrderTarget() == NULL ){
                        Squads[0].Marines[i]->attack(  closest );
					  }
					  else if( Squads[0].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Marines[i]->attack(  closest );
					  }
					  continue;
				  }

		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

			//get to the bunkaa!!!
			if( Squads[0].Bunkers.size() > 0 ){
				Squads[0].Bunkers[0]->load( Squads[0].Marines[i] );
				continue;
			}
				if( Squads[0].Marines[i]->getPosition().getDistance( DefaultPos ) > 3*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Squads[0].Marines[i]->attack(  DefaultPos );
					  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}  
	}

	for(unsigned int i=0; i< Squads[0].Medics.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

	  // if we can't shoot, run away  if(kite)
	  if (  ( Squads[0].Medics[i]->getHitPoints() < 25 )  )
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
              /*
		      BOOST_FOREACH( Unit close, Marines )
              {
				  if( close->getPosition().getDistance( InfoMan->PosEnemyBase ) < closeDist ){
			         Unit closestUnit = close;
			         int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  */
			  BOOST_FOREACH( Unit close, Squads[0].Marines )
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
				if( Squads[0].Medics[i]->getPosition().getDistance( DefaultPos ) > 6*32 ){
			      Squads[0].Medics[i]->attack(  DefaultPos );//regular move
				  continue;
				}

			}

			continue;
		}
  
	}

	for(unsigned int i=0; i< Squads[0].SCVs.size(); i++){
		/*
		if( Squads[0].SCVs[i]->getDistance(  InfoMan->PosMainChoke ) > 3*32 ){
			Squads[0].SCVs[i]->move(  InfoMan->PosMainChoke );
			continue;
		}
		*/

		if( Squads[0].SCVs[i]->getDistance(  InfoMan->PosMainChoke ) <= 2*32 ){

			//Squads[0].SCVs[i]->attack(  InfoMan->PosMainChoke );
			//continue;
			
			if( !Squads[0].SCVs[i]->isHoldingPosition() ){
			  Squads[0].SCVs[i]->holdPosition();
			  continue;
			}
			
		}
		
		Squads[0].SCVs[i]->move(  InfoMan->PosOurNat );

	}

}

void MacroManager::OnFrameZergSquad(){

	for(int i=0; i<Squads.size(); i++){
		if( Squads[i].SQstate == SQ_Defend_Base ){
			if( ZState == Z_Init || ZState == Z_Pool_Defend){
			  OnFrameZergSquadDefence(i);
			}
			if( ZState == Z_Choke_Defend || ZState == Z_Plug_Ramp 
				|| ZState ==  Z_SparksTerran
				|| (ZState == Z_Expand_Defend || ZState == Z_Expand_Defend_2_hatch || ZState == Z_Expand_Defend_3_hatch)
				
				){
				OnFrameZergSquadDefenceChoke(i);
			}
		}
		if( Squads[i].SQstate == SQ_Attack_Base ){
			OnFrameZergSquadAttack(i);
		}
	}

}


void MacroManager::OnFrameZergFlowChartPlugRamp()
{


	ZState = Z_Plug_Ramp;
	ProdMan->removeFromBuildOrder(BWAPI::UnitTypes::Terran_Command_Center);
	//add a bunker to the main choke
	//ProdMan->addToQueueClose(BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition(InfoMan->PosMainChoke), InfoMan->OurMainregion);

	//get turrets up, get stim+medics, be safe vs mutalisks + lurkers
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Engineering_Bay);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Refinery);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Academy);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Factory);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Starport);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Science_Facility);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Command_Center);


}


void MacroManager::OnFrameZergFlowChartSparksTerran()
{
	CCmanager[0]->SCVsonGas = 3;

	OnFrameZergFlowChartPlugRamp();
	return;

	ZState = Z_SparksTerran;
	ProdMan->removeFromBuildOrder(BWAPI::UnitTypes::Terran_Command_Center);
	//add a bunker to the main choke
	//ProdMan->addToQueueClose(BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition(InfoMan->PosMainChoke), InfoMan->OurMainregion);

	//get turrets up, get stim+medics, be safe vs mutalisks + lurkers
	//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks);
	//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Engineering_Bay);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Academy);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Refinery);
	//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);
	//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Academy);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks);
	ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot);


}




void MacroManager::OnFrameZergFlowChart() {





	//check to see if we should protect vs 4/5 pool
	// && CurrentStrat != "2 Port Wraith"
	int Frames4Bunker = 4200; //total frames to decide plug ramp or bunker vs early pool
							  //		if( ZState == Z_Init && Broodwar->getFrameCount() <= Frames4Bunker ){
	if (ZState == Z_Init && Broodwar->getFrameCount() <= Frames4Bunker) {
		bool GetRax = false;
		//check to see if we need spawning pool defence
		BOOST_FOREACH(Unit enemy, Broodwar->enemy()->getUnits()) {
			if (enemy->getType() == BWAPI::UnitTypes::Zerg_Zergling) {
				ZState = Z_Pool_Defend;
				GetRax = true;
				EarlyPool = true;
				//ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 20);
				//ProdMan->addToQueue(BWAPI::UnitTypes::Terran_Bunker);
				//ProdMan->removeFromQueue(BWAPI::UnitTypes::Terran_Command_Center);
			}
			if (enemy->getType() == BWAPI::UnitTypes::Zerg_Spawning_Pool
				&& !enemy->isConstructing()) {
				ZState = Z_Pool_Defend;
				GetRax = true;
				EarlyPool = true;
				//ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 20);
				//ProdMan->addToQueue(BWAPI::UnitTypes::Terran_Bunker);
				//ProdMan->removeFromQueue(BWAPI::UnitTypes::Terran_Command_Center);
			}
		}

		int TotalHatch = 0;
		for (int b = 0; b<InfoMan->EnemyBuildings.size(); b++) {
			if (InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Zerg_Hatchery) {
				TotalHatch++;
			}
		}
		//dont use, might be some 9/10 hatch into 2 hatch ling allin
		if (TotalHatch >= 2) { //12 hatch
			/*
			GetRax = false;
			EarlyPool = false;
			ZState = Z_Expand_Defend;
			*/
		}
		if (GetRax == true) {
			ProdMan->BuildingSupplyOrder.clear();
			//ProdMan->addToQueue( BWAPI::UnitTypes::Terran_Bunker);
			//ProdMan->addToQueueBegin( BWAPI::UnitTypes::Terran_Barracks);

			//ProdMan->addToQueueBegin( BWAPI::UnitTypes::Terran_Bunker);

			//place bunker between refinery and CC
			BWAPI::TilePosition BunkerCloseRef = BWAPI::TilePositions::Unknown;
			if (CCmanager.size() > 0 && CCmanager[0]->Geysers.size() > 0) {
				Broodwar->printf("Checking bunker location");
				BWAPI::TilePosition GeyserTile = CCmanager[0]->Geysers[0]->getTilePosition();

				if (CCmanager[0]->LandLocation.y < GeyserTile.y - 2) { //ref is down
					BunkerCloseRef = BWAPI::TilePosition(CCmanager[0]->LandLocation.x, CCmanager[0]->LandLocation.y + 3);
				}
				if (CCmanager[0]->LandLocation.y > GeyserTile.y + 3) { //ref is up
					BunkerCloseRef = BWAPI::TilePosition(CCmanager[0]->LandLocation.x, CCmanager[0]->LandLocation.y - 2);
				}
				if (CCmanager[0]->LandLocation.x > GeyserTile.x + 2) { //ref is left
					BunkerCloseRef = BWAPI::TilePosition(CCmanager[0]->LandLocation.x - 3, CCmanager[0]->LandLocation.y);
				}
				if (CCmanager[0]->LandLocation.x < GeyserTile.x - 4) { //ref is right
					BunkerCloseRef = BWAPI::TilePosition(CCmanager[0]->LandLocation.x + 6, CCmanager[0]->LandLocation.y);
				}

			}
			if (BunkerCloseRef == BWAPI::TilePositions::Unknown) {
				ProdMan->addToQueueBegin(BWAPI::UnitTypes::Terran_Bunker);
			}
			else {
				ProdMan->addToQueueBeginTile(BWAPI::UnitTypes::Terran_Bunker, BunkerCloseRef);
			}

			if (ProdMan->TotalTypeInQueue(BWAPI::UnitTypes::Terran_Factory) == 1) {
				ProdMan->removeFromQueue(BWAPI::UnitTypes::Terran_Factory);
				//ProdMan->addToBuildOrderBegin(BWAPI::UnitTypes::Terran_Factory);
			}
			ProdMan->removeFromQueue(BWAPI::UnitTypes::Terran_Command_Center);
			ProdMan->removeFromQueue(BWAPI::UnitTypes::Terran_Command_Center);


			Broodwar->printf("also adding barracks to build order");
			/*
			BO newBO2;
			newBO2.type = BWAPI::UnitTypes::Terran_Supply_Depot;
			newBO2.supply = 16;
			ProdMan->BuildingSupplyOrder.insert( ProdMan->BuildingSupplyOrder.begin(), newBO2 );
			*/

			ProdMan->addToBuildOrderBegin(BWAPI::UnitTypes::Terran_Supply_Depot);
			//ProdMan->removeFromBuildOrder(BWAPI::UnitTypes::Terran_Academy);

			//ProdMan->addToBuildOrderBegin(BWAPI::UnitTypes::Terran_Academy);
			ProdMan->addToBuildOrderBegin(BWAPI::UnitTypes::Terran_Supply_Depot);
			/*
			BO newBO;
			newBO.type = BWAPI::UnitTypes::Terran_Barracks;
			newBO.supply = 16;
			ProdMan->BuildingSupplyOrder.insert( ProdMan->BuildingSupplyOrder.begin(), newBO );
			*/
			ProdMan->addToBuildOrderBegin(BWAPI::UnitTypes::Terran_Barracks);
			//BuildingSupplyOrder
			//ProdMan->addToQueue( BWAPI::UnitTypes::Terran_Barracks); //need more marines in order to get the expansion safe
			CCmanager[0]->SCVsonGas = 0;
		}

	}

	
	if( ZState == Z_Init && Broodwar->getFrameCount() > Frames4Bunker ){
  

		int TotalHatch = 0;
		for (int b = 0; b<InfoMan->EnemyBuildings.size(); b++) {
			if (InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Zerg_Hatchery) {
				TotalHatch++;
			}
		}
		if (TotalHatch >= 2) { //12 hatch
			ZState = Z_Expand_Defend;

			/*
			//make sure to check if this isn't just a 2 hatch ling all-in
			bool GoRamp = false;
			BOOST_FOREACH(Unit enemy, Broodwar->enemy()->getUnits()) {
				
				if (enemy->getType() == BWAPI::UnitTypes::Zerg_Zergling) {
					ZState = Z_Plug_Ramp;
					GoRamp = true;
				}				
				if (enemy->getType() == BWAPI::UnitTypes::Zerg_Spawning_Pool
					) {
					if (!enemy->isConstructing()) {
						GoRamp = true;
					}

					if (GoRamp == true) {
						//OnFrameZergFlowChartPlugRamp();
						OnFrameZergFlowChartSparksTerran();
					}

				}

			}
			*/


		}
		else {

			bool GoRamp = false;
			BOOST_FOREACH(Unit enemy, Broodwar->enemy()->getUnits()) {
				
				if (enemy->getType() == BWAPI::UnitTypes::Zerg_Zergling) {
					ZState = Z_Pool_Defend;
					GoRamp = true;
				}
				
				if (enemy->getType() == BWAPI::UnitTypes::Zerg_Spawning_Pool
					&& !enemy->isConstructing()) {
					//ZState = Z_Pool_Defend;
					GoRamp = true;
				}
			}
			/*
			BOOST_FOREACH(BuildingInfo enemy, InfoMan->EnemyBuildings ) {

			}
			*/


			if (GoRamp == true) {
				//OnFrameZergFlowChartPlugRamp();
				OnFrameZergFlowChartSparksTerran();
			}
			else {
				ZState = Z_Expand_Defend;
			}

		}


	}
	

	//check if it is a 2 hatch ling all-in
	int TotalHatch = 0;
	for (int b = 0; b<InfoMan->EnemyBuildings.size(); b++) {
		if (InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Zerg_Hatchery) {
			TotalHatch++;
		}
	}

	int MarinesNeededPoolDefense = 6;
	if (TotalHatch > 1) {
		MarinesNeededPoolDefense += 8;
	}

	//after defending a 4 pool, go 1 base to be sure
	if (ZState == Z_Pool_Defend && Squads[0].Marines.size() > MarinesNeededPoolDefense) {
		//OnFrameZergFlowChartPlugRamp();
		OnFrameZergFlowChartSparksTerran();
	}




	if (ZState == Z_Init) {
		if (Squads[0].Marines.size() > 4) {
			ZState = Z_Choke_Defend;
			if (Squads[0].Bunkers.size() > 0) {
				Squads[0].Bunkers[0]->unloadAll();
			}
			CCmanager[0]->SCVsonGas = 3;
		}
	}

	/*
	if (ZState == Z_Pool_Defend) {
		if (Squads[0].Marines.size() > 5) {
			ZState = Z_Choke_Defend;
			if (Squads[0].Bunkers.size() > 0) {
				Squads[0].Bunkers[0]->unloadAll();
			}
			CCmanager[0]->SCVsonGas = 3;
		}
	}
	*/


	if (ZState == Z_Plug_Ramp) {
		if (Squads[0].Marines.size() > 11) {
			//ZState = Z_Choke_Defend;
			ZState = Z_Expand_Defend_2_hatch;
			MacroMan->PlaceNatBunker();

			/*
			if (Squads[0].Bunkers.size() > 0) {
				Squads[0].Bunkers[0]->unloadAll();
			}
			*/

			CCmanager[0]->SCVsonGas = 3;
			for (int i = 0; i<Squads[0].SCVs.size(); i++) {
				CCmanager[0]->addSCV(Squads[0].SCVs[i]);
			}
			Squads[0].SCVs.clear();
		}

	}



	//if( ZState == Z_Choke_Defend ||  ZState == Z_Init){
	if (ProdMan->TotalTypeInQueue(BWAPI::UnitTypes::Terran_Command_Center) > 0
		 &&  ( ZState == Z_Init || ZState == Z_Choke_Defend)  ) {
		ZState = Z_Expand_Defend;
	}
	//}

	//once you have enough marines, take the natural choke and defend it from lurker/muta that are incoming
	// at this point zerg should be on 2 hatch
	if ((ZState == Z_Choke_Defend )) {
		//ZState = Z_Expand_Defend_2_hatch;
	    //MacroMan->PlaceNatBunker();

	}


	if (ZState == Z_Expand_Defend
		 &&
		(
		ProdMan->TotalTypeInQueue(BWAPI::UnitTypes::Terran_Command_Center) > 0
			|| CCmanager.size() > 1
			)
		) {



		int TotalExtractor = 0;
		for (int b = 0; b<InfoMan->EnemyBuildings.size(); b++) {
			if (InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Zerg_Extractor) {
				TotalExtractor++;
			}
		}

		int TotalHatch = 0;
		for (int b = 0; b<InfoMan->EnemyBuildings.size(); b++) {
			if (InfoMan->EnemyBuildings[b].type == BWAPI::UnitTypes::Zerg_Hatchery) {
				TotalHatch++;
			}
		}

		if (TotalExtractor >= 1 && TotalHatch < 3) { // 2 hatch play
			ZState = Z_Expand_Defend_2_hatch;

			Broodwar->printf("Switch to 2 Hatch Defence");

			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Refinery, 19);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Academy, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks, 23);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Factory, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Refinery, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Starport, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Science_Facility, 23);



		}
		else {
			ZState = Z_Expand_Defend_3_hatch;

			Broodwar->printf("Switch to 3  Hatch Defence");

			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Refinery, 19);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Barracks, 23);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Academy, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 14);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Factory, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Factory, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Refinery, 23);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			//ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Starport, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Supply_Depot, 23);
			ProdMan->addSupplyBO(BWAPI::UnitTypes::Terran_Science_Facility, 23);

		}
	}


}



void MacroManager::OnFrameZerg()
{




	//add new tank positions
	BOOST_FOREACH( BWAPI::Unit lurker, Broodwar->enemy()->getUnits() ){

		if( (lurker->getType() == BWAPI::UnitTypes::Zerg_Lurker
			&&  lurker->isBurrowed() )
			||  !lurker->isDetected()   ){
			bool exists = false;
			for( int j=0; j<ELurkerPositions.size(); j++){
				// if(  lurker->getPosition() == ELurkerPositions[j] ){
				if(  lurker->getID() == ELurkerPositions[j].UnitID ){
					exists = true;
					break;
				}
			}
			if( exists == false) {
				LurkerPos newLurk;
				newLurk.FramesLastSeen = Broodwar->getFrameCount();
				newLurk.position = lurker->getPosition();
				newLurk.UnitID = lurker->getID();
				newLurk.lurker = lurker;
				ELurkerPositions.push_back( newLurk );
			}
		}
	}

	for(int i=0; i< ELurkerPositions.size(); i++){
		if( ELurkerPositions[i].lurker != NULL ){
			if( ELurkerPositions[i].lurker->isDetected() 
				&& 
				(!ELurkerPositions[i].lurker->isBurrowed()
				|| ELurkerPositions[i].lurker->getPosition() != ELurkerPositions[i].position
				)
				){
					ELurkerPositions.erase( ELurkerPositions.begin() + i );
					i--;
					continue;
			}
		}

		Broodwar->drawCircleMap( ELurkerPositions[i].position.x , ELurkerPositions[i].position.y , 3, BWAPI::Colors::Cyan, true );

	}


	OnFrameZergSquad();

		if( Broodwar->self()->supplyUsed()/2 >= 11 && ScoutMan == NULL ){
			Broodwar->printf("Start Scouting Zerg");
			ScoutMan = new ScoutManager(1);
		}



		OnFrameZergFlowChart();

		//if nat CC is building. Get a bunker
		if (ProdMan->TotalTypeInQueue(BWAPI::UnitTypes::Terran_Command_Center) > 0  ) {
			MacroMan->PlaceNatBunker();
		}

		//ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Academy ) > 0
		//build bunkers after academy has finished
		if( ProdMan->Academy != NULL
			&& (ZState == Z_Expand_Defend || ZState == Z_Expand_Defend_2_hatch || ZState == Z_Expand_Defend_3_hatch)
			&& NatLurkerTurretPlace == false ){

			NatLurkerTurretPlace = true;

			//MacroMan->PlaceNatBunker();


			int ClosestChokeDist = 99999;
			BWAPI::Position BunkerPos = BWAPI::Positions::None;
			BWAPI::Position natPos = BWAPI::Position(InfoMan->OurNat);
			BWAPI::Position mainPos = InfoMan->PosOurBase; //BWAPI::Position(InfoMan->PosOurBase);
			BWTA::Region* natRegion = BWTA::getRegion(BWAPI::TilePosition(natPos));
			BWTA::Region* mainRegion = BWTA::getRegion(BWAPI::TilePosition(mainPos));

			BOOST_FOREACH(BWTA::Chokepoint*  choke, BWTA::getChokepoints()) {
				if ((choke->getRegions().first == natRegion && choke->getRegions().second != mainRegion) ||
					(choke->getRegions().second == natRegion && choke->getRegions().first != mainRegion)) {
					if (choke->getCenter().getDistance(InfoMan->PosEnemyBase) < ClosestChokeDist) {
						ClosestChokeDist = choke->getCenter().getDistance(InfoMan->PosEnemyBase);
						BunkerPos = choke->getCenter();
					}
				}
			}

			//add two bunkers and a turret in case of lurkers	  
			bool ExtraLurkerBunk = true;


			if (BunkerPos != BWAPI::Positions::None) {

				float closePnat = 0.35;
				float closePunk = 1 - closePnat;
				BWAPI::Position MissleTurretPos((int)((BunkerPos.x * closePunk) + (closePnat* InfoMan->PosOurNat.x)), (int)((BunkerPos.y * closePunk) + (closePnat * InfoMan->PosOurNat.y)));

				//ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition( BunkerPos ),false );
				//ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),false );
				ProdMan->addToQueueClose(BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition(BunkerPos), BWTA::getRegion(InfoMan->OurNat));
				if (Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && ExtraLurkerBunk == true) {
					// ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),false );
					//ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),BWTA::getRegion( InfoMan->OurNat )  );
					//ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition( MissleTurretPos ),false );
					ProdMan->addToQueueClose(BWAPI::UnitTypes::Terran_Missile_Turret, BWAPI::TilePosition(MissleTurretPos), BWTA::getRegion(InfoMan->OurNat));
				}
			}



		}

		/*
		if( (Squads[0].Medics.size() >= 4 || Broodwar->getFrameCount() > 10500)
			&& MicroZerg->MMpush == false 
			&& MicroZerg->RetreatIpush == false ){
				MicroZerg->StartIPush();
		}
		*/

		if ( ( Squads[0].Medics.size() > 0  || Squads[0].Marines.size() > 9)
			&& ZState == Z_SparksTerran) {
			ZState = Z_Push;
			Squads[0].SQstate = SQ_Attack_Base;
		}


		if( ZState == Z_Choke_Defend 
			|| (ZState == Z_Expand_Defend || ZState == Z_Expand_Defend_2_hatch || ZState == Z_Expand_Defend_3_hatch) ){

			if(  !TurretPlaced
				&& ProdMan->EngineeringBay != NULL ){
					TurretPlaced = true;
					ProdMan->PlaceTurrets();

			}

			//if( CurrentStrat == "Goliath M&M" ){
			if( ProdMan->Armory.size() > 0  ){				
				// || Broodwar->getFrameCount() > 9000

			  if( Squads[0].Tanks.size() > 0 ){
			    ZState = Z_Push;
				Squads[0].SQstate = SQ_Defend_Base;
			    /*
			    if( Squads[0].Vultures.size() > 0){
				  Squads[0].Vultures[0]->attack( InfoMan->PosEnemyBase);
			    }
			    */
			  }
			}

			if (Squads[0].ScienceVessels.size() > 0 && ZState == Z_Choke_Defend) {
				ZState = Z_Push;
				Squads[0].SQstate = SQ_Attack_Base;
			}

			if( CurrentStrat == "Standard Flowchart" 
				&& 
				(ZState == Z_Expand_Defend || ZState == Z_Expand_Defend_2_hatch || ZState == Z_Expand_Defend_3_hatch) ){
				
				// || Broodwar->getFrameCount() > 9000

				if( Squads[0].ScienceVessels.size() == 0  && Broodwar->getFrameCount() > 20000 
					&&  EarlyPool == false   ){
			    ZState = Z_Push;
			    Squads[0].SQstate = SQ_Attack_Base;
				}



				if ( Broodwar->getFrameCount() > 20000
					&& EarlyPool == true) {
					ZState = Z_Push;
					Squads[0].SQstate = SQ_Attack_Base;
				}
				if (Broodwar->getFrameCount() > 18500
					&& Squads[0].ScienceVessels.size() > 0 && Squads[0].Tanks.size() > 0 && Squads[0].Marines.size() > 5
					&& EarlyPool == true) {
					ZState = Z_Push;
					Squads[0].SQstate = SQ_Attack_Base;
				}


				if( (Squads[0].ScienceVessels.size() > 0  && Squads[0].Tanks.size() > 1 && Squads[0].Marines.size() > 5)
					){
			    ZState = Z_Push;
			    Squads[0].SQstate = SQ_Attack_Base;
			    /*
			    if( Squads[0].Vultures.size() > 0){
				  Squads[0].Vultures[0]->attack( InfoMan->PosEnemyBase);
			    }
			    */
			  }

			  /*
			  if( Squads[0].ScienceVessels.size() > 0 && ZState == Z_Choke_Defend ){
			    ZState = Z_Push;
			    Squads[0].SQstate = SQ_Attack_Base;
			  }
			  */


			}



			/*
			//retreat when needed
			if( ZState == Z_Push && Squads[0].Tanks.size() == 0 && Squads[0].Marines.size() < 10 ){
			    ZState = Z_Expand_Defend;
			    Squads[0].SQstate = SQ_Attack_Base;
			} 
			*/


			/*
			if(ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center) > 0){
			  ZState = Z_Push;
			  Squads[0].SQstate = SQ_Attack_Base;
			  if( Squads[0].Vultures.size() > 0){
				  Squads[0].Vultures[0]->attack( InfoMan->PosEnemyBase);
			  }
			}
			*/
		}

		/*
		if( Squads[0].SQstate == SQ_Defend_Base && Squads[0].Tanks.size() > 0){
			Squads[0].SQstate = SQ_Attack_Base;
		}
		*/


}


void MacroManager::OnFrameTerranSquadAttack(int id)
{

	BWAPI::Position attackPosT = InfoMan->PosEnemyBase;

	if( currentBuildingT2 != NULL ){
		if( Broodwar->isVisible( BWAPI::TilePosition( attackPosT2 ) ) ){
			if( !currentBuildingT2->exists() ){
				//remove building from list
				for(int i=0; i<InfoMan->EnemyBuildings.size(); i++){
					if( InfoMan->EnemyBuildings[i].building->getID() == currentBuildingT2->getID() ){
						InfoMan->EnemyBuildings.erase( InfoMan->EnemyBuildings.begin() + i );
						break;
					}
				}
				currentBuildingT2 = NULL;
			}
			//if( Broodwar->getUnitsOnTile
		}
	}


	if( (currentBuildingT2 == NULL || attackPosT2 == BWAPI::Positions::None) && InfoMan->EnemyBuildings.size() > 0 ){
		NextBuildingMMt();
	}

	//Broodwar->drawCircleMap( attackPos.x() , attackPos.y() ,5,Colors::Purple,true);
	if( ( currentBuildingT2 == NULL || attackPosT2 == BWAPI::Positions::None ) && HuntT2 == false && HuntAllowedT2 == true
		 && InfoMan->EnemyBuildings.size() == 0 ){
		HuntT2 = true;
		std::vector< Position > PossiblePos;
		BOOST_FOREACH( BWTA::BaseLocation* close, BWTA::getBaseLocations() )
        {
			PossiblePos.push_back( BWAPI::Position( close->getTilePosition() ) );
		}
		attackPosT2 = PossiblePos[ rand() %  PossiblePos.size()  ];
		Broodwar->printf("New location to look");
	}
	if( InfoMan->EnemyBuildings.size() > 0 && HuntP == true){
		HuntT2 = false;
		NextBuildingMMt();
	}
	if(  HuntT2 == true && HuntAllowedT2 == true ){
		if( Broodwar->isVisible(  BWAPI::TilePosition( attackPosT2 ) ) ){
			HuntT2 = false;
			attackPosT2 = BWAPI::Positions::None;
		}
	}
	
	if( attackPosT2 != BWAPI::Positions::Invalid &&  attackPosT2 != BWAPI::Positions::None &&  attackPosT2 != BWAPI::Positions::Unknown){
	   Broodwar->drawCircleMap( attackPosT2.x, attackPosT2.y, 5 , BWAPI::Colors::Purple,true);
	}



	/*
	if( (currentBuildingT2 == NULL || attackPosT == BWAPI::Positions::None) && InfoMan->EnemyBuildings.size() > 0 ){
		NextBuildingMMt();
	}
	//Broodwar->drawCircleMap( attackPos.x() , attackPos.y() ,5,Colors::Purple,true);
	if( ( currentBuildingT2 == NULL || attackPosT == BWAPI::Positions::None ) && HuntT2 == false && HuntAllowedT2 == true
		 && InfoMan->EnemyBuildings.size() == 0 ){
		HuntT2 = true;
		std::vector< Position > PossiblePos;
		BOOST_FOREACH( BWTA::BaseLocation* close, BWTA::getBaseLocations() )
        {
			PossiblePos.push_back( BWAPI::Position( close->getTilePosition() ) );
		}
		attackPosT = PossiblePos[ rand() %  PossiblePos.size()  ];
		Broodwar->printf("New location to look");
	}
	if( InfoMan->EnemyBuildings.size() > 0 && HuntT2 == true){
		HuntT2 = false;
		NextBuildingMMt();
	}
	if(  HuntT2 == true && HuntAllowedT2 == true ){
		if( Broodwar->isVisible(  BWAPI::TilePosition( attackPosT ) ) ){
			HuntT2 = false;
			attackPosT = BWAPI::Positions::None;
		}
	}
	*/








	//get enemy bunkers
	//std::vector< BuildingInfo > BunkerInfo;
	std::vector< BWAPI::Position > BunkerInfo;
	for(int i=0; i<InfoMan->EnemyBuildings.size(); i++){
		if( InfoMan->EnemyBuildings[i].type == BWAPI::UnitTypes::Terran_Bunker
			     ){
			BunkerInfo.push_back( InfoMan->EnemyBuildings[i].position);
		}
	}



	//get tank closest to attackposT2
	int tankAttackCloseDist = 99999;
	BWAPI::Unit tankClosestAttack = NULL;
	BOOST_FOREACH( BWAPI::Unit tank, Squads[0].Tanks ){
		if(  tank->getDistance( attackPosT2) < tankAttackCloseDist ){
			tankAttackCloseDist =  tank->getDistance( attackPosT2 );
			tankClosestAttack = tank;
		}
	}

	//get tank closest to attackposT2
	int goliathAttackCloseDist = 99999;
	BWAPI::Unit goliathClosestAttack = NULL;
	BOOST_FOREACH(BWAPI::Unit goliath, Squads[0].Goliaths) {
		if (goliath->getDistance(attackPosT2) < goliathAttackCloseDist) {
			goliathAttackCloseDist = goliath->getDistance(attackPosT2);
			goliathClosestAttack = goliath;
		}
	}




	//get a list of enemy units
	std::vector< BWAPI::Unit > EnemyUnits;
	BOOST_FOREACH( Unit close, Broodwar->enemy()->getUnits() ){

		//if(  close->getType().isBuilding() && close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
		//	continue;
		//}

		EnemyUnits.push_back( close );
	}

	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 9*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Bunker = false;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Terran_Bunker){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Terran_Bunker 
				&& close->getPlayer() == Broodwar->enemy()
				&& Squads[0].Marines[i]->getDistance( close->getPosition() ) < 12*32  ){
				Bunker = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
		for( int j=0; j<BunkerInfo.size(); j++){
			if(  Squads[0].Marines[i]->getDistance( BunkerInfo[j] ) < 12*32  ){
				Bunker = true;
			}
		}
		//also stay away from tanks
		for( int j=0; j<ETankPositions.size(); j++){
			if(  Squads[0].Marines[i]->getDistance( ETankPositions[j] ) < 12*32  ){
				Bunker = true;
			}
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 ) 
		  || (Squads[0].Marines[i]->getHitPoints() < 25 && closest != NULL &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 )
	        ||  Bunker == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Marines[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Marines[i]->getPosition().getDistance( attackPosT2 ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
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

		                  Squads[0].Marines[i]->attack(  attackPosT2 );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}


	for(unsigned int i=0; i<Squads[0].Tanks.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistEAttack = 11*32;
		//Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius( MaxDistEAttack );
		Unit closest = NULL;
		Unit closestTank = NULL;
		int closestTankDist = 999999;
		//Unit closestHT = NULL;
		//Unit closestReavu = NULL;
		bool ReasonForSiege = false; //dont siege all the time

		Unit SupplyDepot = NULL;

		int closeDist = 9999999;
		//BOOST_FOREACH( Unit close, EnemyClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {
			if( Squads[0].Tanks[i]->getDistance( close->getPosition() ) > MaxDistEAttack ){
				continue;
			}

			if( close->getPlayer() != Broodwar->enemy() ){
				continue;
			}

			if(  close->getType().isBuilding() && close->isLifted() ){
				continue;
			}

			//if(  close->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine ){
				//continue;
			//}
			/*
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon ){
				continue;
			}
			*/
			if(  close->getType() == BWAPI::UnitTypes::Unknown ){
				continue;
			}

			if( close->getType().isFlyer()  ){
				continue;
			}

			if( close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				|| close->getType() == BWAPI::UnitTypes::Terran_Goliath
				|| close->getType() == BWAPI::UnitTypes::Terran_Medic  ){
				ReasonForSiege = true;
			}

			if( (close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				) && closestTankDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )
				  ){
				closestTank = close;
				closestTankDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
			}


			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//for sieging within range, use 12*32
		//for a contain strategy, use 14*32
		int enemyTankRange = 14*32;

		int TotalSiegedinRange = 0;
			for(int e=0; e<ETankPositions.size(); e++ ){
				if( ETankPositions[e].getDistance( Squads[0].Tanks[i]->getPosition() ) < 14*32 ){
					TotalSiegedinRange++;
				}
			}

		//only attack when you outnumber the tanks 3:1
		//if(  Squads[0].Tanks.size()  > ETankPositions.size() * 3 ){
		if(  Squads[0].Tanks.size()  > TotalSiegedinRange * 2 ){
			enemyTankRange = 11;
		}

		//siege 
		for( int j=0; j< ETankPositions.size(); j++){
			if(  Squads[0].Tanks[i]->getDistance( ETankPositions[j] ) <= enemyTankRange ){

				if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
					&& Squads[0].Tanks[i]->getOrder() != BWAPI::Orders::Sieging ){
			     Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			    continue;
		      }
              continue;
			}

		}

		for( int j=0; j<BunkerInfo.size(); j++){
			if(  Squads[0].Tanks[i]->getDistance( BunkerInfo[j] ) <= 11*32  ){
				if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode){
			     Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			     continue;
		       }
				continue;
			}
		}

		if( closest == NULL ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		} else if( closeDist > enemyTankRange ){ // 12*32
		//unsiege if there are no attacking buildings and tanks
        if( closest->getType() != BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode &&
			closest->getType() != BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode &&
			closest->getType() != BWAPI::UnitTypes::Terran_Bunker ){
		  if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
			continue;
		  }
		}
	  }

		if( closest == NULL ){
			Squads[0].Tanks[i]->attack(  attackPosT2 );
			continue;
		}

		bool SpiderMineClose = false;
		if( closest != NULL ){
			if( closest->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine ){
				SpiderMineClose = true;
			}
		}

	  // if we can't shoot, run away  if(kite)
	  if ( ( closeDist < 4*32 &&  Squads[0].Tanks[i]->getGroundWeaponCooldown() != 0 )
		  &&  Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
		  &&  SpiderMineClose == false )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Tanks[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

		//if there are no enemies nearby, attack;
		if( closest != NULL  ){ //|| closeDist > 3*32
			if(  closeDist > 11*32 ){
					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }
				continue;
			}
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );

					  //siege up for sunken, bunker and cannon
			          //  closest->getType() == BWAPI::UnitTypes::Terran_Bunker
					  if( ReasonForSiege == true ){
						if( Squads[0].Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
			              Squads[0].Tanks[i]->useTech( BWAPI::TechTypes::Tank_Siege_Mode);
						  continue;
		                 }
					  }


					  
					  //prioiritize
					  if( closestTank != NULL){
						  if(  closestTank->getDistance( Squads[0].Tanks[i] ) < 12*32){
							   closest = closestTank;
						  }
					  }

					  if( Squads[0].Tanks[i]->getOrderTarget() == NULL ){
                        Squads[0].Tanks[i]->attack(  closest );
					  }
					  else if( Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Tanks[i]->attack(  closest );
					  }

				  //Broodwar->printf("Attack");
				
			
			continue;
		}


	  
	}





	for(unsigned int i=0; i<Squads[0].Vultures.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		int MaxDistEAttack = 9*32;
		//Unitset EnemyClose = Squads[0].Vultures[i]->getUnitsInRadius( MaxDistEAttack  );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Bunker = false;
		//BOOST_FOREACH( Unit close, EnemyClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {

			if( Squads[0].Vultures[i]->getDistance( close->getPosition() ) > MaxDistEAttack ){
				continue;
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Terran_Bunker){
				continue;
			}

			if(  close->getType().isFlyer() ){
				continue;
			}

			if(  close->getType() == BWAPI::UnitTypes::Terran_Bunker 
				&& close->getPlayer() == Broodwar->enemy()
				&& Squads[0].Vultures[i]->getDistance( close->getPosition() ) < 8*32  ){
				Bunker = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		for( int j=0; j<BunkerInfo.size(); j++){
			if(  Squads[0].Vultures[i]->getDistance( BunkerInfo[j] ) < 12*32  ){
				Bunker = true;
			}
		}
		
		//also stay away from tanks
		int TotalETanksClose = 0;
		for( int j=0; j<ETankPositions.size(); j++){
			if(  Squads[0].Vultures[i]->getDistance( ETankPositions[j] ) < 14*32  ){
				TotalETanksClose++;
				//Bunker = true;
			}
		}

		if( TotalETanksClose >= 2 ){
			Bunker = true;
		}




		//in case of a vulture push, stick together
		if(  tankClosestAttack == NULL && goliathClosestAttack == NULL ){
        int vultureClose = 0;
		  for( int j=0; j<Squads[0].Vultures.size(); j++){
			if( i != j && Squads[0].Vultures[i]->getDistance( Squads[0].Vultures[j] ) < 4*32  ){
				vultureClose++;
			}
		}
		  if( vultureClose == 0 ){
			  Bunker = true;
		  }
		}


		/*
		//check if a friendly tank is close. Gotta protect that tank.
		bool TankNearyBy = false;
		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Vultures[i] ) < 6*32
				 ||  tankClosestAttack->getDistance( attackPosT2 ) < Squads[0].Vultures[i]->getDistance( attackPosT2 )
				 ){
				TankNearyBy = true;
			}
		}
		*/

		//check if a friendly goliath is close. Gotta protect that tank.
		bool GoliathNearyBy = false;
		if (goliathClosestAttack != NULL) {
			if (goliathClosestAttack->getDistance(Squads[0].Vultures[i]) < 4 * 32
				|| goliathClosestAttack->getDistance(attackPosT2) < Squads[0].Vultures[i]->getDistance(attackPosT2)
				) {
				GoliathNearyBy = true;
			}
		}


		/*
		if( Squads[0].Tanks.size() < 2 && Squads[0].Goliaths.size() > 0 ){
			TankNearyBy = true;
		}
		*/

		if( closest != NULL ){
			if( closest->getType() == BWAPI::UnitTypes::Terran_SCV
				&&  closeDist < 2*32  ){
					Bunker = true;
			}

		}

	  // if we can't shoot, run away  if(kite)
		//  Squads[0].Vultures[i]->getHitPoints() < 25 &&
		//( closeDist < 4*32 &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 ) 
		 // || ( closest != NULL &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0 )
	  if (   Bunker == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Vultures[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }

	  /*
	  if ( TankNearyBy == false )
	  {
		if( tankClosestAttack != NULL ){
			Squads[0].Vultures[i]->move( tankClosestAttack->getPosition() );
		  continue;
		}
	  }
	  */

	  if (GoliathNearyBy == false)
	  {
		  if (goliathClosestAttack != NULL) {
			  Squads[0].Vultures[i]->move(goliathClosestAttack->getPosition());
			  continue;
		  }
	  }


	  //if the enemies areo ut of weapons reach and outside the main or natural, just keep moving
	  if( BWTA::getRegion( Squads[0].Vultures[i]->getTilePosition() )  != InfoMan->OurMainregion
		  && BWTA::getRegion( Squads[0].Vultures[i]->getTilePosition() )  != InfoMan->NatRegion ){
          if(  closest != NULL ){
			  if(  ( closest->getType() == BWAPI::UnitTypes::Terran_Vulture || closest->getType() == BWAPI::UnitTypes::Terran_SCV )
				  && closeDist > 6*32 ){
				  Squads[0].Vultures[i]->move(  attackPosT2 );
				  continue;
			  }
		  }
	  }




		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Vultures[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Vultures[i]->getPosition().getDistance( attackPosT2 ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[0].Vultures[i]->getOrderTarget() == NULL ){
                        Squads[0].Vultures[i]->attack(  closest );
					  }
					  else if( Squads[0].Vultures[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Vultures[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Vultures[i]->attack(  attackPosT2 );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}





	for(unsigned int i=0; i<Squads[0].Goliaths.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		int MaxDistEAttack =  15*32;
		//Unitset EnemyClose = Squads[0].Goliaths[i]->getUnitsInRadius( MaxDistEAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		bool Bunker = false;
		Unit tankToAttack = NULL;

		int closestSpiderMineDist = 9999999;
		Unit SpiderMine = NULL;

		//BOOST_FOREACH( Unit close, EnemyClose )
		BOOST_FOREACH( Unit close, EnemyUnits )
        {

			if( Squads[0].Goliaths[i]->getDistance( close->getPosition() ) > MaxDistEAttack ){
				continue;
			}

			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Terran_Bunker){
				continue;
			}

			//dont chase after wraiths
			if (close->getType() == BWAPI::UnitTypes::Terran_Wraith
				 ) {
				continue;
			}


			if (
				close->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine 
				&& closestSpiderMineDist > Squads[0].Goliaths[i]->getDistance(close->getPosition())) {
				closestSpiderMineDist = Squads[0].Goliaths[i]->getDistance(close->getPosition());
				SpiderMine = close;
			}

			if( close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ||
				close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ){
				tankToAttack = close;
			}

			if(  close->getType() == BWAPI::UnitTypes::Terran_Bunker 
				&& close->getPlayer() == Broodwar->enemy()
				&& Squads[0].Goliaths[i]->getDistance( close->getPosition() ) < 8*32  ){
				Bunker = true;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Goliaths[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Goliaths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//dont chase after wraiths
		if (closest  != NULL && closest->getType() == BWAPI::UnitTypes::Terran_Wraith
			&& Squads[0].Goliaths[i]->getDistance(closest->getPosition()) > 5 * 31) {
			Squads[0].Goliaths[i]->move(attackPosT2);
			continue;
		}
		if (Squads[0].Goliaths[i]->getTarget() != NULL) {
			if (Squads[0].Goliaths[i]->getTarget()->getType() == BWAPI::UnitTypes::Terran_Wraith
				) {
				Squads[0].Goliaths[i]->move(attackPosT2);
				continue;
			}
		}
		for( int j=0; j<BunkerInfo.size(); j++){
			if(  Squads[0].Goliaths[i]->getDistance( BunkerInfo[j] ) < 12*32  ){
				Bunker = true;
			}
		}

		//also stay away from tanks
		int TotalETanksClose = 0;
		for( int j=0; j<ETankPositions.size(); j++){
			if(  Squads[0].Goliaths[i]->getDistance( ETankPositions[j] ) < 15*32  ){
				TotalETanksClose++;
				//Bunker = true;
			}
		}

		if( TotalETanksClose >= 10 ){
			Bunker = true;
		}

		/*
		//prioritize spider mines
		if (closestSpiderMineDist < 6 * 32
			&& SpiderMine != NULL) {
			if (Squads[0].Goliaths[i]->getTarget() == NULL) {
				Squads[0].Goliaths[i]->attack(SpiderMine);
			}
			else if (Squads[0].Goliaths[i]->getTarget()->getID() != SpiderMine->getID()) {
				Squads[0].Goliaths[i]->attack(SpiderMine);
			}
			continue;
		}
		*/

		if ( TotalETanksClose < 2 ){
			if(  tankToAttack != NULL){
				if(  Squads[0].Goliaths[i]->getTarget() == NULL){
					Squads[0].Goliaths[i]->attack( tankToAttack );
				} else if ( Squads[0].Goliaths[i]->getTarget()->getID() !=  tankToAttack->getID() ){
					Squads[0].Goliaths[i]->attack( tankToAttack );
				}
              continue;
			}
		}
		/*
		//in case of a goliath push, stick together
		if(  tankClosestAttack == NULL ){
        int goliathClose = 0;
		  for( int j=0; j<Squads[0].Goliaths.size(); j++){
			if( i != j && Squads[0].Goliaths[i]->getDistance( Squads[0].Goliaths[j] ) < 4*32  ){
				goliathClose++;
			}
		}
		  if( goliathClose == 0 ){
			  Bunker = true;
		  }
		}
		*/


		bool TankNearyBy = false;
		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].Goliaths[i] ) < 6*32
				 ||  tankClosestAttack->getDistance( attackPosT2 ) < Squads[0].Goliaths[i]->getDistance( attackPosT2 )
				 ){
				TankNearyBy = true;
			}
		}


		/*
		if( Squads[0].Tanks.size() < 2  ){
			TankNearyBy = true;
		}
		*/



		bool SpiderMineClose = false;
		if( closest != NULL ){
			if( closest->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine ){
				SpiderMineClose = true;
			}
		}



	  // if we can't shoot, run away  if(kite)
		//  || (Squads[0].Goliaths[i]->getHitPoints() < 25 && closest != NULL &&  Squads[0].Goliaths[i]->getGroundWeaponCooldown() != 0 )
	  if ( (( closeDist < 2*32 &&  Squads[0].Goliaths[i]->getGroundWeaponCooldown() != 0 ) 
	        ||  Bunker == true 
			) && SpiderMineClose == false )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].Goliaths[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }


	  /*
	  if ( TankNearyBy == false )
	  {
		if( tankClosestAttack != NULL ){
			Squads[0].Goliaths[i]->move( tankClosestAttack->getPosition() );
		  continue;
		}
		//Marines[i].state = M_Fleeing;
	  }
	  */


	  //if the enemies areo ut of weapons reach and outside the main or natural, just keep moving
	  if( BWTA::getRegion( Squads[0].Goliaths[i]->getTilePosition() )  != InfoMan->OurMainregion
		  && BWTA::getRegion( Squads[0].Goliaths[i]->getTilePosition() )  != InfoMan->NatRegion ){
          if(  closest != NULL ){
			  if(  ( closest->getType() == BWAPI::UnitTypes::Terran_Vulture || closest->getType() == BWAPI::UnitTypes::Terran_SCV )
				  && closeDist > 6*32 ){
				  Squads[0].Goliaths[i]->move(  attackPosT2 );
				  continue;
			  }
			  if(   closest->getType() == BWAPI::UnitTypes::Terran_Wraith && closeDist > 8*32 ){
				  Squads[0].Goliaths[i]->move(  attackPosT2 );
				  continue;
			  }
		  }
	  }


		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].Goliaths[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Goliaths[i]->getPosition().getDistance( attackPosT2 ) > 6*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
                  if( closest != NULL ){
					  if( Squads[0].Goliaths[i]->getOrderTarget() == NULL ){
                        Squads[0].Goliaths[i]->attack(  closest );
					  }
					  else if( Squads[0].Goliaths[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Goliaths[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].Goliaths[i]->attack(  attackPosT2 );

					  }
				  }
				  //Broodwar->printf("Attack");
				}
			}
			continue;
		}


	  
	}


	for(unsigned int i=0; i<Squads[0].BattleCruisers.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].BattleCruisers[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;

		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if(  close->getType().isBuilding() || !close->isDetected() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].BattleCruisers[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].BattleCruisers[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}
		
		//if there are no enemies nearby, attack;
		if( closest == NULL ||  Squads[0].BattleCruisers[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Squads[0].BattleCruisers[i]->getPosition().getDistance( attackPos ) > 6*32 ){
					/*
                  if( closest != NULL ){
					  if( Squads[0].BattleCruisers[i]->getOrderTarget() == NULL ){
                        Squads[0].BattleCruisers[i]->attack(  closest );
					  }
					  else if( Squads[0].BattleCruisers[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].BattleCruisers[i]->attack(  closest );
					  }
				  } else {

					  //prevent spam
					  if( Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackUnit &&
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AtkMoveEP ){

		                  Squads[0].BattleCruisers[i]->attack(  attackPosT2 );

					  }
				  }
				  //Broodwar->printf("Attack");
				  */

					  //prevent spam
					  if( Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackUnit &&
						  Squads[0].BattleCruisers[i]->getOrder() != BWAPI::Orders::AtkMoveEP ){

		                  Squads[0].BattleCruisers[i]->attack(  attackPosT2 );

					  }


				}
			
			continue;
		}
	  

	}







		for(unsigned int i=0; i<Squads[0].SCVs.size(); i++){


		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		int MaxDistAttack = 10*32;
		Unitset UnitsClose = Squads[0].SCVs[i]->getUnitsInRadius( MaxDistAttack );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			
			//if(  Squads[0].SCVs[i]->getDistance( close->getPosition() ) > MaxDistAttack ){
				//continue;
			//}
			
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Terran_Bunker ){
				continue;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].SCVs[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].SCVs[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		/*
	  // if we can't shoot, run away  if(kite)
		if (   Sunken == true || closeDist < 6*32
		    ||  isUnderDS )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Medics[i]->getPosition().x, Medics[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);
		  		  if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::Move){
		   Squads[0].SCVs[i]->move( fleePosition );
				  }
		continue;
		//Marines[i].state = M_Fleeing;
	  }
	  */


	  //check if a unit nearby needs healing
	  Unit needsHeal = NULL;
		BOOST_FOREACH( Unit close, UnitsClose )
        {
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->self() 
				&& close->getHitPoints() != close->getType().maxHitPoints()
				&& ( close->getType() == BWAPI::UnitTypes::Terran_Goliath 
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				|| close->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode) ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL &&  !Squads[0].SCVs[i]->isRepairing() ){
			Squads[0].SCVs[i]->repair( needsHeal );
			continue;
		}


		//determine when to flee
		bool Bunker = false;
		for( int j=0; j<BunkerInfo.size(); j++){
			if(  Squads[0].SCVs[i]->getDistance( BunkerInfo[j] ) < 12*32  ){
				Bunker = true;
			}
		}
		
		//also stay away from tanks
		int TotalETanksClose = 0;
		for( int j=0; j<ETankPositions.size(); j++){
			if(  Squads[0].SCVs[i]->getDistance( ETankPositions[j] ) < 14*32  ){
				TotalETanksClose++;
				//Bunker = true;
			}
		}

		if( TotalETanksClose >= 1 ){
			Bunker = true;
		}


		/*
		bool TankNearyBy = false;
		if( tankClosestAttack != NULL ){
			if( tankClosestAttack->getDistance( Squads[0].SCVs[i] ) < 6*32
				 ||  tankClosestAttack->getDistance( attackPosT2 ) < Squads[0].SCVs[i]->getDistance( attackPosT2 )
				 ){
				TankNearyBy = true;
			}
		}

		if( Squads[0].Tanks.size() == 0 ){
			TankNearyBy = true;
		}
		*/

	  // if we can't shoot, run away  if(kite)
		//  Squads[0].Vultures[i]->getHitPoints() < 25 &&
		// || TankNearyBy == false
	  if ( ( closeDist < 6*32  )  
	        ||  Bunker == true )
	  {
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  BWAPI::Position fleePosition( InfoMan->PosOurBase );
       
		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		Squads[0].SCVs[i]->move( fleePosition );
		continue;
		//Marines[i].state = M_Fleeing;
	  }



		//if there are no enemies nearby, attack;
		if( closest == NULL ){ //|| closeDist > 3*32
			if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
			  Unit closestUnit = NULL;
			  int closeDist = 999999;


					  //prevent spam
					  if( Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].SCVs[i]->getOrder() != BWAPI::Orders::AttackUnit ){

		                  Squads[0].SCVs[i]->attack(  attackPosT2 );

					  }
                /*
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 
				if( Squads[0].SCVs[i]->getPosition().getDistance( attackPos ) > 6*32 ){
			      Squads[0].SCVs[i]->move(  attackPos );//regular move
				}
				*/

			}
			continue;
		}


		}




	if( FloatingBarracks != NULL){

		if( !FloatingBarracks->isLifted() ){
			FloatingBarracks->lift();
			return;
		}

		//float such that there is vision for a bunker
		BWAPI::Position bunkerPos = BWAPI::Positions::None;
		for(int i=0; i<InfoMan->EnemyBuildings.size(); i++){
			if( InfoMan->EnemyBuildings[i].type == BWAPI::UnitTypes::Terran_Bunker ){
				bunkerPos = InfoMan->EnemyBuildings[i].position;
			}
		}

		if( bunkerPos != BWAPI::Positions::None){
			if( FloatingBarracks->getDistance( bunkerPos ) > 7*30){
			  FloatingBarracks->move( bunkerPos );
			  return;
			}
			//dont get too close
			if( FloatingBarracks->getDistance( bunkerPos ) < 7*30){
			  FloatingBarracks->move( InfoMan->PosOurBase );
			  return;
			}
		} else {


			if( Squads[0].Tanks.size() > 0 ){

				int ClosestETank = 99999;
				BWAPI::Position ETankPos = BWAPI::Positions::None;
			    for( int j=0; j<ETankPositions.size(); j++){
					if( ClosestETank > ETankPositions[j].getDistance( Squads[0].Tanks[0]->getPosition() ) ){
						ClosestETank = ETankPositions[j].getDistance( Squads[0].Tanks[0]->getPosition() );
						ETankPos = ETankPositions[j];
					}
			    }

				if( ETankPos != BWAPI::Positions::None){
			      if( FloatingBarracks->getDistance( ETankPos ) > 7*30){
			      FloatingBarracks->move( ETankPos );
			      return;
			      }
			     //dont get too close
			     if( FloatingBarracks->getDistance( ETankPos ) < 7*30){
			      FloatingBarracks->move( InfoMan->PosOurBase );
			      return;
			     }
				}

				FloatingBarracks->move( Squads[0].Tanks[0]->getPosition() );
			}
		}


	}



}

void MacroManager::OnFrameTerranSquadDefence(int id)
{





	BWAPI::Position DefaultPos =  chokePos;

	bool DefendNat = false;
	if (Squads[0].Goliaths.size() > 1) {
		DefendNat = true;
		DefaultPos = InfoMan->PosOurNat;
		InfoMan->NatRegion->getChokepoints();
		BWTA::Region* regChoice = NULL;
		int closestReg = 999999;
		BOOST_FOREACH(BWTA::Chokepoint* choke, InfoMan->NatRegion->getChokepoints()) {
			if (choke->getRegions().first != InfoMan->OurMainregion
				&& choke->getRegions().second != InfoMan->OurMainregion) {
				if (choke->getRegions().first != InfoMan->NatRegion
					&& choke->getRegions().first->getCenter().getDistance(InfoMan->PosEnemyBase) < closestReg) {
					closestReg = choke->getRegions().first->getCenter().getDistance(InfoMan->PosEnemyBase);
					regChoice = choke->getRegions().first;
				}
				if (choke->getRegions().second != InfoMan->NatRegion
					&& choke->getRegions().second->getCenter().getDistance(InfoMan->PosEnemyBase) < closestReg) {
					closestReg = choke->getRegions().second->getCenter().getDistance(InfoMan->PosEnemyBase);
					regChoice = choke->getRegions().second;
				}

			}
		}

		if (regChoice != NULL) {
			DefaultPos = regChoice->getCenter();
		}

	}

	//BWAPI::Position DefaultPos =  WDMan->ToRetreat;


	for(unsigned int i=0; i<Squads[0].Marines.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Marines[i]->getUnitsInRadius( 30*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}


		/*
		if( closest != NULL && closeDist > 6*32 ){
			if(  Squads[0].Bunkers.size() > 0 ){
			  Squads[0].Bunkers[0]->unload( Squads[0].Marines[i] );
			  continue;
			}
		}
		*/

		//get to the bunkaa!!!
			if( Squads[0].Bunkers.size() > 0   ){
				//Squads[0].Bunkers[0]->load( Squads[0].Marines[i] );
				Squads[0].Marines[i]->rightClick( Squads[0].Bunkers[0]);
				continue;
			}


	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ( ( closeDist < 4*32 &&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0
			&& Squads[0].Marines[i]->getDistance( WDMan->ToRetreat ) > 3*32 )
			||  
			(
			BWTA::getRegion( Squads[0].Marines[i]->getTilePosition() ) != InfoMan->OurMainregion
				&& DefendNat == false
				)

			)
	  {
		  /*
			//get to the bunkaa!!!
			if( Squads[0].Bunkers.size() > 0 ){
				//Squads[0].Bunkers[0]->load( Squads[0].Marines[i] );
				Squads[0].Marines[i]->rightClick( Squads[0].Bunkers[0]);
				continue;
			} else {
			*/
		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Marines[i]->move( fleePosition );
		     continue;
			//}
		//Marines[i].state = M_Fleeing;
	  }

      if( Squads[0].Bunkers.size() > 0 ){
		  if(  Squads[0].Marines[i]->getDistance( CCmanager[0]->CommandCenter ) > 7*32 ){
			  Squads[0].Marines[i]->move( Squads[0].Bunkers[0]->getPosition() );
			  continue;
		  }
	  }


		if( Squads[0].Marines[i]->getGroundWeaponCooldown() == 0  ){

                  if( closest != NULL ){
					  if( Squads[0].Marines[i]->getOrderTarget() == NULL ){
                        Squads[0].Marines[i]->attack(  closest );
					  }
					  else if( Squads[0].Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Marines[i]->attack(  closest );
					  }
					  continue;
				  }

		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

			//get to the bunkaa!!!
			if( Squads[0].Bunkers.size() > 0 ){
				Squads[0].Bunkers[0]->load( Squads[0].Marines[i] );
				continue;
			}
				if( Squads[0].Marines[i]->getPosition().getDistance( DefaultPos ) > 3*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Squads[0].Marines[i]->attack(  DefaultPos );
					  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}  
	}




	for(unsigned int i=0; i<Squads[0].Vultures.size(); i++){
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );
		
		Unitset EnemyClose = Squads[0].Vultures[i]->getUnitsInRadius( 15*32 );
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH( Unit close, EnemyClose )
        {
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Squads[0].Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}



	  // if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		//BWTA::getRegion( Squads[0].Vultures[i]->getPosition() ) != InfoMan->OurMainregion 
		if ( ( closeDist < 5*32 &&  Squads[0].Vultures[i]->getGroundWeaponCooldown() != 0
			&& Squads[0].Vultures[i]->getDistance( WDMan->ToRetreat ) > 5*32 )
			||  
			(Squads[0].Vultures[i]->getDistance( InfoMan->PosMainChoke ) < 2*32 && DefendNat == false)
			||
			(
				BWTA::getRegion(Squads[0].Vultures[i]->getTilePosition()) != InfoMan->OurMainregion
				&& DefendNat == false
				)
			)
	  {



		//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
		  //BWAPI::Position fleePosition( InfoMan->PosOurBase );
 		  BWAPI::Position fleePosition( WDMan->ToRetreat ); 

		//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

		     Squads[0].Vultures[i]->move( fleePosition );
		     continue;

		//Marines[i].state = M_Fleeing;
	  }


		if( Squads[0].Vultures[i]->getGroundWeaponCooldown() == 0  ){

                  if( closest != NULL ){
					  if( Squads[0].Vultures[i]->getOrderTarget() == NULL ){
                        Squads[0].Vultures[i]->attack(  closest );
					  }
					  else if( Squads[0].Vultures[i]->getOrderTarget()->getID() != closest->getID() ){
		                Squads[0].Vultures[i]->attack(  closest );
					  }
					  continue;
				  }

		}

		//if there are no enemies nearby, attack;
		if( closest == NULL  ){ //|| closeDist > 3*32
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

				if( Squads[0].Vultures[i]->getPosition().getDistance( DefaultPos ) > 3*32 ){
			      //Marines[i].state = M_Attacking;
			      //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
					  //prevent spam
					  if( Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  Squads[0].Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){
		                  Squads[0].Vultures[i]->attack(  DefaultPos );
					  }
				  //Broodwar->printf("Attack");
				}
			
			continue;
		}  
	}





	for (unsigned int i = 0; i<Squads[0].Goliaths.size(); i++) {
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );

		Unitset EnemyClose = Squads[0].Goliaths[i]->getUnitsInRadius(30 * 32);
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH(Unit close, EnemyClose)
		{
			if (close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Goliaths[i]->getDistance(close->getPosition())) {
				closeDist = Squads[0].Goliaths[i]->getDistance(close->getPosition());
				closest = close;
			}
		}

		// if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		if ((closeDist < 4 * 32 && Squads[0].Goliaths[i]->getGroundWeaponCooldown() != 0
			&& Squads[0].Goliaths[i]->getDistance(WDMan->ToRetreat) > 3 * 32)
			|| 
			(
				BWTA::getRegion(Squads[0].Goliaths[i]->getTilePosition()) != InfoMan->OurMainregion
				&& DefendNat == false
				)
			)
		{
			BWAPI::Position fleePosition(WDMan->ToRetreat);
			Squads[0].Goliaths[i]->move(fleePosition);
			continue;
		}


		if (Squads[0].Goliaths[i]->getGroundWeaponCooldown() == 0) {
			if (closest != NULL) {
				if (Squads[0].Goliaths[i]->getOrderTarget() == NULL) {
					Squads[0].Goliaths[i]->attack(closest);
				}
				else if (Squads[0].Goliaths[i]->getOrderTarget()->getID() != closest->getID()) {
					Squads[0].Goliaths[i]->attack(closest);
				}
				continue;
			}

		}

		//if there are no enemies nearby, attack;
		if (closest == NULL) { //|| closeDist > 3*32
			if (Squads[0].Goliaths[i]->getPosition().getDistance(DefaultPos) > 3 * 32) {
				//Marines[i].state = M_Attacking;
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
				//prevent spam
				if (Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile &&
					Squads[0].Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit) {
					Squads[0].Goliaths[i]->attack(DefaultPos);
				}
				//Broodwar->printf("Attack");
			}
			continue;
		}
	}





	for (unsigned int i = 0; i<Squads[0].Tanks.size(); i++) {
		//Broodwar->printf("%s",Marines[i]->getOrder().c_str() );

		Unitset EnemyClose = Squads[0].Tanks[i]->getUnitsInRadius(15 * 32);
		Unit closest = NULL;
		int closeDist = 9999999;
		BOOST_FOREACH(Unit close, EnemyClose)
		{
			if (close->getPlayer() == Broodwar->enemy() && closeDist > Squads[0].Tanks[i]->getDistance(close->getPosition())) {
				closeDist = Squads[0].Tanks[i]->getDistance(close->getPosition());
				closest = close;
			}
		}



		// if we can't shoot, run away  if(kite)
		//&&  Squads[0].Marines[i]->getGroundWeaponCooldown() != 0 
		//BWTA::getRegion( Squads[0].Vultures[i]->getPosition() ) != InfoMan->OurMainregion 
		if ((closeDist < 5 * 32 && Squads[0].Tanks[i]->getGroundWeaponCooldown() != 0
			&& Squads[0].Tanks[i]->getDistance(WDMan->ToRetreat) > 5 * 32)
			||
			Squads[0].Tanks[i]->getDistance(InfoMan->PosMainChoke) < 2 * 32
			||
			BWTA::getRegion(Squads[0].Tanks[i]->getTilePosition()) != InfoMan->OurMainregion
			)
		{



			//BWAPI::Position fleePosition(Marines[i].marine->getPosition() - closest->getPosition() + Marines[i].marine->getPosition());
			//BWAPI::Position fleePosition( InfoMan->PosOurBase );
			BWAPI::Position fleePosition(WDMan->ToRetreat);

			//BWAPI::Broodwar->drawLineMap(Marines[i]->getPosition().x, Marines[i]->getPosition().y, 
			//fleePosition.x, fleePosition.y, BWAPI::Colors::Cyan);

			Squads[0].Tanks[i]->move(fleePosition);
			continue;

			//Marines[i].state = M_Fleeing;
		}


		if (Squads[0].Tanks[i]->getGroundWeaponCooldown() == 0) {

			if (closest != NULL) {
				if (Squads[0].Tanks[i]->getOrderTarget() == NULL) {
					Squads[0].Tanks[i]->attack(closest);
				}
				else if (Squads[0].Tanks[i]->getOrderTarget()->getID() != closest->getID()) {
					Squads[0].Tanks[i]->attack(closest);
				}
				continue;
			}

		}

		//if there are no enemies nearby, attack;
		if (closest == NULL) { //|| closeDist > 3*32
							   //BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase ); 

			if (Squads[0].Tanks[i]->getPosition().getDistance(DefaultPos) > 3 * 32) {
				//Marines[i].state = M_Attacking;
				//BWAPI::Position attackPos = BWAPI::Position( InfoMan->EnemyBase );
				//prevent spam
				if (Squads[0].Tanks[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Squads[0].Tanks[i]->getOrder() != BWAPI::Orders::AttackTile &&
					Squads[0].Tanks[i]->getOrder() != BWAPI::Orders::AttackUnit) {
					Squads[0].Tanks[i]->attack(DefaultPos);
				}
				//Broodwar->printf("Attack");
			}

			continue;
		}
	}




}

void MacroManager::OnFrameTerranSquad(){

	//only send commands during the command frame
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0) {
		return;
	}


	for(int i=0; i<Squads.size(); i++){
		if( Squads[i].SQstate == SQ_Defend_Base ){
			if( TState == T_Choke_Defend || TState == T_Hold_BBS ){
			  OnFrameTerranSquadDefence(i);
			}
		}
		if( Squads[i].SQstate == SQ_Attack_Base ){
			OnFrameTerranSquadAttack(i);
		}
	}



}

void MacroManager::OnFrameTerran(){


	//add new tank positions
	BOOST_FOREACH( BWAPI::Unit tanks, Broodwar->enemy()->getUnits() ){

		if( tanks->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ){
			bool exists = false;
			for( int j=0; j<ETankPositions.size(); j++){
				if(  tanks->getPosition() == ETankPositions[j] ){
					exists = true;
					break;
				}
			}
			if( exists == false) {
				ETankPositions.push_back( tanks->getPosition() );
			}
		}

	}

	//check if tank positions are still valid
	for( int j=0; j<ETankPositions.size(); j++){
		if( Broodwar->isVisible( BWAPI::TilePosition( ETankPositions[j] ) ) ){
			bool stillThere = false;
	      BOOST_FOREACH( BWAPI::Unit tanks, Broodwar->enemy()->getUnits() ){
		   if( tanks->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
			   &&  tanks->getDistance( ETankPositions[j] ) < 1*32 ){
				   stillThere = true;
		     }
		  }
		  if( stillThere == false){
			  ETankPositions.erase( ETankPositions.begin() + j );
			  j--;
			  continue;
		  }
		}
	}

		//ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition( InfoMan->PosMainChoke) , true);
		//WallBunkerPlaced = true;


	if( TState == T_Choke_Defend  
		&& Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Marine ) > Squads[0].Marines.size() 
		 && WallBunkerPlaced == false
		 && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Factory ) == 0 ){
			 ProdMan->addToQueueBegin( BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition( InfoMan->PosMainChoke)  );
		//ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker, BWAPI::TilePosition( InfoMan->PosMainChoke) , true);
		WallBunkerPlaced = true;
		TState = T_Hold_BBS;
		//remove the factory to prioritise bunker to hold BBS
		if( ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Factory ) > 0 ){
			for( int b=0;b<ProdMan->BuildingsQueue.size(); b++ ){
				if( ProdMan->BuildingsQueue[b].type == BWAPI::UnitTypes::Terran_Factory ){
					//cancel the building
					if( ProdMan->BuildingsQueue[b].Started == true ){
						if( ProdMan->BuildingsQueue[b].building != NULL ){
							ProdMan->BuildingsQueue[b].building->cancelConstruction();
						}
					}

					if( ProdMan->BuildingsQueue[b].scv != NULL){
						CCmanager[0]->addSCV( ProdMan->BuildingsQueue[b].scv );
					}
					ProdMan->BuildingsQueue.erase( ProdMan->BuildingsQueue.begin() + b);
					ProdMan->addToBuildOrderBegin( BWAPI::UnitTypes::Terran_Factory );
					CCmanager[0]->SCVsonGas = 0;
					break;
				}
			}
		}
	}


	if( TState == T_Hold_BBS &&  Squads[0].Marines.size() > 2  ){
		TState = T_Choke_Defend;
		CCmanager[0]->SCVsonGas = 3;
	}


	int TotalCCenemy = 0;
	BOOST_FOREACH(BuildingInfo building, InfoMan->EnemyBuildings) {
		if (building.type == BWAPI::UnitTypes::Terran_Command_Center) {
			TotalCCenemy++;
		}
		if (building.type == BWAPI::UnitTypes::Terran_Bunker) {
			EnemyTerranHasBunker = true;
		}
		//EnemyTerranHasBunker
	}
	//rush the opponent in case of a fast expand
	if (TotalCCenemy > 1) {
		TState = T_Push;
		CCmanager[0]->SCVsonGas = 3;
		Squads[0].SQstate = SQ_Attack_Base;
	}


	// && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) > 2 
	// && Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) > 2 
	// Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Goliath) > 1 
	// 4
	if( (TState == T_Choke_Defend ||  TState == T_Hold_BBS)
		&& Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Goliath) > 4 ){

	  TState = T_Push;
	  CCmanager[0]->SCVsonGas = 3;
	  Squads[0].SQstate = SQ_Attack_Base;


	  MyRectangle area = bManager->getBuildRectangle( BWAPI::TilePosition( InfoMan->PosMainChoke ) , BWAPI::UnitTypes::Terran_Missile_Turret);
	  bManager->mapArea(area, 0,2);
	  bManager->mapWalkable(area, false);
	  //bManager->constructionPlaced

	  /*
	  if( ProdMan->Barracks.size() > 0 ){
		  FloatingBarracks = ProdMan->Barracks[0];
		  ProdMan->Barracks.erase( ProdMan->Barracks.begin() + 0 );
	  }
	  */
	}


	//get floating barracks
	if( TState == T_Push ){

      if( FloatingBarracks != NULL){
		  if( !FloatingBarracks->exists() ){
			  FloatingBarracks = NULL;
		  }
	  }

	  if( FloatingBarracks == NULL && ProdMan->Barracks.size() > 0 ){
		  FloatingBarracks = ProdMan->Barracks[0];
		  ProdMan->Barracks.erase( ProdMan->Barracks.begin() + 0 );
	  }

	}



	/*
	//switch to golaiths in case of wraiths or mass vulture
	if( TState == T_Push ){

		int TotalEWraith = Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Wraith );
		int TotalEVulture = Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture );
		//BOOST_FOREACH( BWAPI::Unit enemy , Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Wraith ) ){
		//}

		if( TotalEVulture > Squads[0].Vultures.size() + 2 ){
			for(int i=0; i<FollowupT.size(); i++ ){
				if( FollowupT[i] == BWAPI::UnitTypes::Terran_Armory ){
					FollowupT.erase( FollowupT.begin() + i);
					ProdMan->addToQueueBegin( BWAPI::UnitTypes::Terran_Armory );
					break;
				}
			}
		}

		if( TotalEWraith > 0 ){
			for(int i=0; i<FollowupT.size(); i++ ){
				if( FollowupT[i] == BWAPI::UnitTypes::Terran_Armory ){
					FollowupT.erase( FollowupT.begin() + i);
					ProdMan->addToQueueBegin( BWAPI::UnitTypes::Terran_Armory );
					break;
				}
			}
		}

	}
	*/


	OnFrameTerranSquad();

}



void MacroManager::RemoveDead(){

	for(int i=0; i<Squads.size(); i++){
	  for(int j=0; j<Squads[i].Marines.size(); j++){
		if( !Squads[i].Marines[j]->exists() ){
			Squads[i].Marines.erase(Squads[i].Marines.begin() + j );
			j--;
			continue;
		}
	  }
	  for(int j=0; j<Squads[i].Medics.size(); j++){
		if( !Squads[i].Medics[j]->exists() ){
			Squads[i].Medics.erase(Squads[i].Medics.begin() + j );
			j--;
			continue;
		}
	  }
	  for(int j=0; j<Squads[i].FireBats.size(); j++){
		if( !Squads[i].FireBats[j]->exists() ){
			Squads[i].FireBats.erase(Squads[i].FireBats.begin() + j );
			j--;
			continue;
		}
	  }
	  for(int j=0; j<Squads[i].Tanks.size(); j++){
		if( !Squads[i].Tanks[j]->exists() ){
			Squads[i].Tanks.erase(Squads[i].Tanks.begin() + j );
			j--;
			continue;
		}
	  }
	  for(int j=0; j<Squads[i].SCVs.size(); j++){
		if( !Squads[i].SCVs[j]->exists() ){
			Squads[i].SCVs.erase(Squads[i].SCVs.begin() + j );
			j--;
			continue;
		}
	  }

	  for(int j=0; j<Squads[i].ScienceVessels.size(); j++){
		if( !Squads[i].ScienceVessels[j]->exists() ){
			Squads[i].ScienceVessels.erase(Squads[i].ScienceVessels.begin() + j );
			j--;
			continue;
		}
	  }

	  for(int j=0; j<Squads[i].BattleCruisers.size(); j++){
		if( !Squads[i].BattleCruisers[j]->exists() ){
			Squads[i].BattleCruisers.erase(Squads[i].BattleCruisers.begin() + j );
			j--;
			continue;
		}
	  }

	  for(int j=0; j<Squads[i].Vultures.size(); j++){
		if( !Squads[i].Vultures[j]->exists() ){
			Squads[i].Vultures.erase(Squads[i].Vultures.begin() + j );
			j--;
			continue;
		}
	  }

	  for(int j=0; j<Squads[i].Bunkers.size(); j++){
		if( !Squads[i].Bunkers[j]->exists() ){
			Squads[i].Bunkers.erase(Squads[i].Bunkers.begin() + j );
			j--;
			continue;
		}
	  }

	}

}

void MacroManager::OnFrame(){
	RemoveDead();

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
      //display tanks
	  for( int j=0; j<ETankPositions.size(); j++){
		  Broodwar->drawCircleMap( ETankPositions[j].x, ETankPositions[j].y , 3, BWAPI::Colors::Red, true );
	  }
	}

	/*
    if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
      return;
    }
	*/

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		OnFrameZerg();
	}
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
	  OnFrameProtoss();
	}
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
	  OnFrameTerran();
	}

}


BWAPI::UnitType MacroManager::BarrackSuggest(){

	/*
	if( ProdMan->getAvailableMinerals() < 50 ){
		return BWAPI::UnitTypes::None;
	}
	*/

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){

		if( ZState == Z_Init ||  ZState == Z_Pool_Defend
			|| Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Marine) == 0 ){
			return BWAPI::UnitTypes::Terran_Marine;
		}

		if( ProdMan->Academy == NULL ) {
			return BWAPI::UnitTypes::Terran_Marine;
		}
		if( Squads[0].Medics.size() * 5 <  Squads[0].Marines.size() && ProdMan->getAvailableGas() > 50 ){
			return BWAPI::UnitTypes::Terran_Medic;
		}
		return BWAPI::UnitTypes::Terran_Marine;

	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){

	if( ProdMan->getAvailableMinerals() < 50 && ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 1  ){
		return BWAPI::UnitTypes::None;
	}



		if( PState == P_Wall){

			if( MacroMan->CurrentStrat == "OneFact3Rax"
				&& ProdMan->Academy != NULL 
				&& ProdMan->getAvailableGas() > 150
				&& MacroMan->Squads[0].Marines.size() > MacroMan->Squads[0].Medics.size()*4
				){
				return BWAPI::UnitTypes::Terran_Medic;
			}


			if( MacroMan->Squads[0].Marines.size() > 3 
				&& ProdMan->TotalTypeInQueue( BWAPI::UnitTypes::Terran_Command_Center ) == 0 
				&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Command_Center ) == 1
				&& Broodwar->self()->minerals() < 450 ){
				return BWAPI::UnitTypes::None;
			}

			if( ProdMan->FactoryNoAddon.size() > 0 && Squads[0].Marines.size() > 1){
				return BWAPI::UnitTypes::None;
			}
			if( ProdMan->FactoryShop.size() > 0  ){
				if( ProdMan->FactoryShop[0]->getTrainingQueue().size() == 0 
					&& ProdMan->getAvailableMinerals() < 170 ){
					return BWAPI::UnitTypes::None;
				}
			}
			return BWAPI::UnitTypes::Terran_Marine;
		}
		if( ProdMan->Academy == NULL ){
			return BWAPI::UnitTypes::Terran_Marine;
		}
		if( ProdMan->ScienceFacility != NULL ){
			if(  ProdMan->ScienceFacility->getAddon() != NULL ){
				if( Broodwar->self()->hasResearched( BWAPI::TechTypes::Lockdown) 
					||  ProdMan->ScienceFacility->getAddon()->isRepairing() ){
						if( Squads[0].Ghosts.size() < 8 &&  Squads[0].ScienceVessels.size() > 0 ){
					  return BWAPI::UnitTypes::Terran_Ghost;
				    }
				}
			}
		}

		int ReservedGas = ProdMan->getAvailableGas();
		if( Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Science_Vessel ) == 0 ){
			ReservedGas -= 250;
			if( MacroMan->CurrentStrat  == "OneFact3Rax" ){ //this strategy doesn't use science vessels
				ReservedGas += 200;
			}
		}
		// ProdMan->getAvailableGas() > 50 
		if( Squads[0].Medics.size() * 4 <  Squads[0].Marines.size() && ReservedGas > 50 ){
			return BWAPI::UnitTypes::Terran_Medic;
		}
		// ProdMan->getAvailableGas() > 250
		if( Squads[0].FireBats.size() * 6 <  Squads[0].Marines.size() && ReservedGas > 250 ){
			return BWAPI::UnitTypes::Terran_Firebat;
		}

		/*
		if( Squads[0].FireBats.size() * 4 <  Squads[0].Marines.size() && ProdMan->getAvailableGas() > 150 ){
			return BWAPI::UnitTypes::Terran_Firebat;
		}
		*/
		return BWAPI::UnitTypes::Terran_Marine;
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){

		//&& ProdMan->getAvailableMinerals() > 50
		// Squads[0].Marines.size()  <= (ProdMan->FactoryNoAddon.size() + ProdMan->FactoryShop.size() )
		if( MacroMan->TState == T_Choke_Defend &&
			Squads[0].Marines.size()  <= 0
			 ){
			return BWAPI::UnitTypes::Terran_Marine;
		}     

		if( MacroMan->TState == T_Hold_BBS &&
			Squads[0].Marines.size()  <= 3 
			 ){
			return BWAPI::UnitTypes::Terran_Marine;
		}   

	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Unknown
		|| Broodwar->enemy()->getRace() == BWAPI::Races::Random ){
         return BWAPI::UnitTypes::Terran_Marine;
	}

	return BWAPI::UnitTypes::None;
}

BWAPI::UnitType MacroManager::FactorySuggest(){

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
	
		if( ProdMan->FactoryShop.size() >= 1 ){
			return BWAPI::UnitTypes::Terran_Machine_Shop;
		}

		if( (Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) > 0
			&& ProdMan->FactoryShop.size() == 0) ){
			return BWAPI::UnitTypes::Terran_Machine_Shop;
		}
	
		if(Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) == 0){
			return BWAPI::UnitTypes::Terran_Vulture;
		}

		if( ProdMan->Armory.size() > 0){
			return BWAPI::UnitTypes::Terran_Goliath;
		}
	}


	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
		return BWAPI::UnitTypes::Terran_Machine_Shop;
		/*
		if( ProdMan->FactoryShop.size() < 2){
			return BWAPI::UnitTypes::Terran_Machine_Shop;
		}
		else {
			return BWAPI::UnitTypes::Terran_Vulture;
		}
		*/

	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){

		//return BWAPI::UnitTypes::Terran_Machine_Shop;


		//Default Terran
		/*
		if(Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) == 0){
		  //return BWAPI::UnitTypes::Terran_Vulture;
		}
			//marine rush, build vultures
		if( Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Marine ) > 3
			|| Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Firebat ) > 0  ){
			//return BWAPI::UnitTypes::Terran_Vulture;
		}

		return BWAPI::UnitTypes::Terran_Machine_Shop;
		*/

		//Goliath pressure

		//the opponent has a bunker, use tanks to take it out
		if (EnemyTerranHasBunker == true) {
			return BWAPI::UnitTypes::Terran_Machine_Shop;
		}

		if (Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Armory) > 0
			&& ProdMan->getAvailableGas() > 50) {
			return BWAPI::UnitTypes::Terran_Goliath;
		}
		else {
			return BWAPI::UnitTypes::Terran_Vulture;
		}



		//other Terran

		if(Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) >= 2
			&& ProdMan->FactoryShop.size() == 0 
			&& Broodwar->self()->incompleteUnitCount(  BWAPI::UnitTypes::Terran_Machine_Shop ) == 0  ){

				//Broodwar->self()->incompleteUnitCount(  )

			//return BWAPI::UnitTypes::Terran_Machine_Shop;

		}

		/*
		if(Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) >= 3
			&& ProdMan->FactoryShop.size() == 1 ){
			return BWAPI::UnitTypes::Terran_Machine_Shop;

		}
		*/


		if( Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Wraith ) > 0
			&&  MacroMan->Squads[0].Goliaths.size() == 0
			&&  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Armory ) > 0 ){
			//return BWAPI::UnitTypes::Terran_Goliath;
		}

		if( Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Armory ) > 0
			&& ProdMan->getAvailableGas() > 150 ){
			//return BWAPI::UnitTypes::Terran_Goliath;
		}

		//if(Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) == 0){
			return BWAPI::UnitTypes::Terran_Vulture;
		//}

	}

	return BWAPI::UnitTypes::None;
}

BWAPI::UnitType MacroManager::FactorySuggestShop(){

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		//if( ProdMan->getAvailableGas() > 100 && ProdMan->getAvailableMinerals() > 150){
			//return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
		//if( CurrentStrat == "Goliath M&M"){
		 if( ProdMan->Armory.size() > 0 ){
		  if( Squads[0].Goliaths.size() > 2){
			return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
		  }
		  return BWAPI::UnitTypes::Terran_Goliath;
		}
		if( CurrentStrat == "Standard Flowchart"){

			if (ZState == Z_Push) {
				return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
			}

			if (ProdMan->getAvailableGas() > 250
				&& Squads[0].Tanks.size() < 3) {
				return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
			}

			//on 1 base play, reserve some gas for tanks
			if (CCmanager.size() == 1
				&& Squads[0].Tanks.size() > 0) {
				return BWAPI::UnitTypes::Terran_Vulture;
			}


			return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;

		    //return BWAPI::UnitTypes::Terran_Goliath;

			/*
			int TotalTanks = 5;
			if(  Squads[0].Tanks.size() < TotalTanks &&  150 <= ProdMan->getAvailableMinerals() && 100 <= ProdMan->getAvailableGas()){
		      return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
			}
			if(  Squads[0].Tanks.size() >= TotalTanks &&  75 <= ProdMan->getAvailableMinerals()  ){
				return BWAPI::UnitTypes::Terran_Vulture;
			}
			*/

		}
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){

		if( PState == P_Wall ){
		  int TotalZealotCount = 0;
		  for( int i=0; i<InfoMan->EnemyCombatUnits.size(); i++ ){
			  if( InfoMan->EnemyCombatUnits[i]->getType() == BWAPI::UnitTypes::Protoss_Zealot ){
				  TotalZealotCount++;
			  }
		  }
		  if( TotalZealotCount > 3 && Squads[0].Vultures.size() <  Squads[0].Tanks.size() ){
			  return BWAPI::UnitTypes::Terran_Vulture;
		  }
		}
		
		if (ZealotRushExpected == true && Squads[0].Vultures.size() * 2 < Squads[0].Tanks.size()) {
			return BWAPI::UnitTypes::Terran_Vulture;
		}

		return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
		/*
		// && ProdMan->getAvailableMinerals() > 150 
		if( ProdMan->getAvailableGas() > 100 ){
			return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
		} else if( ProdMan->getAvailableMinerals() > 75 ){
			return BWAPI::UnitTypes::Terran_Vulture;
		}
		*/
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){

		int TotalWraith = 0;
		for(int i=0; i<InfoMan->EnemyCombatUnits.size(); i++){
			if( InfoMan->EnemyCombatUnits[i]->getType() == BWAPI::UnitTypes::Terran_Wraith ){
				TotalWraith++;
			}
		}



		int TotalOurGoliath = Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Goliath );
		if( (TotalWraith > 2  || TotalOurGoliath == 0 )
			&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Armory ) > 0 
			&& (TotalOurGoliath < 2 || TotalOurGoliath < TotalWraith - 2 )  ){
			return BWAPI::UnitTypes::Terran_Goliath;
		}

		//go for more goliaths in case of a lot of vultures
		if(  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Armory ) > 0 
			&& Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Goliath ) < 3
			&& Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Vulture) > Squads[0].Vultures.size() + 2 
			  ){
			//return BWAPI::UnitTypes::Terran_Goliath;
		}

		/*
		//marine rush, build vultures
		if( Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Marine ) > 4
			|| Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Firebat ) > 0 ){
			return BWAPI::UnitTypes::Terran_Vulture;
		}
		*/

		// ProdMan->getAvailableGas() > 100  && ProdMan->getAvailableMinerals() > 150
		if( ProdMan->getAvailableGas() > 100    ){

			return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;

				if(  Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) > 0
					||  Broodwar->enemy()->completedUnitCount( BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode) > 0
					|| ETankPositions.size() > 0
					 ){
			return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
				} else {
					if(  Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Armory ) > 0 ){
						//return BWAPI::UnitTypes::Terran_Goliath;
						return BWAPI::UnitTypes::Terran_Vulture;
					} else {
					  return BWAPI::UnitTypes::Terran_Vulture;
					}
				}
		} else {
			return BWAPI::UnitTypes::Terran_Vulture;
		}

		return BWAPI::UnitTypes::Terran_Vulture;

	}

	return BWAPI::UnitTypes::None;

}



BWAPI::UnitType MacroManager::StarPortNoTSuggest(){

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		//if( ProdMan->getAvailableGas() > 100 && ProdMan->getAvailableMinerals() > 150){
			//return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;

		if( CurrentStrat == "Standard Flowchart"){

			return BWAPI::UnitTypes::Terran_Control_Tower;
            /*
			if( Broodwar->self()->completedUnitCount( BWAPI::UnitTypes::Terran_Wraith ) == 0 ){
				return BWAPI::UnitTypes::Terran_Wraith;
			} else {
				return BWAPI::UnitTypes::Terran_Control_Tower;
			}
			*/


		}
		if( CurrentStrat == "2 Port Wraith"){
			return BWAPI::UnitTypes::Terran_Wraith;
		}
	}

	return BWAPI::UnitTypes::None;

}


BWAPI::UnitType MacroManager::StarPortCTSuggest(){

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
		//if( ProdMan->getAvailableGas() > 100 && ProdMan->getAvailableMinerals() > 150){
			//return BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;

		if( CurrentStrat == "Standard Flowchart"){
			//start off with valkryies
			if(   Squads[0].ScienceVessels.size() < 3 ){
				return BWAPI::UnitTypes::Terran_Science_Vessel;
			} else {
				return BWAPI::UnitTypes::Terran_Battlecruiser;
			}
		}
		/*
		if( CurrentStrat == "2 Port Wraith"){
			return BWAPI::UnitTypes::Terran_Wraith;
		}
		*/
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
		return BWAPI::UnitTypes::Terran_Battlecruiser;

	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
		if(   Squads[0].ScienceVessels.size() < 3 ){
		   return BWAPI::UnitTypes::Terran_Science_Vessel;
		}
	}

	//return BWAPI::UnitTypes::None;
	return BWAPI::UnitTypes::None;

}

void MacroManager::AddSCV( Unit unit){

	if( unit == NULL){
		return;
	}

	if( unit->getPlayer() == Broodwar->enemy() ){
		return;
	}


	if( unit->getType() == BWAPI::UnitTypes::Terran_SCV){ // && ZState == Z_Pool_Defend
		Squads[0].SCVs.push_back(unit);
	}


}


void MacroManager::PlaceNatBunker(){

	/*
	if( Broodwar->enemy()->getRace() != BWAPI::Races::Zerg){
		return;
	}

	*/

	if( NatBunkerPlace == true){
		return;
	}

	Broodwar->printf("Adding bunker to the natural");

	  int ClosestChokeDist = 99999;
	  BWAPI::Position BunkerPos = BWAPI::Positions::None;
	  BWAPI::Position natPos = BWAPI::Position(InfoMan->OurNat);
	  BWAPI::Position mainPos = InfoMan->PosOurBase; //BWAPI::Position(InfoMan->PosOurBase);
	  BWTA::Region* natRegion = BWTA::getRegion(BWAPI::TilePosition(natPos) );
	  BWTA::Region* mainRegion = BWTA::getRegion(BWAPI::TilePosition(mainPos ) );

       BOOST_FOREACH( BWTA::Chokepoint*  choke, BWTA::getChokepoints() ){
		   if( (choke->getRegions().first == natRegion && choke->getRegions().second != mainRegion ) ||
		   (choke->getRegions().second == natRegion && choke->getRegions().first != mainRegion ) ){
			 if( choke->getCenter().getDistance( InfoMan->PosEnemyBase ) < ClosestChokeDist ){
			   ClosestChokeDist = choke->getCenter().getDistance( InfoMan->PosEnemyBase );
			   BunkerPos = choke->getCenter();
			 }
	     }
       }

	   //add two bunkers and a turret in case of lurkers	  
	   bool ExtraLurkerBunk = false;


	   if( BunkerPos != BWAPI::Positions::None ){

		   float closePnat = 0.25;
		   BWTA::BaseLocation* natBase = 0;
		   BOOST_FOREACH(BWTA::BaseLocation* base, BWTA::getRegion(InfoMan->OurNat)->getBaseLocations()) {
			   natBase = base;
		   }
		    //BWTA::BaseLocation* natBase = BWTA::getRegion(InfoMan->OurNat)->getBaseLocations().;
			//TODO: check if this is really the right distance for turret placement
			if (natBase != 0 && natBase->getPosition().getDistance(BunkerPos) < 8 * 32) {
				closePnat += 0.05;
			}

		   float closePunk = 1 - closePnat;
		   BWAPI::Position MissleTurretPos(  (int)((BunkerPos.x * closePunk) + (closePnat* InfoMan->PosOurNat.x) ) ,(int)((BunkerPos.y * closePunk) + (closePnat * InfoMan->PosOurNat.y )) );


		   //ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition( BunkerPos ),false );
		   //ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),false );
		   ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),BWTA::getRegion( InfoMan->OurNat )  );
		   if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && ExtraLurkerBunk == true ){
		    // ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),false );
		     //ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , BWAPI::TilePosition( BunkerPos ),BWTA::getRegion( InfoMan->OurNat )  );
		     //ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition( MissleTurretPos ),false );
		     ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition( MissleTurretPos ), BWTA::getRegion( InfoMan->OurNat ) );
		   }

		   //add an extra turret if you are defending against a 2 hatch
		   if (ZState == Z_Expand_Defend_2_hatch) {
			   ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , BWAPI::TilePosition( MissleTurretPos ),false );
		   }

		   TileBunkerPos = BWAPI::TilePosition( BunkerPos );
		   NatBunkerPlace = true;
	   } else {

		   if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg && ExtraLurkerBunk == true ){
		     ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Missile_Turret , InfoMan->OurNat, false );
		     ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , InfoMan->OurNat, false );
		   }
		   ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , InfoMan->OurNat, false );
		   //ProdMan->addToQueueClose( BWAPI::UnitTypes::Terran_Bunker , InfoMan->OurNat, BWTA::getRegion( InfoMan->OurNat ) );
		   NatBunkerPlace = true;
	   }

}

void MacroManager::AddUnit( Unit unit){

	//Broodwar->printf("added unit to macro manager");

	if( unit == NULL){
		return;
	}

	if( unit->getPlayer() == Broodwar->enemy() ){
		return;
	}


	//Command canter complete
	if( unit->getType() == BWAPI::UnitTypes::Terran_Command_Center
		&& unit->getDistance( InfoMan->PosOurNat ) < 5*32  ){

		//PlaceNatBunker();
	}

	/*
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
		MicroProtoss->AddUnit( unit);
		return;
	}
	*/


	if( unit->getType() == BWAPI::UnitTypes::Terran_Marine){

		if( Squads.size() > 1 && PState == P_Push ){
			if( Squads[1].Marines.size() < 2 ){
		        Squads[1].Marines.push_back(unit);
			} else {
		      Squads[0].Marines.push_back(unit);
			}
		} else {
		  Squads[0].Marines.push_back(unit);
		}
		// Squads[0].Marines.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Medic){

		if( Squads.size() > 1 && PState == P_Push ){
			if( Squads[1].Medics.size() < 2 ){
		        Squads[1].Medics.push_back(unit);
			} else {
		      Squads[0].Medics.push_back(unit);
			}
		} else {
		  Squads[0].Medics.push_back(unit);
		}
		//Squads[0].Medics.push_back(unit);


	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Firebat){

		if( Squads.size() > 1 && PState == P_Push ){
			if( Squads[1].FireBats.size() < 1 ){
		        Squads[1].FireBats.push_back(unit);
			} else {
		      Squads[0].FireBats.push_back(unit);
			}
		} else {
		  Squads[0].FireBats.push_back(unit);
		}
		//Squads[0].FireBats.push_back(unit);

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
	if( unit->getType() == BWAPI::UnitTypes::Terran_Wraith){
		Squads[0].Wraiths.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Valkyrie){
		Squads[0].Valkryies.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Ghost ){
		Squads[0].Ghosts.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Bunker){

		Squads[0].Bunkers.push_back(unit);

		if( BWTA::getRegion( unit->getTilePosition() ) != InfoMan->OurMainregion  ){
			NatBunkerChoke = unit;
		}

	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel){
		Squads[0].ScienceVessels.push_back(unit);
	}
	if( unit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser){
		Squads[0].BattleCruisers.push_back(unit);
	}

	/*
	if( unit->getType() == BWAPI::UnitTypes::Terran_SCV && ZState == Z_Plug_Ramp
		&& Squads[0].SCVs.size() < 4  ){
		Squads[0].SCVs.push_back(unit);
	}
	*/

}


void MacroManager::onUnitDestroy(Unit unit){

	if( unit == NULL){
		return;
	}


	if( unit->getType() == BWAPI::UnitTypes::Protoss_Reaver ||  unit->getType() == BWAPI::UnitTypes::Protoss_Shuttle ){

		//push out when a reaver or shuttle dies
	  if( PState == P_Defend_Nat  ){
	    PState = P_Push;
	    Squads[0].SQstate = SQ_Attack_Base;
	    StartAttackFrame = Broodwar->getFrameCount();
	   
	  }

	}

	for(int i=0; i<ELurkerPositions.size(); i++){
		if( ELurkerPositions[i].UnitID == unit->getID() ){
			ELurkerPositions.erase( ELurkerPositions.begin() + i);
			i--;
			break;
		}
	}

	if( NoNeedTank == false && unit->getType() == BWAPI::UnitTypes::Protoss_Nexus ){
		NoNeedTank = true;
	}

	if( unit->getType().isBuilding() && unit->getPlayer() == Broodwar->enemy() ){
      HuntAllowedP = true;
	  HuntAllowedT2 = true;
	}

    if( currentBuildingP != NULL ){

	  if(  unit->getID() == currentBuildingP->getID()  ){
		HuntAllowedP = true;
	    NextBuildingMMp();
		//ProdMan->addToQueue( BWAPI::UnitTypes::Terran_Starport );
	  }

	}

    if( currentBuildingT2 != NULL ){

	  if(  unit->getID() == currentBuildingT2->getID()  ){
		HuntAllowedT2 = true;
	    NextBuildingMMt();
		//ProdMan->addToQueue( BWAPI::UnitTypes::Terran_Starport );
	  }

	}

	/*
	if( currentBuilding == NULL ){
		return;
	}
	*/

	if( currentBuilding != NULL ){
	//switch to wraiths after the main command center is destroyed
	  if(  unit->getID() == currentBuilding->getID()  ){
		HuntAllowed = true;
	    NextBuildingMM();
		//ProdMan->addToQueue( BWAPI::UnitTypes::Terran_Starport );
	  }
	}

}