#include "Common.h"
#include "BuildOrderManager.h"
#include <boost/foreach.hpp>



void BuildOrderManager::TwoPortWraithvsZerg(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "2 Port Wraith";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 30;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.WallNeeded = true;
	BOs.push_back(newBo);


}

void BuildOrderManager::PlusOne5Rax(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "+1 5 Rax";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );

	//standard followup
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Bunker;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Comsat_Station;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Comsat_Station;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Missile_Turret;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Missile_Turret;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("2 Hatch Muta");
	newBo.StrongCounterBy.push_back("2 Hatch Lurker");
	//newBo.WeakCounterBy.push_back("Early Speedling");
	//unrolling early speedling
	newBo.WeakCounterBy.push_back("9 pool speed");
	newBo.WeakCounterBy.push_back("3 Hatch Lurker");
	newBo.StrongCounterTo.push_back("3 Hatch Before Pool");//    Fast 3rd Base
	newBo.WeakCounterTo.push_back("3 Hatch Muta");//    Fast 3rd Base

	newBo.WallNeeded = false;
	BOs.push_back(newBo);


}

void BuildOrderManager::OneFactoryStarPort(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Factory Starport";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Science_Facility;
	newBo.BOpieces.push_back( addPiece );
	addPiece.Building = BWAPI::UnitTypes::Terran_Missile_Turret;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Missile_Turret;
	newBo.BOpieces.push_back( addPiece );


	newBo.WeakCounterBy.push_back("2 Hatch Lurker");
	newBo.WeakCounterBy.push_back("3 Hatch Lurker");
	newBo.WeakCounterTo.push_back("2 Hatch Muta");//    Fast 3rd Base

	newBo.WallNeeded = false;
	BOs.push_back(newBo);


}

void BuildOrderManager::OneRaxFEvsZerg(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Rax FE";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );

	newBo.WallNeeded = false;
	BOs.push_back(newBo);


}

void BuildOrderManager::OneRaxGas(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Rax Gas";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterTo.push_back("9 pool");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);


}

void BuildOrderManager::CC14vsZerg(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "14 CC";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 30;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 36;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 40;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 42;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 46;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 50;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("9 pool");
	newBo.StrongCounterBy.push_back("5 pool");
	newBo.StrongCounterBy.push_back("4 pool");
	newBo.WeakCounterBy.push_back("12 pool");
	newBo.WeakCounterBy.push_back("3 hatch before pool");
	newBo.WeakCounterTo.push_back("12 hatch");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);


}

void BuildOrderManager::TwoRaxFE(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Rax FE";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterBy.push_back( "3 Hatch Muta" );

	newBo.WallNeeded = false;
	BOs.push_back(newBo);


}

void BuildOrderManager::AyumiBuild(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Ayumi Build";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 20;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 35;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 37;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 38;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 44;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("4 pool");
	newBo.StrongCounterBy.push_back("5 pool");
	newBo.StrongCounterBy.push_back("3 Hatch Zerling");
	newBo.StrongCounterBy.push_back("2 Hatch Muta");
	newBo.StrongCounterBy.push_back("2 Hatch Lurker");
	newBo.WeakCounterBy.push_back( "9 pool" );
	newBo.StrongCounterTo.push_back("3 Hatch Muta");
	newBo.StrongCounterTo.push_back("3 Hatch Muta");

	newBo.StrongMap.push_back("Neo Requiem");
	newBo.StrongMap.push_back("Peaks of Baekdu");
	newBo.WeakMap.push_back("Destination");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);


}


void BuildOrderManager::BBSvsZerg(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "BBS";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.WallNeeded = false;
	BOs.push_back(newBo);


}

void BuildOrderManager::BunkerRush(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "8 Rax";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("4 pool");
	newBo.StrongCounterBy.push_back("9 pool");
	newBo.WeakCounterBy.push_back("12 pool");
	newBo.StrongCounterTo.push_back("12 Hatch");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}

void BuildOrderManager::FantasyBuild(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Fantasy Build";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );


	newBo.WeakCounterBy.push_back("2 Hatch Muta");
	newBo.WeakCounterBy.push_back("3 Hatch Hydra");
	newBo.StrongCounterTo.push_back("3 Hatch Muta");

	newBo.StrongMap.push_back("Destination");
	newBo.StrongMap.push_back("Medusa");
	newBo.StrongMap.push_back("Raid Assault II");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}

void BuildOrderManager::iloveoovFakeMech(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "iloveoov Fake Mech";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Armory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Control_Tower;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Missile_Turret;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Missile_Turret;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterBy.push_back("9 pool");
	newBo.WeakCounterTo.push_back("2 Hatch Muta");//2 Hatchery Builds
	newBo.WeakCounterTo.push_back("2 Hatch Lurker");//2 Hatchery Builds
	newBo.WeakCounterTo.push_back("3 Hatch Hydra");//Anti-mech openings 
	newBo.WeakCounterTo.push_back("2 Hatch Hydra");//Anti-mech openings 


	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}

void BuildOrderManager::TvZ9MinutePush(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "TvZ 9 Minute Push";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 26;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 32;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 37;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 38;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 44;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 49;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 51;
	addPiece.Building = BWAPI::UnitTypes::Terran_Comsat_Station;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 51;
	addPiece.Building = BWAPI::UnitTypes::Terran_Comsat_Station;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 53;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 60;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 61;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );	
	addPiece.supply = 73;
	addPiece.Building = BWAPI::UnitTypes::Terran_Science_Facility;
	newBo.BOpieces.push_back( addPiece );	
	addPiece.supply = 73;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );	
	addPiece.supply = 75;
	addPiece.Building = BWAPI::UnitTypes::Terran_Control_Tower;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 85;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 85;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 85;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("1 Hatch Lurker");
	newBo.StrongCounterBy.push_back("13 Pool Muta");
	newBo.WeakCounterBy.push_back("2 Hatch Muta");//2 Hatchery Builds
	newBo.WeakCounterBy.push_back("2 Hatch Lurker");//2 Hatchery Builds
	newBo.WeakCounterTo.push_back("3 Hatch Muta");//Fast Hive Tech. 
	newBo.WeakCounterTo.push_back("3 Hatch Lurker");//Fast Hive Tech. 

	newBo.StrongMap.push_back("Python");
	newBo.StrongMap.push_back("Luna");
	newBo.StrongMap.push_back("Fighting Spirit");
	newBo.WeakMap.push_back("Destination");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}

