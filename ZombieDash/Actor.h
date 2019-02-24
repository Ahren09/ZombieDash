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
    
    bool checkOverlap(Actor* other, int A, int C);
    
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
    
};

class Exit: public ActivatingObject
{
public:
    Exit(StudentWorld* gw, double startX, double startY);
    ~Exit();
    
};

class Pit: public ActivatingObject
{
public:
    Pit(StudentWorld* gw, double startX, double startY);
    ~Pit();
    
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
};

class VaccineGoodie: public Goodie
{
public:
    VaccineGoodie(StudentWorld* gw, double startX, double startY);
    ~VaccineGoodie();
    
    
};

class GasCanGoodie: public Goodie
{
public:
    GasCanGoodie(StudentWorld* gw, double startX, double startY);
    ~GasCanGoodie();
    
};

class LandmineGoodie: public Goodie
{
public:
    LandmineGoodie(StudentWorld* gw, double startX, double startY);
    ~LandmineGoodie();
    
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
    
    
};

//*************
// MARK: Human
class Human: public Agent
{
public:
    Human(StudentWorld* gw, int imageID, double startX, double startY);
    virtual ~Human();
};

class Penelope: public Human
{
public:
    Penelope(StudentWorld* gw, double startX, double startY);
    ~Penelope();
    virtual void doSomething();
    
private:
    int flamethower_count;
    int mine_count;
    int vaccine_count;
    int infectionCount;
    bool infectedStatus;
    
};

class Citizen: public Human
{
public:
    Citizen(StudentWorld* gw, double startX, double startY);
    ~Citizen();
};


//*************
// MARK: Zombie

class Zombie: public Agent
{
public:
    Zombie(StudentWorld* gw, double startX, double startY);
    virtual ~Zombie();
    
    
};

class DumbZombie: public Zombie
{
public:
    DumbZombie(StudentWorld* gw, double startX, double startY);
    ~DumbZombie();
    
};

class SmartZombie: public Zombie
{
public:
    SmartZombie(StudentWorld* gw, double startX, double startY);
    ~SmartZombie();
    
};



#endif // ACTOR_H
