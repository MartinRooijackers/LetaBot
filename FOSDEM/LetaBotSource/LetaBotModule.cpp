
#include "LetaBotModule.h"
#include "WraithMicro.h"

#include <vector>
#include <string>
#include <cmath>

#include <algorithm>

bool analyzed = false;
//bool useMCTS = true;

using namespace BWAPI;


struct BattleScore{
	std::string EnemyName;
	std::string EnemyRace;
	std::string Map;
	int OurUnitKilledScore;
	int EnemyUnitKilledScore;
	bool Won;
};


struct Stored{
	std::string EnemyName;
	int spawnLocations;
	std::string strategy;
	bool won;
	std::string mapHash;
};

struct StratPick{
	Strategies strategy;
	int won;
	int lost;
};

std::vector<Stored> IOdata;
std::vector<StratPick> IOuse;

std::string EnemyRaceInit; //in case the opponent plays random
int UnitsKilledScore;
int UnitsDiedScore;
std::vector<BattleScore> BattleScores;

//use file I/O to determine the current best strategy
//bool useIO = false;


BWAPI::AIModule * __NewAIModule()
{
	return new LetaBotModule();
}
 
LetaBotModule::LetaBotModule()  {}
LetaBotModule::~LetaBotModule() {}

void LetaBotModule::onStart()
{
	BWAPI::Broodwar->setLocalSpeed(0);
	//BWAPI::Broodwar->setFrameSkip(240);

	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
	//BWAPI::Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);

	//use file I/O to determine the current best strategy
	useIO = false;



	//retrieve battle score for paper
	EnemyRaceInit = Broodwar->enemy()->getRace().getName();
	UnitsKilledScore = 0;
	UnitsDiedScore = 0;
	std::ifstream fin("bwapi-data/read/battlescore.txt");
	if( !fin.fail() ){
	  int total;
	  fin>>total;
	  BattleScores.clear();
	  for(int i=0; i<total; i++){
		//std::vector<BattleScore> BattleScores;
		BattleScore addBattleS;
		std::string getLine;
		getline (fin,getLine);
		getline (fin,getLine);
		addBattleS.EnemyName = getLine;
		getline (fin,getLine);
		addBattleS.EnemyRace = getLine;
		getline (fin,getLine);
		addBattleS.Map = getLine;
		fin>>addBattleS.EnemyUnitKilledScore;
		fin>>addBattleS.OurUnitKilledScore;
		fin>>addBattleS.Won;
		BattleScores.push_back( addBattleS );
	  }
	}

	/*
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
	  CurrentStrategy = Bunker_Rush_Two;
	  InitialStrategy = Bunker_Rush_Two;
	} else {
	  CurrentStrategy = Bunker_Rush;
	  InitialStrategy = Bunker_Rush;
	}
	*/
	
	//CurrentStrategy = Worker_Rush;
	//InitialStrategy = Worker_Rush;

	  CurrentStrategy = Macro_Strat;
	  InitialStrategy = Macro_Strat;

	//in case of Terran or Random, worker rush
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Unknown
		 ){
	  //CurrentStrategy = Three_Fac_Vulture;
	  //InitialStrategy = Three_Fac_Vulture;
		//CurrentStrategy = BBS;
	  //InitialStrategy = BBS;
	  //CurrentStrategy = Two_Rax_SCV_Rush;
	  //InitialStrategy = Two_Rax_SCV_Rush;
	  //CurrentStrategy = Macro_Strat;
	  //InitialStrategy = Macro_Strat;
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Zerg ){
	  //CurrentStrategy = Eight_rax;
	  //InitialStrategy = Eight_rax;
	  //CurrentStrategy = Two_Rax_SCV_Rush;
	  //InitialStrategy = Two_Rax_SCV_Rush;
	  CurrentStrategy = Macro_Strat;
	  InitialStrategy = Macro_Strat;
	  //CurrentStrategy = BBS;
	  //InitialStrategy = BBS;
	}

	if( Broodwar->enemy()->getRace() == BWAPI::Races::Protoss ){
	  //CurrentStrategy = One_Fact_3_Rax;
	  //InitialStrategy = One_Fact_3_Rax;
	  CurrentStrategy = Macro_Strat;
	  InitialStrategy = Macro_Strat;
	}

	//last time my bot will cheese. MCTS will  be used next time :)
	if( Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
	  //CurrentStrategy = One_Fact_3_Rax;
	  //InitialStrategy = One_Fact_3_Rax;
	  CurrentStrategy = Macro_Strat;
	  InitialStrategy = Macro_Strat;
		//InitialStrategy = ProxyRax;
		//CurrentStrategy = ProxyRax;
	}



	//boost::algorithm::to_lower(str);
	std::string lowerCaseName = Broodwar->enemy()->getName();
	std::transform(lowerCaseName.begin(), lowerCaseName.end(), lowerCaseName.begin(), ::tolower);

	// XelnagaII , krasi0 , Iron Bot, Bee Bot

	/*
	//do a worker rush vs IceLab
	if( lowerCaseName == "krasi0"
		|| lowerCaseName == "xelnagaii" 
        || lowerCaseName == "iron bot"
        || lowerCaseName == "ironbot"
        || lowerCaseName == "beebot"

		|| Broodwar->enemy()->getName() == "XelnagaII" 
        || Broodwar->enemy()->getName() == "krasi0"
        || Broodwar->enemy()->getName() == "Krasi0"
        || Broodwar->enemy()->getName() == "Iron bot"
        || Broodwar->enemy()->getName() == "Ironbot"
        || Broodwar->enemy()->getName() == "BeeBot"

		){
	  CurrentStrategy = Worker_Rush;
	  InitialStrategy = Worker_Rush;


	}
	*/

	/*
	//do a worker rush vs IceLab
	if( lowerCaseName == "icelab"
		|| lowerCaseName == "icestarcraft" 
        || lowerCaseName == "icebot"
        || lowerCaseName == "ice"
        || lowerCaseName == "icecraft"

		|| Broodwar->enemy()->getName() == "IceStarCraft" 
        || Broodwar->enemy()->getName() == "IceBot"
        || Broodwar->enemy()->getName() == "Ice"
        || Broodwar->enemy()->getName() == "IceCraft"

		
		|| Broodwar->enemy()->getName() == "Iron bot" 
        || Broodwar->enemy()->getName() == "Iron Bot"
        || Broodwar->enemy()->getName() == "IronBot"
        || Broodwar->enemy()->getName() == "Ironbot"
		

		){
	  //CurrentStrategy = Worker_Rush;
	 // InitialStrategy = Worker_Rush;

	  CurrentStrategy = BBS_Rush;
	  InitialStrategy = BBS_Rush;

	}
	*/

	/*
	if(    lowerCaseName == "marian devecka"

		|| Broodwar->enemy()->getName() == "Marian Devecka"  ){

	  CurrentStrategy = BBS_Rush;
	  InitialStrategy = BBS_Rush;

	}
	*/

	 // CurrentStrategy = Worker_Rush;
	 // InitialStrategy = Worker_Rush;

	IOdata.clear();

	// Influence map + Build orders

	Broodwar->sendText("LetaBot version 0.40 , FOSDEM version.");

	//std::vector<Unit> WS;

	WMicro = new WraithMicro();
	BWTA::readMap();
	BWTA::analyze();
    analyzed  = true;
    //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);	

	InfoMan = new InformationManager();
    bManager = new BuildingManager();

	
	bool UseWall = true;
	/*
	//old fix for hearth breaker ridge. With the update of the wall building this is no longer needed
	if( Broodwar->mapHash() == "d9757c0adcfd61386dff8fe3e493e9e8ef9b45e3"
		|| Broodwar->mapFileName().find( "Heartbreaker" ) != std::string::npos ){
        UseWall = false;
	}
	*/
	/*
	if( UseWall == true ){
	   bManager->ReadWallData();
	}
	*/

	
	if( bManager != NULL &&  bManager->useWallIO == true ){
	   bManager->ReadWallData();
	}
	


	CCmanager.clear();

	BaseManager* newBM;

	for (auto& unit : Broodwar->self()->getUnits()) {
		if (unit->getType().isResourceDepot())
		{
			newBM = new BaseManager(unit, unit->getTilePosition());
		}
	}


	for (auto& unit : Broodwar->self()->getUnits()) {
		if (unit->getType().isWorker())
		{
			newBM->addSCV(unit);
		}
	}


	CCmanager.push_back( newBM );

    ProdMan = new ProductionManager();
	WDMan = new WorkerDefenceManager();
	//InfoMan = new InformationManager();
	GMan = new GeneralManager();
	RPMan = new RepairManager();
	ScanMan = new ScannerManager();
	MacroMan = new MacroManager();
	BOMan = new BuildOrderManager();

	//stategies specifically for 8/9 rax
	WraithMan = new WraithManager();
	SKMan = new SKManager();
	//WAMan = new WorkerAttackManager();
	
	//special micro managers
    MicroZerg = new MicroManagerZerg();
    MicroProtoss = new MicroManagerProtoss();
    MicroTerran = new MicroManagerTerran();


	if( CurrentStrategy == Worker_Rush){
		ScoutMan = new ScoutManager( 3 );
	}
	if (CurrentStrategy == ProxyRax) {
		ScoutMan = new ScoutManager(2);
	}


}

