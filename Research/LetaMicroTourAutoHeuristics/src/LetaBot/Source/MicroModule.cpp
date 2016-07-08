#include "MicroModule.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <algorithm>
#include <set>
#include <utility>
#include <fstream> 
//#include <iostream> 

#include <windows.h>
//#include <stdio.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/filesystem.hpp>


MicroModule::MicroModule()
{
	AllOurDummyUnits.clear();
	AllEnemyDummyUnits.clear();

	GenerateMoves.clear();

	OurTypes.clear();
	EnemyTypes.clear();

	UnitsSavedScore = 0;
	UnitsLossRound = 0;
	StoreScore = false;
	NewRound = true;

	//ID = 0;//TODO: check and fix bugs related to this

	for( int x=0; x<Broodwar->mapWidth(); x++){
		for( int y=0; y<Broodwar->mapHeight(); y++){
			Walkable[x][y] = 0;
		}
	}

	for( int x=0; x<Broodwar->mapWidth()*4; x++){
		for( int y=0; y<Broodwar->mapHeight()*4; y++){
			if( Broodwar->isWalkable( x, y) ){
			    Walkable[ (x/4) ][ (y/4) ] = 1;
			}
	  }
	}


	//std::vector<  std::pair < BWAPI::UnitType, int > > allTypes;

	BOOST_FOREACH( BWAPI::UnitType type, BWAPI::UnitTypes::allUnitTypes() ){
		std::pair< BWAPI::UnitType, int > newPair;
		newPair.first = type;
		newPair.second = 0;
		allTypes.push_back(newPair);
	}
	/*
	std::set<BWAPI::UnitType>::iterator  *it;
	const std::set<UnitType>& HelpF = BWAPI::UnitTypes::allUnitTypes();
    for(it=HelpF.begin(); it!=HelpF.end(); ++it){

	}
	*/

}

BWAPI::UnitType  MicroModule::IntToUnitType(int value)
{
	if( value == -1 ){
		return BWAPI::UnitTypes::AllUnits;
	}
	if( value > allTypes.size() ){
		Broodwar->printf( "int to unit type value error %d",value);

		return BWAPI::UnitTypes::AllUnits;
	}
	return allTypes[value].first;
}

int MicroModule::UnitTypetoInt( BWAPI::UnitType type)
{
	for( int i=0; i<allTypes.size(); i++){
		if( allTypes[i].first == type){
			return i;
		}
	}
	Broodwar->printf( "unit type not found in allTypes variable");
	return -1;

}


std::string MicroModule::ActionToStr( Actions action )
{


	switch(action){
     case Flee: return "Flee";
     case Nothing: return "Nothing";
	 case Attack_Closest: return "Attack_Closest";
     case Attack_Closest_Type: return "Attack_Closest_Type";
     case Attack_Lowest_HP: return "Attack_Lowest_HP";
     case Attack_Lowest_HP_Type: return "Attack_Lowest_HP_Type";
     case Attack_Move: return "Attack_Move";
     case Move_Up: return "Move_Up";
     case Move_Down: return "Move_Down";
     case Move_Left: return "Move_Left";
     case Move_Right: return "Move_Right";
     case Move_UpLeft: return "Move_UpLeft";
     case Move_UpRight: return "Move_UpRight";
     case Move_DownLeft: return "Move_DownLeft";
     case Move_DownRight: return "Move_DownRight";
     case Patrol: return "Patrol";
     case Heal: return "Heal";
     case StimPack: return "StimPack";
     case Siege: return "Siege";
     case Place_Mine: return "Place_Mine";
     case Cloack: return "Cloack";
     case LockDown: return "LockDown";
     case Yamato: return "Yamato";
     case Irradiate: return "Irradiate";
     case EMP: return "EMP";
     case Def_Matrix: return "Def_Matrix";
     case Burrow: return "Burrow";
     case Plaguuu: return "Plaguuu";
     case Dark_Swarm: return "Dark_Swarm";
     case Consume: return "Consume";
     case Ensnare: return "Ensnare";
     case Spawn_Broodling: return "Spawn_Broodling";
     case Storm: return "Storm";
     case Build_Scarab: return "Build_Scarab";
     case Feedback: return "Feedback";
     case Maelstrom: return "Maelstrom";
     case Mind_Control: return "Mind_Control";
     case Load_Reaver: return "Load_Reaver";
     case Unload_Reaver: return "Unload_Reaver";
     case Build_Interceptor: return "Build_Interceptor";
     case Stasis_Field: return "Stasis_Field";
     case Disruption_Web: return "Disruption_Web";
     default: return "Action_Not_Found";
	}
   //return "Action_Not_Found";
}


Actions MicroModule::StrToAction( std::string action )
{

	if( action == "Flee"){
		return Flee;
	}
	if( action ==  "Nothing"){
		return Nothing;
	}
	if( action == "Attack_Closest"){
		return Attack_Closest;
	}
	if( action == "Attack_Closest_Type"){
		return Attack_Closest_Type;
	}
	if( action == "Attack_Lowest_HP"){
		return Attack_Lowest_HP;
	}
	if( action == "Attack_Move"){
		return Attack_Move;
	}
	if( action == "Move_Up"){
		return Move_Up;
	}
	if( action == "Move_Down"){
		return Move_Down;
	}
	if( action == "Move_Left"){
		return Flee;
	}
	if( action == "Move_Right"){
		return Move_Right;
	}
	if( action == "Move_UpLeft"){
		return Move_UpLeft;
	}
	if( action == "Move_UpRight"){
		return Move_UpRight;
	}
	if( action == "Move_DownLeft"){
		return Move_DownLeft;
	}
	if( action == "Move_DownRight"){
		return Move_DownRight;
	}
	if( action == "Patrol"){
		return Patrol;
	}
	if( action == "Heal"){
		return Heal;
	}
	if( action == "StimPack"){
		return StimPack;
	}
	if( action == "Siege"){
		return Siege;
	}
	if( action == "Place_Mine"){
		return Place_Mine;
	}
	if( action == "Cloack"){
		return Cloack;
	}
	if( action == "LockDown"){
		return LockDown;
	}
	if( action == "Yamato"){
		return Yamato;
	}
	if( action == "Irradiate"){
		return Irradiate;
	}
	if( action == "EMP"){
		return EMP;
	}
	if( action == "Def_Matrix"){
		return Def_Matrix;
	}
	if( action == "Burrow"){
		return Burrow;
	}
	if( action == "Plaguuu"){
		return Plaguuu;
	}
	if( action == "Dark_Swarm"){
		return Dark_Swarm;
	}
	if( action == "Consume"){
		return Consume;
	}
	if( action == "Ensnare"){
		return Ensnare;
	}
	if( action == "Spawn_Broodling"){
		return Spawn_Broodling;
	}
	if( action == "Storm"){
		return Storm;
	}
	if( action == "Build_Scarab"){
		return Build_Scarab;
	}
	if( action == "Feedback"){
		return Feedback;
	}
	if( action == "Maelstrom"){
		return Maelstrom;
	}
	if( action == "Mind_Control"){
		return Mind_Control;
	}
	if( action == "Load_Reaver"){
		return Load_Reaver;
	}
	if( action == "Unload_Reaver"){
		return Unload_Reaver;
	}
	if( action == "Build_Interceptor"){
		return Build_Interceptor;
	}
	if( action == "Stasis_Field"){
		return Stasis_Field;
	}
	if( action == "Disruption_Web"){
		return Disruption_Web;
	}
	return Nothing;

}


CompareType MicroModule::StrToCompType( std::string compare )
{

	if( compare == "Larger"){
		return Larger;
	}
	if( compare == "Smaller"){
		return Smaller;
	}
	if( compare == "Equal"){
		return Equal;
	}
	if( compare == "Not_Equal"){
		return Not_Equal;
	}
	return Smaller;

}

std::string MicroModule::CompTypeToStr( CompareType compare )
{


	switch(compare){
     case Larger: return "Larger";
     case Smaller: return "Smaller";
     case Equal: return "Equal";
     case Not_Equal: return "Not_Equal";
     default: return "Comp tpye not found";
	}

}


Conditionals MicroModule::StrToConditionals( std::string condition )
{

	if( condition == "Distance_Closest_Enemy"){
		return Distance_Closest_Enemy;
	}
	if( condition == "This_HP"){
		return This_HP;
	}
	if( condition == "Total_Enemy_Dist"){
		return Total_Enemy_Dist;
	}
	if( condition == "Total_Friendly_Dist"){
		return Total_Friendly_Dist;
	}
	if( condition == "Total_Enemy_Type_Dist"){
		return Total_Enemy_Type_Dist;
	}
	if( condition == "Total_Enemy_Up_Dist"){
		return Total_Enemy_Up_Dist;
	}
	if( condition == "Total_Enemy_Down_Dist"){
		return Total_Enemy_Down_Dist;
	}
	if( condition == "Total_Enemy_Left_Dist"){
		return Total_Enemy_Left_Dist;
	}
	if( condition == "Total_Enemy_Right_Dist"){
		return Total_Enemy_Right_Dist;
	}
	if( condition == "This_Energy"){
		return This_Energy;
	}
	if( condition == "This_Weapon_Cooldown"){
		return This_Weapon_Cooldown;
	}
	if( condition == "Size_Cond"){
		return Size_Cond;
	}
	if( condition == "This_Cloacked"){
		return This_Cloacked;
	}
	if( condition == "This_Mines"){
		return This_Mines;
	}
	if( condition == "This_Burrowed"){
		return This_Burrowed;
	}
	if( condition == "This_Not_Burrowed"){
		return This_Not_Burrowed;
	}
	if( condition == "Closest_Burrowed_Lurker"){
		return Closest_Burrowed_Lurker;
	}


}

std::string MicroModule::ConditionalsToStr( Conditionals condition )
{


	switch(condition){
	case Distance_Closest_Enemy: return "Distance_Closest_Enemy";
	case This_HP: return "This_HP";
	case Total_Enemy_Dist: return "Total_Enemy_Dist";
	case Total_Friendly_Dist: return "Total_Friendly_Dist";
	case Total_Enemy_Type_Dist: return "Total_Enemy_Type_Dist";
	case Total_Enemy_Up_Dist: return "Total_Enemy_Up_Dist";
	case Total_Enemy_Down_Dist: return "Total_Enemy_Down_Dist";
	case Total_Enemy_Left_Dist: return "Total_Enemy_Left_Dist";
	case Total_Enemy_Right_Dist: return "Total_Enemy_Right_Dist";
    case This_Energy: return "This_Energy";
	case This_Weapon_Cooldown: return "This_Weapon_Cooldown";
	case Size_Cond: return "Size_Cond";
	case This_Cloacked: return "This_Cloacked";
	case This_Mines: return "This_Mines";
	case This_Burrowed: return "This_Burrowed";
	case This_Not_Burrowed: return "This_Not_Burrowed";
	case Closest_Burrowed_Lurker: return "Closest_Burrowed_Lurker";
     default: return "Conditional not found";
	}

}

