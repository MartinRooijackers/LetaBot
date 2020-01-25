#pragma once

//#include "Common.h"



class ScannerManager{
public:
	//std::vector<Unit> RepairSCVS;
	std::vector< Unit > Scanners;

	int LastScanFrame;

	ScannerManager();
	void OnFrame();
	void CheckBaseScan(); //use scan to check where the enemy base is
	void AddScanner( BWAPI::Unit scanner);
	//~ScoutManager() {};

	void AutoScanCloack();

};