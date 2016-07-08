
#include "LetaBotModule.h"

#include <vector>
#include <boost/foreach.hpp>

using namespace BWAPI;

BWAPI::AIModule * __NewAIModule()
{
	return new LetaBotModule();
}
 
LetaBotModule::LetaBotModule()  {}
LetaBotModule::~LetaBotModule() {}


#include <fstream>
#include <string>

#define max(a,b)            (((a) > (b)) ? (a) : (b))

enum SCVSTATE{
  MovingToMineral,
  GatheringMineral,
  ReturningMineral,
  ExtraMoveToMineral, //used for the path trick
  ExtraMoveToPos,
  NoTrick //used for the path trick
};

bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;

BWAPI::Position CCtrick;//path finding trick with CC
Unit* ourCC;
Unit* closestMineral;
std::ofstream fout;

//SCV spawns at 3728, 456

int TotalSCV = 4;
int BuildDepotAt = 8;
const int Mining_Time = 80;

//BuildIn: 3922,3930
//EvenSplit: 4458,4458,4466,4490,4474,4458
//Queue: 4522,4538,4522,4530,4506,4546

enum Algorithm{
  BuildIn,
  EvenSplit,
  Queue,
  EvenSplitPath,
  QueuePath,
  Search
};

Algorithm CurrentAlg = BuildIn;
//bool BeginSplit = false; //To prevent onUnitcreate from controlling our units


struct MoveData{

	int Startx;
	int Starty;
	int Endx;
	int Endy;
	bool MovingToMineral;
	bool set;//Check if this should be recorded
	int MineralX;
	int MineralY;
	int StartTime;//Just for calculation of totalFrames
	int TotalFrames;
};


class SCV{
public:
  Unit* scv;
  SCVSTATE SCVstate;
  Unit* mineralPatch;
  int StartedMoving; //frame in which the scv started moving
  int StartedMining; //frame in which the scv started mining
  MoveData SCVMov;

  BWAPI::Position Intermidiate;
  Unit* interMineral;

  int ExpectedBackBase;
  int ExpectedArrivalBase;
  int ExpectedStartMining;

  SCV( Unit* s, Unit* mineral ){
  scv = s;
  SCVSTATE SCVstate = MovingToMineral;
  mineralPatch = mineral;
  StartedMoving=0;
  StartedMining=0;
  }

};
std::vector<SCV> SCVs;



struct Mineral{
  int MinToCC;
  int CCToMin;
  int MinX;
  int MinY;
  int ID;
  Unit* miner;
  int SCVcount;
  std::vector<SCV*> Queue;
  Unit* mineralTrick;//mineral which allows for mineral trick to speed up SCV
  BWAPI::Position posTrick;//Position to allow path finding trick
  int Facing; //0 = scv enters left, 1 = scv enters up
};
std::vector<Mineral> Minerals;
std::vector<MoveData> MoveDatas;

void SplitWorkersClosest(){
    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
        (*i)->rightClick(closestMineral);
	  }  
	  else if ((*i)->getType().isResourceDepot())
      {
        (*i)->train(Broodwar->self()->getRace().getWorker());
	  }
	}

  return;

}


int MaxFramesS = 500;//Maximum frames tree search depth

int BestMineral(int SCVindex, int depth){
 return 0;
}

void SearchChoice(Unit* unit){

	int GetIndex = -1;
	//Check if we already have this SCV in storage
	bool exists = false;
	for(unsigned int i=0; i<SCVs.size(); i++){
		if( SCVs[i].scv->getID() == unit->getID() ){
          exists = true;
		}
	}
	if( !exists ){//store it
         SCV newSCV = SCV( unit ,Minerals[0].miner);
         SCVs.push_back(newSCV);
	}
    //Get the index of the scv
	for(unsigned int i=0; i<SCVs.size(); i++){
		if( SCVs[i].scv->getID() == unit->getID() ){
          GetIndex = i;
		}
	}
}


int getIDmineral(int minID){
	for(int i=0; i<Minerals.size(); i++){
		if(  Minerals[i].miner->getID() == minID){
			return i;
		}
	}
	return -1;
}

void QueuePathChoice(Unit* unit){

	int Fastest = 99999999;
	int choiceJ = 0;
	for(unsigned int j=0; j<Minerals.size();j++){
      int TotWork = 0;
	  /*
	  for(unsigned int k=0; k<Minerals[j].Queue.size(); k++){
		  int travel = max( 0,  (Broodwar->getFrameCount()-Minerals[j].Queue[k]->StartedMoving) - TotWork );
          int miningtime = Mining_Time;
		  if(  Minerals[j].Queue[k]->StartedMining > Minerals[j].Queue[k]->StartedMoving ){//already started mining
            miningtime =  Mining_Time - (Broodwar->getFrameCount() - Minerals[j].Queue[k]->StartedMining);
		  }
          TotWork += travel + miningtime;
	  }
	  */
	  //int MyWork = max( 0,Minerals[j].CCToMin - TotWork ) + TotWork + Mining_Time + Minerals[j].MinToCC;
	  int ToMineral = 0;
	  int ToMstored = 0;
	  int ToCC = 0;
	  int ToCCstored = 0;
	  int MineralLocX = 0;
	  int MineralLocY = 0;

      //First find the spot in which we will end
	  for(unsigned int m=0; m<MoveDatas.size(); m++){
		  if(  MoveDatas[m].MineralX == Minerals[j].miner->getPosition().x() &&
			   MoveDatas[m].MineralY == Minerals[j].miner->getPosition().y() &&
			   MoveDatas[m].Startx == unit->getPosition().x() &&
			   MoveDatas[m].Starty == unit->getPosition().y() &&
			   MoveDatas[m].MovingToMineral == true){
              MineralLocX  = MoveDatas[m].Endx;
              MineralLocY  = MoveDatas[m].Endy;
			  ToMineral += MoveDatas[m].TotalFrames;
			  ToMstored++;
		  }
	  }
	  if( ToMstored != 0 ){
	    ToMineral = (int)(ToMineral/ToMstored);
	  } else {//not enough data
        ToMineral = Minerals[j].CCToMin;
		  //ToMineral = 0;
	  }
      //Now find the distance from the mineral back to the command center
	  for(unsigned int m=0; m<MoveDatas.size(); m++){
		  if(  MoveDatas[m].MineralX == Minerals[j].miner->getPosition().x() &&
			   MoveDatas[m].MineralY == Minerals[j].miner->getPosition().y() &&
			   //MoveDatas[m].Startx == MineralLocX &&
			   //MoveDatas[m].Starty == MineralLocY &&
			   MoveDatas[m].MovingToMineral == false){
               ToCC += MoveDatas[m].TotalFrames;
			   ToCCstored++;
		  }
	  }
	  if( ToCCstored != 0  ){
      ToCC = (int)(ToCC/ToCCstored);
	  } else {//not enoguh data
      ToCC = Minerals[j].MinToCC;
		 //ToCC= 0;
	  }


       //Calculate work of workers in queue
	  for(unsigned int k=0; k<Minerals[j].Queue.size(); k++){
		  int travel = max( 0,  ( ToMineral - (Broodwar->getFrameCount()-Minerals[j].Queue[k]->StartedMoving) ) - TotWork );
          int miningtime = Mining_Time;
		  if(  Minerals[j].Queue[k]->StartedMining > Minerals[j].Queue[k]->StartedMoving ){//already started mining
            miningtime =  Mining_Time - (Broodwar->getFrameCount() - Minerals[j].Queue[k]->StartedMining);
		  }
          TotWork += travel + miningtime;
	  }

	  int MyWork = max( 0,ToMineral - TotWork ) + TotWork + Mining_Time + ToCC;
	  if( Fastest > MyWork ){
        Fastest = MyWork;
        choiceJ = j;
	  }
	}

	//Check if we already have this SCV in storage
	bool exists = false;
	for(unsigned int i=0; i<SCVs.size(); i++){
		if( SCVs[i].scv->getID() == unit->getID() ){
          exists = true;
		}
	}

	if( !exists ){//store it
         SCV newSCV = SCV( unit ,Minerals[choiceJ].miner);
         SCVs.push_back(newSCV);
	}
    //TODO: now update mineral queue
	for(unsigned int i=0; i<SCVs.size(); i++){
		if( SCVs[i].scv->getID() == unit->getID() ){
		 if(  Minerals[choiceJ].mineralTrick == NULL && Minerals[choiceJ].posTrick == BWAPI::Positions::None ){
		  SCVs[i].SCVstate = MovingToMineral;
		  SCVs[i].interMineral = NULL;
		  SCVs[i].Intermidiate = BWAPI::Positions::None;
		  SCVs[i].StartedMoving = Broodwar->getFrameCount();
		  SCVs[i].mineralPatch = Minerals[choiceJ].miner;
		  SCVs[i].scv->gather(Minerals[choiceJ].miner);
		  Minerals[choiceJ].Queue.push_back( &SCVs[i] ); //Add this worker to the queue
		  //mineral has mineral trick
			} else if( Minerals[choiceJ].posTrick != BWAPI::Positions::None ){
				Broodwar->printf("Setting SCV to mineral trick");
				SCVs[i].SCVstate = ExtraMoveToPos;
		    SCVs[i].StartedMoving = Broodwar->getFrameCount();
		    SCVs[i].mineralPatch = Minerals[choiceJ].miner;
		    SCVs[i].Intermidiate = Minerals[choiceJ].posTrick;
			SCVs[i].interMineral = NULL;
		    SCVs[i].scv->move( Minerals[choiceJ].posTrick );
		    Minerals[choiceJ].Queue.push_back( &SCVs[i] ); //Add this worker to the queue
			} else {
				SCVs[i].SCVstate = ExtraMoveToMineral;
		  SCVs[i].StartedMoving = Broodwar->getFrameCount();
		  SCVs[i].mineralPatch = Minerals[choiceJ].miner;
		  SCVs[i].interMineral = Minerals[choiceJ].mineralTrick;
		  SCVs[i].Intermidiate = BWAPI::Positions::None;
		  SCVs[i].scv->gather(Minerals[choiceJ].mineralTrick);
		  Minerals[choiceJ].Queue.push_back( &SCVs[i] ); //Add this worker to the queue
			}
		  Broodwar->printf("choiceQueue:%d",choiceJ);
		  Broodwar->printf("Total Work:%d",Fastest);
		  //Record of moving data
		  SCVs[i].SCVMov.set=true;
		  SCVs[i].SCVMov.Startx=SCVs[i].scv->getPosition().x();
		  SCVs[i].SCVMov.Starty=SCVs[i].scv->getPosition().y();
          SCVs[i].SCVMov.StartTime=Broodwar->getFrameCount();
          SCVs[i].SCVMov.MovingToMineral = true;
		  SCVs[i].SCVMov.MineralX=SCVs[i].mineralPatch->getPosition().x();
          SCVs[i].SCVMov.MineralY=SCVs[i].mineralPatch->getPosition().y();

		}
	}

}


