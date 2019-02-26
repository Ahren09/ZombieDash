#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include "Level.h"
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    
    //$$ CHECK P19
    virtual int move();
    virtual void cleanUp();
    bool checkAllOverlap(Actor* a, int X, int Y);
    
    void recordLevelFinishedIfAllCitizensGone()
    { gameFinished=true; }
    
    int getCitizenCount() const
    { return citizen_count; }
    
    int getZombieCount() const
    { return zombie_count; }
    
    void recordCitizenGone()
    { citizen_count--; }
    Penelope* getPenelope() const
    { return pene; }
    
    void addActor(Actor* actor)
    {
        m_actors.push_back(actor);
    }
    
    void killByFlameIfAppropriate(Flame* flame);
    
    void increaseZombieCount()
    { zombie_count++; }
    
    void decreaseZombieCount()
    { zombie_count--; }
    
    void decreaseCitizenCount()
    { citizen_count--; }
    
    
//    void killByLandmineIfAppropriate(Landmine* landmine)
//    {
//        for(list<Actor*>::iterator it=m_actors.begin();it!=m_actors.end();it++)
//        {
//            if((*it)->canKillByLandmine() && checkOverlapByTwoObjects(landmine, (*it)))
//            {
//                
//                landmine->explode((*it)->getDirection());
//            }
//        }
//        
//    }
    
    void infectByVomitIfAppropriate(Vomit* vomit);
    
    
    //Try to introduce a Flame object at specified xy location
    void introduceFlameIfAppropriate(Landmine* landmine, double x, double y, Direction dir);
    
    
    // Is creation of a flame blocked at the indicated location?
    bool isFlameBlockedAt(double x, double y) const;
    
    int computeDistance(double a_x, double a_y, double b_x, double b_y)
    {
        double d_x=abs(a_x-b_x);
        double d_y=abs(a_y-b_y);
        return d_x * d_x + d_y * d_y;
    }
    
    bool checkOverlap(double a_x, double a_y, double b_x, double b_y) const
    {
        double d_x=abs(a_x-b_x);
        double d_y=abs(a_y-b_y);
        return d_x * d_x + d_y * d_y <=100;
    }
    
    bool checkOverlapByOneObject(double x, double y, Actor* other) const
    { return checkOverlap(x,y,other->getX(), other->getY()); }
    
    bool checkOverlapByTwoObjects(Actor* me, Actor* other) const
    {  return checkOverlap(me->getX(),me->getY(), other->getX(), other->getY()); }
    
    bool getNewPositionWithDir(Direction dir, double& x, double& y);
    
    bool checkValidPosition(double& x, double& y)
    { return x>=0 && y>=0 && x<VIEW_WIDTH && VIEW_HEIGHT; }
    
    //return true if current tick is even tick
    //update tick status
    bool checkTick() const
    { return isEvenTick; }
    
    bool updateTick()
    {   isEvenTick = !isEvenTick; }
    
    bool locateNearestVomitTrigger(double x, double y, Actor* &target, double& distance);
    
    bool determineNewPosition(Direction dir, double x, double y, double distance);
    
    bool isAgentMovementBlockedAt(Agent* ag, double x, double y) const;
    
private:
    std::list<Actor*> m_actors;
    int citizen_count;
    int zombie_count;
    Penelope* pene;
    
    bool gameFinished;
    
    bool isEvenTick;
    
    
};

#endif // STUDENTWORLD_H_
