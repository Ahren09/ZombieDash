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
    virtual int move();
    virtual void cleanUp();
    
    int getCitizenCount() const
    { return citizen_count; }
    void decreaseCitizenCount()
    { citizen_count--; }
    
    int getZombieCount() const
    { return zombie_count; }
    void increaseZombieCount()
    { zombie_count++; }
    void decreaseZombieCount()
    { zombie_count--; }
    
    Penelope* getPenelope() const
    { return pene; }
    void recordLevelFinishedIfAllCitizensGone()
    { gameFinished=true; }
    
    void addActor(Actor* actor)
    { m_actors.push_back(actor); }
    
    //Check whether current tick is even tick
    bool checkTick() const
    { return isEvenTick; }
    
    // For each actor overlapping a, activate a if appropriate.
    void activateOnAppropriateActors(Actor* a);
    
    // Try to introduce a Flame object with specified coordinates
    void introduceFlameIfAppropriate(double x, double y);
    
    // Check whether creation of a flame is blocked at the indicated location
    bool isFlameBlockedAt(double x, double y) const;
    
    // Compute distance between two points
    int computeDistance(double a_x, double a_y, double b_x, double b_y) const;
    
    //Check whether the two Objects overlap
    bool checkOverlap(double a_x, double a_y, double b_x, double b_y) const;
    
    bool checkOverlapByOneObject(double x, double y, Actor* other) const
    { return checkOverlap(x,y,other->getX(), other->getY()); }
    
    bool checkOverlapByTwoObjects(Actor* me, Actor* other) const
    {  return checkOverlap(me->getX(),me->getY(), other->getX(), other->getY()); }
    
    // Check whether the specified position is out of bound
    bool checkValidPosition(double& x, double& y)
    { return x>=0 && y>=0 && x<VIEW_WIDTH && VIEW_HEIGHT; }
    
    // Called by Zombie
    // Locate nearest Citizen or Penelope that can cause zombie to vomit. Returns true if Zombie is triggered to vomit
    bool locateNearestVomitTrigger(double x, double y, Actor* &target, double& distance);
    
    // Called by Zombie
    // Locate nearest Citizen or Penelope that can cause a Zombie to move closer. Returns true if Zombie decides to move along this direction
    bool locateNearestCitizenTrigger(double zombie_x, double zombie_y, double& target_x, double& target_y, Actor* target, double& distance, bool& isThreat) const;
    
    // Called by Citizen
    // Locate nearest Zombie that can cause Citizen to move. Returns true if the nearest Zombie is within 80px
    bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance);
    
    // Determine whether an attempted move is blocked
    bool isBlocked(double a_x, double a_y, double c_x, double c_y) const;
    
    // Returns true if there is some objects blocking the movement of the Agent
    bool isAgentMovementBlockedAt(Agent* ag, double x, double y) const;

    // Attempt each Actor to use exit if appropriate
    void useExit(Exit* exit);
    
    //Called by Citizen
    //Get Citizen's distance to Penelope, set p_x and p_y to Penelope's position
    //Returns true if distance less than 80px
    bool getPenelopeDist(double x, double y, double& p_x, double& p_y, double& dist_p);
    
    
    void triggerLandmineIfAppropriate(Landmine* landmine);
    
    void writeStatus();
    
private:
    std::list<Actor*> m_actors;
    int citizen_count;
    int zombie_count;
    Penelope* pene;
    bool gameFinished;
    bool isEvenTick;
};

#endif // STUDENTWORLD_H_