void QueueChoice(Unit* unit){

	int Fastest = 99999999;
	int choiceJ = 0;
	for(unsigned int j=0; j<Minerals.size();j++){
      int TotWork = 0;
	  /*
	  for(unsigned int k=0; k<Minerals[j].Queue.size(); k++){
		  int travel = max( 0,  (Broodwar->getFrameCount()-Minerals[j].Queue[k]->StartedMoving) - TotWork );
          int miningtime = Mining_Time;
		  if(  Minerals[j].Queue[k]->StartedMining > Minerals[j].Queue[k]->StartedMoving ){//already started mining
            miningtime =  Mining_Time - (Broodwar->getFrameCount() - Minerals[j].Queue[k]->StartedMining);
		  }
          TotWork += travel + miningtime;
	  }
	  */
	  //int MyWork = max( 0,Minerals[j].CCToMin - TotWork ) + TotWork + Mining_Time + Minerals[j].MinToCC;
	  int ToMineral = 0;
	  int ToMstored = 0;
	  int ToCC = 0;
	  int ToCCstored = 0;
	  int MineralLocX = 0;
	  int MineralLocY = 0;

      //First find the spot in which we will end
	  for(unsigned int m=0; m<MoveDatas.size(); m++){
		  if(  MoveDatas[m].MineralX == Minerals[j].miner->getPosition().x() &&
			   MoveDatas[m].MineralY == Minerals[j].miner->getPosition().y() &&
			   MoveDatas[m].Startx == unit->getPosition().x() &&
			   MoveDatas[m].Starty == unit->getPosition().y() &&
			   MoveDatas[m].MovingToMineral == true){
              MineralLocX  = MoveDatas[m].Endx;
              MineralLocY  = MoveDatas[m].Endy;
			  ToMineral += MoveDatas[m].TotalFrames;
			  ToMstored++;
		  }
	  }
	  if( ToMstored != 0 ){
	    ToMineral = (int)(ToMineral/ToMstored);
	  } else {//not enoguh data
        ToMineral = Minerals[j].CCToMin;
	  }
      //Now find the distance from the mineral back to the command center
	  for(unsigned int m=0; m<MoveDatas.size(); m++){
		  if(  MoveDatas[m].MineralX == Minerals[j].miner->getPosition().x() &&
			   MoveDatas[m].MineralY == Minerals[j].miner->getPosition().y() &&
			   MoveDatas[m].Startx == MineralLocX &&
			   MoveDatas[m].Starty == MineralLocY &&
			   MoveDatas[m].MovingToMineral == false){
               ToCC += MoveDatas[m].TotalFrames;
			   ToCCstored++;
		  }
	  }
	  if( ToCCstored != 0  ){
      ToCC = (int)(ToCC/ToCCstored);
	  } else {//not enoguh data
      ToCC = Minerals[j].MinToCC;
	  }


       //Calculate work of workers in queue
	  for(unsigned int k=0; k<Minerals[j].Queue.size(); k++){
		  int travel = max( 0,  ( ToMineral - (Broodwar->getFrameCount()-Minerals[j].Queue[k]->StartedMoving) ) - TotWork );
          int miningtime = Mining_Time;
		  if(  Minerals[j].Queue[k]->StartedMining > Minerals[j].Queue[k]->StartedMoving ){//already started mining
            miningtime =  Mining_Time - (Broodwar->getFrameCount() - Minerals[j].Queue[k]->StartedMining);
		  }
          TotWork += travel + miningtime;
	  }

	  int MyWork = max( 0,ToMineral - TotWork ) + TotWork + Mining_Time + ToCC;
	  if( Fastest > MyWork ){
        Fastest = MyWork;
        choiceJ = j;
	  }
	}

	//Check if we already have this SCV in storage
	bool exists = false;
	for(unsigned int i=0; i<SCVs.size(); i++){
		if( SCVs[i].scv->getID() == unit->getID() ){
          exists = true;
		}
	}

	if( !exists ){//store it
         SCV newSCV = SCV( unit ,Minerals[choiceJ].miner);
         SCVs.push_back(newSCV);
	}
    //TODO: now update mineral queue
	for(unsigned int i=0; i<SCVs.size(); i++){
		if( SCVs[i].scv->getID() == unit->getID() ){
		  SCVs[i].SCVstate = MovingToMineral;
		  SCVs[i].StartedMoving = Broodwar->getFrameCount();
		  SCVs[i].mineralPatch = Minerals[choiceJ].miner;
		  SCVs[i].scv->gather(Minerals[choiceJ].miner);
		  Minerals[choiceJ].Queue.push_back( &SCVs[i] ); //Add this worker to the queue
		  Broodwar->printf("choiceQueue:%d",choiceJ);
		  Broodwar->printf("Total Work:%d",Fastest);
		  //Record of moving data
		  SCVs[i].SCVMov.set=true;
		  SCVs[i].SCVMov.Startx=SCVs[i].scv->getPosition().x();
		  SCVs[i].SCVMov.Starty=SCVs[i].scv->getPosition().y();
          SCVs[i].SCVMov.StartTime=Broodwar->getFrameCount();
          SCVs[i].SCVMov.MovingToMineral = true;
		  SCVs[i].SCVMov.MineralX=SCVs[i].mineralPatch->getPosition().x();
          SCVs[i].SCVMov.MineralY=SCVs[i].mineralPatch->getPosition().y();

		}
	}

}



//Sp;lit based on Path Queue
void SplitWorkersPathQueue(){

    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
		  	QueuePathChoice((*i));
	  }  
	}
	for(unsigned int k=0; k<Minerals.size(); k++){
		if( Minerals[k].Queue.size() > 0 ){
			//Broodwar->printf(" Queue front %d",  Minerals[k].Queue[0]->scv->getID() );
		}
	}

  return;
}

//Sp;lit based on Queue
void SplitWorkersQueue(){

    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
		  	QueueChoice((*i));
	  }  
	}
	for(unsigned int k=0; k<Minerals.size(); k++){
		if( Minerals[k].Queue.size() > 0 ){
			//Broodwar->printf(" Queue front %d",  Minerals[k].Queue[0]->scv->getID() );
		}
	}

  return;
}



void AssignEvenSplit(Unit* unit){


			int lowSCV = 10;
			for( unsigned int j=0; j<Minerals.size(); j++ ){
				if(  lowSCV > Minerals[j].SCVcount ){
                  lowSCV = Minerals[j].SCVcount;
				}
			}
			//Broodwar->printf(";lowSCV:%d",lowSCV);
		  int minMin = 9999;
		  int closMin = 0;
		  for(unsigned int j=0; j<Minerals.size(); j++){

			  /*   
			 //minerals with mineral trick only need 2 minerals max
			 //only works if you use the mineral trick
			 if(  Minerals[j].mineralTrick != NULL &&  Minerals[j].SCVcount >= 2 ){
				 continue;
			 }
			 */

			  if( (minMin > (Minerals[j].CCToMin + Minerals[j].MinToCC) ) && (Minerals[j].SCVcount == lowSCV) ){
                minMin = Minerals[j].CCToMin + Minerals[j].MinToCC;
                closMin = j;
			  }
		  }
		  unit->gather(Minerals[closMin].miner);
         SCV newSCV = SCV( unit ,Minerals[closMin].miner);
         SCVs.push_back(newSCV);
		 Minerals[closMin].SCVcount++;
		 Broodwar->printf("choice3:%d",closMin);

}

