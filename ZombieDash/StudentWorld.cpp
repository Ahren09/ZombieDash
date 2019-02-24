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
    int citizen_count=0;
    
}

StudentWorld::~StudentWorld()
{
    cleanUp();
    
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

int StudentWorld::move()
{
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
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        
        /*
         //if citi / Pene escape
         if(citizen_count==0 && pene->getX()== && pene->getY()==)
         {
         playSound(SOUND_LEVEL_FINISHED);
         return GWSTATUS_FINISHED_LEVEL;
         }
         */
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

//Check whether a - the Actor passed in - overlap with a specific position
bool StudentWorld::checkAllOverlap(Actor* a, int X, int Y)
{
    for(list<Actor*>::iterator it=m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it) == a) continue;
        if(a->checkOverlap(*it, X, Y))
            return true;
    }
    return false;
    
    
}
