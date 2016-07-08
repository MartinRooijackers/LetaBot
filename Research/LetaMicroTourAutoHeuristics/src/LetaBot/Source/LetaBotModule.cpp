
#include "LetaBotModule.h"
#include "WraithMicro.h"

#include <vector>

bool analyzed = false;

using namespace BWAPI;

BWAPI::AIModule * __NewAIModule()
{
	return new LetaBotModule();
}
 
LetaBotModule::LetaBotModule()  {}
LetaBotModule::~LetaBotModule() {}

void LetaBotModule::onStart()
{

	//BWAPI::Broodwar->setLocalSpeed(-1);
	BWAPI::Broodwar->setLocalSpeed(0);
	//BWAPI::Broodwar->setFrameSkip(240);

	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);


	VMan = new VultureManager();
	LvlSelect = new LevelSelector();
	MicroMod = new MicroModule();

}

void LetaBotModule::onEnd(bool isWinner) 
{


}

void LetaBotModule::onFrame()
{

	/*
	if( VMan != NULL ){
		VMan->OnFrame();
	}
	*/

	LvlSelect->OnFrame();
	MicroMod->OnFrame();

	//std::string MouseCoord = "";
	//Broodwar->drawTextScreen(  Broodwar->getMousePosition().x() + 50, Broodwar->getMousePosition().y() , "%d %d", Broodwar->getScreenPosition().x(), Broodwar->getScreenPosition().y());

}

void LetaBotModule::onUnitDestroy(BWAPI::Unit * unit)
{


}

void LetaBotModule::onUnitMorph(BWAPI::Unit * unit)
{

}

void LetaBotModule::onUnitCreate(BWAPI::Unit * unit)
{ 

}

void LetaBotModule::onUnitShow(BWAPI::Unit * unit)
{ 
	//VMan->AddVulture(unit);
	//Broodwar->printf( unit->getType().c_str() );
	LvlSelect->AddCivilian(unit);
	MicroMod->AddUnit(unit);
}

void LetaBotModule::onUnitDiscover(Unit* unit)
{

}

void LetaBotModule::onUnitHide(BWAPI::Unit * unit)
{ 

}

void LetaBotModule::onUnitRenegade(BWAPI::Unit * unit)
{ 

}

void LetaBotModule::onNukeDetect(BWAPI::Position target)
{ 

}

void LetaBotModule::onUnitComplete(BWAPI::Unit *unit){


}


void LetaBotModule::onSendText(std::string text){

	//s for slow, standrd speed
	if( text == "s"){
		BWAPI::Broodwar->setLocalSpeed(-1);
	}
	//m for medium, twice as fast
	if( text == "m"){
		BWAPI::Broodwar->setLocalSpeed(20);
	}
	//f for fast
	if( text == "f"){
		BWAPI::Broodwar->setLocalSpeed(0);
	}

	Broodwar->sendText( text.c_str() );

}


void LetaBotModule::onReceiveText(Player* player, std::string text){

	LvlSelect->onReceiveText( player, text);

}


DWORD WINAPI AnalyzeThread()
{
  
}