//A so called perfect split. After that maintain the amount of workers per patch
void SplitWorkersEven(){
  //for(unsigned int j=0; j<Minerals.size(); j++){
   // Broodwar->printf("minMin:%d",Minerals[j].CCToMin);
   // Broodwar->printf("minMin:%d",Minerals[j].MinToCC);
 // }

    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
		  	AssignEvenSplit((*i));
	  }  
	}
	  //for(unsigned int j=0; j<Minerals.size(); j++){
       //Broodwar->printf("SCV count:%d",Minerals[j].SCVcount);
      //}
  return;
}


//calculate the move position for the pathfinding trick
BWAPI::Position GetTrickPath( Unit* mineral ){

	BWAPI::Position result = BWAPI::Position( mineral->getPosition().x()-16 , mineral->getPosition().y() - 40 );
	//mineral->getPosition()
    return result;
}

void AssignEvenSplitPath(Unit* unit){

		 int lowSCV = 10;
		 for( unsigned int j=0; j<Minerals.size(); j++ ){
		   if(  lowSCV > Minerals[j].SCVcount ){
              lowSCV = Minerals[j].SCVcount;
		   }
		 }
		 //Broodwar->printf(";lowSCV:%d",lowSCV);
		 int minMin = 9999;
		 int closMin = 0;
		 for(unsigned int j=0; j<Minerals.size(); j++){
			 //minerals with mineral trick only need 2 minerals max
			 if(  Minerals[j].mineralTrick != NULL &&  Minerals[j].SCVcount >= 2 ){
				 continue;
			 }
			if( (minMin > (Minerals[j].CCToMin + Minerals[j].MinToCC) ) && (Minerals[j].SCVcount == lowSCV) ){
              minMin = Minerals[j].CCToMin + Minerals[j].MinToCC;
              closMin = j;
			}
		 }

		 //unit->gather(Minerals[closMin].miner);
		 
		 if(  Minerals[closMin].mineralTrick != NULL ){
			 unit->gather( Minerals[closMin].mineralTrick );
           SCV newSCV = SCV( unit ,Minerals[closMin].miner);
           SCVs.push_back(newSCV);
		   SCVs[ SCVs.size()-1 ].SCVstate = ExtraMoveToMineral;
		   SCVs[ SCVs.size()-1 ].interMineral = Minerals[closMin].mineralTrick;
		   SCVs[ SCVs.size()-1 ].Intermidiate = BWAPI::Positions::None;
		 } else if( Minerals[closMin].posTrick != BWAPI::Positions::None ){

		   unit->move( Minerals[closMin].posTrick );
           SCV newSCV = SCV( unit ,Minerals[closMin].miner);
           SCVs.push_back(newSCV);
		   SCVs[ SCVs.size()-1 ].Intermidiate = Minerals[closMin].posTrick;
		   SCVs[ SCVs.size()-1 ].SCVstate = ExtraMoveToPos;

		 } else {
		   unit->gather( Minerals[closMin].miner );
           SCV newSCV = SCV( unit ,Minerals[closMin].miner);
           SCVs.push_back(newSCV);
		   //SCVs[ SCVs.size()-1 ].Intermidiate = GetTrickPath( Minerals[closMin].miner );
		   SCVs[ SCVs.size()-1 ].SCVstate = NoTrick;
		   SCVs[ SCVs.size()-1 ].Intermidiate = BWAPI::Positions::None;
		 }

		 Minerals[closMin].SCVcount++;
		 Broodwar->printf("choice3:%d",closMin);

}

//A so called perfect split. After that maintain the amount of workers per patch
void SplitWorkersEvenPath(){
  //for(unsigned int j=0; j<Minerals.size(); j++){
   // Broodwar->printf("minMin:%d",Minerals[j].CCToMin);
   // Broodwar->printf("minMin:%d",Minerals[j].MinToCC);
 // }

    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
		  	AssignEvenSplitPath((*i));
	  }  
	}
	  //for(unsigned int j=0; j<Minerals.size(); j++){
       //Broodwar->printf("SCV count:%d",Minerals[j].SCVcount);
      //}
  return;
}



void ReadMoveData(){
	std::ifstream MovDat;
	if(  CurrentAlg == Queue ){
		MovDat.open("bwapi-data/read/moveDataQueue2.txt");
     } else {
		MovDat.open("bwapi-data/write/moveData.txt");
	}
	if( MovDat.fail() ){
     Broodwar->printf("Could not open Movement data");
	 return;
	}
	std::string getInput;
     int totalM;
	 MovDat>>totalM;
	 for(int i=0; i<totalM; i++){
	   MoveData newMoveData; 
       MovDat>>getInput>>newMoveData.MineralX;
       MovDat>>getInput>>newMoveData.MineralY;
       MovDat>>getInput>>newMoveData.Startx;
       MovDat>>getInput>>newMoveData.Starty;
       MovDat>>getInput>>newMoveData.Endx;
       MovDat>>getInput>>newMoveData.Endy;
       MovDat>>getInput;
	   if( getInput == "ToMineral" ){
		   newMoveData.MovingToMineral = true;
	   } else {
		   newMoveData.MovingToMineral = false;
	   }
       MovDat>>getInput>>newMoveData.TotalFrames;
       MoveDatas.push_back(newMoveData);
	 }
}

void ChooseAlg(){
	std::ifstream AlgDat("bwapi-data/read/MineralALG.txt");
	int choice = 0;
	if( AlgDat.fail() ){
     Broodwar->printf("Could not open algorithm data");
	 return;
	}
	AlgDat>>choice;
	if( choice == 0 ){
		CurrentAlg = BuildIn;
	}
	if( choice == 1 ){
		CurrentAlg = EvenSplit;
	}
	if( choice == 2 ){
		CurrentAlg = Queue;
	}
	if( choice == 3 ){
		CurrentAlg = EvenSplitPath;
	}
	if( choice == 4 ){
		CurrentAlg = QueuePath;
	}
}



int walkData[128*4][128*4];
int coopPathData[128*4][128*4];

bool isLeft(BWAPI::Position a, BWAPI::Position b, BWAPI::Position c){
     return ((b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x())) >= 0;
}


void FillWalkData(){
	for(int x=0; x<Broodwar->mapWidth()*4; x++){
	  for(int y=0; y<Broodwar->mapHeight()*4; y++){
		  walkData[x][y]=0;
	  }
	}
	//Block minerals locations
	BOOST_FOREACH(Unit* mineral, Broodwar->getMinerals() ){
		for(int x=mineral->getTilePosition().x()*4; x<(mineral->getTilePosition().x()+2)*4; x++){
	    for(int y=mineral->getTilePosition().y()*4; y<(mineral->getTilePosition().y()+1)*4; y++){
		  walkData[x][y] = 1;
	    }
	  }
	}
    
	/*
	Minerals[2].mineralTrick = Minerals[1].miner;
	Minerals[3].mineralTrick = Minerals[1].miner;
	Minerals[5].mineralTrick = Minerals[6].miner;
	Minerals[7].mineralTrick = Minerals[6].miner;
	*/

	
	//BWAPI::Position trickPos( Minerals[0].miner->getPosition().x() + 32, Minerals[0].miner->getPosition().y() - 32 );
	//Minerals[0].posTrick = trickPos;


    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
	  if ((*i)->getType() == BWAPI::UnitTypes::Terran_Command_Center )
      {
		  CCtrick = BWAPI::Position( (*i)->getTilePosition().x()*32 - 8,(*i)->getTilePosition().y()*32 - 8  );
		  ourCC = (*i);
	  }
    }

	//CCtrick = BWAPI::Position( Comm  )

	//determine mineral trick
	//decide which part the mineral goes to
	BWAPI::Position Point1 = ourCC->getPosition();
	BWAPI::Position Point2( ourCC->getPosition().x() + 32*10, ourCC->getPosition().y() + 32*10 );
	Broodwar->drawLineMap( Point1.x(),Point1.y(), Point2.x(), Point2.y() , BWAPI::Colors::Orange );
	for(int i=0; i<Minerals.size(); i++){
		Minerals[i].mineralTrick = NULL;
		bool isLeftM = isLeft( Point1, Point2, Minerals[i].miner->getPosition() );
		if( isLeftM == true){ //scv arrives at top of mineral
			Minerals[i].Facing = 1;
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Blue,true );
		} else { //scv arrives at left  of mineral
			Minerals[i].Facing = 0;
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Cyan,true );
		}
	}


	for(int i=0; i<Minerals.size(); i++){
		//bool isLeftM = isLeft( Point1, Point2, Minerals[i].miner->getPosition() );
		if( Minerals[i].Facing == 1){ //scv arrives at top of mineral
			std::vector<int> PossibleTrick;
			for(int k=0; k<Minerals.size(); k++){
				if( i != k && Minerals[k].miner->getDistance( Minerals[i].miner ) <= 1*16
					&& Minerals[k].mineralTrick == NULL
					&& Minerals[k].Facing == 1
					&& Minerals[k].miner->getPosition().y() < Minerals[i].miner->getPosition().y() ){
						PossibleTrick.push_back(k);
				}
			}
			Broodwar->printf("%i",PossibleTrick.size());
			if( PossibleTrick.size() == 2){
				Minerals[ PossibleTrick[0] ].mineralTrick = Minerals[i].miner;
				Minerals[ PossibleTrick[1] ].mineralTrick = Minerals[i].miner;
			}
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Blue,true );
		} else { //scv arrives at left  of mineral
			std::vector<int> PossibleTrick;
			for(int k=0; k<Minerals.size(); k++){
				if( i != k && Minerals[k].miner->getDistance( Minerals[i].miner ) <= 1*16
					&& Minerals[k].mineralTrick == NULL
					&& Minerals[k].Facing == 0
					&& Minerals[k].miner->getPosition().x() < Minerals[i].miner->getPosition().x() ){
						PossibleTrick.push_back(k);
				}
			}
			Broodwar->printf("%i",PossibleTrick.size());
			if( PossibleTrick.size() == 2){
				Minerals[ PossibleTrick[0] ].mineralTrick = Minerals[i].miner;
				Minerals[ PossibleTrick[1] ].mineralTrick = Minerals[i].miner;
			}
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Cyan,true );
		}
	}


	//check for calculate path trick

	for(int i=0; i<Minerals.size(); i++){
		//bool isLeftM = isLeft( Point1, Point2, Minerals[i].miner->getPosition() );
		if( Minerals[i].Facing == 1){ //scv arrives at top of mineral
			std::vector<int> PossibleTrick;
			for(int k=0; k<Minerals.size(); k++){
				if( i != k && Minerals[k].miner->getDistance( Minerals[i].miner ) <= 1*16 ){
						PossibleTrick.push_back(k);
				}
			}
			//Broodwar->printf("%i",PossibleTrick.size());
			if( PossibleTrick.size() == 1 && Minerals[i].mineralTrick == NULL ){
                BWAPI::Position trickPos( Minerals[i].miner->getPosition().x() - 32, Minerals[i].miner->getPosition().y() + 32 );
				Minerals[ i ].posTrick = trickPos;
			}
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Blue,true );
		} else { //scv arrives at left  of mineral
			std::vector<int> PossibleTrick;
			for(int k=0; k<Minerals.size(); k++){
				if( i != k && Minerals[k].miner->getDistance( Minerals[i].miner ) <= 1*16 ){
						PossibleTrick.push_back(k);
				}
			}
			//Broodwar->printf("%i",PossibleTrick.size());
			if( PossibleTrick.size() == 1 && Minerals[i].mineralTrick == NULL ){
                BWAPI::Position trickPos( Minerals[i].miner->getPosition().x() + 32, Minerals[i].miner->getPosition().y() - 32 );
				Minerals[ i ].posTrick = trickPos;
			}
			
		}
	}



}


