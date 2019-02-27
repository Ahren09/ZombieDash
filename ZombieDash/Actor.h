#ifndef ACTOR_H
#define ACTOR_H

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;
class Goodie;
class Penelope;
class Exit;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* gw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
    virtual void doSomething()=0;
    
    StudentWorld* getWorld() const
    { return m_world; }
    
    bool isAlive() const
    { return aliveStatus; }
    
    virtual void setDead()
    { aliveStatus=false; }
    
    // If this is an activated object, perform its effect on a (e.g., for an
    // Exit have a use the exit).
    virtual void activateIfAppropriate(Actor* a);
    
    // If this object uses exits, use the exit.
    virtual void useExitIfAppropriate(Exit* exit){}
    
    // If this object can die by falling into a pit or burning, die.
    virtual void dieByFallOrBurnIfAppropriate();
    
    // If this object can be infected by vomit, get infected.
    virtual void beVomitedOnIfAppropriate();
    
    // If this object can pick up goodies, pick up g
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    // Does this object block agent movement?
    virtual bool blocksMovement() const;
    
    virtual bool canKillByFlame() const
    { return true; }
    
    virtual bool canKillByPit() const
    { return false; }
    
    virtual bool canInfectByVomit() const
     { return false; }
    
    virtual void setInfectionStatus();
    
    // Does this object block flames?
    virtual bool blocksFlame() const
    { return false; }
    
    virtual bool blocksAgent() const
    { return false; }
    
    // Can this object cause a zombie to vomit?
    virtual bool triggersZombieVomit() const;
    
    // Does this object trigger landmines only when they're active?
    virtual bool triggersOnlyActiveLandmines() const;
    
    virtual void isZombie()
    { return true; }
    
    
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    
private:
    
    StudentWorld* m_world;
    bool aliveStatus; //YES, aliveStatus should be valid for Pene / citizen / zombie, even pick-ups
};





class Wall: public Actor
{
public:
    Wall(StudentWorld* gw, double startX, double startY);
    virtual void doSomething();
    virtual void setDead(){}
    
    bool blocksFlame(Actor* flame) const
    { return true; }
    
    virtual bool canKillByFlame() const
    { return false; }
    
    virtual bool blocksAgent() const
    { return true; }
};

//##########################
// MARK: - Activating Objects
//##########################



class ActivatingObject: public Actor
{
public:
    ActivatingObject(StudentWorld* gw, int id, double startX, double startY, Direction dir = 0, int depth = 0);
};

class Exit: public ActivatingObject
{
public:
    Exit(StudentWorld* gw, double startX, double startY);
    void doSomething();
    bool blocksFlame(Actor* flame) const
    { return true; }
    virtual void activateIfAppropriate(Actor* a);
    
    virtual bool canKillByFlame() const
    { return false; }
    
    
};

class Pit: public ActivatingObject
{
public:
    Pit(StudentWorld* gw, double startX, double startY);
    void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    
    virtual bool canKillByFlame() const
    { return false; }
    
    //void killByPitIfAppropriate(Actor* a);
    
};

class Flame: public ActivatingObject
{
public:
    Flame(StudentWorld* gw, double startX, double startY, Direction dir);
    void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    int getCount()
    { return active_count; }
    void decCount()
    { active_count--; }
    virtual bool canKillByFlame() const
    { return false; }
    
    void killByFlameIfAppropriate(Actor* a);
    
private:
    int active_count;
};

class Vomit: public ActivatingObject
{
public:
    Vomit(StudentWorld* gw, double startX, double startY, Direction dir);
   
    void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    int getCount()
    { return active_count; }
    void decCount()
    { active_count--; }
    
    void infectByVomitIfAppropriate(Actor* a);
    
private:
    int active_count;
    
};

class Landmine: public ActivatingObject
{
public:
    Landmine(StudentWorld* gw, double startX, double startY);
    
    void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    void explode();
    virtual void dieByFallOrBurnIfAppropriate(); //Can call explode()
    
private:
    //return activation status and count down according to ticks
    bool activationCountDown()
    {
        if(activation_count>0)
            activation_count--;
        else activation_status=true;
        return activation_status;
    }
    int activation_count;
    bool activation_status;
};

//*************
// MARK: Goodie

class Goodie: public ActivatingObject
{
public:
    Goodie(StudentWorld* gw, int imageID, double startX, double startY);
    
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void pickUp(Penelope* p);
    
    virtual bool canKillByLandmine() const
    { return true; }
    
};

class VaccineGoodie: public Goodie
{
public:
    VaccineGoodie(StudentWorld* gw, double startX, double startY);
    
    void doSomething();
    void pickUp(Penelope* p);
    
};

class GasCanGoodie: public Goodie
{
public:
    GasCanGoodie(StudentWorld* gw, double startX, double startY);
    
