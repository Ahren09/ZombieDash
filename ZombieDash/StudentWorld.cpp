#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    citizen_count=0;
    zombie_count=0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
    
}

bool determineNewPosition(Direction dir, double& x, double& y, double distance)
{
    
    switch(dir)
    {
        case GraphObject::left:
            x-=distance;
            break;
        case GraphObject::right:
            x+=distance;
            break;
        case GraphObject::up:
            y+=distance;
            break;
        case GraphObject::down:
            y-=distance;
            break;
    }
    bool X_OutOfBound = x<0 || x >= VIEW_WIDTH;
    bool Y_OutOfBound = y<0 || y >= VIEW_HEIGHT;
    
    return X_OutOfBound || Y_OutOfBound;
    
}

//Check whether the new location is still in StudentWorld
//Returns false if x y coordinates are out of bound, with x,y remaining unchanged.
//Returns true otherwise, and set x,y to new values
bool StudentWorld::getNewPositionWithDir(Direction dir, double& x, double& y)
{
    if( dir==GraphObject::left || dir == GraphObject::right )
        return determineNewPosition(dir, x, y, SPRITE_WIDTH);
    else return determineNewPosition(dir, x, y, SPRITE_HEIGHT);
}


int StudentWorld::init()
{
    Level lev(assetPath());
    //Create string containing name of file
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    oss<<"level";
    oss.fill('0');
    oss<<setw(2)<<getLevel();
   
    oss<<".txt";
    string levelFile=oss.str();
    
    Level::LoadResult result=lev.loadLevel(levelFile);
    if(result == Level::load_fail_file_not_found)
        cerr<<"Cannot find level01.txt data file"<<endl;
    else if(result == Level::load_fail_bad_format)
        cerr<<"Your level was improperly formatted"<<endl;
    else if (result == Level::load_success)
    {
        for(int y=0;y<LEVEL_HEIGHT;y++)
        {
            for(int x=0;x<LEVEL_WIDTH;x++)
            {
                Level::MazeEntry ge = lev.getContentsOf(x, y);
                switch(ge){
                    case Level::wall:{
                        Actor *actor = new Wall(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                    }break;
                        
                    case Level::player:
                    {
                        pene = new Penelope(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
                    }break;
                    
                    case Level::citizen:{
                        Actor* actor = new Citizen(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                    
                    case Level::pit:{
                        Actor* actor = new Pit(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    case Level::vaccine_goodie:{
                        Actor* actor = new VaccineGoodie(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    case Level::gas_can_goodie:{
                        Actor* actor = new GasCanGoodie(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    case Level::landmine_goodie:{
                        Actor* actor = new LandmineGoodie(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    case Level::exit:{
                        Actor* actor = new Exit(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    case Level::dumb_zombie:{
                        Actor* actor = new DumbZombie(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    case Level::smart_zombie:{
                        Actor* actor = new SmartZombie(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT);
                        m_actors.push_back(actor);
                        
                    }break;
                        
                    
                    
                }
                
                
            }
            
        }
        
        
        
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

//##########################
//MARK: - Move
//##########################

int StudentWorld::move()
{
    updateTick();
    
    //move all Actors in a for-loop
    if(getLives()>0)
        pene->doSomething();
    for(list<Actor*>::iterator it=m_actors.begin(); it!=m_actors.end(); it++)
    {
        if((*it)->isAlive())
            (*it)->doSomething();
        
        //Penelope dies
        if(getLives()==0)
        {
            return GWSTATUS_PLAYER_DIED;
        }
        
        
         //if citizen Penelope has escaped
         if(gameFinished)
         {
             playSound(SOUND_LEVEL_FINISHED);
             return GWSTATUS_FINISHED_LEVEL;
         }
        
    }
    for(list<Actor*>::iterator it=m_actors.begin(); it!=m_actors.end(); it++)
    {
        //Actor dies
        if( ! (*it)->isAlive())
        {
            delete *it;
            *it=nullptr;
            m_actors.erase(it);
        }
    }
    
    
    
   
    
    return GWSTATUS_CONTINUE_GAME;
    
}

//Check whether the given position is in StudentWorld
//AND NOT being blocked by an exit or a Wall
bool StudentWorld::isFlameBlockedAt(double x, double y) const
{
    bool flag=false;
    for(list<Actor*>::const_iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it)->blocksFlame() && checkOverlapByOneObject(x, y, (*it)))
            flag=true;
    }
    return flag;
}

//
void StudentWorld::activateOnAppropriateActors(Actor* a)
{
    for(list<Actor*>::iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        a->activateIfAppropriate(*it);
    }
}

void StudentWorld::introduceFlameIfAppropriate(double x, double y)
{
    if(!isFlameBlockedAt(x, y))
    {
        addActor(new Flame(this, x,y, Actor::up));
    }
}

void StudentWorld::useExit(Exit* exit)
{
    pene->useExitIfAppropriate(exit);
    for(list<Actor*>::iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        (*it)->useExitIfAppropriate(exit);
    }
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, Actor* &target, double& distance)
{
    double dist;
    distance=INT_MAX;
    double target_x=INT_MAX,target_y=INT_MAX;
    for(list<Actor*>::const_iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it)->canInfectByVomit())
        {
            target_x=(*it)->getX();
            target_y=(*it)->getY();
            dist=(x-target_x)*(x-target_x)+(y-target_y)*(y-target_y);
            if(distance>dist)
            {
                distance=dist;
                target=(*it);
            }
        }
    }
    return distance<=100;
    
}

bool StudentWorld::locateNearestCitizenTrigger(double zombie_x, double zombie_y, double& target_x, double& target_y, Actor* target, double& distance, bool& isThreat) const
{
    double dist;
    distance=INT_MAX;
    
    for(list<Actor*>::const_iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it)->canInfectByVomit())
        {
            double x=(*it)->getX();
            double y=(*it)->getY();
            dist=(zombie_x-x)*(zombie_x-x)+(zombie_y-y)*(zombie_y-y);
            if(distance>dist)
            {
                distance=dist;
                target=(*it);
                target_x=x;
                target_y=y;
            }
        }
    }
    return distance<=6400;
}

//Locate nearest Zombie to the Citizen
//Called by Citizen
//Returns true if there is zombie closer than 80 pixels
bool StudentWorld::locateNearestCitizenThreat(double citizen_x, double citizen_y, double& otherX, double& otherY, double& dist_z)
{
    double dist;
    dist_z=INT_MAX;
    double x=INT_MAX, y=INT_MAX;
    for(list<Actor*>::iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it)->isZombie())
        {
            
            x=(*it)->getX();
            y=(*it)->getY();
            
            double distance=(citizen_x-x)*(citizen_x-x)+(citizen_y-y)*(citizen_y-y);
            if(distance<dist)
            {
                dist_z=distance;
                otherX=x;
                otherY=y;
            }
        }
    }
    return dist_z<=6400;
    
}

bool StudentWorld::isAgentMovementBlockedAt(Agent* ag, double x, double y) const
{
    for(list<Actor*>::const_iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it)->blocksAgent() && checkOverlapByOneObject(x, y,(*it)) && (*it)!=ag)
        {
            return true;
        }
    }
    return false;
    
}

//##########################
//MARK: - cleanUp
//##########################

void StudentWorld::cleanUp()
{
        for(std::list<Actor*>::iterator it=m_actors.begin();it != m_actors.end();it++)
        {
            delete *it;
            (*it) = nullptr;
        }
        m_actors.erase(m_actors.begin(),m_actors.end());
        delete pene;
        pene=nullptr;
}
