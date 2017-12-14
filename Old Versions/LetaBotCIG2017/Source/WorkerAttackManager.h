#pragma once

#include "Common.h"


enum WorkerAttackState{
	Moving_Base,
	Attacking_Base,
	Drill_Flee,
	Fleeing_Nat,
	Repairing,
	Defend_Repair
};

struct SCvAttack{
	Unit* scv;
	WorkerAttackState state;
	int LastUnstuckFrame;
};

class WorkerAttackManager {
public:

	BWAPI::Position EnemyPos;
	BWAPI::Position EnemyNat;


	std::vector<SCvAttack> AttackSCVS;
	bool StopWorkerRush; //rush should be stopped if it removed the enemy resource depot

	WorkerAttackManager();
	void OnFrame();
	void AddSCV( BWAPI::Unit* scv);
	//~ScoutManager() {};

};