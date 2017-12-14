#pragma once

#include <BWAPI.h>
#include <vector>

using namespace BWAPI;


enum WraithState{
	IdleWraith,
	Moving,
	Attacking
};

struct WraithStruct{
	
	Unit* Wraith;
	WraithState currentState;
	BWAPI::Position MovingTo;
	BWAPI::Position CurrentPosition;
	int stopped_frames;//total amount of frames in which the wraith has the same position
};


class WraithMicro{
public:
	std::vector< WraithStruct > Wraiths;
	WraithMicro( );
	void MoveWraits();
	void AddWraith( Unit* unit);
};