void MicroModule::RemoveDeadLoop( std::vector< BWAPI::Unit* >& VectorLoop ){

	for( int i=0; i<VectorLoop.size(); i++){
		if( !VectorLoop[i]->exists() ){
			VectorLoop.erase( VectorLoop.begin() + i);
			i--;
			continue;
		}
	}

}

void MicroModule::RemoveDead(){

	for( int i=0; i<AllOurDummyUnits.size(); i++){
		if( !AllOurDummyUnits[i]->exists() ){
			AllOurDummyUnits.erase( AllOurDummyUnits.begin() + i);
			i--;
			continue;
		}
	}

	for( int i=0; i<AllEnemyDummyUnits.size(); i++){
		if( !AllEnemyDummyUnits[i]->exists() ){
			AllEnemyDummyUnits.erase( AllEnemyDummyUnits.begin() + i);
			i--;
			continue;
		}
	}


	RemoveDeadLoop(OurBuildings);
	RemoveDeadLoop(EnemyBuildings);

	RemoveDeadLoop(SCVs);
	RemoveDeadLoop(Marines);
	RemoveDeadLoop(MarinesStimRange);
	RemoveDeadLoop(Medics);
	RemoveDeadLoop(FireBats);
	RemoveDeadLoop(Ghosts);
	RemoveDeadLoop(Goliaths);
	RemoveDeadLoop(Tanks);
	RemoveDeadLoop(TanksSiege);
	RemoveDeadLoop(Vultures);
	RemoveDeadLoop(Wraiths);
	RemoveDeadLoop(Dropships);
	RemoveDeadLoop(Valkyries);
	RemoveDeadLoop(ScienceVessels);
	RemoveDeadLoop(BattleCruisers);

}


bool MicroModule::ConditionCheck( BWAPI::Unit* unit,Conditionals condition,CompareType compare,int ValueToCompare,int Distance,BWAPI::UnitType CompareType )
{
	//check if there is a unit at a certain distance
	if( condition == Distance_Closest_Enemy){
		int distance = 99999;
		for(int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < distance){
				distance = unit->getDistance( AllEnemyDummyUnits[i]->getPosition() );

			}
		}
		if( compare == Smaller){
			if(  distance < Distance ){
				return true;
			}
		}
		if( compare == Larger){
			if(  distance > Distance ){
				return true;
			}
		}
		return false;
	}

	if( condition == This_HP){
		if( compare == Smaller){
			if( unit->getHitPoints() < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  unit->getHitPoints() > ValueToCompare){
				return true;
			}
		}
		return false;
	}



	if( condition == Total_Enemy_Dist){
	    int TotalEnemy = 0;
		for(int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < Distance ){
				TotalEnemy++;
			}
		}
		if( compare == Smaller){
			if( TotalEnemy < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  TotalEnemy > ValueToCompare){
				return true;
			}
		}
		return false;
	}


	if( condition == Total_Enemy_Type_Dist){
	    int TotalEnemy = 0;
		for(int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getType() == CompareType &&
				unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < Distance ){
				TotalEnemy++;
			}
		}
		if( compare == Smaller){
			if( TotalEnemy < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  TotalEnemy > ValueToCompare){
				return true;
			}
		}
		return false;
	}

	if( condition == Total_Friendly_Dist){
	    int TotalFriend = 0;
		for(int i=0; i<AllOurDummyUnits.size(); i++){
			if( unit->getDistance( AllOurDummyUnits[i]->getPosition() ) < Distance ){
				TotalFriend++;
			}
		}
		if( compare == Smaller){
			if( TotalFriend < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  TotalFriend > ValueToCompare){
				return true;
			}
		}
		return false;
	}


	if( condition == Total_Enemy_Up_Dist
		|| condition == Total_Enemy_Down_Dist
		|| condition == Total_Enemy_Left_Dist
		|| condition == Total_Enemy_Right_Dist){
	    int TotalEnemy = 0;
		for(int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( AllEnemyDummyUnits[i]->getPosition().x() < unit->getPosition().x() && condition == Total_Enemy_Left_Dist
				&& unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < Distance ){
				TotalEnemy++;
			}
			if( AllEnemyDummyUnits[i]->getPosition().x() > unit->getPosition().x() && condition == Total_Enemy_Right_Dist
				&& unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < Distance ){
				TotalEnemy++;
			}
			if( AllEnemyDummyUnits[i]->getPosition().y() < unit->getPosition().y() && condition == Total_Enemy_Up_Dist
				&& unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < Distance ){
				TotalEnemy++;
			}
			if( AllEnemyDummyUnits[i]->getPosition().y() > unit->getPosition().y() && condition == Total_Enemy_Down_Dist
				&& unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < Distance ){
				TotalEnemy++;
			}
		}
		if( compare == Smaller){
			if( TotalEnemy < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  TotalEnemy > ValueToCompare){
				return true;
			}
		}
		return false;
	}


	if( condition == This_Energy){
		if( compare == Smaller){
			if( unit->getEnergy() < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  unit->getEnergy() > ValueToCompare){
				return true;
			}
		}
		return false;
	}


	if( condition == This_Weapon_Cooldown){
		if( compare == Smaller){
			if( unit->getGroundWeaponCooldown() < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  unit->getGroundWeaponCooldown() > ValueToCompare){
				return true;
			}
		}
		return false;
	}

	if( condition == This_Cloacked ){
		if( unit->isCloaked() ){
			return true;
		} else {
			return false;
		}
	}

	if( condition == This_Mines ){
		if( compare == Smaller){
			if( unit->getSpiderMineCount() < ValueToCompare ){
				return true;
			}
		}
		if( compare == Larger){
			if(  unit->getSpiderMineCount() > ValueToCompare){
				return true;
			}
		}
		return false;
	}


	if( condition == This_Burrowed ){
		if( unit->isBurrowed() ){
			return true;
		} else {
			return false;
		}
	}


	if( condition == This_Not_Burrowed ){
		if( !unit->isBurrowed() ){
			return true;
		} else {
			return false;
		}
	}

	if( condition == Closest_Burrowed_Lurker){
		int distance = 99999;
		for(int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getType() == BWAPI::UnitTypes::Zerg_Lurker &&
				unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < distance){
				distance = unit->getDistance( AllEnemyDummyUnits[i]->getPosition() );

			}
		}

		if( compare == Smaller){
			if( distance < Distance ){
				return true;
			}
		}
		if( compare == Larger){
			if(  distance > Distance ){
				return true;
			}
		}
		return false;

	}

	//unknown conditional
	Broodwar->printf("Unknown conditional");
	return false;

}