void BuildOrderManager::BlindBO(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "Martin Rooijackers";
	newBo.Name = "BlindBO";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10; //defend against 4 pool
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );

	//slight deviation, Go academy first for the Blind trick
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;//Extra rax for the blind trick
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;//Extra rax for the blind trick
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Science_Facility;
	newBo.BOpieces.push_back( addPiece );


	//go two more factories, in base CC, then complete mech switch.
	//Might do that if the BattleCruiser switch + resotiation doesn't work



	newBo.WeakCounterTo.push_back("2 Hatch Muta");//Fast Hive Tech. 
	newBo.WeakCounterTo.push_back("2 Hatch Lurker");//Fast Hive Tech. 
	newBo.WeakCounterTo.push_back("1 Hatch Lurker");//Fast Hive Tech. 

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}



void BuildOrderManager::CardinalAllin(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Zerg;
	newBo.Creator = "CardinalAllin";
	newBo.Name = "CardinalAllin";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10; //defend against 4 pool
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );


	//slight deviation, Go academy first for the Blind trick
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;//Extra rax for the blind trick
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Science_Facility;
	newBo.BOpieces.push_back( addPiece );


	//go two more factories, in base CC, then complete mech switch.
	//Might do that if the BattleCruiser switch + resotiation doesn't work



	newBo.WeakCounterTo.push_back("3 Hatch Muta");//Fast Hive Tech. 
	newBo.WeakCounterTo.push_back("3 Hatch Lurker");//Fast Hive Tech. 


	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::HydraLurker(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "HydraLurker";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );


	newBo.Transitions.clear();

	newBo.StrongCounterBy.push_back( "2/3+ Factory Tanks" );
	newBo.WeakCounterTo.push_back("SK Terran");

	newBo.StrongMap.push_back("Arcadia");
	newBo.StrongMap.push_back("Andromeda");
	newBo.StrongMap.push_back("Python");

	BOs.push_back(newBo);
}



void BuildOrderManager::ThreeHatchZergling(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "3 Hatch Zergling";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );

	newBo.Transitions.clear();

	newBo.StrongCounterBy.push_back("All fast techs + walling");
	newBo.StrongCounterBy.push_back("Efficient scouting");

	newBo.StrongCounterTo.push_back("Greedy Builds");
	newBo.StrongCounterTo.push_back("Non-walled Terran Builds");

	BOs.push_back(newBo);
}

void BuildOrderManager::ThreeHatchMuta(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "3 Hatch Muta";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spire;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Evolution_Chamber;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("1 rax Gas");//early vulture harass
	newBo.StrongCounterBy.push_back("7 Barracks");
	newBo.StrongCounterBy.push_back("Ayumi Build");
	newBo.WeakCounterBy.push_back("Bunker Rush");//Bunker Rush
	newBo.WeakCounterBy.push_back("BBS");

	newBo.StrongCounterTo.push_back("1 Base fast Dropship or Tank push");
	newBo.StrongCounterTo.push_back("2 Barracks, Academy, Expansion");
	newBo.WeakCounterTo.push_back("2 rax FE");//2 Barracks, Academy, Expansion

	newBo.StrongMap.push_back("Python");
	newBo.StrongMap.push_back("Fantasy II");
	newBo.StrongMap.push_back("Namja Iyagi");
	newBo.StrongMap.push_back("Heartbreak Ridge");
	newBo.StrongMap.push_back("Fighting Spirit");

	newBo.WeakMap.push_back("Gaema Gowon");
	newBo.WeakMap.push_back("Blue Storm");

	BOs.push_back(newBo);
}


void BuildOrderManager::ThreeHatchLurker(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "3 Hatch Lurker";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Evolution_Chamber;
	newBo.BOpieces.push_back( addPiece );

	newBo.Transitions.clear();

	BOs.push_back(newBo);
}


void BuildOrderManager::ThreeHatchBeforePool(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "3 Hatch Before Pool";

	BOpiece addPiece;
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );


	newBo.Transitions.clear();
	newBo.Transitions.push_back("3 Hatch Muta");
	newBo.Transitions.push_back("3 Hatch Lurker");

	//newBo.StrongCounterBy.push_back( "Any barracks first build, any 2 Rax before CC build" );
	//Manual unroll of this counter
    newBo.StrongCounterBy.push_back( "8 Rax" );
    newBo.StrongCounterBy.push_back("BBS");
    newBo.StrongCounterBy.push_back("2 rax FE");

	newBo.WeakCounterTo.push_back("14 CC");

	BOs.push_back(newBo);
}

void BuildOrderManager::NinePool(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "9 Pool";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Drone;
	newBo.BOpieces.push_back( addPiece );
	//Skip extractor trick
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );


	newBo.Transitions.clear();
	newBo.Transitions.push_back("2 Hatch Muta");
	newBo.Transitions.push_back("2 Hatch Lurker");

	newBo.WeakCounterBy.push_back( "2 Rax FE" );
	newBo.StrongCounterTo.push_back("14 CC");
	newBo.StrongCounterTo.push_back("BBS");
	newBo.WeakCounterTo.push_back("1 Rax FE");

	BOs.push_back(newBo);
}

void BuildOrderManager::NinePoolSpeed(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "9 Pool Speed";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Drone;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );


	newBo.Transitions.clear();
	newBo.Transitions.push_back("2 Hatch Muta");
	newBo.Transitions.push_back("2 Hatch Lurker");


	newBo.WeakCounterBy.push_back( "2 Rax FE" );
	newBo.StrongCounterTo.push_back("14 CC");
	newBo.StrongCounterTo.push_back("BBS");
	newBo.WeakCounterTo.push_back("1 Rax FE");

	BOs.push_back(newBo);
}

