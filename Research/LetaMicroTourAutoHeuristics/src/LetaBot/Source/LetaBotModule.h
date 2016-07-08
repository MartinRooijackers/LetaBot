#pragma once

#include <BWAPI.h>
#include <iostream>
#include <fstream>
#include <windows.h>
//#include "WraithMicro.h"
#include "Common.h"


DWORD WINAPI AnalyzeThread();
class LetaBotModule : public BWAPI::AIModule
{
	//SparCraftManager		micro;

public:
			
	LetaBotModule();
	~LetaBotModule();

	void	onStart();
	void	onFrame();
	void	onEnd(bool isWinner);
	void	onUnitDestroy(BWAPI::Unit * unit);
	void	onUnitMorph(BWAPI::Unit * unit);
	void	onSendText(std::string text);
	void	onUnitCreate(BWAPI::Unit * unit);
	void	onUnitShow(BWAPI::Unit * unit);
	void	onUnitHide(BWAPI::Unit * unit);
	void	onUnitRenegade(BWAPI::Unit * unit);
	void    onNukeDetect(BWAPI::Position target);
	void    onUnitComplete(BWAPI::Unit *unit);
	void    onUnitDiscover(Unit* unit);
	void    onReceiveText(Player* player, std::string text);
};