void MicroModule::PerformAction( BWAPI::Unit* unit, Actions action, BWAPI::UnitType type ){

	//get closest enemy
	int distance = 99999;
	BWAPI::Unit* closest = NULL;
	int distanceAttack = 99999;
	BWAPI::Unit* closestAttack = NULL;
	int lowHP = 9999;
	BWAPI::Unit* closestLowHP = NULL;
	int AttackRange = unit->getType().groundWeapon().maxRange();
	if( AttackRange < unit->getType().airWeapon().maxRange() ){
		AttackRange = unit->getType().airWeapon().maxRange();
	}
	for(int i=0; i<AllEnemyDummyUnits.size(); i++){
		if( unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < distance){
			distance = unit->getDistance( AllEnemyDummyUnits[i]->getPosition() );
			closest = AllEnemyDummyUnits[i];
		}

		if( AllEnemyDummyUnits[i]->getHitPoints() < lowHP
			 && unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < ( AttackRange + 32 ) ){
			lowHP = AllEnemyDummyUnits[i]->getHitPoints();
			closestLowHP = AllEnemyDummyUnits[i];
		}

		//check if you can hit the target
		if(  ( unit->getType().groundWeapon() != BWAPI::WeaponTypes::None && !AllEnemyDummyUnits[i]->getType().isFlyer() )
			 || ( unit->getType().airWeapon() != BWAPI::WeaponTypes::None && AllEnemyDummyUnits[i]->getType().isFlyer() ) ){
		
			 if( unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < distanceAttack
				 && AllEnemyDummyUnits[i]->isDetected() ){
			   distanceAttack = unit->getDistance( AllEnemyDummyUnits[i]->getPosition() );
			   closestAttack = AllEnemyDummyUnits[i];
		     }
		}

	}

	if( closest == NULL){
		return;
	}

	if( closestAttack == NULL){
		closestAttack = closest;
		//return;
	}

	if( action == Nothing){
		return;
	}

	if( action == Flee){

		BWAPI::Position fleePosition( (unit->getPosition() - closest->getPosition())+(unit->getPosition() - closest->getPosition()) + unit->getPosition());
		unit->move( fleePosition );
		return;
	}

	if( action == Attack_Move){

		if( unit->getOrder() != BWAPI::Orders::AttackMove
			&& unit->getOrder() != BWAPI::Orders::AttackUnit
			&& unit->getOrder() != BWAPI::Orders::AttackTile	){		    
			unit->attack( closestAttack );
		}
		return;
	}

	if( action == Attack_Closest){

		//move all non-attacking casters
		if( unit->getType() == BWAPI::UnitTypes::Terran_Medic
			|| unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel
			|| unit->getType() == BWAPI::UnitTypes::Protoss_Observer
			|| unit->getType() == BWAPI::UnitTypes::Zerg_Defiler
			|| unit->getType() == BWAPI::UnitTypes::Zerg_Queen
			|| unit->getType() == BWAPI::UnitTypes::Protoss_High_Templar
			|| unit->getType() == BWAPI::UnitTypes::Protoss_Dark_Archon){
				//if( unit->getOrder() != BWAPI::Orders::Move){
				  //Broodwar->printf("Move");
				  unit->move( closest->getPosition() );
		          return;
				//}
		}

		/*
		//if it has no weapons, then attack move to the position
		if( (unit->getType().airWeapon() == BWAPI::WeaponTypes::None &&
			unit->getType().groundWeapon() == BWAPI::WeaponTypes::None)
			|| (unit->getType().airWeapon() == BWAPI::WeaponTypes::Unknown &&
			unit->getType().groundWeapon() == BWAPI::WeaponTypes::Unknown) ){
				//unit->attack( closest->getPosition() );
				unit->move( closest->getPosition() );
		        return;
		}
		*/

		if( unit->getOrderTarget() == NULL ){
           unit->attack(  closestAttack );
		   return;
		}
		else if( unit->getOrderTarget()->getID() != closestAttack->getID() ){
		  unit->attack(  closestAttack );

		}
		return;
	}

	if( action == Attack_Closest_Type){
		//TODO: implement type

		distance = 99999;
		closest = NULL;
	    for(int i=0; i<AllEnemyDummyUnits.size(); i++){
		  if( unit->getDistance( AllEnemyDummyUnits[i]->getPosition() ) < distance
			&&  unit->getType() == type ){
			distance = unit->getDistance( AllEnemyDummyUnits[i]->getPosition() );
			closest = AllEnemyDummyUnits[i];
		  }
	    }

		if( closest == NULL ){
          closest = closestAttack;
		}

		if( unit->getOrderTarget() == NULL ){
           unit->attack(  closest );
		   return;
		}
		else if( unit->getOrderTarget()->getID() != closest->getID() ){
		  unit->attack(  closest );
		  return;
		}
		return;
	}

	if( action == Attack_Lowest_HP){
		if( closestLowHP == NULL){
			//closestLowHP = closest;
			closestLowHP = closestAttack;
		}
		if( unit->getOrderTarget() == NULL ){
           unit->attack(  closestLowHP );
		}
		else if( unit->getOrderTarget()->getID() != closestLowHP->getID() ){
		  unit->attack(  closestLowHP );
		}
		return;
	}

	if( action == Attack_Lowest_HP_Type){
		//TODO: implement type

	   //get closest enemy
	   lowHP = 9999;
	   closestLowHP = NULL;
	   for(int i=0; i<AllEnemyDummyUnits.size(); i++){
		if( AllEnemyDummyUnits[i]->getHitPoints() < lowHP
			&&  unit->getType() == type  ){
			lowHP = AllEnemyDummyUnits[i]->getHitPoints();
			closestLowHP = AllEnemyDummyUnits[i];
		}
	   }

	   if( closestLowHP == NULL ){
		  closestLowHP = closestAttack;
	   }

		if( unit->getOrderTarget() == NULL ){
           unit->attack(  closestLowHP );
		}
		else if( unit->getOrderTarget()->getID() != closestLowHP->getID() ){
		  unit->attack(  closestLowHP );
		}
		return;
	}


	if( action == Move_Up || action == Move_Down || action == Move_Left || action == Move_Right
		|| action == Move_UpLeft || action == Move_UpRight || action == Move_DownLeft || action == Move_DownRight){
		int OffsetX = 0;
		int OffsetY = 0;
		if( action == Move_Up){  OffsetY -= 2*32;  }
		if( action == Move_Down){  OffsetY += 2*32;  }
		if( action == Move_Left){  OffsetX -= 2*32;  }
		if( action == Move_Right){  OffsetX += 2*32;  }
		if( action == Move_UpLeft){  OffsetY -= 2*32;OffsetX -= 2*32;  }
		if( action == Move_UpRight){  OffsetY -= 2*32;OffsetX += 2*32;  }
		if( action == Move_DownLeft){  OffsetY += 2*32;OffsetX -= 2*32;  }
		if( action == Move_DownRight){  OffsetY += 2*32;OffsetX += 2*32;  }
		BWAPI::Position MovePos( unit->getPosition().x() + OffsetX,unit->getPosition().y() + OffsetY);
		unit->move( MovePos );
		return;
	}

	if( action == Patrol){

		BWAPI::Position PatrolPosition( (unit->getPosition() - closest->getPosition())+(unit->getPosition() - closest->getPosition()) + unit->getPosition());
		unit->patrol( PatrolPosition );
		return;
	}


	/* Terran */

	if( action == Heal){
		//TODO: implement heal
		if( unit->getOrder() == BWAPI::Orders::HealMove 
			|| unit->getOrder() == BWAPI::Orders::MedicHeal1
			|| unit->getOrder() == BWAPI::Orders::MedicHeal2){
				return;//already healing
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllOurDummyUnits.size(); i++){
			if( AllOurDummyUnits[i]->getID() != unit->getID()
				&& AllOurDummyUnits[i]->getType().isOrganic()
				&& !AllOurDummyUnits[i]->isBeingHealed()
				&& unit->getDistance(AllOurDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllOurDummyUnits[i]);
				closest = AllOurDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
		  unit->useTech( BWAPI::TechTypes::Healing , closest);
		}
		return;
	}

	if( action == StimPack){
		//if( unit->getOrder() == BWAPI::Orders::
		unit->useTech( BWAPI::TechTypes::Stim_Packs );
		return;
	}

	if( action == Siege){
		unit->useTech( BWAPI::TechTypes::Tank_Siege_Mode );
		return;
	}

	if( action == Place_Mine){
		//TODO: currently at own position, should maybe change
		if( unit->getOrder() == BWAPI::Orders::PlaceMine
			|| unit->getOrder() == BWAPI::Orders::VultureMine){
				return;
		}
		unit->useTech( BWAPI::TechTypes::Spider_Mines, unit->getPosition() );
		return;
	}

	if( action == Cloack){

		if( unit->getType() == BWAPI::UnitTypes::Terran_Wraith){
		  unit->useTech( BWAPI::TechTypes::Cloaking_Field );
		}
		if( unit->getType() == BWAPI::UnitTypes::Terran_Ghost){
			unit->useTech( BWAPI::TechTypes::Personnel_Cloaking );
		}

	}

	if( action == LockDown){
		//TODO: make sure that lockdown hits mechenical target

		if( unit->getOrder() == BWAPI::Orders::CastLockdown){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( AllEnemyDummyUnits[i]->getType().isMechanical()
				&& unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closest = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Lockdown , closest);
		}
		return;

	}

	if( action == Yamato){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::FireYamatoGun){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Zerg_Ultralisk
				&& unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closest = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Yamato_Gun , closest);
		}
		return;

	}
	if( action == Irradiate){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::CastIrradiate){
				return;//already casting lockdown
		}
		int closestDist = 99999;
		BWAPI::Unit* closestIR = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( (AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Zerg_Lurker
				|| AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Zerg_Mutalisk )
				&& unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closestIR = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closestIR != NULL){
			unit->useTech( BWAPI::TechTypes::Irradiate , closestIR);
		}
		return;

	}
	if( action == EMP){
		//TODO: decisino on which target to hit
		if( unit->getOrder() == BWAPI::Orders::CastEMPShockwave){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closestEMP = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( (AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Zerg_Defiler
				|| AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Zerg_Queen )
				&& unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closestEMP = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closestEMP != NULL){
			unit->useTech( BWAPI::TechTypes::EMP_Shockwave , closestEMP->getPosition() );
		}
		return;

	}
	if( action == Def_Matrix){
		//TODO: decisino on which target to hit
	}

	/* Zerg */

	if( action == Burrow){
		if( unit->getOrder() == BWAPI::Orders::Burrowing ||  unit->getOrder() == BWAPI::Orders::Unburrowing){
		  return; //finish previous burrow command first
		}
		  unit->useTech( BWAPI::TechTypes::Burrowing );
	}
	if( action == Plaguuu){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::CastPlague){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closest = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Plague , closest->getPosition() );
		}
		return;

	}
	if( action == Dark_Swarm){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::CastDarkSwarm){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closest = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Dark_Swarm , closest->getPosition() );
		}
		return;

	}
	if( action == Consume){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::CastConsume){
				return;//already healing
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllOurDummyUnits.size(); i++){
			if( AllOurDummyUnits[i]->getID() != unit->getID()
				&& AllOurDummyUnits[i]->getType() == BWAPI::UnitTypes::Zerg_Zergling
				&& unit->getDistance(AllOurDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllOurDummyUnits[i]);
				closest = AllOurDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Consume , closest);
		}
		return;

	}
	if( action == Ensnare){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::CastEnsnare){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closest = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Ensnare , closest->getPosition() );
		}
		return;

	}
	if( action == Spawn_Broodling){
		//TODO: decisino on which target to hit

		if( unit->getOrder() == BWAPI::Orders::CastSpawnBroodlings){
				return;//already casting lockdown
		}
		int closestDist = 9999;
		BWAPI::Unit* closest = NULL;
		for( int i=0; i<AllEnemyDummyUnits.size(); i++){
			if( (AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				|| AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Hero_Edmund_Duke_Siege_Mode
				|| AllEnemyDummyUnits[i]->getType() == BWAPI::UnitTypes::Hero_Edmund_Duke_Tank_Mode ) 
				&&
				unit->getDistance(AllEnemyDummyUnits[i]) < closestDist ){
				closestDist = unit->getDistance(AllEnemyDummyUnits[i]);
				closest = AllEnemyDummyUnits[i];
				//unit->useTech( BWAPI::TechTypes::Healing , AllOurDummyUnits[i]);
			}
		}
		if( closest != NULL){
			unit->useTech( BWAPI::TechTypes::Ensnare , closest->getPosition() );
		}
		return;

	}

	/* Protoss */

	if( action == Storm){
		//TODO: decisino on which target to hit
	}
	if( action == Build_Scarab){
		unit->train( BWAPI::UnitTypes::Protoss_Scarab );
	}
	if( action == Feedback){
		//TODO: decisino on which target to hit
	}
	if( action == Maelstrom){
		//TODO: decisino on which target to hit
	}
	if( action == Mind_Control){
		//TODO: decisino on which target to hit
	}
	if( action == Load_Reaver){
		//TODO: decisino on which target to hit
	}
	if( action == Unload_Reaver){
		//TODO: decisino on which target to hit
	}
	if( action == Build_Interceptor){
		unit->train( BWAPI::UnitTypes::Protoss_Interceptor );
	}
	if( action == Stasis_Field){
		//TODO: decisino on which target to hit
	}
	if( action == Disruption_Web){
		//TODO: decisino on which target to hit
	}


}