void BuildOrderManager::OverPool(){
	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "OverPool";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );

	newBo.Transitions.clear();
	newBo.Transitions.push_back("2 Hatch Muta");
	newBo.Transitions.push_back("2 Hatch Lurker");

	BOs.push_back(newBo);
}

void BuildOrderManager::TwelveHatch(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "12 Hatch";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Overlord;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterBy.push_back("8 Rax");
	newBo.WeakCounterBy.push_back("Bunker Rush");
	newBo.WeakCounterBy.push_back("14 CC");
	newBo.StrongCounterBy.push_back("BBS");

	BOs.push_back(newBo);
}

void BuildOrderManager::OneHatchLurker(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "1 Hatch Lurker";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );



	newBo.StrongCounterBy.push_back("2 Port Wraith");
	newBo.StrongCounterBy.push_back("Missile Turret and Bunker at the choke and/or ramp.");
	newBo.StrongCounterBy.push_back("Failure to kill early scout ");

	newBo.StrongCounterTo.push_back("14 CC");
	newBo.WeakCounterTo.push_back("1 Rax FE");
	newBo.WeakCounterTo.push_back("2 Rax FE");

	BOs.push_back(newBo);


}

void BuildOrderManager::TwelvePool(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "1 Hatch Lurker";

	BOpiece addPiece;
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("Wall-In");
	newBo.StrongCounterBy.push_back("2 Port Wraith");
	newBo.StrongCounterBy.push_back("Supply Depot First");

	newBo.StrongCounterTo.push_back("8 Rax");
	newBo.WeakCounterTo.push_back("14 CC");
	newBo.WeakCounterTo.push_back("Barracks First");

	BOs.push_back(newBo);


}

void BuildOrderManager::ThirtheenPoolMuta(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "13 Pool Muta";

	BOpiece addPiece;
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Creep_Colony;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spire;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("2 Rax FE");
	newBo.StrongCounterBy.push_back("One Base Play");
	newBo.WeakCounterBy.push_back("8 Rax");
	newBo.WeakCounterBy.push_back("1 Rax FE");

	newBo.StrongCounterTo.push_back("Fantasy Build");
	newBo.StrongCounterTo.push_back("Iloveoov Fake Mech");
	newBo.StrongCounterTo.push_back("1 Vulture expand into 2 Factory Goliaths ");
	newBo.StrongCounterTo.push_back("1 Vulture expand into Biomech (Goliaths / M&M) ");

	newBo.StrongMap.push_back("Heartbreak Ridge");
	newBo.StrongMap.push_back("Outsider");
	newBo.StrongMap.push_back("Medusa");

	newBo.WeakMap.push_back("Python");
	newBo.WeakMap.push_back("Tau Cross");
	newBo.WeakMap.push_back("Gods Garden");
	newBo.WeakMap.push_back("Destination");

	BOs.push_back(newBo);


}

void BuildOrderManager::FivePool(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "5 pool";

	BOpiece addPiece;
	addPiece.supply = 5;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("8 Rax");
	newBo.StrongCounterBy.push_back("9 Barracks + Bunker");
	newBo.StrongCounterBy.push_back("10 Barracks");
	newBo.WeakCounterBy.push_back("11 Barracks");

	newBo.StrongCounterTo.push_back("14 CC");
	newBo.WeakCounterTo.push_back("1 Rax Gas");
	newBo.WeakCounterTo.push_back("1 Rax FE");

	newBo.StrongMap.push_back("Blood Bath");

	newBo.WeakMap.push_back("Hunters");
	newBo.WeakMap.push_back("Arkanoid");

	BOs.push_back(newBo);


}


void BuildOrderManager::TwoHatchLurker12Hatch(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "2 Hatch Lurker";

	BOpiece addPiece;
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("2 Port Wraith");
	newBo.StrongCounterBy.push_back("1 Rax Gas");
	newBo.WeakCounterBy.push_back("Sparks Terran");
	newBo.WeakCounterBy.push_back("Early Academy Builds ");

	newBo.WeakCounterTo.push_back("1 Rax FE");

	newBo.StrongMap.push_back("Medusa");

	BOs.push_back(newBo);

}


void BuildOrderManager::TwoHatchLurker12Pool(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "2 Hatch Lurker";

	BOpiece addPiece;
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("2 Port Wraith");
	newBo.StrongCounterBy.push_back("1 Rax Gas");
	newBo.WeakCounterBy.push_back("Sparks Terran");
	newBo.WeakCounterBy.push_back("Early Academy Builds ");

	newBo.WeakCounterTo.push_back("1 Rax FE");

	newBo.StrongMap.push_back("Medusa");

	BOs.push_back(newBo);

}


void BuildOrderManager::TwoHatchLurker9Pool(){

	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Zerg;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining";
	newBo.Name = "2 Hatch Lurker";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Spawning_Pool;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hatchery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Lair;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Hydralisk_Den;
	newBo.BOpieces.push_back( addPiece );
	/*
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Zerg_Extractor;
	newBo.BOpieces.push_back( addPiece );
	*/

	newBo.StrongCounterBy.push_back("2 Port Wraith");
	newBo.StrongCounterBy.push_back("1 Rax Gas");
	newBo.WeakCounterBy.push_back("Sparks Terran");
	newBo.WeakCounterBy.push_back("Early Academy Builds ");

	newBo.WeakCounterTo.push_back("1 Rax FE");

	newBo.StrongMap.push_back("Medusa");

	BOs.push_back(newBo);

}




void BuildOrderManager::OneFactFE(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Fact FE";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 20;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 32;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("3 Fact Vults");
	newBo.WeakCounterBy.push_back("1 Port Wraith");
	newBo.WeakCounterBy.push_back("2 Port Wraith");
	newBo.WeakCounterBy.push_back("2 Fact Vults");
	newBo.WeakCounterTo.push_back("Faster One Factory FE");
	newBo.WeakCounterTo.push_back("14 CC"); 


	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}

