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
    
    virtual bool canKillByLandmine() const
    {
        return false;
    }
    
    virtual bool blockFlame() const
    {
        return false;
    }
    
    virtual void useExitIfAppropriate()
    {
        
        
    }
    
    bool getNewPositionWithDir(Direction dir, const double& x, const double& y, double new_x, double new_y)
    {
        new_x=x;
        new_y=y;
        switch(dir)
        {
            case left:
                new_x=x-SPRITE_WIDTH;
                break;
            case right:
                new_x=x+SPRITE_WIDTH;
                break;
            case up:
                new_y=y+SPRITE_WIDTH;
                break;
            case down:
                new_y=y-SPRITE_WIDTH;
                break;
        }
        bool X_OutOfBound = new_x<0 || new_x >= VIEW_WIDTH;
        bool Y_OutOfBound = new_y<0 || new_y >= VIEW_HEIGHT;
        
        return X_OutOfBound || Y_OutOfBound;
        
    }
//    bool checkOverlap(Actor* other, int A, int C);
//    bool checkOverlapAnother(Actor* other);
    
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
    
    bool blockFlame(Actor* flame)
    {
        return true;
    }
    
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
    bool blockFlame(Actor* flame)
    {
        return true;
    }
    
    
    
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
    void doSomething();
    int getCount()
    {
        return active_count;
    }
    
    void decCount()
    {
        active_count--;
    }
    
private:
    int active_count;
    
};

class Vomit: public ActivatingObject
{
public:
    Vomit(StudentWorld* gw, double startX, double startY, Direction dir);
    ~Vomit();
    void doSomething();
    int getCount()
    {
        return active_count;
    }
    
    void decCount()
    {
        active_count--;
    }
    
private:
    int active_count;
    
};

class Landmine: public ActivatingObject
{
public:
    Landmine(StudentWorld* gw, double startX, double startY);
    ~Landmine();
    void doSomething();
    void explode();
    virtual void dieByFallOrBurn();
    
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
    virtual ~Goodie();
    virtual void doSomething();
    
    virtual bool canKillByFlame() const
    {
        return true;
    }
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
    
    virtual bool canKillByFlame() const
    {
        return true;
    }
    
    virtual bool canKillByLandmine() const
    {
        return true;
    }
    
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
    
    bool canInfectByVomit() const
    {
        return true;
    }
    
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
