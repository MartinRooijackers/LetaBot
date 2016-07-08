#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <iostream>
#include <fstream>
#include <windows.h>
//#include "Common.h"


DWORD WINAPI AnalyzeThread();
class LetaBotModule : public BWAPI::AIModule
{

	//BuildingManager* bManager;
	//SparCraftManager		micro;

public:
			
	LetaBotModule();
	~LetaBotModule();

	void	onStart();
	void	onFrame();
	void	onEnd(bool isWinner);
	//void	onUnitDestroy(BWAPI::Unit * unit);
	void	onSendText(std::string text);
	//void	onUnitCreate(BWAPI::Unit * unit);
	//void	onUnitShow(BWAPI::Unit * unit);
	void    onUnitComplete(BWAPI::Unit *unit);


	void drawStats();
	void drawTerrainData();
};