void BuildOrderManager::OnePortWraith(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Port Wraith";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );



	newBo.StrongCounterBy.push_back("3 Fact Vults");
	newBo.WeakCounterBy.push_back("1 Port Wraith");
	newBo.WeakCounterBy.push_back("2 Port Wraith");
	newBo.WeakCounterBy.push_back("2 Fact Vults");
	newBo.WeakCounterTo.push_back("Faster One Factory FE");
	newBo.WeakCounterTo.push_back("14 CC"); 


	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}



void BuildOrderManager::OneRaxFEvsTerran(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Rax FE";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Bunker;
	newBo.BOpieces.push_back( addPiece );

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}

void BuildOrderManager::CC14vsTerran(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "14 CC";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Terran_Bunker;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 26;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("Proxy 5 Rax");
	newBo.StrongCounterBy.push_back("Proxy 8 Rax");
	newBo.StrongCounterBy.push_back("BBS");
	newBo.StrongCounterBy.push_back("Double Expand");
	newBo.WeakCounterTo.push_back("1 Fact FE");

	newBo.StrongMap.push_back("God's Garden");
	newBo.StrongMap.push_back("Andromeda");
	newBo.StrongMap.push_back("Colosseum II");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}



void BuildOrderManager::TwoFactVults(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Fact Vults";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("3 Fact Vults");
	newBo.WeakCounterBy.push_back("1 Port Wraith");
	newBo.WeakCounterBy.push_back("2 Port Wraith");
	newBo.WeakCounterBy.push_back("Two Factory");

	newBo.WeakCounterTo.push_back("1 Fact FE");
	newBo.StrongCounterTo.push_back("14 CC");

	newBo.StrongMap.push_back("Blue Storm");
	newBo.StrongMap.push_back("Ride of Valkyries");
	newBo.StrongMap.push_back("Python");
	newBo.StrongMap.push_back("Othello");

	newBo.WeakMap.push_back("Lost Temple");
	newBo.WeakMap.push_back("Katrina");
	newBo.WeakMap.push_back("Tau Cross");
	newBo.WeakMap.push_back("Andromeda");


	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::ThreeFactVults(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "3 Fact Vults";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 20;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 30;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 32;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("wall-in at natural or main");


	newBo.WeakCounterTo.push_back("1 Fact FE");
	newBo.WeakCounterTo.push_back("1 Port Wraith");
	newBo.StrongCounterTo.push_back("14 CC");
	newBo.StrongCounterTo.push_back("2 Port Wraith");
	newBo.StrongCounterTo.push_back("1 Port Wraith");
	newBo.StrongCounterTo.push_back("any unorthodox FE");

	newBo.StrongMap.push_back("Blue Storm");
	newBo.StrongMap.push_back("Ride of Valkyries");
	newBo.StrongMap.push_back("Python");
	newBo.StrongMap.push_back("Othello");

	newBo.WeakMap.push_back("Lost Temple");
	newBo.WeakMap.push_back("Katrina");
	newBo.WeakMap.push_back("Tau Cross");
	newBo.WeakMap.push_back("Andromeda");


	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::TwoPortWraith(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Port Wraith";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 30;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );


	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}



void BuildOrderManager::BBSvsTerran(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "BBS";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::Proxy5Rax(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Proxy 5 Rax";

	BOpiece addPiece;
	addPiece.supply = 5;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 7;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("BBS");
	newBo.StrongCounterBy.push_back("8 Rax");
	newBo.StrongCounterBy.push_back("Good SCV Micro");

	newBo.StrongCounterTo.push_back("1 Fact FE");
	newBo.StrongCounterTo.push_back("14 CC");

	newBo.StrongMap.push_back("Destination");
	newBo.StrongMap.push_back("Blue Storm");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}



void BuildOrderManager::SiegeExpand(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Siege Expand";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );


	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}




void BuildOrderManager::OneFactoryDoubleExpand(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Factory Double Expand";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 36;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 38;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterTo.push_back("12 Nexus");
	newBo.WeakCounterTo.push_back("14 Nexus");

	newBo.StrongMap.push_back("Circuit Breaker");
	newBo.StrongMap.push_back("Electric Circuit");
	newBo.StrongMap.push_back("Sniper Ridge");
	newBo.StrongMap.push_back("Fighting Spirit");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}



void BuildOrderManager::OneRaxFEvsProtss(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 rax FE";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Bunker;
	newBo.BOpieces.push_back( addPiece );
	//safer variant
	addPiece.supply = 25;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("2 Gate Zealot");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::CC14vsProtss(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "14 CC";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );

	newBo.StrongCounterBy.push_back("3 Gate Dragoon Range");
	newBo.StrongCounterBy.push_back("10/15 Gate");
	newBo.StrongCounterBy.push_back("Bulldog");
	newBo.WeakCounterBy.push_back("Fast Dark Templar");
	
	newBo.WeakCounterTo.push_back("14 Nexus");

	newBo.StrongMap.push_back("Blitz X");
	newBo.StrongMap.push_back("Katrina");
	newBo.StrongMap.push_back("God's Garden");
	newBo.WeakMap.push_back("Neo Requiem");


	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::TwoFactVultsMines(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Fact Vult/Mines";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );

	//Manually added (were not on liquidpedia)
	newBo.StrongCounterTo.push_back("2 Gate DT");
	newBo.StrongCounterTo.push_back("DT Drop");
	newBo.StrongCounterTo.push_back("DT Fast Expand");
	newBo.WeakCounterTo.push_back("Reaver Drop");

	newBo.WeakCounterBy.push_back("10/15 Gates");


	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}



void BuildOrderManager::BBSvsProtss(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "BBS";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );


	//Manually added (were not on liquidpedia)
	newBo.StrongCounterTo.push_back("12 Nexus");
	newBo.StrongCounterTo.push_back("14 Nexus");

	newBo.StrongCounterBy.push_back("2 gate obs");
	newBo.StrongCounterBy.push_back("2 Gate Zealot");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}



void BuildOrderManager::BunkerRushvsProtss(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Bunker Rush";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );


	//Manually added (were not on liquidpedia)
	newBo.StrongCounterTo.push_back("12 Nexus");
	newBo.StrongCounterTo.push_back("14 Nexus");

	newBo.StrongCounterBy.push_back("2 gate obs");
	newBo.StrongCounterBy.push_back("2 Gate Zealot");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::DeepSix(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Deep Six";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	//skip turret ring
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );

	//Manually added (were not on liquidpedia)
	newBo.StrongCounterTo.push_back("2 Base Arbiter");
	newBo.StrongCounterTo.push_back("2 Base Carrier");

	newBo.StrongCounterBy.push_back("1 Gate Reaver");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}




void BuildOrderManager::DoubleArmory(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Double Armory";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Terran_Armory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Terran_Engineering_Bay;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 38;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 46;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 49;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 49;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 50;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Science_Facility;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Armory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
    //comsat stations will be build automatically by the base manager at this point


	newBo.StrongCounterTo.push_back("2 Base Carrier");

	newBo.WeakCounterBy.push_back("2 Base Arbiter");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}



void BuildOrderManager::FD(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "FD";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );



	newBo.WeakCounterBy.push_back("Reaver Drop");
	newBo.WeakCounterBy.push_back("Bulldog");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}


void BuildOrderManager::GundamRush(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Gundam Rush";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Machine_Shop;
	newBo.BOpieces.push_back( addPiece );



	newBo.WeakCounterBy.push_back("2 Gate Range Expand");

	newBo.WallNeeded = false;
	BOs.push_back(newBo);

}



void BuildOrderManager::iloveoovBuild(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "iloveoov Build";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Starport;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Armory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Armory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Factory;
	newBo.BOpieces.push_back( addPiece );
    //CC< factories and Starport . 



	newBo.WeakCounterBy.push_back("2 Base Carrier");
	newBo.WeakCounterBy.push_back("2 Base Arbiter");

	newBo.WeakCounterTo.push_back("12 Nexus");
	newBo.WeakCounterTo.push_back("14 Nexus");
	newBo.WeakCounterTo.push_back("DT Fast Expand");

	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}



void BuildOrderManager::ShallowTwo(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Terran;
	newBo.RaceEnemy = BWAPI::Races::Protoss;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Shallow Two";

	BOpiece addPiece;
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Terran_Barracks;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Terran_Supply_Depot;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Terran_Bunker;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Terran_Refinery;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 26;
	addPiece.Building = BWAPI::UnitTypes::Terran_Academy;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Terran_Command_Center;
	newBo.BOpieces.push_back( addPiece );



	newBo.WallNeeded = true;
	BOs.push_back(newBo);

}




void BuildOrderManager::OneGateCore(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Gate Core";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );



	//newBo.WeakCounterBy.push_back("2 Factory Builds");//
	newBo.WeakCounterBy.push_back("Gundam Rush");

	newBo.WeakCounterTo.push_back("Siege Expand");

	BOs.push_back(newBo);

}


void BuildOrderManager::OneGateReaver(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "1 Gate Reaver";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 26;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 34;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Support_Bay;//liquidpedia satys observatory, mistake?
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 37;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );

	//newBo.WeakCounterBy.push_back("2 Factory Builds");//
	newBo.StrongCounterBy.push_back("iloveoov Build");
	newBo.WeakCounterBy.push_back("JoyO Rush");
	newBo.WeakCounterBy.push_back("Siege Expand");

	newBo.WeakCounterTo.push_back("FD");
	newBo.WeakCounterTo.push_back("Gundam Rush");
	newBo.WeakCounterTo.push_back("2 Fact Vult/Mines");

	BOs.push_back(newBo);

}



void BuildOrderManager::Ten15Gates(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "10/15 Gates";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );





	newBo.WeakCounterBy.push_back("Siege Expand");

	newBo.WeakCounterTo.push_back("14 CC");

	BOs.push_back(newBo);

}