    void doSomething();
    void pickUp(Penelope* p);
    
};

class LandmineGoodie: public Goodie
{
public:
    LandmineGoodie(StudentWorld* gw, double startX, double startY);
    
    void doSomething();
    void pickUp(Penelope* p);
};

//##########################
// MARK: - Agent
//##########################

//Agent is all movable objects
class Agent: public Actor
{
public:
    Agent(StudentWorld* gw, int imageID, double startX, double startY);
    virtual void dieByFallOrBurn()=0;
    virtual bool blocksMovement() const;
    virtual bool triggersOnlyActiveLandmines() const;
    
    virtual bool canKillByPit() const
    { return true; }
    
    virtual bool blocksAgent() const
    { return true; }
    
    
    
};

//*************
// MARK: Human
class Human: public Agent
{
public:
    Human(StudentWorld* gw, int imageID, double startX, double startY);
    
    virtual void doSomething();
    bool getInfectedStatus() const
    { return infectedStatus; }
    int getInfectionCount() const
    { return infectionCount; }
    void setInfectionStatus()
    { infectedStatus=true; }
    
    virtual void clearInfection()
    {
        infectionCount=0;
        infectedStatus=false;
    }
    
    virtual void useExitIfAppropriate(Exit* exit);
    
    virtual void beVomitedOnIfAppropriate();
    
    virtual bool canInfectByVomit() const
    { return true; }
    
private:
    int infectionCount;
    bool infectedStatus;
    
};



class Penelope: public Human
{
public:
    Penelope(StudentWorld* gw, double startX, double startY);
    
    virtual void doSomething();
    void fire(Direction dir); //Fire flame
    
    virtual void dieByFallOrBurnIfAppropriate()
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    // Increase the number of vaccines the object has.
    void increaseVaccines()
    { vaccine_count++; }
    
    // Increase the number of flame charges the object has.
    void increaseFlameCharges()
    { flame_count+=5; }
    
    // Increase the number of landmines the object has.
    void increaseLandmines()
    { mine_count+=2; }
    
    // How many vaccines does the object have?
    int getNumVaccines() const
    { return vaccine_count; }
    
    // How many flame charges does the object have?
    int getNumFlameCharges() const
    { return flame_count; }
    
    // How many landmines does the object have?
    int getNumLandmines() const
    { return mine_count; }
    
    //
    void fireGasCan();
    
    void useLandmine();
    
    void useVaccine();
    
    void useExitIfAppropriate(Exit* exit);
    
    
    
private:
    int flame_count;
    int mine_count;
    int vaccine_count;
    
};



class Citizen: public Human
{
public:
    Citizen(StudentWorld* gw, double startX, double startY);
    virtual void doSomething();
    void useExitIfAppropriate(Exit* exit);
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void clearInfection(){}
    
    virtual void turnIntoZombie();
    
    virtual void moveToNewPosition();
    
    //Returns true if Citizen is on the same row/col as Penelope
    bool pickDirection(double x, double y, double target_x, double target_y,std::vector<Direction> direction_pool);
    
private:
    virtual void 
};


//*************
// MARK: Zombie

class Zombie: public Agent
{
public:
    Zombie(StudentWorld* gw, double startX, double startY);
    
    virtual void computeVomitPosition(double& x,double& y);
    virtual bool vomitIfAppropriate(const double& x, const double& y);
    virtual void doSomething();
    virtual void isZombie()
    { return true; }
    
    virtual void dieByFallOrBurnIfAppropriate()
    {
        setDead();
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
    }
    
    
    virtual void moveToNewPosition();
    
    int getMoves()
    { return moves; }
    
    void setMoves(int n)
    { moves = n; }
    
    void decreaseMoves()
    { moves--; }
    
    virtual void setNewMoves()
    { int moves=randInt(3,10); }
    
    virtual void setNewDirection();
    
private:
    int moves;
};

class DumbZombie: public Zombie
{
public:
    DumbZombie(StudentWorld* gw, double startX, double startY);
    //void doSomething();
    virtual void dieByFallOrBurnIfAppropriate()
    {
        Zombie::dieByFallOrBurnIfAppropriate();
        dropVaccineByChance(getX(), getY());
        getWorld()->increaseScore(1000);
        
    }
    void dropVaccineByChance(const double x, const double y);
    
};

class SmartZombie: public Zombie
{
public:
    SmartZombie(StudentWorld* gw, double startX, double startY);
    //void doSomething();
    
    virtual void dieByFallOrBurnIfAppropriate()
    {
        Zombie::dieByFallOrBurnIfAppropriate();
        getWorld()->increaseScore(2000);
    }
    virtual void setNewDirection();
    
    Direction pickDirection(double x, double y, double target_x, double target_y);
    
};



#endif // ACTOR_H
