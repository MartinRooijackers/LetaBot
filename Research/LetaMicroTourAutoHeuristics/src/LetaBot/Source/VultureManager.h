#pragma once

#include <BWAPI.h>
#include <vector>


using namespace BWAPI;


struct Vulture{
	Unit* vulture;
	BWAPI::Position FirstMine;
	int mineCount;
	int mineFrame;
	BWAPI::Position curMine;
	bool orderGiven;
	int DragoonAttack; //0 = left, 1 = up, 2 = bottom, 3 = right
	int CodeNummer;
};

class VultureManager{
public:
	std::vector< Vulture > Vultures;

	VultureManager();
	void OnFrame();
	void AddVulture( Unit* unit);
	void ReaverDodge();

	int NextAttack; //counter for next attack
};