void BuildOrderManager::TwelveNexus(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "12 Nexus";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );



	newBo.StrongCounterBy.push_back("Bunker Rush");
	newBo.WeakCounterBy.push_back("14 CC");
	//newBo.StrongCounterBy.push_back("Proxy 5 Rax");//TODO: vs Protoss variant not available yet


	newBo.WeakCounterTo.push_back("Siege Expand");
	newBo.WeakCounterTo.push_back("FD");

	BOs.push_back(newBo);

}


void BuildOrderManager::N14Nexus(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "14 Nexus";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 17;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 27;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("Bunker Rush");
	newBo.WeakCounterBy.push_back("14 CC");
	//newBo.StrongCounterBy.push_back("Proxy 5 Rax");//TODO: vs Protoss variant not available yet


	newBo.WeakCounterTo.push_back("Siege Expand");
	newBo.WeakCounterTo.push_back("FD");

	BOs.push_back(newBo);

}



void BuildOrderManager::TwoBaseArbiter(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Base Arbiter";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 20;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Templar_Archives;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );


	BOs.push_back(newBo);

}


/*
void BuildOrderManager::TwoBaseCarrier(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Base Carrier";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );


	BOs.push_back(newBo);

}

*/



void BuildOrderManager::TwoGateDT(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Gate DT";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );

	//no more exact timings from here
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Templar_Archives;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );


	BOs.push_back(newBo);

}



void BuildOrderManager::TwoGateRangeExpand(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Gate Range Expand";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 20;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 31;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterTo.push_back("BBS");
	newBo.WeakCounterTo.push_back("14 CC");
	newBo.StrongCounterTo.push_back("FD");

	BOs.push_back(newBo);

}


void BuildOrderManager::TwoGateZealot(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Gate Zealot";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 9;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );


	BOs.push_back(newBo);

}



void BuildOrderManager::TwoGateObserver(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "2 Gateway Observer";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 25;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 31;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 38;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Observatory;
	newBo.BOpieces.push_back( addPiece );


	BOs.push_back(newBo);

}



void BuildOrderManager::N21Nexus(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "21 Nexus";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 28;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );

	newBo.WeakCounterTo.push_back("FD");
	newBo.WeakCounterTo.push_back("Siege Expand");


	BOs.push_back(newBo);

}