void drawPathData(){
	
	/*
	for(int x=0; x<Broodwar->mapWidth()*4; x++){
		for(int y=0; y<Broodwar->mapHeight()*4; y++){
			if(  walkData[x][y] == 0 ){
				Broodwar->drawCircleMap(x*(32/4),y*(32/4),2,Colors::Green,true);
			}
			else {
				Broodwar->drawCircleMap(x*(32/4),y*(32/4),2,Colors::Red,true);
			}
			
	  }
	}
	*/


	for(int i=0; i<Minerals.size(); i++){
		if( Minerals[i].mineralTrick != NULL ){
			Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Blue,true );
			Broodwar->drawCircleMap(  Minerals[i].mineralTrick->getPosition().x(),Minerals[i].mineralTrick->getPosition().y(),4,BWAPI::Colors::Purple,true );

		}
	}

	/*
	//decide which part the mineral goes to
	BWAPI::Position Point1 = ourCC->getPosition();
	BWAPI::Position Point2( ourCC->getPosition().x() + 32*10, ourCC->getPosition().y() + 32*10 );
	Broodwar->drawLineMap( Point1.x(),Point1.y(), Point2.x(), Point2.y() , BWAPI::Colors::Orange );
	for(int i=0; i<Minerals.size(); i++){
		bool isLeftM = isLeft( Point1, Point2, Minerals[i].miner->getPosition() );
		if( isLeftM == true){ //scv arrives at top of mineral
			for( int j=-3; j<3; j++){
			  Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x() + 32*j,Minerals[i].miner->getPosition().y() - 32,4,BWAPI::Colors::Purple,true );
			}
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y() - 32,4,BWAPI::Colors::Purple,true );
			Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Blue,true );
		} else { //scv arrives at left of mineral
			for( int j=-3; j<3; j++){
			  Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x() - 32,Minerals[i].miner->getPosition().y() + 32*j,4,BWAPI::Colors::Purple,true );
			}
			//Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x() - 32,Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Purple,true );
			Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Cyan,true );
		}
	}
	*/

	for(int i=0; i<Minerals.size(); i++){
		if( Minerals[i].posTrick != BWAPI::Positions::None ){
			Broodwar->drawCircleMap(Minerals[i].posTrick.x(),Minerals[i].posTrick.y(),2,Colors::Orange,true);
			Broodwar->drawCircleMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(),4,BWAPI::Colors::Yellow,true );
		}
	}

	//Broodwar->drawCircleMap(Minerals[0].posTrick.x(),Minerals[0].posTrick.y(),2,Colors::Purple,true);

	Broodwar->drawCircleMap(CCtrick.x(),CCtrick.y(),2,Colors::Brown,true);

	for(int i=0; i<Minerals.size(); i++){
		Broodwar->drawTextMap(  Minerals[i].miner->getPosition().x(),Minerals[i].miner->getPosition().y(), "%d", Minerals[i].MinToCC + Minerals[i].CCToMin );
	}
	
}




void LetaBotModule::onStart()
{
	SCVs.clear();
  SCVs.reserve(50);
  //BWAPI::Broodwar->setLocalSpeed(0);
  ChooseAlg();
  ReadMoveData();

  fout.open("bwapi-data/write/minerals.txt");

  //Broodwar->sendText("Hello world!");
  //Broodwar->printf("The map is %s, a %d player map",Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
  // Enable some cheat flags
  Broodwar->enableFlag(Flag::UserInput);
  // Uncomment to enable complete map information
  //Broodwar->enableFlag(Flag::CompleteMapInformation);

  //read map information into BWTA so terrain analysis can be done in another thread
  BWTA::readMap();
  //BWTA::analyze();
  analyzed=false;
  analysis_just_finished=false;

  //show_bullets=false;
  //show_visibility_data=false;


	int counter = 0;
	std::ifstream fin("bwapi-data/read/MineralData.txt");
	for(int j=0; j<8;j++){
      Mineral newMin;
	  fin>>newMin.ID;
	  fin>>newMin.MinX>>newMin.MinY;
      fin>>newMin.MinToCC;
	  fin>>newMin.CCToMin;
	  newMin.miner=NULL;
	  newMin.SCVcount=0;
	  newMin.mineralTrick = NULL;
	  newMin.posTrick = BWAPI::Positions::None;
	  //Broodwar->printf("X: %d ok",newMin.MinX);
	  for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
       {
		   if(  (*m)->getPosition().x() == newMin.MinX && (*m)->getPosition().y() == newMin.MinY ){
             newMin.miner = *m;
		   }
	   }
	  Minerals.push_back(newMin);
	  /*
	  //Broodwar->printf("Nr: %d ok",newMin.miner->getID());
	  if( newMin.miner == NULL ){
        Broodwar->printf("Error in mineral patch");
	  } else {
		  //Broodwar->printf("Nr: %d ok",newMin.miner->getID());
	  }
	  */
	}

	 //Determine mineral closest to CC
	  int minimum = 999999;
	  for(unsigned int i=0; i<Minerals.size(); i++){
		if( minimum > Minerals[i].MinToCC + Minerals[i].CCToMin ){
          minimum = Minerals[i].MinToCC + Minerals[i].CCToMin;
		  closestMineral = Minerals[i].miner;
		}
	  }
	  FillWalkData();
     //First determine initial split
	  if( CurrentAlg == BuildIn ){
		  Broodwar->printf("Using build in AI");
	  SplitWorkersClosest();
	  }
	  if( CurrentAlg == EvenSplit ){
		  Broodwar->printf("Using Even Split)");
	     SplitWorkersEven();
	  }
	  if( CurrentAlg == Queue ){
		  Broodwar->printf("Using simple Queue");
        SplitWorkersQueue();
	  }
	  if( CurrentAlg == EvenSplitPath ){
		  Broodwar->printf("Using Even Split path");
	     SplitWorkersEvenPath();
	  }
	  if( CurrentAlg == QueuePath ){
		  Broodwar->printf("Using path Queue");
        SplitWorkersPathQueue();
	  } 

}


void LetaBotModule::onEnd(bool isWinner)
{
	if( CurrentAlg != QueuePath ){
		return;
	}
	return;
  //write move data to file
	std::ofstream MoveFout("bwapi-data/write/moveData.txt");
  MoveFout<<MoveDatas.size()<<"\n";
  for(unsigned int i=0; i<MoveDatas.size(); i++){
	  MoveFout<<"MineralX: "<<MoveDatas[i].MineralX<<"\n";
	  MoveFout<<"MineralY: "<<MoveDatas[i].MineralY<<"\n";
	  MoveFout<<"Startx: "<<MoveDatas[i].Startx<<"\n";
	  MoveFout<<"Starty: "<<MoveDatas[i].Starty<<"\n";
	  MoveFout<<"Endx: "<<MoveDatas[i].Endx<<"\n";
	  MoveFout<<"Endy: "<<MoveDatas[i].Endy<<"\n";
	  if( MoveDatas[i].MovingToMineral == true ){
	    MoveFout<<"ToMineral\n";
	  } else {
	    MoveFout<<"FromMineral\n";
	  }
	  MoveFout<<"TotalTime: "<<MoveDatas[i].TotalFrames<<"\n";
  }



}