void LetaBotModule::onEnd(bool isWinner) 
{
	//Broodwar->printf("game has ended");



	//retrieve battle score for paper
	std::ofstream fout("bwapi-data/write/battlescore.txt");

		BattleScore addBattleS;
		addBattleS.EnemyName = Broodwar->enemy()->getName();
		//addBattleS.EnemyRace = Broodwar->enemy()->getRace().getName();
		addBattleS.EnemyRace =  EnemyRaceInit;
		addBattleS.Map =  Broodwar->mapName();
		//addBattleS.EnemyUnitKilledScore = Broodwar->self()->getKillScore();
		//addBattleS.OurUnitKilledScore = Broodwar->enemy()->getKillScore();
		//addBattleS.EnemyUnitKilledScore = Broodwar->self()->getUnitScore();
		//addBattleS.OurUnitKilledScore = Broodwar->enemy()->getUnitScore();
		addBattleS.EnemyUnitKilledScore = UnitsKilledScore;
		addBattleS.OurUnitKilledScore = UnitsDiedScore;

		addBattleS.Won = isWinner;
		BattleScores.push_back( addBattleS );

		int total = BattleScores.size();
		fout<<total<<"\n";
	for(int i=0; i<total; i++){
		//std::vector<BattleScore> BattleScores;
		//BattleScore addBattleS;
		fout<<BattleScores[i].EnemyName<<"\n";
		fout<<BattleScores[i].EnemyRace<<"\n";
		fout<<BattleScores[i].Map<<"\n";
		fout<<BattleScores[i].EnemyUnitKilledScore<<"\n";
		fout<<BattleScores[i].OurUnitKilledScore<<"\n";
		fout<<BattleScores[i].Won<<"\n";
	}



}