void BuildOrderManager::Bulldog(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Bulldog";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 20;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 37;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );



	newBo.WeakCounterTo.push_back("Siege Expand");


	BOs.push_back(newBo);

}


void BuildOrderManager::DTDrop(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "DT Drop";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 15;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 21;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 29;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 33;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );

	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Templar_Archives;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("2 Fact Vult/Mines");


	BOs.push_back(newBo);

}


void BuildOrderManager::DTFastExpand(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "DT Fast Expand";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 12;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 16;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 23;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Templar_Archives;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Nexus;
	newBo.BOpieces.push_back( addPiece );


	newBo.StrongCounterBy.push_back("2 Fact Vult/Mines");


	BOs.push_back(newBo);

}



void BuildOrderManager::GasStealElevatorRush(){


	BuildOrder newBo;
	newBo.YourRace = BWAPI::Races::Protoss;
	newBo.RaceEnemy = BWAPI::Races::Terran;
	newBo.Creator = "Dennis Soemers Text mining. Manual adjustments by Martin Rooijackers";
	newBo.Name = "Gas Steal Elevator Rush";

	BOpiece addPiece;
	addPiece.supply = 8;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 10;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 11;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Gateway;
	newBo.BOpieces.push_back( addPiece );

	//gas steal
	addPiece.supply = 13;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Assimilator;
	newBo.BOpieces.push_back( addPiece );

	addPiece.supply = 14;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 18;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 22;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 24;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Pylon;
	newBo.BOpieces.push_back( addPiece );
	addPiece.supply = 27;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	newBo.BOpieces.push_back( addPiece );

	addPiece.supply = 0;
	addPiece.Building = BWAPI::UnitTypes::Protoss_Robotics_Support_Bay;
	newBo.BOpieces.push_back( addPiece );


	BOs.push_back(newBo);

}


void BuildOrderManager::SwitchBO( std::string BOname )
{

	for( int i=0; i<BOs.size(); i++){
       if( BOs[i].Name == BOname ){
		   SwitchBO( BOs[i] );
		   //break;
		   return;
	   }
	}

	Broodwar->printf( "Wrong name in Build order switch" );
	return;

}

void BuildOrderManager::SwitchBO( BuildOrder newBo )
{

	std::vector< BWAPI::UnitType > MyBuildings;
	Broodwar->self()->getUnits();

	BOOST_FOREACH( BWAPI::Unit unit, Broodwar->self()->getUnits() ){

		if( unit->getType().isBuilding()
			&& unit->getType() != BWAPI::UnitTypes::Terran_Command_Center ){

			MyBuildings.push_back( unit->getType() );

		}

	}

	//check which buildings are still missing from the BO
	std::vector< BWAPI::UnitType > BOmissing;
	for(int i=0; i<newBo.BOpieces.size(); i++){
		BOmissing.push_back( newBo.BOpieces[i].Building );
	}
	for(int i=0; i<BOmissing.size(); i++){
	  for(int j=0; j<MyBuildings.size(); j++){
		  if( BOmissing[i] == MyBuildings[j]){

			  BOmissing.erase( BOmissing.begin() + i );  
			  MyBuildings.erase( MyBuildings.begin() + j);
			  break;
		  }
	  }
	}

	//for all the buildings missing, add them to the production manager
	ProdMan->BuildingOrder.clear();
	for(int i=0; i<BOmissing.size(); i++){
		ProdMan->BuildingOrder.push_back( BOmissing[i] );
	}



}


ZState BuildOrderManager::CreateNewZstate( ZergState state )
{

	if( state ==  Zerg_Init ){
	   ZState newState;
	   newState.thisState = Zerg_Init;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );

	   newState.Transitions.push_back( Early_Pool );
	   newState.Transitions.push_back( Two_Hatch );
	   return newState;
	}

	if( state ==  Early_Pool ){
	   ZState newState;
	   newState.thisState = Early_Pool;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Bunker );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );

	   newState.Transitions.push_back( Counter_One_Base );
	   return newState;
	}


	if( state ==  Counter_One_Base ){
	   ZState newState;
	   newState.thisState = Counter_One_Base;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Bunker );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );

	   newState.Transitions.push_back( Counter_One_Base_Lurker );
	   newState.Transitions.push_back( Counter_One_Base_Muta );
	   return newState;
	}


	if( state ==  Counter_One_Base_Lurker ){
	   ZState newState;
	   newState.thisState = Counter_One_Base_Lurker;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Bunker );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Science_Facility );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   return newState;
	}


	if( state ==  Counter_One_Base_Muta ){
	   ZState newState;
	   newState.thisState = Counter_One_Base_Muta;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Bunker );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Science_Facility );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   return newState;
	}


	if( state ==  Two_Hatch ){
	   ZState newState;
	   newState.thisState = Two_Hatch;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );

	   newState.Transitions.push_back( Counter_Two_Hatch_Tech );
	   return newState;
	}

	if( state ==  Counter_Two_Hatch_Tech ){
	   ZState newState;
	   newState.thisState = Counter_Two_Hatch_Tech;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );


	   newState.Transitions.push_back( Counter_Two_Hatch_Lurker );
	   newState.Transitions.push_back( Counter_Two_Hatch_Muta );
	   return newState;
	}

	if( state ==  Counter_Two_Hatch_Lurker ){
	   ZState newState;
	   newState.thisState = Counter_Two_Hatch_Lurker;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   return newState;
	}


	if( state ==  Counter_Two_Hatch_Muta ){
	   ZState newState;
	   newState.thisState = Counter_Two_Hatch_Muta;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   //newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   return newState;
	}


	if( state ==  Three_Hatch ){
	   ZState newState;
	   newState.thisState = Three_Hatch;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   //newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   newState.Transitions.push_back( Counter_Three_Hatch_Lurker );
	   newState.Transitions.push_back( Counter_Three_Hatch_Muta );
	   return newState;
	}


	if( state ==  Counter_Three_Hatch_Lurker ){
	   ZState newState;
	   newState.thisState = Counter_Three_Hatch_Lurker;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Science_Facility );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   return newState;
	}


	if( state ==  Counter_Three_Hatch_Muta ){
	   ZState newState;
	   newState.thisState = Counter_Three_Hatch_Muta;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Science_Facility );

	   newState.Transitions.push_back( Zerg_Mid_Game );
	   return newState;
	}

	if( state ==  Zerg_Mid_Game ){
	   ZState newState;
	   newState.thisState = Zerg_Mid_Game;
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot  );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Supply_Depot );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Command_Center );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Refinery );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Engineering_Bay );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Academy );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Barracks );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Factory );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Starport );
	   newState.BuildOrder.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
	   return newState;
	}



}

