#pragma once

#include <BWAPI.h>
#include <vector>


using namespace BWAPI;

//Battle box
// 0 3600 1560 4728
// 0 3600 1560 4520  if you dont want buildings

enum Conditionals{
	Distance_Closest_Enemy,   //distance to the closest enemy
	This_HP,                   //Get hp of this unit
	Total_Enemy_Dist,  //total amount of enemies in a given distance (or ALL)
	Total_Friendly_Dist, //total amount of friendly in a given distance (or ALL)
	Total_Enemy_Type_Dist,
	Total_Enemy_Up_Dist,
	Total_Enemy_Down_Dist,
	Total_Enemy_Left_Dist,
	Total_Enemy_Right_Dist,
	This_Weapon_Cooldown,
	Closest_Burrowed_Lurker, //highly specific but dirly needed
    This_Energy,

	//Terran Specific
	This_Cloacked,
	This_Mines,
	//Zerg specific

	This_Burrowed,
	This_Not_Burrowed,

	//Protoss specific
	This_Shield,

	Size_Cond
};

enum CompareType{
	Larger,
	Smaller,
	Equal,
	Not_Equal


};

enum Actions{
   Flee,   //Flee based on the position of the closest unit
   Nothing, //do no action
   Attack_Closest,
   Attack_Closest_Type,
   Attack_Lowest_HP,
   Attack_Lowest_HP_Type, // Attack the lowest HP unit of a certain type
   Attack_Move,  //Simple attack move to the closest enemy
   //movement
   Move_Up,
   Move_Down,
   Move_Left,
   Move_Right,
   Move_UpLeft,
   Move_UpRight,
   Move_DownLeft,
   Move_DownRight,
   //unit specefic abilities
   Patrol, //patrol micro for vultures and valkryies
   //Terran
   Heal,
   StimPack,
   Siege,
   Place_Mine,
   Cloack,
   LockDown,
   Yamato,
   Irradiate,
   EMP,
   Def_Matrix,
   //Zerg
   Burrow, //lurkers only for now
   Plaguuu,
   Dark_Swarm,
   Consume,
   Ensnare,
   Spawn_Broodling,
   //Protoss
   Storm,
   //Hallucination,  //going to add this one later
   Build_Scarab,
   Feedback,
   Maelstrom,
   Mind_Control,
   Load_Reaver,   //Shuttle action
   Unload_Reaver, //Shuttle action
   Build_Interceptor,
   Stasis_Field,
   //Recaaaaaaal,  adding this one later
   Disruption_Web,



   Action_Size
};

struct Condition{
	Conditionals condition;
	CompareType compare;
	int ValueToCompare;
	int Distance;
	BWAPI::UnitType CompareUnitType;
};

struct IfThen{
	std::vector<Condition> Conditions;
	Actions action; //the action that is considered
	BWAPI::UnitType ThisType; //in case of a certain target
	int ActionValue; //how much value this action should get
	bool IfThenAction; //if this results in an action or a unit type selection

};

struct GenMoves{
	std::vector<IfThen> IfStatements;



	BWAPI::UnitType ThisType; //the unit type that these ifstatements are for
	BWAPI::UnitType ToAttack; //in case of a certain target
	Actions action;

};

class MicroModule{
public:


	int UnitsSavedScore; //how many of our units stayed alive
	int UnitsLossRound;//how many of our units stayed alive
	bool StoreScore; //check if the score should be stored now
	bool NewRound; //check if the score should be stored now
	bool PlayingLeft; //whether or not this bot is currently playing at the left side

	std::vector< BWAPI::UnitType > OurTypes;
	std::vector< BWAPI::UnitType > EnemyTypes;

	std::vector<GenMoves> GenerateMoves;
	int ID;    //identifier for the auto heuristics
	int score; //Current score of this IfThen Set
	int IDlevel; //the current level of this ID
	std::vector<int> ScoreAllMoves; //score of all possible heuristics

	std::vector<  std::pair < BWAPI::UnitType, int > > allTypes;
	BWAPI::UnitType IntToUnitType(int value);
	int UnitTypetoInt( BWAPI::UnitType type);

	void CheckAction( BWAPI::Unit* unit );
	bool ConditionCheck( BWAPI::Unit* unit,Conditionals condition,CompareType compare,int ValueToCompare,int Distance,BWAPI::UnitType CompareType );
    void PerformAction( BWAPI::Unit* unit, Actions action, BWAPI::UnitType type );