void clearCoopData(){
	//int coopPathData[128*4][128*4];
	for(int x=0; x<128*4; x++){
	  for(int y=0; y<128*4; y++){
		coopPathData[x][y] = 0;
	  }
	}
    for(unsigned int j=0; j<SCVs.size(); j++){
		int startX = ( SCVs[j].scv->getPosition().x() )/(32/4);
		int startY = ( SCVs[j].scv->getPosition().y() )/(32/4);
		if( SCVs[j].scv->getVelocityX() > 1.1 ){
			startX++;
		}
		if( SCVs[j].scv->getVelocityX() < -1.1 ){
			startX--;
		}
		if( SCVs[j].scv->getVelocityY() > 1.1 ){
			startY++;
		}
		if( SCVs[j].scv->getVelocityY() < -1.1 ){
			startY--;
		}

		for(int x=startX-2; x<startX+4; x++ ){
		  for(int y=startY-2; y<startY+4;y++ ){
			  coopPathData[x][y] = SCVs[j].scv->getID();
		  }
		}
    }

	for(int x=0; x<Broodwar->mapWidth()*4; x++){
		for(int y=0; y<Broodwar->mapHeight()*4; y++){
			if(  coopPathData[x][y] > 0 ){
				Broodwar->drawCircleMap(x*(32/4),y*(32/4),2,Colors::Red,true);
			}			
	  }
	}

}


bool collisionAvoid(Unit* scv){
	//int coopPathData[128*4][128*4];
		int startX = ( scv->getPosition().x() )/(32/4);
		int startY = ( scv->getPosition().y() )/(32/4);
		if( scv->getVelocityX() > 1.1 ){
			startX++;
		}
		if( scv->getVelocityX() < -1.1 ){
			startX--;
		}
		if( scv->getVelocityY() > 1.1 ){
			startY++;
		}
		if( scv->getVelocityY() < -1.1 ){
			startY--;
		}

		for(int x=startX-2; x<startX+4; x++ ){
		  for(int y=startY-2; y<startY+4;y++ ){
			  if( coopPathData[x][y] > 0 && coopPathData[x][y] != scv->getID() ){
				  return true;
			  }
		  }
		}
    

	return false;

}