void BuildOrderManager::InitFlowChartsZ(){


	//ZState newState;
	//newState.thisState = Zerg_Init;
	//newState.Transitions.push_back( Early_Pool );


	FlowZerg.AllStates.clear();
	FlowZerg.AllStates.push_back( CreateNewZstate( Zerg_Init ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Early_Pool ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_One_Base ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_One_Base_Lurker ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_One_Base_Muta ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Two_Hatch ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_Two_Hatch_Tech ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_Two_Hatch_Lurker ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_Two_Hatch_Muta ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Three_Hatch ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_Three_Hatch_Lurker ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Counter_Three_Hatch_Muta ) );
	FlowZerg.AllStates.push_back( CreateNewZstate( Zerg_Mid_Game ) );


	//FlowZerg.currentState =  CreateNewZstate( Zerg_Init );


}

void BuildOrderManager::InitFlowCharts(){

	InitFlowChartsZ();


}

BuildOrderManager::BuildOrderManager(){


	InitFlowCharts();

	/*
	//TwoPortWraithvsZerg();
	NinePool();
	NinePoolSpeed();
	OverPool();
	TwelveHatch();
	*/



	//Terran vs Zerg
	  PlusOne5Rax();
	  OneFactoryStarPort();
	//void OneFactoryTanks() //Removed due to it no longer being used in profesionnal play
	  OneRaxFEvsZerg();
	  OneRaxGas();
	  CC14vsZerg(); //14 CC
	  TwoPortWraithvsZerg();
	  TwoRaxFE();
	//void TwoRaxSCVAliin(); //not enough micro in this bot to pull it off
	//void TwoRaxTech(); 
	//void fourBarracksSunkenBreakBuild(); //bot is better equiped with Ayumi Build
	   AyumiBuild();
	  BBSvsZerg();
	  BunkerRush();//8 rax
	  FantasyBuild();
	//void FastNuke();//Very cheesy, only works vs Bots or D/D- level players
	  iloveoovFakeMech();
	//void ProxyFactory();//Very cheesy, requires micro that the bots doesnt have yet
	//void SparksTerran() //Very cheesy, only works vs Bots or D/D- level players
	  TvZ9MinutePush();

	  CardinalAllin();

	  BlindBO();

	//Zerg vs Terran
	  NinePool();
	  NinePoolSpeed();
	  OverPool();
	  TwelveHatch();
	  OneHatchLurker();
	  TwelvePool();
	  ThirtheenPoolMuta();
	  FivePool();
	  TwoHatchLurker12Hatch();
	  TwoHatchLurker12Pool();
	  TwoHatchLurker9Pool();
	  HydraLurker();
	  ThreeHatchBeforePool();
	  ThreeHatchLurker();
	  ThreeHatchMuta();
	  ThreeHatchZergling();

	//Terran vs Terran
	  OneFactFE();
	  OnePortWraith();
	  OneRaxFEvsTerran();
	  CC14vsTerran();
	  TwoFactVults();
	  ThreeFactVults();
	  TwoPortWraith();
	  BBSvsTerran();
	  Proxy5Rax();


	//Terran vs Protoss
	  SiegeExpand();
	  OneFactoryDoubleExpand();
	  OneRaxFEvsProtss();
	  CC14vsProtss();
	  TwoFactVultsMines();
	  BBSvsProtss();
	  BunkerRushvsProtss();
	  DeepSix();
	  DoubleArmory();
	//void FakeFakeDouble();//to much micro heuristics needed for now
	//void FastDrop();//no drop micro available yet
	  FD();
	  GundamRush();
	//void Hiya4Fac();//needs an algorithm for economic cuts
	  iloveoovBuild();
	//void JoyORush();
	//void Nada2Fac();
	  ShallowTwo();


	//ProtossvsTerran
	  OneGateCore();
	 OneGateReaver();
	 Ten15Gates();
	 TwelveNexus();
	 N14Nexus();
	 TwoBaseArbiter();
	//void TwoBaseCarrier();//No exact timings yet here. Gotta ask on teamliquid.net . for now use heuristic to detect carrier
	 TwoGateDT();
	 TwoGateRangeExpand();
	 TwoGateZealot();
	 TwoGateObserver();
	N21Nexus();
	Bulldog();
	DTDrop();
	DTFastExpand();
	GasStealElevatorRush();


	EnemyBo = "Unknown"; //at the start you simply dont know yet.

	if(Broodwar->enemy()->getRace() == BWAPI::Races::Random || Broodwar->enemy()->getRace() == BWAPI::Races::Unknown){
      StartedRandom = true;
	} else {
      StartedRandom = false;
	}


	//the initial strategeis versus each race
	std::string InitvsZerg = "BlindBO";
	std::string InitvsProtoss = "Siege Expand";
	std::string InitvsTerran = "1 Fact FE";
	std::string InitvsRandom = "Siege Expand";

	//set initial BO
	for(int i=0; i<BOs.size(); i++){
		/*
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg){
		  if( BOs[i].Name == InitvsZerg){
			myBo = BOs[i];
			SwitchBO( BOs[i] );
			break;
		  }
		}
		*/
		/*
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss){
		  if( BOs[i].Name == InitvsProtoss){
			myBo = BOs[i];
			SwitchBO( BOs[i] );
			break;
		  }
		}
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran){
		  if( BOs[i].Name == InitvsTerran){
			myBo = BOs[i];
			SwitchBO( BOs[i] );
			break;
		  }
		}
		if( Broodwar->enemy()->getRace() == BWAPI::Races::Unknown){
		  if( BOs[i].Name == InitvsRandom){
			myBo = BOs[i];
			SwitchBO( BOs[i] );
			break;
		  }
		}
		*/
	}


	/*
    //set 9 pool as default BO
	for( int i=0; i<BOs.size(); i++){
		if( BOs[i].Name == "9 Pool"){
			EnemyBo = BOs[i].Name;
		}
	}
	*/
}

