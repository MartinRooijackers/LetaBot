#pragma once

#include <BWAPI.h>
#include <vector>


using namespace BWAPI;

//X Y coordinates of selectors
//Dual raceward: 424 32
//Single level: 552 112
//Random Select: 160 50

//Zerg Select: 0 336
//Terran Select: 160 320
//Protoss Select: 320 320

//Give up (Red):  2900 1944 
//Give up (Blue):  2960 1944 
//Offer Draw (Red):  2900 2064
//Offer Draw (Red):  2960 2064


//level select box
// 0 0 600 240
// race select box
// 0 260 380 450

//Civilian backup spot
// 5500 0 5500 580

enum SelectState{
	Intro,
	Level_Select,
	Race_Select,
	Game_Started
};

class LevelSelector{
public:

	std::string Race;
	std::string Level;
	bool SelectLevel;

	SelectState CurrentState;

    BWAPI::Unit* LevelCivilian;//civilian used for selecting tpye of level
    BWAPI::Unit* RaceCivilian;//civilian used for selecting race you want to play

	LevelSelector();
	void OnFrame();
	void AddCivilian( Unit* unit);
	void onReceiveText(Player* player, std::string text);

};