void LetaBotModule::onFrame()
{
  //if (show_visibility_data)
   // drawVisibilityData();

  if (Broodwar->isReplay())
    return;

  drawPathData();
  /*
  //Make sure that the scv stays at the mineral patch
  if( !mySCV->isCarryingMinerals() && mySCV->getOrderTarget() != closestMineral ){
	  //mySCV->gather(closestMineral);
  }
  if( !secondSCV->isCarryingMinerals() && secondSCV->getOrderTarget() != closestMineral ){
	  //secondSCV->gather(closestMineral);
  }
  */
  /*
  if( !mySCV->isCarryingMinerals() && mySCV->getOrder() != BWAPI::Orders::MiningMinerals && closestMineral->isBeingGathered() ){
	  mySCV->gather(closestMineral);
  }
  if( !secondSCV->isCarryingMinerals() && secondSCV->getOrder() != BWAPI::Orders::WaitForMinerals && closestMineral->isBeingGathered() ){
	  secondSCV->gather(closestMineral);
  }
  */

/*
int SCVstate = MovingToMineral;
int MovingTime = 0;
int BeginGTime = 0;
int EndGTime = 0;
int ReturnTime = 0;
*/
  /*
  if( mySCV->getOrder() == BWAPI::Orders::MiningMinerals && SCVstate == MovingToMineral  ){
    BeginGTime = Broodwar->getFrameCount();
	SCVstate = GatheringMineral;
  }
   if( mySCV->getOrder() == BWAPI::Orders::ReturnMinerals && SCVstate == GatheringMineral ){
    SCVstate=ReturningMineral;
	EndGTime = Broodwar->getFrameCount();
  }
   if( mySCV->getOrder() == BWAPI::Orders::MoveToMinerals && SCVstate == ReturningMineral ){
    SCVstate=MovingToMineral;
	ReturnTime = Broodwar->getFrameCount();
	MovingTime = Broodwar->getFrameCount();
  }
  */

  /*
  if( mySCV->getOrder() == BWAPI::Orders::MiningMinerals && !gathering){
    gathering=true;
	StartGather = Broodwar->getFrameCount();
  }
  //mySCV->getOrder() == BWAPI::Orders::ReturnMinerals
  if( mySCV->isCarryingMinerals() && gathering){
    gathering=false;
	EndGather = Broodwar->getFrameCount();
  }
  */
    TotalSCV = 0;
	bool hasSuppDet = false;
    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
	  if ((*i)->getType().isWorker() )
      {
       TotalSCV++;
	  }
	  if ((*i)->getType() == BWAPI::UnitTypes::Terran_Supply_Depot )
      {
        hasSuppDet = true;
	  }
    }
  //Make a new SCV if not already queued
   for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isResourceDepot())
      {
		  if(  (*i)->getTrainingQueue().size() < 1 && TotalSCV < 18){
			  if(  TotalSCV == BuildDepotAt && !hasSuppDet ){//wait untill supply depot is finished
				  break;
			  }
            (*i)->train(Broodwar->self()->getRace().getWorker());
		  }
	  }
	}

   /*
   for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->isTraining() )
      {
		  Broodwar->printf("frames left:%d", (*i)->getRemainingTrainTime() );
	  }
	}
   */


   if(CurrentAlg == EvenSplitPath){
	   clearCoopData();
	   for(unsigned int j=0; j<SCVs.size(); j++){
		   if( SCVs[j].scv == NULL || SCVs[j].mineralPatch == NULL ){
             continue;
		   }
		   if(  SCVs[j].SCVstate == NoTrick ){
		     if( SCVs[j].scv->isCarryingMinerals() || SCVs[j].scv->getOrderTarget() == NULL ){
              continue;
		     }
		     if( SCVs[j].scv->getOrderTarget()->getID()  !=  SCVs[j].mineralPatch->getID()){
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
		     }
			 continue;
		   }
		   //Broodwar->drawTextScreen( 200, j*10 + 100, "distance %d", SCVs[j].scv->getDistance( SCVs[j].Intermidiate ) );
		   //if( SCVs[j].SCVstate == MovingToMineral ){
             //Broodwar->drawTextScreen( 300, j*10 + 100, "Current state is MovingToMineral");
             //Broodwar->printf("Current printd is MovingToMineral");
		   //}
		   if( SCVs[j].scv->getOrderTarget() != NULL &&
			   SCVs[j].SCVstate == ExtraMoveToMineral &&
			   SCVs[j].scv->getOrderTarget()->getID() != SCVs[j].interMineral->getID()  ){
			   SCVs[j].scv->gather( SCVs[j].interMineral );
			 //Broodwar->printf("Gathering mineral");
		   }
		   
		   /*
		   //do not do the trick with more than 1 SCV
		   if(  SCVs[j].SCVstate == ExtraMoveToPos &&
			   SCVs[j].Intermidiate != BWAPI::Positions::None &&
			   Minerals[ getIDmineral(SCVs[j].mineralPatch->getID() ) ].SCVcount > 1  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch");
		   }
		   */

		   if(  SCVs[j].SCVstate == ExtraMoveToPos &&
			   SCVs[j].Intermidiate != BWAPI::Positions::None &&
			   SCVs[j].scv->getOrder() != BWAPI::Orders::Move  ){
				   SCVs[j].scv->move( SCVs[j].Intermidiate );
			 //Broodwar->printf("Gathering mineral");
		   }
		   if( SCVs[j].scv->getOrderTarget() != NULL &&
			   SCVs[j].SCVstate == MovingToMineral &&
			   SCVs[j].scv->getOrderTarget()->getID() != SCVs[j].mineralPatch->getID()   ){
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			 //Broodwar->printf("Gathering mineral");
		   }
		   /*
		   if( SCVs[j].SCVstate == ExtraMoveToMineral && SCVs[j].scv->getDistance(  SCVs[j].interMineral ) < 5  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch");
			 //Broodwar->printf("Gathering mineral");
		   }
		   */

		   if( SCVs[j].SCVstate == ExtraMoveToPos && SCVs[j].scv->getDistance(  SCVs[j].mineralPatch ) < 40  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch");
			 //Broodwar->printf("Gathering mineral");
		   }

		   if( SCVs[j].SCVstate == ExtraMoveToPos && collisionAvoid(SCVs[j].scv) == true  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Collsion: using gather");
		   }
		   if( SCVs[j].SCVstate == MovingToMineral  && SCVs[j].Intermidiate != BWAPI::Positions::None
			    && collisionAvoid(SCVs[j].scv) == false
			    && SCVs[j].scv->getDistance(  SCVs[j].mineralPatch ) >= 40
				&& SCVs[j].scv->getDistance(  SCVs[j].Intermidiate ) < 10 ){
			   SCVs[j].SCVstate = ExtraMoveToPos;
			   SCVs[j].scv->move( SCVs[j].Intermidiate );
			   Broodwar->printf("No Collsion: using move");
		   }
		   if( SCVs[j].SCVstate == ExtraMoveToPos && SCVs[j].scv->getDistance(  SCVs[j].Intermidiate ) < 20  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch: postion too close");
			 //Broodwar->printf("Gathering mineral");
		   }


		   if( SCVs[j].SCVstate == ExtraMoveToMineral && SCVs[j].scv->getDistance(  SCVs[j].mineralPatch ) < 40  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch");
			 //Broodwar->printf("Gathering mineral");
		   }

		   if( SCVs[j].SCVstate == MovingToMineral && SCVs[j].scv->getOrder() == BWAPI::Orders::MiningMinerals  ){
			   SCVs[j].SCVstate = GatheringMineral;
			 //Broodwar->printf("Gathering mineral");
		   }
		   if( SCVs[j].SCVstate == GatheringMineral &&  SCVs[j].scv->isCarryingMinerals()  ){
             SCVs[j].SCVstate = ReturningMineral;
		   }
		   /*
		   //return path trick
		   if( SCVs[j].SCVstate == ReturningMineral &&  SCVs[j].scv->isCarryingMinerals()  ){
			   bool possiblu = true;
			   for(int k=0; k<SCVs.size(); k++){
				   if(  j == k ){continue;}
				   if( SCVs[j].scv->getDistance( SCVs[k].scv ) < 20  ){
					   possiblu = false;
				   }
			   }
			   if(  possiblu == false || SCVs[j].scv->getDistance(ourCC) < 16 || SCVs[j].scv->getDistance(ourCC) > 32){
				   //|| Minerals[getIDmineral(  SCVs[j].mineralPatch->getID() )].MinToCC < 45 ){
				   if( SCVs[j].scv->getOrder() != BWAPI::Orders::ReturnMinerals ){
				     SCVs[j].scv->returnCargo(); //ourCC
				   }
			   } else {
				   SCVs[j].scv->move(CCtrick);
			   }
		   }
		   */

		   if( SCVs[j].SCVstate == ReturningMineral &&  !SCVs[j].scv->isCarryingMinerals()  ){
			   if( SCVs[j].Intermidiate != BWAPI::Positions::None ){
				   SCVs[j].SCVstate = ExtraMoveToPos;
				   SCVs[j].scv->move( SCVs[j].Intermidiate );
			   } else if(  SCVs[j].interMineral != NULL ){
                SCVs[j].SCVstate = ExtraMoveToMineral;
			    SCVs[j].scv->gather( SCVs[j].interMineral );
			   } 

			 //SCVs[j].Intermidiate = GetTrickPath( SCVs[j].mineralPatch );
		   }
		   //if( SCVs[j].SCVstate == ExtraMoveToMineral &&  (SCVs[j].scv->getOrderTarget() == NULL || SCVs[j].scv->getOrderTarget()->getID()  !=  SCVs[j].mineralPatch->getID() ) ){
			   //SCVs[j].scv->gather( SCVs[j].mineralPatch );
		   //}
	   }
   }

   if(CurrentAlg == EvenSplit){
	   for(unsigned int j=0; j<SCVs.size(); j++){
		   if( SCVs[j].scv == NULL || SCVs[j].mineralPatch == NULL ){
             continue;
		   }
		   if( SCVs[j].scv->isCarryingMinerals() || SCVs[j].scv->getOrderTarget() == NULL ){
            continue;
		   }
		   if( SCVs[j].scv->getOrderTarget()->getID()  !=  SCVs[j].mineralPatch->getID()){
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
		   }
	   }
   }


   if(CurrentAlg == Queue){

	   unsigned int totalQueue = 0;
	   for(unsigned int i=0;i<Minerals.size(); i++){
		   totalQueue+=Minerals[i].Queue.size();
	   }
	   if( totalQueue > SCVs.size() ){
    	 Broodwar->printf("Something wrong with your Queue imp;le");     
	   }

	   for(unsigned int j=0; j<SCVs.size(); j++){
		   //Make sure each SCV stays at its mining patch
		   if( SCVs[j].scv == NULL || SCVs[j].mineralPatch == NULL ){
             continue;
		   }
		   //if( SCVs[j].scv->isCarryingMinerals() || SCVs[j].scv->getOrderTarget() == NULL ){
            //continue;
		   //}
		   if( !SCVs[j].scv->isCarryingMinerals() && !(SCVs[j].scv->getOrderTarget() == NULL)  ){
		     if(  SCVs[j].scv->getOrderTarget()->getID()  !=  SCVs[j].mineralPatch->getID()){
			     SCVs[j].scv->gather( SCVs[j].mineralPatch );
		     }
	       }
		   if( SCVs[j].SCVMov.set == true &&  SCVs[j].SCVstate == MovingToMineral && 
			   (SCVs[j].scv->getOrder() == BWAPI::Orders::MiningMinerals || SCVs[j].scv->getOrder() == BWAPI::Orders::WaitForMinerals ) ){
				   SCVs[j].SCVMov.Endx = SCVs[j].scv->getPosition().x();
				   SCVs[j].SCVMov.Endy = SCVs[j].scv->getPosition().y();
                   SCVs[j].SCVMov.TotalFrames=Broodwar->getFrameCount()-SCVs[j].SCVMov.StartTime;
                   MoveDatas.push_back( SCVs[j].SCVMov );
				   SCVs[j].SCVMov.set = false;
		   }
		   if( SCVs[j].SCVstate == MovingToMineral && SCVs[j].scv->getOrder() == BWAPI::Orders::MiningMinerals  ){
             SCVs[j].SCVstate = GatheringMineral;
             SCVs[j].StartedMining =  Broodwar->getFrameCount();
			 //Broodwar->printf("Gathering mineral");
		   }
		   if( SCVs[j].SCVstate == GatheringMineral &&  SCVs[j].scv->isCarryingMinerals()  ){
             SCVs[j].SCVstate = ReturningMineral;			 
			 //Remove this SCV from the queue
			 bool removed = false;
			 for(unsigned int k=0; k<Minerals.size(); k++){
				 if( Minerals[k].Queue.size() == 0 ){//no SCV here
                   continue;
				 }
				 //Broodwar->printf("Returning mineral %d", Minerals[k].Queue.size());
				 //Broodwar->printf("Returning mineral %d", Minerals[k].Queue[0].scv->getID() );
				 //Go through all positions. Can be that our scv got there earlier than expected
				 for(unsigned int q=0; q<Minerals[k].Queue.size(); q++){
				  if(  Minerals[k].Queue[q]->scv->getID() == SCVs[j].scv->getID() ){
					 removed = true;
					 //Broodwar->printf("Removing from queue %d", Minerals[k].Queue[0].scv->getID() );
                     //Broodwar->printf("Removing from queue %d", Minerals[k].Queue[0]->StartedMining );
                     //Broodwar->printf("Mining start time %d", SCVs[j].StartedMining );
					 Minerals[k].Queue.erase( Minerals[k].Queue.begin()+q );//remove the SCV from the queueu
				   }
				 }
				 
			 }
			 if( !removed ){
               Broodwar->printf("SCV was not in queue");
			 }
             //Also store move data
		     SCVs[j].SCVMov.set=true;
		     SCVs[j].SCVMov.Startx=SCVs[j].scv->getPosition().x();
		     SCVs[j].SCVMov.Starty=SCVs[j].scv->getPosition().y();
             SCVs[j].SCVMov.StartTime=Broodwar->getFrameCount();
             SCVs[j].SCVMov.MovingToMineral = false;
		     SCVs[j].SCVMov.MineralX=SCVs[j].mineralPatch->getPosition().x();
             SCVs[j].SCVMov.MineralY=SCVs[j].mineralPatch->getPosition().y();
		   }
		   if( SCVs[j].SCVstate == ReturningMineral &&  !SCVs[j].scv->isCarryingMinerals()  ){
			   //Pick a new location to mine
               SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].StartedMoving = Broodwar->getFrameCount();

                //Store move data
			   if( SCVs[j].SCVMov.set == true){
				   SCVs[j].SCVMov.Endx = SCVs[j].scv->getPosition().x();
				   SCVs[j].SCVMov.Endy = SCVs[j].scv->getPosition().y();
                   SCVs[j].SCVMov.TotalFrames=Broodwar->getFrameCount()-SCVs[j].SCVMov.StartTime;
                   MoveDatas.push_back( SCVs[j].SCVMov );
				   SCVs[j].SCVMov.set = false;
			   }

			   QueueChoice( SCVs[j].scv );//Get new mineral patch for unit
			}

	   }

   }

   if(CurrentAlg == QueuePath){
	   clearCoopData();
	   unsigned int totalQueue = 0;
	   for(unsigned int i=0;i<Minerals.size(); i++){
		   totalQueue+=Minerals[i].Queue.size();
	   }
	   if( totalQueue > SCVs.size() ){
    	 Broodwar->printf("Something wrong with your Queue imp;le");     
	   }

	   for(unsigned int j=0; j<SCVs.size(); j++){
		   //Make sure each SCV stays at its mining patch
		   if( SCVs[j].scv == NULL || SCVs[j].mineralPatch == NULL ){
             continue;
		   }
		   //if( SCVs[j].scv->isCarryingMinerals() || SCVs[j].scv->getOrderTarget() == NULL ){
            //continue;
		   //}
		   if( !SCVs[j].scv->isCarryingMinerals() && !(SCVs[j].scv->getOrderTarget() == NULL)  ){
		     if(  SCVs[j].scv->getOrderTarget()->getID()  !=  SCVs[j].mineralPatch->getID()
				  &&  SCVs[j].SCVstate == MovingToMineral ){
			     SCVs[j].scv->gather( SCVs[j].mineralPatch );
		     }
			 if( SCVs[j].interMineral != NULL ){
		       if(  SCVs[j].scv->getOrderTarget()->getID()  !=  SCVs[j].interMineral->getID() 
				 &&  SCVs[j].SCVstate == ExtraMoveToMineral ){
			     SCVs[j].scv->gather( SCVs[j].interMineral );
		       }
			 }
		     if(  SCVs[j].SCVstate == ExtraMoveToPos &&
			   SCVs[j].Intermidiate != BWAPI::Positions::None &&
			   SCVs[j].scv->getOrder() != BWAPI::Orders::Move  ){
				   SCVs[j].scv->move( SCVs[j].Intermidiate );
			   //Broodwar->printf("Gathering mineral");
		     }
	       }
		   if( !SCVs[j].scv->isCarryingMinerals() && (SCVs[j].scv->getOrderTarget() == NULL)  ){
			   if(  SCVs[j].SCVstate == ExtraMoveToPos ){
			     //SCVs[j].SCVstate = MovingToMineral;
			     //SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   }
			   if(  SCVs[j].SCVstate == MovingToMineral ){
			     SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   }
		   }
		   if( SCVs[j].SCVMov.set == true &&  
			   ((SCVs[j].SCVstate == MovingToMineral ) )			   
			   && 
			   (SCVs[j].scv->getOrder() == BWAPI::Orders::MiningMinerals || SCVs[j].scv->getOrder() == BWAPI::Orders::WaitForMinerals ) ){
				   SCVs[j].SCVMov.Endx = SCVs[j].scv->getPosition().x();
				   SCVs[j].SCVMov.Endy = SCVs[j].scv->getPosition().y();
                   SCVs[j].SCVMov.TotalFrames=Broodwar->getFrameCount()-SCVs[j].SCVMov.StartTime;
                   MoveDatas.push_back( SCVs[j].SCVMov );
				   SCVs[j].SCVMov.set = false;
		   }

		   if( SCVs[j].SCVstate == ExtraMoveToMineral && SCVs[j].scv->getDistance(  SCVs[j].mineralPatch ) < 40  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch");
			 //Broodwar->printf("Gathering mineral");
		   }

		   //move Position state changes
		   if( SCVs[j].SCVstate == ExtraMoveToPos && SCVs[j].scv->getDistance(  SCVs[j].mineralPatch ) < 40  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch");
			 //Broodwar->printf("Gathering mineral");
		   }
		   if( SCVs[j].SCVstate == ExtraMoveToPos && collisionAvoid(SCVs[j].scv) == true  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Collsion: using gather");
		   }
		   if( SCVs[j].SCVstate == MovingToMineral  && SCVs[j].Intermidiate != BWAPI::Positions::None
			    && collisionAvoid(SCVs[j].scv) == false
			    && SCVs[j].scv->getDistance(  SCVs[j].mineralPatch ) >= 40
				&& SCVs[j].scv->getDistance(  SCVs[j].Intermidiate ) < 10 ){
			   SCVs[j].SCVstate = ExtraMoveToPos;
			   SCVs[j].scv->move( SCVs[j].Intermidiate );
			   Broodwar->printf("No Collsion: using move");
		   }
		   if( SCVs[j].SCVstate == ExtraMoveToPos && SCVs[j].scv->getDistance(  SCVs[j].Intermidiate ) < 20  ){
			   SCVs[j].SCVstate = MovingToMineral;
			   SCVs[j].scv->gather( SCVs[j].mineralPatch );
			   Broodwar->printf("Now changing to mineral patch: postion too close");
			 //Broodwar->printf("Gathering mineral");
		   }

		   if( SCVs[j].SCVstate == MovingToMineral && SCVs[j].scv->getOrder() == BWAPI::Orders::MiningMinerals  ){
             SCVs[j].SCVstate = GatheringMineral;
             SCVs[j].StartedMining =  Broodwar->getFrameCount();
			 //Broodwar->printf("Gathering mineral");
		   }
		   if( SCVs[j].SCVstate == GatheringMineral &&  SCVs[j].scv->isCarryingMinerals()  ){
             SCVs[j].SCVstate = ReturningMineral;			 
			 //Remove this SCV from the queue
			 bool removed = false;
			 for(unsigned int k=0; k<Minerals.size(); k++){
				 if( Minerals[k].Queue.size() == 0 ){//no SCV here
                   continue;
				 }
				 //Broodwar->printf("Returning mineral %d", Minerals[k].Queue.size());
				 //Broodwar->printf("Returning mineral %d", Minerals[k].Queue[0].scv->getID() );
				 //Go through all positions. Can be that our scv got there earlier than expected
				 for(unsigned int q=0; q<Minerals[k].Queue.size(); q++){
				  if(  Minerals[k].Queue[q]->scv->getID() == SCVs[j].scv->getID() ){
					 removed = true;
					 //Broodwar->printf("Removing from queue %d", Minerals[k].Queue[0].scv->getID() );
                     //Broodwar->printf("Removing from queue %d", Minerals[k].Queue[0]->StartedMining );
                     //Broodwar->printf("Mining start time %d", SCVs[j].StartedMining );
					 Minerals[k].Queue.erase( Minerals[k].Queue.begin()+q );//remove the SCV from the queueu
				   }
				 }
				 
			 }
			 if( !removed ){
               Broodwar->printf("SCV was not in queue");
			 }
             //Also store move data
		     SCVs[j].SCVMov.set=true;
		     SCVs[j].SCVMov.Startx=SCVs[j].scv->getPosition().x();
		     SCVs[j].SCVMov.Starty=SCVs[j].scv->getPosition().y();
             SCVs[j].SCVMov.StartTime=Broodwar->getFrameCount();
             SCVs[j].SCVMov.MovingToMineral = false;
		     SCVs[j].SCVMov.MineralX=SCVs[j].mineralPatch->getPosition().x();
             SCVs[j].SCVMov.MineralY=SCVs[j].mineralPatch->getPosition().y();
		   }
		   if( SCVs[j].SCVstate == ReturningMineral &&  !SCVs[j].scv->isCarryingMinerals()  ){
			   //Pick a new location to mine
			   if( SCVs[j].interMineral == NULL ){
               SCVs[j].SCVstate = MovingToMineral;
			   } else {
				   SCVs[j].SCVstate = ExtraMoveToMineral;
			   }
			   SCVs[j].StartedMoving = Broodwar->getFrameCount();

                //Store move data
			   if( SCVs[j].SCVMov.set == true){
				   SCVs[j].SCVMov.Endx = SCVs[j].scv->getPosition().x();
				   SCVs[j].SCVMov.Endy = SCVs[j].scv->getPosition().y();
                   SCVs[j].SCVMov.TotalFrames=Broodwar->getFrameCount()-SCVs[j].SCVMov.StartTime;
                   MoveDatas.push_back( SCVs[j].SCVMov );
				   SCVs[j].SCVMov.set = false;
			   }

			   QueuePathChoice( SCVs[j].scv );//Get new mineral patch for unit
			}

	   }

   }




  //drawStats();
  Broodwar->drawTextScreen(200,0,"Frames: %d",Broodwar->getFrameCount());
  if(  Broodwar->getFrameCount()%500==0 ){
    fout<<Broodwar->self()->gatheredMinerals()<<"\n";
  }
  if(  Broodwar->getFrameCount()>8005 ){
    Broodwar->leaveGame();
  }

}



