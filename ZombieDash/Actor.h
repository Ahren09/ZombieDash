#ifndef ACTOR_H
#define ACTOR_H

#include "GraphObject.h"
#include "GameConstants.h"

/*
 GraphObject(int imageID, double startX, double startY, int startDirection = 0, int depth = 0);
 double getX() const;                               // in pixels (0-255)
 double getY() const;                               // in pixels (0-255)
 virtual void moveTo(double x, double y);           // in pixels (0-255)
 int getDirection() const;                          // in degrees (0-359)
 void setDirection(Direction d);                    // {up, down, left, right}
 */
class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* gw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
    virtual ~Actor()=default;
    virtual void doSomething()=0;
    
    StudentWorld* getWorld() const
    {
        return m_world;
    }
    
    virtual void setDead()
    {
        aliveStatus=false;
    }
    
    bool isAlive() const
    {
        return aliveStatus;
    }
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    
    virtual bool canKillByFlame() const
    {
        return false;
    }
    
    virtual bool canInfectByVomit() const
    {
        return false;
    }
    
    virtual bool canKillByLandmine()
    {
        return false;
    }
    
    bool checkOverlap(Actor* other, int A, int C);
    bool checkOverlapAnother(Actor* other);
    
private:
    StudentWorld* m_world;
    bool aliveStatus; //YES, aliveStatus should be valid for Pene / citizen / zombie, even pick-ups
};





class Wall: public Actor
{
public:
    Wall(StudentWorld* gw, double startX, double startY);
    virtual void doSomething();
    virtual void setDead();
    
};


//##########################
// MARK: - Activating Objects
//##########################



class ActivatingObject: public Actor
{
public:
    ActivatingObject(StudentWorld* gw, int id, double startX, double startY, Direction dir = 0, int depth = 0);
    virtual ~ActivatingObject();
    virtual void doSomething();
};

class Exit: public ActivatingObject
{
public:
    Exit(StudentWorld* gw, double startX, double startY);
    ~Exit();
    void doSomething();
    
    
};

class Pit: public ActivatingObject
{
public:
    Pit(StudentWorld* gw, double startX, double startY);
    ~Pit();
    void kill();
    void doSomething();
    
};

class Flame: public ActivatingObject
{
public:
    Flame(StudentWorld* gw, double startX, double startY, Direction dir);
    ~Flame();
    
};

class Vomit: public ActivatingObject
{
public:
    Vomit(StudentWorld* gw, double startX, double startY, Direction dir);
    ~Vomit();
    
};

class Landmine: public ActivatingObject
{
public:
    Landmine(StudentWorld* gw, double startX, double startY);
    ~Landmine();
    
};

//*************
// MARK: Goodie

class Goodie: public ActivatingObject
{
public:
    Goodie(StudentWorld* gw, int imageID, double startX, double startY);
    virtual ~Goodie();
    virtual void doSomething();
};

class VaccineGoodie: public Goodie
{
public:
    VaccineGoodie(StudentWorld* gw, double startX, double startY);
    ~VaccineGoodie();
    void doSomething();
    
};

class GasCanGoodie: public Goodie
{
public:
    GasCanGoodie(StudentWorld* gw, double startX, double startY);
    ~GasCanGoodie();
    void doSomething();
};

class LandmineGoodie: public Goodie
{
public:
    LandmineGoodie(StudentWorld* gw, double startX, double startY);
    ~LandmineGoodie();
    void doSomething();
};

//##########################
// MARK: - Agent
//##########################

//Agent is all movable objects
class Agent: public Actor
{
public:
    Agent(StudentWorld* gw, int imageID, double startX, double startY);
    virtual ~Agent();
    virtual void dieByFallOrBurn()=0;
    
};

//*************
// MARK: Human
class Human: public Agent
{
public:
    Human(StudentWorld* gw, int imageID, double startX, double startY);
    virtual ~Human();
    virtual void doSomething();
    bool getInfectedStatus() const
    {
        return infectedStatus;
    }
    int getInfectionCount() const
    {
        return infectionCount;
    }
    void increaseInfectionCount()
    {
        infectionCount++;
    }
    void clearInfectedStatus()
    {
        infectionCount=0;
        infectedStatus=false;
    }
    virtual void setDead();
    
private:
    int infectionCount;
    bool infectedStatus;
    
};



class Penelope: public Human
{
public:
    Penelope(StudentWorld* gw, double startX, double startY);
    ~Penelope();
    virtual void doSomething();
    void fire(Direction dir); //Fire flame
    
    
    void addMine();
    void useVaccine();
    void increaseVaccines()
    {
        vaccine_count++;
    }
    
    // Increase the number of flame charges the object has.
    void increaseFlameCharges()
    {
        flame_count++;
    }
    
    // Increase the number of landmines the object has.
    void increaseLandmines()
    {
        mine_count++;
    }
    
    // How many vaccines does the object have?
    int getNumVaccines() const
    {
        return vaccine_count;
    }
    
    // How many flame charges does the object have?
    int getNumFlameCharges() const
    {
        return flame_count;
    }
    
    // How many landmines does the object have?
    int getNumLandmines() const
    {
        return mine_count;
    }
    
    void useFlame();
    
    void useLandmine();
    
private:
    int flame_count;
    int mine_count;
    int vaccine_count;
    
};



class Citizen: public Human
{
public:
    Citizen(StudentWorld* gw, double startX, double startY);
    ~Citizen();
    void setDead();
    void dieOfInfection();
    void doSomething();
};


//*************
// MARK: Zombie

class Zombie: public Agent
{
public:
    Zombie(StudentWorld* gw, double startX, double startY);
    virtual ~Zombie();
    virtual void doSomething();
    virtual void setDead()
    {
        Actor::setDead();
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
        
    }
    
    
};

class DumbZombie: public Zombie
{
public:
    DumbZombie(StudentWorld* gw, double startX, double startY);
    ~DumbZombie();
    void doSomething();
    
};

class SmartZombie: public Zombie
{
public:
    SmartZombie(StudentWorld* gw, double startX, double startY);
    ~SmartZombie();
    void doSomething();
    
};



#endif // ACTOR_H
