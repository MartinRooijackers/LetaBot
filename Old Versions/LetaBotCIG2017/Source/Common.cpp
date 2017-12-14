#include "Common.h"

bool LanLatency = true;



Strategies CurrentStrategy;
Strategies InitialStrategy;

ProductionManager* ProdMan;
BuildingManager* bManager;
std::vector<BaseManager*> CCmanager;
InformationManager* InfoMan;
ScoutManager* ScoutMan;
WorkerDefenceManager* WDMan;
GeneralManager* GMan;
RepairManager* RPMan;
ScannerManager* ScanMan;
WraithManager* WraithMan;
SKManager* SKMan;
WraithMicro* WMicro;
MacroManager* MacroMan;
BuildOrderManager* BOMan;
WorkerAttackManager* WAMan;
MarineRushManager* MarineRush;


MicroManagerZerg *MicroZerg;
MicroManagerProtoss *MicroProtoss;
MicroManagerTerran *MicroTerran;