void LetaBotModule::onSendText(std::string text)
{

  if (text=="/analyze")
  {
    if (analyzed == false)
    {
      Broodwar->printf("Analyzing map... this may take a minute");
      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
    }
  } else
  {
    if( text == "f" ){
		BWAPI::Broodwar->setLocalSpeed(0);
	}
    if( text == "m" ){
		BWAPI::Broodwar->setLocalSpeed(20);
	}
    if( text == "s" ){
		BWAPI::Broodwar->setLocalSpeed(40);
	}
    //Broodwar->printf("You typed '%s'!",text.c_str());
    //Broodwar->sendText("%s",text.c_str());
  }
}

/*

void ExampleAIModule::onUnitCreate(BWAPI::Unit* unit)
{
  if (Broodwar->getFrameCount()>1)
  {
    if (!Broodwar->isReplay())
      Broodwar->sendText("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
    else
    {
      //if we are in a replay, then we will print out the build order
      //(just of the buildings, not the units).
      if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral()==false)
      {
        int seconds=Broodwar->getFrameCount()/24;
        int minutes=seconds/60;
        seconds%=60;
        Broodwar->sendText("%.2d:%.2d: %s creates a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
      }
    }
  }
}


*/

DWORD WINAPI AnalyzeThread()
{
  BWTA::analyze();
  analyzed   = true;
  analysis_just_finished = true;
  return 0;
}

