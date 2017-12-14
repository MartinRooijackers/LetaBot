#pragma once


#include <BWAPI.h>
#include <vector>
#include <string>

using namespace BWAPI;

//part of a BO
struct BOpiece{
	int supply;
	BWAPI::UnitType Building;

};

struct BuildOrder{
	std::vector< BOpiece > BOpieces;
	std::vector< std::string > WeakCounterBy;
	std::vector< std::string > StrongCounterBy;
	std::vector< std::string > WeakCounterTo;
	std::vector< std::string > StrongCounterTo;
	std::vector< std::string > WeakMap;
	std::vector< std::string > StrongMap;
	std::vector< std::string > Transitions;
	std::string Name;
	std::string Creator; //person that got the BO into this code, not the (pro-)gamer that created the BO
	BWAPI::Race YourRace;
	BWAPI::Race RaceEnemy;
	bool WallNeeded;//if a wall-in is required



};


//States of the TvZ flowchart
enum ZergState{

	Zerg_Init,  //building the depot and barracks
	Early_Pool, //defend against an early pool build
	Counter_One_Base, //counter against a fast pool into tech build
	Counter_One_Base_Lurker, //counter against a fast pool into tech build
	Counter_One_Base_Muta, //counter against a fast pool into tech build
	Two_Hatch,  //Zerg goes for standard 12 Hatch
	Counter_Two_Hatch_Tech, //counter a 12 hatch into tech
	Counter_Two_Hatch_Lurker, //counter 2 hatch lurker
	Counter_Two_Hatch_Muta, //counter 2 hatch muta
	Three_Hatch, //Zerg goes for standard 3 Hatch build
	Counter_Three_Hatch_Lurker, //counter 3 hatch lurker
	Counter_Three_Hatch_Muta, //counter 3 hatch muta
	Zerg_Mid_Game  //from here on, it is the mid game. Rely on influence maps from here


};

struct ZState{

	ZergState thisState;
	std::vector<BWAPI::UnitType> BuildOrder;
	std::vector< ZergState > Transitions;

};

struct FlowChartZerg{

	ZState currentState;
	std::vector< ZState > AllStates;

};


class BuildOrderManager{
public:
	std::vector< BuildOrder > BOs;

	int LastScanFrame;

	BuildOrderManager();
	void OnFrame();
	//~ScoutManager() {};

	//BO extracted

	std::string EnemyBo;
	BuildOrder myBo;


	//Flowchart datastructures
	FlowChartZerg FlowZerg;
	bool StartedRandom;
	//flowchart functions
	void InitFlowCharts();
	void InitFlowChartsZ();
	ZState CreateNewZstate( ZergState state );
	void CheckTransitionZerg();
	void ChangeBORandScout();


	void SwitchBO( std::string BOname);
	void SwitchBO( BuildOrder newBo );


	//Terran vs Zerg
	void PlusOne5Rax();
	void OneFactoryStarPort();
	//void OneFactoryTanks() //Removed due to it no longer being used in profesionnal play
	void OneRaxFEvsZerg();
	void OneRaxGas();
	void CC14vsZerg(); //14 CC
	void TwoPortWraithvsZerg();
	void TwoRaxFE();
	//void TwoRaxSCVAliin(); //not enough micro in this bot to pull it off
	//void TwoRaxTech(); 
	//void fourBarracksSunkenBreakBuild(); //bot is better equiped with Ayumi Build
	void  AyumiBuild();
	void BBSvsZerg();
	void BunkerRush();//8 rax
	void FantasyBuild();
	//void FastNuke();//Very cheesy, only works vs Bots or D/D- level players
	void iloveoovFakeMech();
	//void ProxyFactory();//Very cheesy, requires micro that the bots doesnt have yet
	//void SparksTerran() //Very cheesy, only works vs Bots or D/D- level players
	void TvZ9MinutePush();

	//CardinalAllin Build orders ( TeamLiquid user)
	void CardinalAllin();
	//Blind BO  . My very own ridiculous BO that only works vs other bots
	void BlindBO();

	//Zerg vs Terran
	void NinePool();
	void NinePoolSpeed();
	void OverPool();
	void TwelveHatch();
	void OneHatchLurker();
	void TwelvePool();
	void ThirtheenPoolMuta();
	void FivePool();
	void TwoHatchLurker12Hatch();
	void TwoHatchLurker12Pool();
	void TwoHatchLurker9Pool();
	void HydraLurker();
	void ThreeHatchBeforePool();
	void ThreeHatchLurker();
	void ThreeHatchMuta();
	void ThreeHatchZergling();
	// Fake two hatch lurker

	//Terran vs Terran
	void OneFactFE();
	void OnePortWraith();
	void OneRaxFEvsTerran();
	void CC14vsTerran();
	void TwoFactVults();
	void ThreeFactVults();
	void TwoPortWraith();
	void BBSvsTerran();
	void Proxy5Rax();


	//Terran vs Protoss
	void SiegeExpand();
	void OneFactoryDoubleExpand();
	void OneRaxFEvsProtss();
	void CC14vsProtss();
	void TwoFactVultsMines();
	void BBSvsProtss();
	void BunkerRushvsProtss();
	void DeepSix();
	void DoubleArmory();
	//void FakeFakeDouble();//to much micro heuristics needed for now
	//void FastDrop();//no drop micro available yet
	void FD();
	void GundamRush();
	//void Hiya4Fac();//needs an algorithm for economic cuts
	void iloveoovBuild();
	//void JoyORush();
	//void Nada2Fac();
	void ShallowTwo();


	//ProtossvsTerran
	void OneGateCore();
	void OneGateReaver();
	void Ten15Gates();
	void TwelveNexus();
	void N14Nexus();
	void TwoBaseArbiter();
	//void TwoBaseCarrier();//No exact timings yet here. Gotta ask on teamliquid.net . for now use heuristic to detect carrier
	void TwoGateDT();
	void TwoGateRangeExpand();
	void TwoGateZealot();
	void TwoGateObserver();
	void N21Nexus();
	void Bulldog();
	void DTDrop();
	void DTFastExpand();
	void GasStealElevatorRush();

};