void BuildOrderManager::ChangeBORandScout()
{

	Broodwar->printf("Change BO");

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg){

		  //MacroMan->CurrentStrat = "2 rax FE";
		  MacroMan->CurrentStrat = "Standard Flowchart";
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 10);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 13);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 13);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Bunker, 15);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 16);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 20);
		  
		  /*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 23);	
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 19);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 14);
		  */

		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 23);
		  /*
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Engineering_Bay, 23);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 23);		
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 23);
		  */

		  /*
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 23);
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
		  */


		  // Goliath + M&M
		  /*
	      //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 16);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 17);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 20);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Armory, 22);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 24);
		  ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 28);

		  MacroMan->FollowupZ.push_back(BWAPI::UnitTypes::Terran_Factory);
		  MacroMan->FollowupZ.push_back(BWAPI::UnitTypes::Terran_Refinery);
		  */
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss){

		  MacroMan->CurrentStrat = "Siege Expand";

		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 15);
	      ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 19);
		  //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Academy, 18);
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
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Academy);
		  //skip turret ring
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Starport );
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery);
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  //FollowupP.push_back( BWAPI::UnitTypes::Terran_Refinery );
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Science_Facility );
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);
		  MacroMan->FollowupP.push_back( BWAPI::UnitTypes::Terran_Barracks);


	}


	  if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
		  MacroMan->CurrentStrat = "3 fact vult";
		  //CurrentStrat = "2 port wraith";
		  if(  MacroMan->CurrentStrat == "3 fact vult"){
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 9);
	        //ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Barracks, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Refinery, 11);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 15);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 18);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 20);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 22);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 30);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Factory, 32);
	        ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Supply_Depot, 36);
		    ProdMan->addSupplyBO( BWAPI::UnitTypes::Terran_Command_Center, 32);

			//FollowupT.push_back( BWAPI::UnitTypes::t );
			//FollowupT.push_back( BWAPI::UnitTypes::Terran_Command_Center );
			MacroMan->FollowupT.push_back( BWAPI::UnitTypes::Terran_Armory );
			MacroMan->FollowupT.push_back( BWAPI::UnitTypes::Terran_Academy );
			MacroMan->FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			MacroMan->FollowupT.push_back( BWAPI::UnitTypes::Terran_Factory );
			MacroMan->FollowupT.push_back( BWAPI::UnitTypes::Terran_Starport );
			MacroMan->FollowupT.push_back( BWAPI::UnitTypes::Terran_Science_Facility );


		  }
	  }


}

void BuildOrderManager::OnFrame(){


	Broodwar->drawTextScreen( 450,70,EnemyBo.c_str() );
	Broodwar->drawTextScreen( 450,80,"%s", Broodwar->enemy()->getRace().c_str() );

	if (Broodwar->enemy()->getRace() == BWAPI::Races::Zerg) {
		Broodwar->drawTextScreen(450, 90, "Zerg state  %d", MacroMan->ZState );

	}

	//initial build order now done, go to the midgame
	if( Broodwar->getFrameCount() > 20000){
		return;
	}

	
	if( StartedRandom == true && Broodwar->enemy()->getRace() != BWAPI::Races::Unknown ){
		StartedRandom = false;
		ChangeBORandScout();
	}
	

	int BestScore = -9999;

	//not much use determining BO if we haven't scouted the enemy yet
	if( InfoMan->EnemyBuildings.size() == 0){
		return;
	}

	for(int j=0; j<BOs.size(); j++){

		if( !(Broodwar->self()->getRace() == BOs[j].RaceEnemy
			&& Broodwar->enemy()->getRace() == BOs[j].YourRace ) ){
             continue;
		}

		std::vector<BWAPI::UnitType>  EnemyBuildingsCopy;
	  for(int i=0; i<InfoMan->EnemyBuildings.size(); i++){
		  if( InfoMan->EnemyBuildings[i].building->getType().isResourceDepot()
			  && InfoMan->EnemyBuildings[i].building->getType() != BWAPI::UnitTypes::Zerg_Lair
			  && InfoMan->EnemyBuildings[i].building->getType() != BWAPI::UnitTypes::Zerg_Hive ){
			  if( InfoMan->EnemyBase != BWAPI::TilePositions::Unknown ){
				  if(  InfoMan->EnemyBuildings[i].position.getDistance( BWAPI::Position(InfoMan->EnemyBase) ) > 4*32 ){
			        EnemyBuildingsCopy.push_back( InfoMan->EnemyBuildings[i].type );
				  }
			  }
		  } else {
			  EnemyBuildingsCopy.push_back( InfoMan->EnemyBuildings[i].type );
		  }
			  
	  }

	  int BuildAvailable = 0;
	  int BuildMissing = 0;
		  for(int k=0; k<BOs[j].BOpieces.size(); k++){
			  if( BOs[j].BOpieces[k].Building.isBuilding() ){
				  bool isThere = false;
				  for( int m=0; m<EnemyBuildingsCopy.size(); m++){
					  if( EnemyBuildingsCopy[m] == BOs[j].BOpieces[k].Building ){
						  isThere = true;
						  EnemyBuildingsCopy.erase( EnemyBuildingsCopy.begin() + m);
						  break;
					  }
				  }
				  if( isThere ){
					  BuildAvailable++;
				  } else {
					  BuildMissing++;
				  }
			  }
		  }


	  int score = BuildAvailable - BuildMissing;

	  if( EnemyBuildingsCopy.size() > 0){
		  score -= EnemyBuildingsCopy.size()*5;
	  }

	  if( BestScore < score){
		  BestScore = score;
		  EnemyBo = BOs[j].Name;
	  }
	}

}