void LetaBotModule::drawStats()
{
  std::set<Unit*> myUnits = Broodwar->self()->getUnits();
  Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
  std::map<UnitType, int> unitTypeCounts;
  for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
  {
    if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end())
    {
      unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
    }
    unitTypeCounts.find((*i)->getType())->second++;
  }
  int line=1;
  for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++)
  {
    Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
    line++;
  }

  //draw position of where units end up
  if( CurrentAlg == EvenSplitPath){
	  //for(unsigned int j=0; j<SCVs.size(); j++){
		//  Broodwar->drawCircleMap( SCVs[j].Intermidiate.x() , SCVs[j].Intermidiate.y() ,3,Colors::Purple,true);
	  //}
  }

}


Unit* builder = NULL;

void LetaBotModule::onUnitComplete(BWAPI::Unit *unit)
{
	//TotalSCV++;

	//Broodwar->sendText("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	//Get building worker back to work
	if( unit->getType() == BWAPI::UnitTypes::Terran_Supply_Depot ){
		BOOST_FOREACH(Unit* scv, Broodwar->self()->getUnits()){

			if(  builder == NULL){
				continue;
			}

      //for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
      //{
		if (scv->getType().isWorker() && scv->getID() == builder->getID() ) //(*i)->getOrder() != BWAPI::Orders::MiningMinerals )
        {
		  if( CurrentAlg == BuildIn){//Build in algorithm only uses closest mineral patch
	        scv->gather(closestMineral);
		  }
		  if( CurrentAlg == EvenSplit){//Build in algorithm only uses closest mineral patch
			Broodwar->printf("Builder return to work");
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == scv->getID()  ){
					Broodwar->printf("Builder already assigned?:%d",SCVs.size() );
					SCVs[j].scv->gather(SCVs[j].mineralPatch);
                  return; //already assigned
				}
			}
			Broodwar->printf("Builder should be assigned");
			AssignEvenSplit(scv);
		 }
		if( CurrentAlg == EvenSplitPath){//Build in algorithm only uses closest mineral patch
			Broodwar->printf("Builder return to work");
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == scv->getID()  ){
					Broodwar->printf("Builder already assigned?:%d",SCVs.size() );
					SCVs[j].scv->gather(SCVs[j].mineralPatch);
                    return; //already assigned
				}
			}
			Broodwar->printf("Builder should be assigned");
			AssignEvenSplitPath(scv);
		 }

		 if( CurrentAlg == Queue){//Build in algorithm only uses closest mineral patch

      //Make sure once again that this unit is not already assigned
	  for( unsigned int k=0; k<Minerals.size(); k++ ){
		  for( unsigned  int q=0; q<Minerals[k].Queue.size(); q++ ){
			  if(  Minerals[k].Queue[q]->scv->getID() == scv->getID() ){
				  Minerals[k].Queue.erase( Minerals[k].Queue.begin()+q);
			  }
	    }
	  }
	  for(unsigned int k=0; k<SCVs.size(); k++ ){
		  if( SCVs[k].scv->getID() == scv->getID() ){
			  SCVs.erase(  SCVs.begin() + k );
			  break;
		  }
	  }


			Broodwar->printf("Builder return to queue");
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == scv->getID()  ){
					Broodwar->printf("Builder already assigned Queue?:%d",SCVs.size() );
					SCVs[j].scv->gather(SCVs[j].mineralPatch);
                  return; //already assigned
				}
			}
			Broodwar->printf("Assigning builder to queue");
			QueueChoice(scv);
		 }


		 if( CurrentAlg == QueuePath){//Build in algorithm only uses closest mineral patch

      //Make sure once again that this unit is not already assigned
	  for( unsigned int k=0; k<Minerals.size(); k++ ){
		  for( unsigned  int q=0; q<Minerals[k].Queue.size(); q++ ){
			  if(  Minerals[k].Queue[q]->scv->getID() == scv->getID() ){
				  Minerals[k].Queue.erase( Minerals[k].Queue.begin()+q);
			  }
	    }
	  }
	  for(unsigned int k=0; k<SCVs.size(); k++ ){
		  if( SCVs[k].scv->getID() == scv->getID() ){
			  SCVs.erase(  SCVs.begin() + k );
			  break;
		  }
	  }


			Broodwar->printf("Builder return to queue");
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == scv->getID()  ){
					Broodwar->printf("Builder already assigned Queue?:%d",SCVs.size() );
					//reassing to queue
			        //SCVs.erase(  SCVs.begin() + j );
					//QueuePathChoice(scv);
                  return; //already assigned
				}
			}
			Broodwar->printf("Assigning builder to queue");
			QueuePathChoice(scv);
		 }


	    }//if is worker and not gathering minerals
      }

		return;
	}


    TotalSCV = 0;
    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
	  if ((*i)->getType().isWorker() )
      {
       TotalSCV++;
	  }
    }

	//from this point on, only use SCV
	if (!unit->getType().isWorker() ){
      return;
	}
	if( TotalSCV != BuildDepotAt ){//Send this SCV to mine
		if( CurrentAlg == BuildIn){//Build in algorithm only uses closest mineral patch
	        unit->gather(closestMineral);
		}
		if( CurrentAlg == EvenSplit){//send scv according to split
            //return;
	        //unit->gather(closestMineral);
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == unit->getID()  ){
				  if( unit->getOrder() != BWAPI::Orders::MiningMinerals ){
					  unit->gather( SCVs[j].mineralPatch );
				  }
                  return; //already assigned
				}
			}
			AssignEvenSplit(unit);
		}
		if( CurrentAlg == EvenSplitPath){//send scv according to split
            //return;
	        //unit->gather(closestMineral);
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == unit->getID()  ){
				  if( unit->getOrder() != BWAPI::Orders::MiningMinerals ){
					  unit->gather( SCVs[j].mineralPatch );
				  }
                  return; //already assigned
				}
			}
			AssignEvenSplitPath(unit);
		}

		if( CurrentAlg == Queue){//Send scv to best queue
            //return;
	        //unit->gather(closestMineral);
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == unit->getID()  ){
				  //Broodwar->printf("unit already assigned");
				  if( unit->getOrder() != BWAPI::Orders::MiningMinerals ){
					  unit->gather( SCVs[j].mineralPatch );
				  }
                  return; //already assigned
				}
			}
			QueueChoice(unit);
		}
		if( CurrentAlg == QueuePath){//Send scv to best queue
            //return;
	        //unit->gather(closestMineral);
			for( unsigned int j=0; j<SCVs.size(); j++ ){
				if(  SCVs[j].scv->getID() == unit->getID()  ){
				  //Broodwar->printf("unit already assigned");
				  if( unit->getOrder() != BWAPI::Orders::MiningMinerals ){
					  if( SCVs[j].interMineral == NULL ){
					    unit->gather( SCVs[j].mineralPatch );
					  } else {
					    unit->gather( SCVs[j].interMineral );
					  }
					  //reassing unit
				  }
                  return; //already assigned
				}
			}
			QueuePathChoice(unit);
		}
	} else {
		//get position of the command center, used for building supply depot?
		BWAPI::TilePosition BuildPosition;
	  for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
      {
		 if ((*i)->getType().isResourceDepot() )
        {
		  //BuildPosition = BWAPI::TilePosition((*i)->getPosition().x()-5, (*i)->getPosition().y());
			BWAPI::Position bPos = BWAPI::Position( 3664, 416);
          BuildPosition = BWAPI::TilePosition(  bPos);
          BuildPosition = BWAPI::TilePosition(  BuildPosition.x()-2 ,  BuildPosition.y() );
		  //while( ! BWAPI::Broodwar->canBuildHere( unit, BuildPosition,BWAPI::UnitTypes::Terran_Supply_Depot) ){
            //BuildPosition = BWAPI::TilePosition( BuildPosition.x()-1 ,  BuildPosition.y() );
		  //}
	    }
      }
	  Broodwar->sendText("X: %d , Y: %d" ,BuildPosition.x(),BuildPosition.y());
	  unit->build( BuildPosition, BWAPI::UnitTypes::Terran_Supply_Depot);
	  builder = unit;
	  //remove this unit from the SCVs and Mineral Queue.
	  for( unsigned int k=0; k<Minerals.size(); k++ ){
		  for( unsigned  int q=0; q<Minerals[k].Queue.size(); q++ ){
			  if(  Minerals[k].Queue[q]->scv->getID() == unit->getID() ){
				  Minerals[k].Queue.erase( Minerals[k].Queue.begin()+q);
			  }
	    }
	  }
	  for(unsigned int k=0; k<SCVs.size(); k++ ){
		  if( SCVs[k].scv->getID() == unit->getID() ){
			  SCVs.erase(  SCVs.begin() + k );
			  break;
		  }
	  }

	}
  //if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
    //Broodwar->sendText("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
    //Broodwar->sendText("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}
