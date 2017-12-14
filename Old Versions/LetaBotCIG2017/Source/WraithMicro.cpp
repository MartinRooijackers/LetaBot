#include "WraithMicro.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <math.h>
#include <boost/foreach.hpp>

#include "Common.h"

WraithMicro::WraithMicro(){
	Wraiths.reserve(50);

	for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
		if ( (*i)->getType()  == BWAPI::UnitTypes::Terran_Wraith ){
		  WraithStruct newWraith;
		  newWraith.currentState = IdleWraith;
		  newWraith.Wraith = (*i);
		  Wraiths.push_back( newWraith );
          //WS.push_back( (*i) );
		}
	}

}

void WraithMicro::AddWraith(Unit* unit){
	if(  unit->getType()  == BWAPI::UnitTypes::Terran_Wraith ){
		  WraithStruct newWraith;
		  newWraith.currentState = IdleWraith;
		  newWraith.Wraith = unit;
		  newWraith.stopped_frames = 0;
		  newWraith.CurrentPosition = unit->getPosition();
		  Wraiths.push_back( newWraith );
	}
}

//Wraiths should be constantly on the move
void WraithMicro::MoveWraits(){
	//Broodwar->mapHeight();
 	for(unsigned int i=0; i<Wraiths.size(); i++){

		//remove dead wraiths
		if( !Wraiths[i].Wraith->exists() ){
			Wraiths.erase( Wraiths.begin() + i );
			i--;
			continue;
		}

		if( Wraiths[i].Wraith->getPosition() == Wraiths[i].CurrentPosition ){
			Wraiths[i].stopped_frames++;
			if( Wraiths[i].stopped_frames > 60 ){ //if standing still for more then a second
              Wraiths[i].currentState = IdleWraith;
			}
		} else {
          Wraiths[i].stopped_frames = 0;
		}

		Wraiths[i].CurrentPosition = Wraiths[i].Wraith->getPosition();

		if( Wraiths[i].currentState == IdleWraith ){
          Wraiths[i].currentState = Moving;
		  Wraiths[i].MovingTo = BWAPI::Position(  rand()%(Broodwar->mapWidth()*32) , rand()%(Broodwar->mapHeight()*32) );
		  /*
		  int ranX = InfoMan->EnemyBase.x()*32 + ( rand()%(80*32) - 40*32 );
		  int ranY = InfoMan->EnemyBase.y()*32 + ( rand()%(80*32) - 40*32 );
		  if( ranX < 0 ){
			  ranX = 0;
		  }
		  if( ranX >= 128*32 ){
			  ranX = 128*32 - 1;
		  }
		  if( ranY < 0 ){
			  ranY = 0;
		  }
		  if( ranY >= 128*32 ){
			  ranY = 128*32 - 1;
		  }
		  Wraiths[i].MovingTo = BWAPI::Position(  ranX , ranY );
		  */

		  Wraiths[i].Wraith->move(  Wraiths[i].MovingTo );
		  //Broodwar->printf( "X: %d, Y:%d" , Wraiths[i].MovingTo.x(), Wraiths[i].MovingTo.y() );
		}
		//Broodwar->printf( "Current State: %d", Wraiths[i].currentState );
		if( Wraiths[i].currentState  == Moving &&  Wraiths[i].MovingTo.getDistance(  Wraiths[i].Wraith->getPosition() ) < 65 ){
			Wraiths[i].currentState = IdleWraith;
		}

		if( Wraiths[i].currentState  == Attacking &&  Wraiths[i].MovingTo.getDistance(  Wraiths[i].Wraith->getPosition() ) < 65 ){
			Wraiths[i].currentState = IdleWraith;
		}
		if( Wraiths[i].currentState  == Attacking && Wraiths[i].Wraith->getOrder() == BWAPI::Orders::None  ){
			Wraiths[i].currentState = IdleWraith;
		}
		int minDist = 99999;
		std::set<Unit*> EnemyClose = Wraiths[i].Wraith->getUnitsInRadius( 15*32 );
		Unit* toAttack = NULL;
		Unit* closeDetector = NULL;
		bool toClose = false;
		int closeMuta = 0;
		BOOST_FOREACH( Unit* close, EnemyClose )
        {
			//do not attack larva or cocoon or egg
			if( close->getType() == BWAPI::UnitTypes::Zerg_Larva || close->getType() == BWAPI::UnitTypes::Zerg_Cocoon || close->getType() == BWAPI::UnitTypes::Zerg_Egg ){
				continue;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk  ){
              closeMuta++;
			}
			if( close->getType() == BWAPI::UnitTypes::Zerg_Mutalisk && Wraiths[i].Wraith->getEnergy() > 60 ){
			  Wraiths[i].Wraith->useTech( BWAPI::TechTypes::Cloaking_Field);
			}
			if( close->getPlayer() ==  Broodwar->enemy()  ){//&& !close->getType().isBuilding()
				if( Wraiths[i].Wraith->getPosition().getDistance( close->getPosition() ) < minDist ){
                  minDist = Wraiths[i].Wraith->getPosition().getDistance( close->getPosition() );
				  toAttack = close;
				}
				if( close->getType().isDetector() ){
					closeDetector = close;
				  if( Wraiths[i].Wraith->getPosition().getDistance( close->getPosition() ) < 32*11 ){
                    toClose = true;
				  }
				}
		       //Broodwar->printf( "Enemy found" );
			}
		}
		if(  toAttack == NULL ){
		  BOOST_FOREACH( Unit* close, EnemyClose )
          {
			//do not attack larva or cocoon or egg
			if( close->getType() == BWAPI::UnitTypes::Zerg_Larva || close->getType() == BWAPI::UnitTypes::Zerg_Cocoon || close->getType() == BWAPI::UnitTypes::Zerg_Egg || close->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony ){
				continue;
			}
			if( close->getPlayer() ==  Broodwar->enemy() && close->getType().isBuilding() ){
				if( Wraiths[i].Wraith->getPosition().getDistance( close->getPosition() ) < minDist ){
                  minDist = Wraiths[i].Wraith->getPosition().getDistance( close->getPosition() );
				  toAttack = close;
				}
		       //Broodwar->printf( "Enemy found" );
			}
		  }
		}
		if( toAttack != NULL ){
			//Broodwar->drawCircle(0, toAttack->getPosition().x(),toAttack->getPosition().y(),1, BWAPI::Colors::Red );
			Wraiths[i].currentState = Attacking;
			bool canAttack = false;
			if( toAttack->getType().isFlyer() && Wraiths[i].Wraith->getAirWeaponCooldown() == 0  ){
				canAttack = true;
			}
			if( !toAttack->getType().isFlyer() && Wraiths[i].Wraith->getGroundWeaponCooldown() == 0  ){
				canAttack = true;
			}
			/*
			if( minDist < (toAttack->getType().airWeapon().maxRange()+3*32) ){ //enemy too close
				canAttack = false;
			}
			*/
			
			if(  closeMuta == 0 && toClose ){
				toClose = false;
			}			
			if(  closeMuta > 1 &&  !Wraiths[i].Wraith->isCloaked()  ){
				toClose = true;
			}
			bool SporeClose = false;
	        BOOST_FOREACH( Unit* unit, EnemyClose ){
				//check if spore colony is to close
				if(  unit->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony && Wraiths[i].Wraith->getPosition().getDistance( unit->getPosition() ) < 10*32 ){
			      SporeClose = true;
			   }
		    }
			if( canAttack && !SporeClose && !toClose ){ // !Wraiths[i].Wraith->isDetected()
				Wraiths[i].Wraith->attack( toAttack );
				BWAPI::Broodwar->drawLineMap(Wraiths[i].Wraith->getPosition().x(), Wraiths[i].Wraith->getPosition().y(), 
					toAttack->getPosition().x(), toAttack->getPosition().y(), BWAPI::Colors::Red);
			} else if( !canAttack && !SporeClose && !toClose  ) {
				BWAPI::Position fleePosition( (Wraiths[i].Wraith->getPosition() - toAttack->getPosition())+(Wraiths[i].Wraith->getPosition() - toAttack->getPosition()) + Wraiths[i].Wraith->getPosition());
				BWAPI::Position attackPosition( toAttack->getPosition() );


		     //BWAPI::Broodwar->drawLineMap(Wraiths[i].Wraith->getPosition().x(), Wraiths[i].Wraith->getPosition().y(), 
			//fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);
			  //Wraiths[i].Wraith->move(fleePosition);
			  //Wraiths[i].MovingTo = fleePosition;
			  Wraiths[i].Wraith->move(attackPosition);
			  Wraiths[i].MovingTo = attackPosition;
			} else if( SporeClose || !canAttack || toClose ){
				BWAPI::Position fleePosition( (Wraiths[i].Wraith->getPosition() - toAttack->getPosition())+(Wraiths[i].Wraith->getPosition() - toAttack->getPosition()) + Wraiths[i].Wraith->getPosition());
				//BWAPI::Position attackPosition( toAttack->getPosition() );
		     BWAPI::Broodwar->drawLineMap(Wraiths[i].Wraith->getPosition().x(), Wraiths[i].Wraith->getPosition().y(), 
			fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);
			  Wraiths[i].Wraith->move(fleePosition);
			  Wraiths[i].MovingTo = fleePosition;
			  Wraiths[i].currentState = Moving;
			}

		}
		
		//if ( Wraiths[i].Wraith->getUnitsInRadius( 40 );
	}
	//Broodwar->enemy();
}