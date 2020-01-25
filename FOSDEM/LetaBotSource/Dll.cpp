#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <BWAPI.h>
#include <BWTA.h>

#include "LetaBotModule.h"

//namespace BWAPI { Game* Broodwar; }

extern "C" __declspec(dllexport) void gameInit(BWAPI::Game * game) { BWAPI::BroodwarPtr = game; }

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			//BWAPI::BWAPI_init();
			break;
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

 extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule(BWAPI::Game* game)
{
	//BWAPI::Broodwar=game;
	return new LetaBotModule();
}