void MicroModule::CheckAction( BWAPI::Unit* unit ){




	int GenID = -1;
	for( int i=0; i<GenerateMoves.size(); i++){
		if(  GenerateMoves[i].ThisType.getName() == unit->getType().getName() ){
           GenID = i;
		   break;
		}
	}

	if( GenID == -1){
		//Broodwar->printf("Unit has no corresponding heuristics. Creating one");

			GenMoves newGenMove;
			newGenMove.ThisType = unit->getType();
			newGenMove.ToAttack = BWAPI::UnitTypes::AllUnits;
			IfThen attackThen; //only attack. No conditions
			attackThen.action = Attack_Closest;
			attackThen.ActionValue = 1;
			attackThen.IfThenAction = true;
			attackThen.Conditions.clear();
			newGenMove.IfStatements.push_back( attackThen );
		    GenerateMoves.push_back( newGenMove );
			GenID = GenerateMoves.size() - 1;
		//return;
	}

	std::vector<int> ScoreActions;
	for(int i=0; i<Action_Size; i++){ //fill up the total actions
		ScoreActions.push_back( 0 );//all actions start at 0
	}

	//BWAPI::UnitTypes::allUnitTypes
	

	//reset unit type counter
	for( int i=0; i<allTypes.size(); i++){
			allTypes[i].second = 0;
	}

	/*
	std::vector<int> ScoreUnitType;
	for(int i=0; i< BWAPI::UnitTypes::allUnitTypes ; i++){ //fill up the total actions
		ScoreUnitType.push_back( 0 );//all actions start at 0
	}
	*/

	//get the values based on the conditions
	for(int i=0; i<GenerateMoves[GenID].IfStatements.size(); i++){
		IfThen curIfThen = GenerateMoves[GenID].IfStatements[i];
		bool AllCond = true; //check if all conditions are true
		for( int j=0; j<curIfThen.Conditions.size(); j++){
			if( ConditionCheck( unit, curIfThen.Conditions[j].condition, 
				curIfThen.Conditions[j].compare,curIfThen.Conditions[j].ValueToCompare,curIfThen.Conditions[j].Distance
				, curIfThen.Conditions[j].CompareUnitType )
				== false )
			{
				AllCond = false;
				break;
			}
		}
		if( AllCond == true){
			if( curIfThen.IfThenAction == true ){
			  ScoreActions[ curIfThen.action ] += curIfThen.ActionValue;
			} else { //unit type
				for( int i=0; i<allTypes.size(); i++){
					if(  allTypes[i].first == curIfThen.ThisType ){
						allTypes[i].second += curIfThen.ActionValue;//1;
					}
				}
			}
			//TODO: add the value action to the total
			//PerformAction( unit, curIfThen.action);
		}
	}
	
	int BestActionScore = 0;
	Actions bestAction = Nothing;
	for(int i=0; i<Action_Size; i++){ //fill up the total actions
		if( BestActionScore < ScoreActions[i] ){
			BestActionScore = ScoreActions[i];
			bestAction = static_cast<Actions>(i);
		}
	}

	int BestTypeScore = 0;
	BWAPI::UnitType bestType = BWAPI::UnitTypes::AllUnits;
	for(int i=0; i<allTypes.size(); i++){ //fill up the total actions
		if( BestTypeScore < allTypes[i].second ){
			BestActionScore = allTypes[i].second;
			bestType = allTypes[i].first;
		}
	}


	Broodwar->drawTextMap( unit->getPosition().x(),unit->getPosition().y() + 8, ActionToStr( bestAction ).c_str() );


	for( int i=0; i<GenerateMoves.size(); i++){
		Broodwar->drawTextScreen( 380,100 + i*10, "%d", GenerateMoves[i].IfStatements.size() );
		Broodwar->drawTextScreen( 400,100 + i*10, GenerateMoves[i].ThisType.c_str() );
	}
	

  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 ){
    return;
  }

	PerformAction( unit, bestAction, bestType);

}