	void NewHeuristics(); //new round, new heuristics
	void StoreEnd(); //called after the end of a round

	void LoadHeuristic( std::string FileLoc);
	void StoreHeuristic( std::string FileLoc);
	void StoreHeuristicReadable( std::string FileLoc);
	void GenerateNewHeuristic();
	Actions GenerateAction( BWAPI::UnitType type ); //generate a random action for this unit type
	Conditionals GenerateCondition( BWAPI::UnitType type ); //generate a random action for this unit type
	IfThen GenerateIfThen( BWAPI::UnitType type );
	int ConditionValues( Conditionals condition, BWAPI::UnitType type  );

	std::string ConditionalsToStr( Conditionals condition );
	Conditionals StrToConditionals( std::string condition );

	std::string CompTypeToStr( CompareType compare );
	CompareType StrToCompType( std::string compare );

	std::string ActionToStr( Actions action );
	Actions StrToAction( std::string action );


	//walkable
	bool Walkable [193][193];

	//used for the dummy bot
	std::vector< BWAPI::Unit* > AllOurDummyUnits; //A-move to the opponent
	std::vector< BWAPI::Unit* > AllEnemyDummyUnits; //Unit of the Opponent

	//general
	std::vector< BWAPI::Unit* > OurBuildings;
	std::vector< BWAPI::Unit* > EnemyBuildings;
	std::vector< BWAPI::Unit* > ComsatStations;
	std::vector< BWAPI::Unit* > Larvas;
	std::vector< BWAPI::Unit* > Bunkers;
	std::vector< BWAPI::Unit* > Barracks;

	//Terran units
	std::vector< BWAPI::Unit* > SCVs;
	std::vector< BWAPI::Unit* > Marines;
	std::vector< BWAPI::Unit* > MarinesStimRange;
	std::vector< BWAPI::Unit* > Medics;
	std::vector< BWAPI::Unit* > FireBats;
	std::vector< BWAPI::Unit* > Ghosts;
	std::vector< BWAPI::Unit* > Vultures;
	std::vector< BWAPI::Unit* > Tanks;
	std::vector< BWAPI::Unit* > TanksSiege;
	std::vector< BWAPI::Unit* > Goliaths;
	std::vector< BWAPI::Unit* > Wraiths;
	std::vector< BWAPI::Unit* > Dropships;
	std::vector< BWAPI::Unit* > ScienceVessels;
	std::vector< BWAPI::Unit* > Valkyries;
	std::vector< BWAPI::Unit* > BattleCruisers;
	//Protoss units
	std::vector< BWAPI::Unit* > Probes;
	std::vector< BWAPI::Unit* > Zealots;
	std::vector< BWAPI::Unit* > Dragoons;
	std::vector< BWAPI::Unit* > HighTemplars;
	std::vector< BWAPI::Unit* > DarkTemplars;
	std::vector< BWAPI::Unit* > Reavers;
	std::vector< BWAPI::Unit* > Archons;
	std::vector< BWAPI::Unit* > DarkArchons;
	std::vector< BWAPI::Unit* > Observers;
	std::vector< BWAPI::Unit* > Shuttles;
	std::vector< BWAPI::Unit* > Scouts;
	std::vector< BWAPI::Unit* > Carriers;
	std::vector< BWAPI::Unit* > Arbiters;
	std::vector< BWAPI::Unit* > Corsairs;
	//Zerg units
	std::vector< BWAPI::Unit* > Drones;
	std::vector< BWAPI::Unit* > Zergling;
	std::vector< BWAPI::Unit* > Hydralisk;
	std::vector< BWAPI::Unit* > Lurkers;
	std::vector< BWAPI::Unit* > Ultralisks;
	std::vector< BWAPI::Unit* > Defilers;
	std::vector< BWAPI::Unit* > InfestedTerrans;
	std::vector< BWAPI::Unit* >	Broodlings;
	std::vector< BWAPI::Unit* >	Overlords;
	std::vector< BWAPI::Unit* >	Mutalisks;
	std::vector< BWAPI::Unit* >	Scourges;
	std::vector< BWAPI::Unit* >	Queens;
	std::vector< BWAPI::Unit* >	Guardians;
	std::vector< BWAPI::Unit* >	Devourers;


	MicroModule();
	void OnFrame();
	void PutMicroHere();
	void RemoveDead();
	void RemoveDeadLoop( std::vector< BWAPI::Unit* >& VectorLoop );
	void AddUnit( Unit* unit);


	//Zerg Levels
	void Level1();

};