void LetaBotModule::onFrame()
{

	//Broodwar->printf( Broodwar->enemy()->getRace().c_str() );

	drawTerrainData();
	if( WMicro != NULL ){
	  WMicro->MoveWraits();
	}

	if(GMan != NULL){
	GMan->onFrame();
	}

	if( bManager != NULL ){
	  bManager->drawBuildMap();
	}

	Broodwar->drawTextScreen(300,20,"%d",Broodwar->getFrameCount());

	/*
	if( !bManager->WallCalculated && analyzed){
		if( CurrentStrategy == Wall_in || CurrentStrategy == One_Fact_3_Rax ){
		  bManager->WallOff();//computationally expensive, better do it in another thread
		}
	}
	*/

	if( ProdMan != NULL ){
	  ProdMan->onFrame();
	}

	//CCmanager[0]->onFrame();
	for(unsigned int i=0; i<CCmanager.size(); i++){
	  CCmanager[i]->onFrame();
	}

	if( WDMan != NULL ){
	  WDMan->OnFrame();
	}

	if(  ScoutMan != NULL ){
		ScoutMan->OnFrame();
	}


	if( RPMan != NULL && CurrentStrategy == Macro_Strat ){
		RPMan->OnFrame();
	}

	if( ScanMan != NULL ){
		ScanMan->OnFrame();
	}

	/*
	if( SKMan != NULL && CurrentStrategy == Macro_Strat ){
	  SKMan->onFrame();
	}
	*/


	if( MacroMan != NULL && CurrentStrategy == Macro_Strat ){
		MacroMan->OnFrame();
	}

	if( BOMan != NULL &&  CurrentStrategy == Macro_Strat ){
		BOMan->OnFrame();
	}


	if( WraithMan != NULL){
		WraithMan->onFrame();
	}

	if( MicroZerg != NULL && Broodwar->enemy()->getRace() == BWAPI::Races::Zerg){
		MicroZerg->onFrame();
	}

	if( MicroProtoss != NULL && Broodwar->enemy()->getRace() == BWAPI::Races::Protoss){
		MicroProtoss->onFrame();
	}

	if( MicroTerran != NULL && Broodwar->enemy()->getRace() == BWAPI::Races::Terran ){
		MicroTerran->onFrame();
	}

}

void LetaBotModule::onUnitDestroy(BWAPI::Unit  unit)
{

	if( unit == NULL ){
		return;
	}


	if( !unit->getType().isBuilding()
		&& !unit->getType().isWorker() ){
			if( unit->getPlayer() ==  Broodwar->self() ){
				UnitsDiedScore += unit->getType().destroyScore();
			}
			if( unit->getPlayer() ==  Broodwar->enemy() ){
				UnitsKilledScore += unit->getType().destroyScore();
			}
			//UnitsKilledScore
	}


	if( unit->getType().isBuilding() ){
		bManager->onBuildingDestroy(unit);
	}

	if( InfoMan != NULL) {
	  InfoMan->onUnitDestroy(unit);
	}


	/*
	  if( SKMan != NULL && CurrentStrategy == Macro_Strat ){
		  SKMan->onBuildingDestroyed(unit);
	  
	  }
	  */

	/*
	  if(  WraithMan != NULL  ){
		 WraithMan->onBuildingDestroyed(unit);
	  }
	  */

	  if( ProdMan != NULL ){
		  ProdMan->onUnitDestroy(unit);
	  }


	  if( MacroMan != NULL ){
		  MacroMan->onUnitDestroy(unit);
	  }

}