void MicroModule::PutMicroHere(){

  //if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    //return;

	bool LanLatency = true;

	for( int i=0; i<SCVs.size(); i++){

		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if( close->getType().isFlyer() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > SCVs[i]->getDistance( close->getPosition() )  ){
				closeDist = SCVs[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}
		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

		if ( isUnderDS )
	     {
		   BWAPI::Position fleePosition( (SCVs[i]->getPosition() - closest->getPosition())+(SCVs[i]->getPosition() - closest->getPosition()) + SCVs[i]->getPosition());
		    if( SCVs[i]->getOrder() != BWAPI::Orders::Move){
		       SCVs[i]->move( fleePosition );
			   continue;
		    }		   
		}

		//if the cooldown is 0, attack
		if( SCVs[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
                  if( closest != NULL ){
					  if( SCVs[i]->getOrderTarget() == NULL ){
                        SCVs[i]->attack(  closest );
					  }
					  else if( SCVs[i]->getOrderTarget()->getID() != closest->getID() ){
		                SCVs[i]->attack(  closest );
					  }
				  } else {
					  //prevent spam
					  if( SCVs[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  SCVs[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  SCVs[i]->getOrder() != BWAPI::Orders::AttackUnit ){
							  if( AllEnemyDummyUnits.size() > 0){
								  SCVs[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
							  }
					  }
				  }			
			continue;
		}
	}

	for( int i=0; i<Marines.size(); i++){


		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		Unit* lowestHPinRange = NULL; //enemy with lowest HP in attack range
		int lowestHP = 99999;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Marines[i]->getDistance( close->getPosition() )  ){
				closeDist = Marines[i]->getDistance( close->getPosition() );
				closest = close;
			}
			if( lowestHP > ( close->getHitPoints() + close->getShields() )
				 &&  Marines[i]->getDistance( close->getPosition() ) < 5*32 ){
				lowestHPinRange = close; 
				lowestHP = close->getHitPoints() + close->getShields();
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

		//use stim before engaging enemy
		if( closest != NULL && !Marines[i]->isStimmed() && Marines[i]->getHitPoints() == 40
			&& closeDist < 6*32 ){
			 Marines[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		/*
		if(  lowestHPinRange != NULL){
			if( Marines[i]->getOrderTarget() == NULL){
				Marines[i]->attack( lowestHPinRange );
			} else if( Marines[i]->getOrderTarget() != lowestHPinRange ){
				Marines[i]->attack( lowestHPinRange );
			}
			continue;
	     }
		 */
		int Laten = Broodwar->getLatencyFrames();

		// Marines[i]->getGroundWeaponCooldown() != 0
		if ( ( closeDist < 4*32 && Marines[i]->getGroundWeaponCooldown() > Laten && LanLatency == true ) 
			||  isUnderDS )
	     {
		   BWAPI::Position fleePosition( (Marines[i]->getPosition() - closest->getPosition())+(Marines[i]->getPosition() - closest->getPosition()) + Marines[i]->getPosition());
		    if( Marines[i]->getOrder() != BWAPI::Orders::Move){
		       Marines[i]->move( fleePosition );
			   continue;
		    }		   
		}



		//if the cooldown is 0, attack
		// Marines[i]->getGroundWeaponCooldown() == 0
		if( Marines[i]->getGroundWeaponCooldown() <= Laten){ //|| closeDist > 3*32

		  if(  lowestHPinRange != NULL && lowestHP < 35 ){
				if( Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
					Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){
			if( Marines[i]->getOrderTarget() == NULL){
				Marines[i]->attack( lowestHPinRange );
			} else if( Marines[i]->getOrderTarget() != lowestHPinRange ){
				Marines[i]->attack( lowestHPinRange );
			}
				}
			continue;
	       }

			 /*
                  if( closest != NULL ){
					  if( Marines[i]->getOrderTarget() == NULL ){
                        Marines[i]->attack(  closest );
					  }
					  else if( Marines[i]->getOrderTarget()->getID() != closest->getID() ){
		                Marines[i]->attack(  closest );
					  }
				  } else {
				  */
					  //prevent spam
				if( Marines[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Marines[i]->getOrder() != BWAPI::Orders::AttackTile && 
					Marines[i]->getOrder() != BWAPI::Orders::AttackUnit ){
						if(  closest != NULL ){
						  Marines[i]->attack(  closest->getPosition() );
						}
						if( AllEnemyDummyUnits.size() > 0){
						  Marines[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
						}
				} else {
					if( Marines[i]->getOrderTarget() != NULL ){
						if( Marines[i]->getDistance( Marines[i]->getOrderTarget() ) > 4*32 ){
							if( closest != NULL ){
						      Marines[i]->attack(  closest->getPosition() );
							}
					     }
				    }
				}				
			continue;
		}
	}



	for(unsigned int i=0; i<Medics.size(); i++){
		//std::set<Unit*> UnitsClose = Squads[0].Medics[i]->getUnitsInRadius( 10*32 );
		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Medics[i]->getDistance( close->getPosition() )  ){
				closeDist = Medics[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		if( closest == NULL){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

	  // if we can't shoot, run away  if(kite)
	  if (  ( Medics[i]->getHitPoints() < 25 )
		  ||  isUnderDS   )
	  {
		   BWAPI::Position fleePosition( (Medics[i]->getPosition() - closest->getPosition())+(Medics[i]->getPosition() - closest->getPosition()) + Medics[i]->getPosition());
		    if( Medics[i]->getOrder() != BWAPI::Orders::Move){
		       Medics[i]->move( fleePosition );
			   continue;
		    }		  
		continue;
	  }

	  //check if a unit nearby needs healing
	  Unit* needsHeal = NULL;
	  for(int j=0; j<AllOurDummyUnits.size(); j++){
			BWAPI::Unit* close = AllOurDummyUnits[j];
			if(  close->getType().isBuilding() ){
				continue;
			}
			if( close->getPlayer() == Broodwar->self() 
				&& close->getHitPoints() != close->getType().maxHitPoints()  
				&& !close->isBeingHealed()
				&& close->getType().isOrganic() ){
				needsHeal = close;
			}
		}

		if( needsHeal != NULL ){
			if( Medics[i]->getOrderTarget() == NULL ){
				Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			else if( Medics[i]->getOrderTarget()->getID() != needsHeal->getID() ){
				Medics[i]->useTech( BWAPI::TechTypes::Healing , needsHeal);
		   }
			continue;
		}

		//if there are no enemies nearby, attack;
		if( needsHeal == NULL ){ //|| closeDist > 3*32

			  Unit* closestUnit = NULL;
			  int closeDist2 = 999999;

			  for(int j=0; j<Marines.size(); j++){
			      BWAPI::Unit* close = Marines[j];
				  if( close->getType().isOrganic() ){
			         closestUnit = close;
					 break;
			         //int closeDist = close->getPosition().getDistance( InfoMan->PosEnemyBase );
				  }
		      }
			  
			  if(  closestUnit != NULL){
				  //Medics[i]->attack(  closestUnit->getPosition() );//heal move
				  Medics[i]->move(  closestUnit->getPosition() );//regular move
			  }
			continue;
		}
  
	}





	for( int i=0; i<FireBats.size(); i++){

		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if( close->getType().isFlyer() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > FireBats[i]->getDistance( close->getPosition() )  ){
				closeDist = FireBats[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		//use stim before engaging enemy
		if( closest != NULL && !FireBats[i]->isStimmed() && FireBats[i]->getHitPoints() == 50
			&& closeDist < 5*32 ){
			 FireBats[i]->useTech( BWAPI::TechTypes::Stim_Packs );
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}

		/*
		if ( isUnderDS )
	     {
		   BWAPI::Position fleePosition( (FireBats[i]->getPosition() - closest->getPosition())+(FireBats[i]->getPosition() - closest->getPosition()) + FireBats[i]->getPosition());
		    if( FireBats[i]->getOrder() != BWAPI::Orders::Move){
		       FireBats[i]->move( fleePosition );
			   continue;
		    }		   
		}
		*/

		//if the cooldown is 0, attack
		if( FireBats[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
                  if( closest != NULL ){
					  if( FireBats[i]->getOrderTarget() == NULL ){
                        FireBats[i]->attack(  closest );
					  }
					  else if( FireBats[i]->getOrderTarget()->getID() != closest->getID() ){
		                FireBats[i]->attack(  closest );
					  }
				  } else {
					  //prevent spam
					  if( FireBats[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  FireBats[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  FireBats[i]->getOrder() != BWAPI::Orders::AttackUnit ){
							  if( AllEnemyDummyUnits.size() > 0){
								  FireBats[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
							  }
					  }
				  }			
			continue;
		}
	}





	for( int i=0; i<Vultures.size(); i++){


		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if( close->getType().isFlyer() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Vultures[i]->getDistance( close->getPosition() )  ){
				closeDist = Vultures[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		if ( ( closeDist < 5*32 && Vultures[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
			||  isUnderDS )
	     {
		   BWAPI::Position fleePosition( (Vultures[i]->getPosition() - closest->getPosition())+(Vultures[i]->getPosition() - closest->getPosition()) + Vultures[i]->getPosition());
		    if( Vultures[i]->getOrder() != BWAPI::Orders::Move){
		       Vultures[i]->move( fleePosition );
			   continue;
		    }		   
		}

		//if the cooldown is 0, attack
		if( Vultures[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Vultures[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Vultures[i]->getOrder() != BWAPI::Orders::AttackTile && 
					Vultures[i]->getOrder() != BWAPI::Orders::AttackUnit ){
						if(  closest != NULL ){
						  Vultures[i]->attack(  closest->getPosition() );
						}
						if( AllEnemyDummyUnits.size() > 0){
						  Vultures[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
						}
				} else {
					if( Vultures[i]->getOrderTarget() != NULL ){
						if( Vultures[i]->getDistance( Vultures[i]->getOrderTarget() ) > 5*32 ){
							if( closest != NULL ){
						      Vultures[i]->attack(  closest->getPosition() );
							}
					     }
				    }
				}				
			continue;
		}
	}




	for( int i=0; i<Goliaths.size(); i++){


		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Goliaths[i]->getDistance( close->getPosition() )  ){
				closeDist = Goliaths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		if ( ( closeDist < 5*32 && Goliaths[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
			||  isUnderDS )
	     {
		   BWAPI::Position fleePosition( (Goliaths[i]->getPosition() - closest->getPosition())+(Goliaths[i]->getPosition() - closest->getPosition()) + Goliaths[i]->getPosition());
		    if( Goliaths[i]->getOrder() != BWAPI::Orders::Move){
		       Goliaths[i]->move( fleePosition );
			   continue;
		    }		   
		}

		//if the cooldown is 0, attack
		if( Goliaths[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Goliaths[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Goliaths[i]->getOrder() != BWAPI::Orders::AttackTile && 
					Goliaths[i]->getOrder() != BWAPI::Orders::AttackUnit ){
						if(  closest != NULL ){
						  Goliaths[i]->attack(  closest->getPosition() );
						}
						if( AllEnemyDummyUnits.size() > 0){
						  Goliaths[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
						}
				} else {
					if( Goliaths[i]->getOrderTarget() != NULL ){
						if( Goliaths[i]->getDistance( Goliaths[i]->getOrderTarget() ) > 5*32 ){
							if( closest != NULL ){
						      Goliaths[i]->attack(  closest->getPosition() );
							}
					     }
				    }
				}				
			continue;
		}
	}




	for( int i=0; i<Wraiths.size(); i++){


		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Wraiths[i]->getDistance( close->getPosition() )  ){
				closeDist = Wraiths[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		if ( ( closeDist < 5*32 && Wraiths[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
			||  isUnderDS )
	     {
		   BWAPI::Position fleePosition( (Wraiths[i]->getPosition() - closest->getPosition())+(Wraiths[i]->getPosition() - closest->getPosition()) + Wraiths[i]->getPosition());
		    if( Wraiths[i]->getOrder() != BWAPI::Orders::Move){
		       Wraiths[i]->move( fleePosition );
			   continue;
		    }		   
		}

		//if the cooldown is 0, attack
		if( Wraiths[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
				if( Wraiths[i]->getOrder() != BWAPI::Orders::AttackMove &&
					Wraiths[i]->getOrder() != BWAPI::Orders::AttackTile && 
					Wraiths[i]->getOrder() != BWAPI::Orders::AttackUnit ){
						if(  closest != NULL ){
						  Wraiths[i]->attack(  closest->getPosition() );
						}
						if( AllEnemyDummyUnits.size() > 0){
						  Wraiths[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
						}
				} else {
					if( Wraiths[i]->getOrderTarget() != NULL ){
						if( Wraiths[i]->getDistance( Wraiths[i]->getOrderTarget() ) > 5*32 ){
							if( closest != NULL ){
						      Wraiths[i]->attack(  closest->getPosition() );
							}
					     }
				    }
				}				
			continue;
		}
	}
	


	for( int i=0; i<BattleCruisers.size(); i++){


		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > BattleCruisers[i]->getDistance( close->getPosition() )  ){
				closeDist = BattleCruisers[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		if ( ( closeDist < 5*32 && BattleCruisers[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
			||  isUnderDS )
	     {
		   BWAPI::Position fleePosition( (BattleCruisers[i]->getPosition() - closest->getPosition())+(BattleCruisers[i]->getPosition() - closest->getPosition()) + BattleCruisers[i]->getPosition());
		    if( BattleCruisers[i]->getOrder() != BWAPI::Orders::Move){
		       BattleCruisers[i]->move( fleePosition );
			   continue;
		    }		   
		}

		//if the cooldown is 0, attack
		if( BattleCruisers[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
                  if( closest != NULL ){
					  if( BattleCruisers[i]->getOrderTarget() == NULL ){
                        BattleCruisers[i]->attack(  closest );
					  }
					  else if( BattleCruisers[i]->getOrderTarget()->getID() != closest->getID() ){
		                BattleCruisers[i]->attack(  closest );
					  }
				  } else {
					  //prevent spam
					  if( BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackUnit ){
							  if( AllEnemyDummyUnits.size() > 0){
								  BattleCruisers[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
							  }
					  }
				  }					
			continue;
		}
	}
	

	/*
	for( int i=0; i<Tanks.size(); i++){


		Unit* closest = NULL;
		int closeDist = 9999999;
		bool Sunken = false;
		int totSunk = 0;
		for( int j=0; j<AllEnemyDummyUnits.size(); j++)
        {
			BWAPI::Unit* close = AllEnemyDummyUnits[j];
			if( !close->isDetected() ){
				continue;
			}
			if(  close->getType().isFlyer() ){
				continue;
			}
			if(  close->getType().isBuilding() &&  close->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Overlord ){
				continue;
			}
			if(  close->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony){
				Sunken = true;
				totSunk++;
			}
			if( close->getPlayer() == Broodwar->enemy() && closeDist > Tanks[i]->getDistance( close->getPosition() )  ){
				closeDist = Tanks[i]->getDistance( close->getPosition() );
				closest = close;
			}
		}

		//no more enemies
		if( closest == NULL ){
			continue;
		}

		//leave sieged up tanks alone
		if( Tanks[i]->getType() == BWAPI::UnitTypes::Hero_Edmund_Duke_Siege_Mode
			|| Tanks[i]->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode){
			continue;
		}

		bool isUnderDS = false;
		if( closest != NULL){
			if( closest->isUnderDarkSwarm()  ){
				isUnderDS = true;
			}
		}



		if ( ( closeDist < 5*32 && BattleCruisers[i]->getGroundWeaponCooldown() != 0 && LanLatency == true ) 
			||  isUnderDS )
	     {
		   BWAPI::Position fleePosition( (BattleCruisers[i]->getPosition() - closest->getPosition())+(BattleCruisers[i]->getPosition() - closest->getPosition()) + BattleCruisers[i]->getPosition());
		    if( BattleCruisers[i]->getOrder() != BWAPI::Orders::Move){
		       BattleCruisers[i]->move( fleePosition );
			   continue;
		    }		   
		}

		//if the cooldown is 0, attack
		if( BattleCruisers[i]->getGroundWeaponCooldown() == 0  ){ //|| closeDist > 3*32
                  if( closest != NULL ){
					  if( BattleCruisers[i]->getOrderTarget() == NULL ){
                        BattleCruisers[i]->attack(  closest );
					  }
					  else if( BattleCruisers[i]->getOrderTarget()->getID() != closest->getID() ){
		                BattleCruisers[i]->attack(  closest );
					  }
				  } else {
					  //prevent spam
					  if( BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackMove &&
						  BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackTile && 
						  BattleCruisers[i]->getOrder() != BWAPI::Orders::AttackUnit ){
							  if( AllEnemyDummyUnits.size() > 0){
								  BattleCruisers[i]->attack(  AllEnemyDummyUnits[0]->getPosition() );
							  }
					  }
				  }					
			continue;
		}
	}
	*/


}






void MicroModule::StoreEnd(){

	std::string FileLoc = "bwapi-data/write/Micro/";
	//FileLoc += boost::lexical_cast<std::string>( Broodwar->self()->gas() ); //convert current level to string
	FileLoc += boost::lexical_cast<std::string>( IDlevel ); //level might be next one by now, so use stored level
	/*
	boost::filesystem::path somepath( FileLoc );
	if( !boost::filesystem::exists( somepath ) ){
      boost::filesystem::create_directories( somepath );
	}
	*/

	/*
    std::wstring stemp = std::wstring(FileLoc.begin(), FileLoc.end());
    LPCWSTR sw = stemp.c_str();

	if(!CreateDirectory( sw  ,NULL))
    {
        //return;
    }
	*/

	//FileLoc += "/";
	if( PlayingLeft ){
	FileLoc += "/Left";
	} else {
	FileLoc += "/Right";
	}

	/*
    std::wstring stemp2 = std::wstring(FileLoc.begin(), FileLoc.end());
    LPCWSTR sw2 = stemp2.c_str();

	if(!CreateDirectory( sw2  ,NULL))
    {
        //return;
    }
	*/


	/*
	FileLoc += "/0.txt";
	Broodwar->printf( FileLoc.c_str() );
	std::ofstream fout( FileLoc.c_str() );

	fout<< OurTypes.size() <<"\n";
    for( int i=0; i<OurTypes.size(); i++){
		fout<< OurTypes[i].getName() <<"\n";
		fout<< 0 <<"\n";
	}
	*/

	int CurrentScore = UnitsSavedScore - UnitsLossRound;

	//if the heuristics are now worse, update
	if( CurrentScore < score ){
		Broodwar->printf("Updating score");
		if( ID < 0){
		  Broodwar->printf("ID is negative!!!!");
		  return;
		}
		if( ID >= ScoreAllMoves.size() ){
		  Broodwar->printf("ID is tooo large!!!!");
		  return;
		}
		ScoreAllMoves[ID] = CurrentScore;
		//write the new scores to file
		std::string ScoreLoc = FileLoc + "/Score.txt";
		std::ofstream fScore( ScoreLoc.c_str() );
		if( fScore.good() ){
		  fScore<<ScoreAllMoves.size()<<"\n";
		  for( int i=0; i<ScoreAllMoves.size(); i++){
		    fScore<<ScoreAllMoves[i]<<"\n";
		  }
		  fScore.close();
		} else {
		  Broodwar->printf("Score file doesnt exist for this level");
		}
	}

	// 1/5 chance that a new heuristic is generated
	if( rand() % 3 == 0 ){ //rand() % 5 == 0
		Broodwar->printf("Generating new heuristic");
		GenerateNewHeuristic();
		std::string HeuristicLoc = FileLoc + "/";
		HeuristicLoc += boost::lexical_cast<std::string>( ScoreAllMoves.size() );
		HeuristicLoc += ".txt";
		StoreHeuristic( HeuristicLoc );

		ScoreAllMoves.push_back( 99999 );//initially high score
		std::string ScoreLoc = FileLoc + "/Score.txt";
		std::ofstream fScore( ScoreLoc.c_str() );
		if( fScore.good() ){
		fScore<<ScoreAllMoves.size()<<"\n";
		for( int i=0; i<ScoreAllMoves.size(); i++){
		  fScore<<ScoreAllMoves[i]<<"\n";
		}
		fScore.close();
		} else {
		  Broodwar->printf("Problem with New Score heuristic");
		}
	}

	OurTypes.clear();
	EnemyTypes.clear();

	AllOurDummyUnits.clear();
	AllEnemyDummyUnits.clear();

}




Actions MicroModule::GenerateAction( BWAPI::UnitType type )
{

	std::vector<int> ActionID;
	//standard actions
	for( int i=0; i<=Patrol; i++){
		ActionID.push_back(i);
	}


	if( type == BWAPI::UnitTypes::Terran_Medic){
		ActionID.push_back( Heal );
		//make heal more likely
		if( rand() % 5 == 0){
			return Heal;
		}
		//ActionID.push_back( Flare );//TODO: implement flare
		//ActionID.push_back( restoration );//TODO: implement flare
	}

	if( type == BWAPI::UnitTypes::Terran_Marine
		|| type == BWAPI::UnitTypes::Hero_Jim_Raynor_Marine
		|| type == BWAPI::UnitTypes::Hero_Gui_Montag
		|| type == BWAPI::UnitTypes::Terran_Firebat ){  //firebat and marine
		ActionID.push_back( StimPack );
	}

	//only the hero unit has siege mode
	if( type == BWAPI::UnitTypes::Hero_Edmund_Duke_Siege_Mode
		|| type == BWAPI::UnitTypes::Hero_Edmund_Duke_Tank_Mode){  //
		ActionID.push_back( Siege );
		if( rand() % 5 == 0){
			return Siege;
		}
	}


	if( type == BWAPI::UnitTypes::Terran_Vulture){  //firebat and marine
		ActionID.push_back( Place_Mine );
		if( rand() % 5 == 0){
			return Place_Mine;
		}
	}

	if( type == BWAPI::UnitTypes::Terran_Wraith){
       ActionID.push_back( Cloack );
	}

	if( type == BWAPI::UnitTypes::Terran_Ghost){
       ActionID.push_back( Cloack );
       //ActionID.push_back( LockDown );//TODO: make sure this is not used vs Zerg
	}

	if( type == BWAPI::UnitTypes::Terran_Battlecruiser){
       ActionID.push_back( Yamato );
	}

	if( type == BWAPI::UnitTypes::Terran_Science_Vessel){
       ActionID.push_back( Irradiate );
       ActionID.push_back( EMP );
       //ActionID.push_back( Def_Matrix);//TODO: decide which unit to matrix
	}


	if( type == BWAPI::UnitTypes::Zerg_Lurker){
       ActionID.push_back( Burrow );
		if( rand() % 5 == 0){
			return Burrow;
		}
	}

	if( type == BWAPI::UnitTypes::Zerg_Defiler){
       ActionID.push_back( Plaguuu );
       ActionID.push_back( Dark_Swarm );
       ActionID.push_back( Consume );

		if( rand() % 10 == 0){
			return Dark_Swarm;
		}
		if( rand() % 10 == 0){
			return Plaguuu;
		}
		if( rand() % 10 == 0){
			return Consume;
		}
	}

	if( type == BWAPI::UnitTypes::Zerg_Queen){
       ActionID.push_back( Ensnare );
       ActionID.push_back( Spawn_Broodling );

		if( rand() % 5 == 0){
			return Ensnare;
		}
		if( rand() % 10 == 0){
			return Spawn_Broodling;
		}

	}


	return static_cast<Actions>(ActionID[ rand() % ActionID.size() ]);


}

Conditionals MicroModule::GenerateCondition( BWAPI::UnitType type )
{

	std::vector<int> ActionID;
	//standard actions
	for( int i=0; i<=Closest_Burrowed_Lurker; i++){
		ActionID.push_back(i);
	}
	//dont include marine in here
	if( type == BWAPI::UnitTypes::Terran_Medic
		|| type == BWAPI::UnitTypes::Terran_Wraith
		|| type == BWAPI::UnitTypes::Terran_Science_Vessel
		|| type == BWAPI::UnitTypes::Terran_Ghost
		|| type == BWAPI::UnitTypes::Terran_Battlecruiser
		|| type == BWAPI::UnitTypes::Zerg_Queen
		|| type == BWAPI::UnitTypes::Zerg_Defiler){
		ActionID.push_back( This_Energy );
	}

	if( type == BWAPI::UnitTypes::Terran_Wraith
		|| type == BWAPI::UnitTypes::Terran_Ghost){
       ActionID.push_back( This_Cloacked );
	}

	if( type == BWAPI::UnitTypes::Terran_Vulture){
       ActionID.push_back( This_Mines );
	}

	if( type == BWAPI::UnitTypes::Zerg_Lurker){
       ActionID.push_back( This_Burrowed );
	}


	//Conditionals randCond;

	return static_cast<Conditionals>(ActionID[ rand() % ActionID.size() ]);
	//return randCond;
}



int MicroModule::ConditionValues( Conditionals condition, BWAPI::UnitType type  )
{


		if( condition == This_Mines){
		  return rand() % 3;
		} 
		if( condition == This_Energy ){
			int maxE = type.maxEnergy();
			if( maxE > 0){
			  return rand() %  type.maxEnergy();
			} else {
				return rand() % 250;
			}
		}
		if( condition == This_HP ){
			int maxHP = type.maxHitPoints();
			if( maxHP > 0){
			  return rand() %  type.maxHitPoints();
			} else {
				return rand() % 100;
			}

		}
		if( condition == This_Weapon_Cooldown ){
			int MaxCoolDown = 0;
			if( type.groundWeapon() != BWAPI::WeaponTypes::None ){
				MaxCoolDown = type.groundWeapon().damageCooldown();
			  //return rand() %  type.groundWeapon().damageCooldown();
			}
			if( type.airWeapon() != BWAPI::WeaponTypes::None ){
				MaxCoolDown = type.airWeapon().damageCooldown();
			  //return rand() %  type.airWeapon().damageCooldown();
			}
			if( MaxCoolDown > 0 ){
				return MaxCoolDown;
			}
			return rand() % 50;
		}

		if( condition == Total_Enemy_Type_Dist ){
		 return rand() % 20;
		}
		return rand() % 50;
}

IfThen MicroModule::GenerateIfThen( BWAPI::UnitType type )
{


	IfThen newIfThen;
	int TotalCond = rand() % 6;
	for(int i=0; i<TotalCond; i++){
		Condition newCond;
		newCond.condition = GenerateCondition( type );
		newCond.compare = static_cast<CompareType>( rand() % 2 );
		newCond.ValueToCompare = ConditionValues( newCond.condition, type );
		newCond.Distance =  ( rand() % 12 ) * 32;
		newCond.CompareUnitType = EnemyTypes[ rand() % EnemyTypes.size() ];

		newIfThen.Conditions.push_back( newCond );
	}

	newIfThen.ActionValue = 100 - rand() % 200; // random value between -100 and 100
	// 1/5 chance of a unit type conditional
	if( rand() % 5 == 0 ){
		newIfThen.IfThenAction = 0;
	} else {
        newIfThen.IfThenAction = 1;
	}
	//newIfThen.IfThenAction = rand() % 2;
	if( newIfThen.IfThenAction == 1){
		newIfThen.action =  static_cast<Actions>( GenerateAction( type ) ); //static_cast<Actions>( rand() % Action_Size );
	} else {
		newIfThen.ThisType = EnemyTypes[ rand() % EnemyTypes.size() ];
	}

	return newIfThen;

}

//for now,  add/remove ifThen   and randomly change conditions of 1 IfThen
void MicroModule::GenerateNewHeuristic()
{

	//TODO: genetic programming

	for( int i=0; i<GenerateMoves.size(); i++){

		// 1/5 change to remove IfThen
		if( rand() % 5 == 0 &&  GenerateMoves[i].IfStatements.size() > 1 ){
			GenerateMoves[i].IfStatements.erase( GenerateMoves[i].IfStatements.begin() + (rand() % GenerateMoves[i].IfStatements.size() ) );
		} else if( GenerateMoves[i].IfStatements.size() < 50 ) { //limit to the total IfThen statements
			//generate 3 new ifthen
			for( int j=0; j<3; j++){

			  GenerateMoves[i].IfStatements.push_back( GenerateIfThen( GenerateMoves[i].ThisType ) );
			}
		}

		/*
		// Randomly change conditionals of 1 IfThen
		if( GenerateMoves[i].IfStatements.size() > 0 ){
			int ChosenIfThen = rand() % GenerateMoves[i].IfStatements.size();
			int NewConditions = rand() % 3; //maxium 10. But 3 will do for now
			GenerateMoves[i].IfStatements[ChosenIfThen].Conditions.clear();
			for( int j=0; j<NewConditions; j++){
				Condition newCond;
				newCond.condition = static_cast<Conditionals>( rand() % Size_Cond );
				newCond.compare = static_cast<CompareType>( rand() % 2 );
				newCond.ValueToCompare = rand() % 50;
				newCond.Distance =  ( rand() % 12 ) * 32;
				newCond.CompareUnitType = EnemyTypes[ rand() % EnemyTypes.size() ];
				GenerateMoves[i].IfStatements[ChosenIfThen].Conditions.push_back( newCond );
			}
		}
		*/

	}


}



/*
//for now,  add/remove ifThen   and randomly change conditions of 1 IfThen
void MicroModule::GenerateNewHeuristic()
{

	//TODO: genetic programming

	for( int i=0; i<GenerateMoves.size(); i++){

		// 1/5 change to remove IfThen
		if( rand() % 5 == 0 &&  GenerateMoves[i].IfStatements.size() > 1 ){
			GenerateMoves[i].IfStatements.erase( GenerateMoves[i].IfStatements.begin() + (rand() % GenerateMoves[i].IfStatements.size() ) );
		} else if( GenerateMoves[i].IfStatements.size() < 50 ) { //limit to the total IfThen statements
			IfThen newIfThen;
			newIfThen.ActionValue = 100 - rand() % 200; // random value between -100 and 100
			// 1/5 chance of a unit type conditional
			if( rand() % 5 == 0 ){
				newIfThen.IfThenAction = 0;
			} else {
              newIfThen.IfThenAction = 1;
			}
			//newIfThen.IfThenAction = rand() % 2;
			if( newIfThen.IfThenAction == 1){
			  newIfThen.action =  static_cast<Actions>( rand() % (Patrol + 1) ); //static_cast<Actions>( rand() % Action_Size );
			} else {
				newIfThen.ThisType = EnemyTypes[ rand() % EnemyTypes.size() ];
			}
			GenerateMoves[i].IfStatements.push_back( newIfThen );
		}

		// Randomly change conditionals of 1 IfThen
		if( GenerateMoves[i].IfStatements.size() > 0 ){
			int ChosenIfThen = rand() % GenerateMoves[i].IfStatements.size();
			int NewConditions = rand() % 3; //maxium 10. But 3 will do for now
			GenerateMoves[i].IfStatements[ChosenIfThen].Conditions.clear();
			for( int j=0; j<NewConditions; j++){
				Condition newCond;
				newCond.condition = static_cast<Conditionals>( rand() % Size_Cond );
				newCond.compare = static_cast<CompareType>( rand() % 2 );
				newCond.ValueToCompare = rand() % 50;
				newCond.Distance =  ( rand() % 12 ) * 32;
				newCond.CompareUnitType = EnemyTypes[ rand() % EnemyTypes.size() ];
				GenerateMoves[i].IfStatements[ChosenIfThen].Conditions.push_back( newCond );
			}
		}

	}


}
*/


//Stores the current GenerateMoves to fileLoc
void MicroModule::StoreHeuristic( std::string FileLoc)
{


	std::ofstream fout( FileLoc.c_str() );
	//Broodwar->printf( FileLoc.c_str() );

	int ExtraTypes = 0;
	for( int i=0; i<OurTypes.size(); i++){
	  bool exists = false;
	  for( int j=0; j<GenerateMoves.size(); j++){
		  if( GenerateMoves[j].ThisType == OurTypes[i]){
			  exists = true;
		  }
	  }
	  if( exists == false){
		  ExtraTypes++;
	  }
	}

	ExtraTypes = GenerateMoves.size() + ExtraTypes;
	fout<<ExtraTypes <<"\n";
    for( int i=0; i<GenerateMoves.size(); i++){
		fout<< UnitTypetoInt( GenerateMoves[i].ThisType ) <<"\n";//unit tyep to int
		fout<< GenerateMoves[i].IfStatements.size() <<"\n";//unit tyep to int
		for(int j=0; j<GenerateMoves[i].IfStatements.size(); j++){
			fout<< GenerateMoves[i].IfStatements[j].Conditions.size()<<"\n";
		  for(int k=0; k<GenerateMoves[i].IfStatements[j].Conditions.size(); k++){
			//fout<< GenerateMoves[i].IfStatements[j].Conditions[k].condition<<" "; //Old integer way
			fout<< ConditionalsToStr(GenerateMoves[i].IfStatements[j].Conditions[k].condition)<<" "; //new str way
			//fout<< GenerateMoves[i].IfStatements[j].Conditions[k].compare<<" "; //Old integer way
			fout<< CompTypeToStr( GenerateMoves[i].IfStatements[j].Conditions[k].compare )<<" "; //new str way
			fout<< GenerateMoves[i].IfStatements[j].Conditions[k].ValueToCompare<<" ";
			fout<< GenerateMoves[i].IfStatements[j].Conditions[k].Distance<<" ";
			fout<< UnitTypetoInt( GenerateMoves[i].IfStatements[j].Conditions[k].CompareUnitType )<<"\n";
		  }
		  fout<< GenerateMoves[i].IfStatements[j].IfThenAction <<" ";
		  if(  GenerateMoves[i].IfStatements[j].IfThenAction == true){
		    //fout<< GenerateMoves[i].IfStatements[j].action <<" "; //Old integer way
		    fout<<  ActionToStr( GenerateMoves[i].IfStatements[j].action ) <<" "; //new str way
		  } else {
		    fout<< UnitTypetoInt( GenerateMoves[i].IfStatements[j].ThisType ) <<" ";
		  }
		  fout<< GenerateMoves[i].IfStatements[j].ActionValue <<"\n";
		}
		//fout<< 0 <<"\n";
	}

	
	for( int i=0; i<OurTypes.size(); i++){
	  bool exists = false;
	  for( int j=0; j<GenerateMoves.size(); j++){
		  if( GenerateMoves[j].ThisType.getName() == OurTypes[i].getName()){
			  exists = true;
		  }
	  }
	  if( exists == false){
		fout<< UnitTypetoInt( OurTypes[i] ) <<"\n";//unit tyep to int
		fout<< 0 <<"\n";//unit type to int
	  }
	}
	
	fout.close();

	StoreHeuristicReadable(  FileLoc);

	//OurTypes.clear();
	//EnemyTypes.clear();

}

//Stores the current GenerateMoves to a readable format
void MicroModule::StoreHeuristicReadable( std::string FileLoc)
{

	std::string newLoc = FileLoc.substr( 0, FileLoc.size() - 4);
	newLoc += "Readable.txt";

	std::ofstream fout( newLoc.c_str() );
	//Broodwar->printf( FileLoc.c_str() );

	int ExtraTypes = 0;
	for( int i=0; i<OurTypes.size(); i++){
	  bool exists = false;
	  for( int j=0; j<GenerateMoves.size(); j++){
		  if( GenerateMoves[j].ThisType == OurTypes[i]){
			  exists = true;
		  }
	  }
	  if( exists == false){
		  ExtraTypes++;
	  }
	}

	ExtraTypes = GenerateMoves.size() + ExtraTypes;
	fout<<ExtraTypes <<"\n";
    for( int i=0; i<GenerateMoves.size(); i++){
		fout<<  GenerateMoves[i].ThisType.getName() <<"\n";//unit tyep to int
		fout<< GenerateMoves[i].IfStatements.size() <<"\n";//unit tyep to int
		for(int j=0; j<GenerateMoves[i].IfStatements.size(); j++){
			fout<< GenerateMoves[i].IfStatements[j].Conditions.size()<<"\n";
		  for(int k=0; k<GenerateMoves[i].IfStatements[j].Conditions.size(); k++){
			fout<< ConditionalsToStr( GenerateMoves[i].IfStatements[j].Conditions[k].condition )<<" ";
			fout<< CompTypeToStr( GenerateMoves[i].IfStatements[j].Conditions[k].compare )<<" ";
			fout<< GenerateMoves[i].IfStatements[j].Conditions[k].ValueToCompare<<" ";
			fout<< GenerateMoves[i].IfStatements[j].Conditions[k].Distance<<" ";
			fout<<  GenerateMoves[i].IfStatements[j].Conditions[k].CompareUnitType.getName() <<"\n";
		  }
		  fout<< GenerateMoves[i].IfStatements[j].IfThenAction <<" ";
		  if(  GenerateMoves[i].IfStatements[j].IfThenAction == true){
		    fout<< ActionToStr( GenerateMoves[i].IfStatements[j].action ) <<" ";
		  } else {
			  fout<<  GenerateMoves[i].IfStatements[j].ThisType.getName() <<" ";
		  }
		  fout<< GenerateMoves[i].IfStatements[j].ActionValue <<"\n";
		}
		//fout<< 0 <<"\n";
	}

	
	for( int i=0; i<OurTypes.size(); i++){
	  bool exists = false;
	  for( int j=0; j<GenerateMoves.size(); j++){
		  if( GenerateMoves[j].ThisType.getName() == OurTypes[i].getName()){
			  exists = true;
		  }
	  }
	  if( exists == false){
		  fout<<  OurTypes[i].getName()  <<"\n";
		fout<< 0 <<"\n";//unit type to int
	  }
	}
	
	fout.close();

}


void MicroModule::NewHeuristics()
{

	//first check if there are existing heuristics
	std::string FileLoc = "bwapi-data/write/Micro/";
	IDlevel = Broodwar->self()->gas();
	FileLoc += boost::lexical_cast<std::string>( Broodwar->self()->gas() ); //convert current level to string

    std::wstring stemp = std::wstring(FileLoc.begin(), FileLoc.end());
    LPCWSTR sw = stemp.c_str();
	/*
	if(!CreateDirectory( sw  ,NULL))
    {
        //return;
    }
	*/

	//FileLoc += "/";
	if( PlayingLeft ){
	FileLoc += "/Left";
	} else {
	FileLoc += "/Right";
	}

    std::wstring stemp2 = std::wstring(FileLoc.begin(), FileLoc.end());
    LPCWSTR sw2 = stemp2.c_str();
	/*
	if(!CreateDirectory( sw2  ,NULL))
    {
        //return;
    }
	*/

	GenerateMoves.clear();//new genmoves, so clear the old ones
	ScoreAllMoves.clear();

	std::string ScoreLoc =  FileLoc + "/Score.txt";
	std::ifstream fin;
	fin.open( ScoreLoc.c_str() );
	//Broodwar->printf( ScoreLoc.c_str() );
	if( !fin.good() ){ 
		//no heuristics yet. Make a new one
		//GenerateMoves.clear();
		ID = 0;
		score = 99999; //initial score is always high
		for( int i=0; i<OurTypes.size(); i++){
			GenMoves newGenMove;
			newGenMove.ThisType = OurTypes[i];
			newGenMove.ToAttack = BWAPI::UnitTypes::AllUnits;
			IfThen attackThen; //only attack. No conditions
			attackThen.Conditions.clear();
			attackThen.action = Attack_Closest;
			attackThen.ActionValue = 1;
			attackThen.IfThenAction = true;
			newGenMove.IfStatements.push_back( attackThen );
		    GenerateMoves.push_back( newGenMove );

		}
		fin.close();
	   std::ofstream fout( ScoreLoc.c_str() );
	   fout<<1<<"\n";
	   fout<<score;
	   fout.close();
	   ScoreAllMoves.push_back(score);
	   std::string HeuristicLoc = FileLoc + "/0.txt";
	   StoreHeuristic(  HeuristicLoc );
	} else {

		//GenerateMoves.clear();

		int TotalScores;
		int BestScore = -9999999;
		int BestID = -1;
		fin>>TotalScores;
		for(int i=0; i<TotalScores; i++){
			int TheScore;
			fin>>TheScore;
			ScoreAllMoves.push_back( TheScore );
			if( BestScore < TheScore){
				BestScore = TheScore;
				BestID = i;
			}
		}

		if( BestID == -1){
			Broodwar->printf("No best ID selected");

		} else {
			score = BestScore;
			ID = BestID;
			//grab the corresponding heuristic file
			FileLoc += "/";
			FileLoc += boost::lexical_cast<std::string>( BestID );  //static_cast<Actions>( BestID );
			FileLoc += ".txt";
			LoadHeuristic(  FileLoc );
		}

	}
	fin.close();

}

void MicroModule::LoadHeuristic( std::string FileLoc){

	std::ifstream fin( FileLoc.c_str() );
    //Broodwar->printf("loading Heuristic");
    Broodwar->printf( FileLoc.c_str() );
	//now load in the heuristics

	if( !fin.good() ){
		Broodwar->printf("Heuristic cannot be loaded");
	}

	int TotalHeuristic;
	fin>>TotalHeuristic;
	//Broodwar->printf("Total Heuristic: %d %d", TotalHeuristic,GenerateMoves.size());
	for(int i=0; i<TotalHeuristic; i++){
	   GenMoves newGenMove;
	   int IntUnitType;
	   fin>>IntUnitType;
	   newGenMove.ThisType = IntToUnitType( IntUnitType );
	   int IfThenTotal;
	   fin>>IfThenTotal;
	  for(int j=0; j<IfThenTotal; j++){
		  IfThen newIfThen;
		  int TotalConditions;
	      fin>>TotalConditions;
	      for(int k=0; k<TotalConditions; k++){
			  Condition newCondition;
			  //int newcondition;
			  std::string newcondition;
			  fin>>newcondition;//the condition
			  //newCondition.condition = static_cast<Conditionals>(newcondition);
			  newCondition.condition = StrToConditionals(newcondition);
			  //int compType;
			  std::string compType;
			  fin>>compType; //compare type:  <,>,==,!=
			  //newCondition.compare = static_cast<CompareType>(compType);
			  newCondition.compare = StrToCompType(compType);
			  fin>>newCondition.ValueToCompare;
			  fin>>newCondition.Distance;
			  int UnitTypeCompare;
			  fin>>UnitTypeCompare;
			  newCondition.CompareUnitType = IntToUnitType( UnitTypeCompare );

			  newIfThen.Conditions.push_back( newCondition );
		  }

		  int IfThenIO;

		  //fin>>newIfThen.IfThenAction;
		  fin>>IfThenIO;
		  if( IfThenIO == 1 ){
			newIfThen.IfThenAction = true;
		    //int ifthenAction;
			std::string ifthenAction;
		    fin>>ifthenAction;
			//newIfThen.action =  static_cast<Actions>(ifthenAction);
			newIfThen.action = StrToAction(ifthenAction);
		  } else {
			newIfThen.IfThenAction = false;
		    int ifthenType;
		    fin>>ifthenType;
			newIfThen.ThisType =  IntToUnitType(ifthenType); 
		  }
		  fin>>newIfThen.ActionValue;

		  newGenMove.IfStatements.push_back( newIfThen );
	  }

	  GenerateMoves.push_back(newGenMove);
	}

	//Broodwar->printf("Total Heuristic: %d %d", TotalHeuristic,GenerateMoves.size());

	return;
}

void MicroModule::OnFrame(){

	RemoveDead();


	/*
	for( int x=0; x<Broodwar->mapWidth(); x++){
		for( int y=0; y<Broodwar->mapHeight(); y++){
			if( Walkable[x][y] == 1 ){
				Broodwar->drawCircleMap(x*32,y*32,2,BWAPI::Colors::Green,true );
			}
		}
	}
	*/


	for( int i=0; i<OurBuildings.size(); i++){
		if( OurBuildings[i]->getType() == BWAPI::UnitTypes::Terran_Comsat_Station ){
			for( int j=0; j<AllEnemyDummyUnits.size(); j++){
				if( !AllEnemyDummyUnits[j]->isDetected() 
					&& Broodwar->getFrameCount() % 24 == 0 ){
					OurBuildings[i]->useTech( BWAPI::TechTypes::Scanner_Sweep , AllEnemyDummyUnits[j]->getPosition() );
					break;
				}
	      }
		}
	}

	for( int i=0; i<AllOurDummyUnits.size(); i++){

		CheckAction( AllOurDummyUnits[i] );

		/*
		//if unit is not attacking, tell it to attack
		if( AllOurDummyUnits[i]->getOrder() != BWAPI::Orders::AttackMove
			&& AllOurDummyUnits[i]->getOrder() != BWAPI::Orders::AttackUnit
			&& AllOurDummyUnits[i]->getOrder() != BWAPI::Orders::AttackTile	){
		    
				if( AllEnemyDummyUnits.size() > 0){
					AllOurDummyUnits[i]->attack( AllEnemyDummyUnits[0]->getPosition() );
				}

		}
		*/

	}

	//NewRound == true
	if( NewRound == true && StoreScore == false
		&& AllEnemyDummyUnits.size() > 0
		&& AllOurDummyUnits.size() > 0){
			StoreScore = true;
			NewRound = false;
			PlayingLeft = true;
			if( AllEnemyDummyUnits[0]->getPosition().x() < 760 ){
				PlayingLeft = false;
			}
			NewHeuristics();
	}

	if( StoreScore && AllEnemyDummyUnits.size() == 0){
		UnitsSavedScore = AllOurDummyUnits.size();
		UnitsLossRound = 0;
		StoreScore = false;
		StoreEnd();
	}
	if( StoreScore && AllOurDummyUnits.size() == 0){
		UnitsLossRound = AllEnemyDummyUnits.size();
		UnitsSavedScore = 0;
		StoreScore = false;
		StoreEnd();
	}


	//get correct level micro
	int thisLevel = Broodwar->self()->gas();
	Broodwar->drawTextScreen( 300,200,"level: %d",thisLevel);
	Broodwar->drawTextScreen( 300,210,"AllEnemyDummyUnits: %d",AllEnemyDummyUnits.size() );
	Broodwar->drawTextScreen( 300,220,"AllOurDummyUnits: %d",AllOurDummyUnits.size() );
	Broodwar->drawTextScreen( 300,230,"Previous Win: %d",UnitsSavedScore);
	Broodwar->drawTextScreen( 300,240,"Previous Loss: %d",UnitsLossRound);
	if( PlayingLeft ){
	  Broodwar->drawTextScreen( 300,250,"Playing Left");
	} else {
	  Broodwar->drawTextScreen( 300,250,"Playing Right");
	}
	Broodwar->drawTextScreen( 200,260,"Top Left Screen loc: %d %d",Broodwar->getScreenPosition().x(),Broodwar->getScreenPosition().y());

	//PutMicroHere();

}

void MicroModule::AddUnit( Unit* unit ){

	if( unit->getType() == BWAPI::UnitTypes::Hero_Samir_Duran){
		//StoreScore = false;
		NewRound = true;
	}


	if( unit->getPlayer() == Broodwar->self() ){

		if( unit->getPosition().x() < 1560 
			&&  unit->getPosition().x() > 1
			&& unit->getPosition().y() > 3600 
			&& unit->getPosition().y() < 4728 ){

				if( unit->getType().getRace() == BWAPI::Races::Terran
					|| unit->getType().getRace() == BWAPI::Races::Protoss
					|| unit->getType().getRace() == BWAPI::Races::Zerg){

				  if( unit->getType().isBuilding()
					  && unit->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
					OurBuildings.push_back(unit);
					return;
				  }

				  AllOurDummyUnits.push_back( unit );

				    bool included = false;
				    for( int j=0; j<OurTypes.size(); j++){
						if( OurTypes[j].getName() == unit->getType().getName() ){
							included = true;
						}
				    }
					if( !included ){
						OurTypes.push_back( unit->getType() );
					}
				}
				
				if( unit->getType() == BWAPI::UnitTypes::Terran_SCV ){
					SCVs.push_back(unit);
					return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Marine ){
						Marines.push_back(unit);
						//Broodwar->printf( "%d", Marines.size() );
						return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Hero_Jim_Raynor_Marine ){
						MarinesStimRange.push_back(unit);
						//Broodwar->printf( "%d", Marines.size() );
						return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Medic ){
					Medics.push_back(unit);
					return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Firebat ){
					FireBats.push_back(unit);
					return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Ghost ){
					//Ghosts.push_back(unit);
					//return;
				}
				// || unit->getType() == BWAPI::UnitTypes::Hero_Jim_Raynor_Vulture
				if( unit->getType() == BWAPI::UnitTypes::Terran_Vulture){
					Vultures.push_back(unit);
					return;
				}
				if(  unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode
					|| unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode){
					Tanks.push_back(unit);
					//return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Hero_Edmund_Duke_Siege_Mode
					|| unit->getType() == BWAPI::UnitTypes::Hero_Edmund_Duke_Tank_Mode){
					TanksSiege.push_back(unit);
					//return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Goliath){
					Goliaths.push_back(unit);
					return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Wraith){
					Wraiths.push_back(unit);
					return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Dropship){
					//No special micro yet
					//Dropships.push_back(unit);
					//return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel){
					//No special micro yet
					ScienceVessels.push_back(unit);
					//return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Valkyrie){
					//No special micro yet
					Valkyries.push_back(unit);
					//return;
				}
				if( unit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser){
					//No special micro yet
					BattleCruisers.push_back(unit);
					//return;
				}

				//AllOurDummyUnits.push_back( unit );
		}

	}

	if( unit->getPlayer() == Broodwar->enemy() ){

		if( unit->getPosition().x() < 1560 
			&&  unit->getPosition().x() > 1
			&& unit->getPosition().y() > 3600 
			&& unit->getPosition().y() < 4728 ){

				if( unit->getType().isBuilding() 
					&& unit->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony ){
					EnemyBuildings.push_back(unit);
					return;
				}

				AllEnemyDummyUnits.push_back( unit );

				    bool included = false;
				    for( int j=0; j<EnemyTypes.size(); j++){
						if( EnemyTypes[j] == unit->getType() ){
							included = true;
						}
				    }
					if( !included ){
						EnemyTypes.push_back( unit->getType() );
					}

		}

	}

}
