#include "LevelSelector.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <algorithm>
//#include <boost/foreach.hpp>


LevelSelector::LevelSelector()
{
	SelectLevel = false;
	Race = "Not Selected";
	Level = "Not Selected";
	LevelCivilian = NULL;
	RaceCivilian = NULL;

	CurrentState = Intro;
	
	//Red = Terran
	if( Broodwar->self()->getColor() == BWAPI::Colors::Red ){
		Race = "Terran";
		Level = "RaceSelect";
		SelectLevel = true;
	}
	//Blue = Zerg
	if( Broodwar->self()->getColor() == BWAPI::Colors::Blue ){
		Race = "Zerg";
		Level = "RaceSelect";
		SelectLevel = true;
	}

}

void LevelSelector::onReceiveText(Player* player, std::string text){

	if( player == Broodwar->self() ){
		return;
	}

	Broodwar->printf("Text received");

	if( text == "Terran" ){
		Race = "Terran";
		Level = "RaceSelect";
		SelectLevel = true;
	}
	if( text == "Protoss" ){
		Race = "Protoss";
		Level = "RaceSelect";
		SelectLevel = true;
	}
	if( text == "Zerg" ){
		Race = "Zerg";
		Level = "RaceSelect";
		SelectLevel = true;
	}
	if( text == "Random" ){
		Race = "Random";
		Level = "Random";
		SelectLevel = true;
	}


}


void LevelSelector::OnFrame(){

	if( SelectLevel == true){

		if( CurrentState == Level_Select && Level == "Random" ){
			if( LevelCivilian == NULL){
				Broodwar->printf("Level select civilian not found");
				return;
			}
			if( !LevelCivilian->exists() ){
				Broodwar->printf("Level select civilian doesn't exist");
				return;
			}
			LevelCivilian->move( BWAPI::Position(160,50) );
		}
		if( CurrentState == Level_Select && Level == "RaceSelect" ){
			if( LevelCivilian == NULL){
				Broodwar->printf("Level select civilian not found");
				return;
			}
			if( !LevelCivilian->exists() ){
				Broodwar->printf("Level select civilian doesn't exist");
				return;
			}
			LevelCivilian->move( BWAPI::Position(424 + 40,32 - 26) );
		}
		if( CurrentState == Race_Select && Level == "RaceSelect" ){
			if( RaceCivilian == NULL){
				Broodwar->printf("Race select civilian not found");
				return;
			}
			if( !RaceCivilian->exists() ){
				Broodwar->printf("Race select civilian doesn't exist");
				return;
			}
			if( Race == "Zerg"){
			  RaceCivilian->move( BWAPI::Position(0,336) );
			}
			if( Race == "Terran"){
			  RaceCivilian->move( BWAPI::Position(160,320) );
			}
			if( Race == "Protoss"){
			  RaceCivilian->move( BWAPI::Position(320,336) );
			}

		}

	}

}

void LevelSelector::AddCivilian( Unit* unit ){

	if( unit->getType() == BWAPI::UnitTypes::Terran_Civilian
		&& unit->getPlayer() == Broodwar->self()  ){

	        Broodwar->printf("Our civilian spotted");

			//check if it is the level select civilian
			if( unit->getPosition().x() < 600 && unit->getPosition().y() < 240 ){
                LevelCivilian = unit;
				RaceCivilian = NULL;
				CurrentState = Level_Select;
	            Broodwar->printf("Level select civilian added");
			}

			if( unit->getPosition().x() < 380 && unit->getPosition().y() < 450
				&& unit->getPosition().y() > 260 ){
                RaceCivilian = unit;
				LevelCivilian = NULL;
				CurrentState = Race_Select;
	            Broodwar->printf("Race select civilian added");
			}

			if( unit->getPosition().x() > 5500 && unit->getPosition().y() < 580){
                RaceCivilian = NULL;
				LevelCivilian = NULL;
				CurrentState = Game_Started;
	            Broodwar->printf("Micro tour started");
			}

	}

}