void LetaBotModule::onUnitMorph(BWAPI::Unit  unit)
{

}

void LetaBotModule::onUnitCreate(BWAPI::Unit  unit)
{ 

}

void LetaBotModule::onUnitShow(BWAPI::Unit  unit)
{ 
	if(  unit->getType().isBuilding() ){
	  bManager->constructionPlaced(unit);
	}
	InfoMan->onUnitShow(unit);

}

void LetaBotModule::onUnitDiscover(Unit unit)
{

}

void LetaBotModule::onUnitHide(BWAPI::Unit  unit)
{ 

}

void LetaBotModule::onUnitRenegade(BWAPI::Unit  unit)
{ 

}

void LetaBotModule::onNukeDetect(BWAPI::Position target)
{ 

}

void LetaBotModule::onUnitComplete(BWAPI::Unit unit){

	if( unit->getPlayer() != Broodwar->self() ){
		return;
	}


	if(  CurrentStrategy == Worker_Rush || CurrentStrategy == BBS_Rush
		|| CurrentStrategy == ProxyRax
		|| CurrentStrategy ==  Normal_Wraith){
	  WMicro->AddWraith(unit);
	}
	if( MacroMan->CurrentStrat == "2 Port Wraith" && CurrentStrategy == Macro_Strat ){
	  WraithMan->AddWraith(unit);
	}

	if( ProdMan != NULL){
	  ProdMan->onUnitComplete(unit);
	}

	if( ScanMan != NULL ){
		ScanMan->AddScanner( unit );
	}

	if( MacroMan != NULL && CurrentStrategy == Macro_Strat ){
		MacroMan->AddUnit(unit);
	}

}


void LetaBotModule::onSendText(std::string text){

	//s for slow, standard speed
	if( text == "s"){
		BWAPI::Broodwar->setLocalSpeed(-1);
		return;
	}
	//m for medium, twice as fast
	if( text == "m"){
		BWAPI::Broodwar->setLocalSpeed(20);
		return;
	}
	//f for fast
	if( text == "f"){
		BWAPI::Broodwar->setLocalSpeed(0);
		return;
	}
	Broodwar->sendText( text.c_str() );
}

void LetaBotModule::drawTerrainData()
{

	if( analyzed == false){
		return;
	}
  //we will iterate through all the base locations, and draw their outlines.
  for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++)
  {
    TilePosition p=(*i)->getTilePosition();
    Position c=(*i)->getPosition();

    //draw outline of center location
    Broodwar->drawBox(CoordinateType::Map,p.x*32,p.y*32,p.x*32+4*32,p.y*32+3*32,Colors::Blue,false);

    //if this is an island expansion, draw a yellow circle around the base location
    if ((*i)->isIsland())
      Broodwar->drawCircle(CoordinateType::Map,c.x,c.y,80,Colors::Yellow,false);
  }


  //we will visualize the chokepoints with red lines
  for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
  {
    for(std::set<BWTA::Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++)
    {
      Position point1=(*c)->getSides().first;
      Position point2=(*c)->getSides().second;
      Broodwar->drawLine(CoordinateType::Map,point1.x,point1.y,point2.x,point2.y,Colors::Red);

	  //draw graph
	  //Broodwar->drawLineMap( (*c)->getRegions().first->getCenter().x(),(*c)->getRegions().first->getCenter().y(), (*c)->getRegions().second->getCenter().y(),(*c)->getRegions().second->getCenter().y(),Colors::White);

    }
	//Broodwar->drawCircleMap((*r)->getCenter().x(),(*r)->getCenter().y(),16,Colors::White,true);

  }

        BWTA::Region* home  = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
        std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
        double min_length=10000;
        BWTA::Chokepoint* choke=NULL;

		/*
        //iterate through all chokepoints and look for the one with the smallest gap (least width)
        for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++)
        {
			//Broodwar->printf("X:%d",(*c)->getCenter().x());
			Broodwar->drawCircleMap((*c)->getCenter().x(),(*c)->getCenter().y(),4,Colors::Purple,true);
			//(*c)->getCenter
        }
		*/

		if( InfoMan != NULL ){
			if( InfoMan->PosMainChoke != BWAPI::Positions::Unknown ){
			   Broodwar->drawCircleMap(InfoMan->PosMainChoke.x,InfoMan->PosMainChoke.y,4,Colors::Purple,true);
			}
		}

}



DWORD WINAPI AnalyzeThread()
{
  BWTA::analyze();
  analyzed   = true;
  return 0;
}
