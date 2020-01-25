#pragma once

#include "Common.h"

#include <vector>

using namespace BWAPI;


struct SCVmineral{
	Unit scv;
	Unit mineralPatch;

	bool useTrick;//whether or not this SCV is using mineral trick
	Unit mineralTrick; //mineral field used for mineral trick
};

struct MineralPatch{
	bool trickPossible;//whether or not this mineral field is available for mineral trick
	Unit mineralTrick; //mineral field used for mineral trick
	Unit mineralPatch;
};

/*
enum FloatState{
	Liftoff,

};
*/

class BaseManager{
public:

	int cycleMineral;

	BaseManager( Unit CC , BWAPI::TilePosition land );
	void addSCV( Unit scv );
	void addMineralField(Unit mineral);
	void CalCulateTrickChoice();//calcualte which mineral fields can use coop path trick
	void CalCulateTrick();//calcualte which mineral fields can use coop path trick
	void buildSCV();
	void onFrame();
	void toGas( int ToSend = 3);

	void MineralGatherChoice();
	void MineralGather(); //mineral gathering with techniques
	void MineralGatherLock(); //mineral gathering using only lock 
	void MineralGatherNoTrick(); //mineral gathering without special techniques


	void LiftToNat();

	Unit getBuilder(); //remove a scv form minerals to build
	Unit getFullHPSCV(); //get full hp SCV for defence

	int SCVsaturation;
	Unit CommandCenter;
	BWAPI::TilePosition LandLocation;//in case the CC has to fly
	bool BaseReady; //base is ready when CC has landed
	bool buildComsat;
	int SCVsonGas;
	int InitialMinerals; //how many minerals this base initiall has

	std::vector<MineralPatch> Minerals;
	std::vector<Unit> Geysers;
	std::vector<Unit> Refinerys;

	std::vector< SCVmineral > MineralSCV;
	std::vector<Unit> GasSCV;

};