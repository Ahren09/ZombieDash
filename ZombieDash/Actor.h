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
    Actor(StudentWorld* gw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0);
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

// MARK: - Activating Objects




class ActivatingObject: public Actor
{
    
    
};

class Exit: public ActivatingObject
{
    
    
};

class Pit: public ActivatingObject
{
    
    
};

class Flame: public ActivatingObject
{
    
    
};

class Vomit: public ActivatingObject
{
    
    
};

class Landmine: public ActivatingObject
{
    
    
};

// MARK: Goodie
class Goodie: public ActivatingObject
{
    
    
};

class VaccineGoodie: public Goodie
{
    
    
};

class GasCanGoodie: public Goodie
{
    
    
};

class LandmineGoodie: public Goodie
{
    
    
};


// MARK: - Agent

//Agent is all movable objects
class Agent: public Actor
{
    
    
    
};

class Human: public Agent
{
    
};

class Penelope: public Human
{
public:
    Penelope(StudentWorld* gw, double startX, double startY);
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
    
    
};



class Zombie: public Agent
{
    
    
};

class DumbZombie: public Zombie
{
    
    
};

class SmartZombie: public Zombie
{
    
    
};



#endif // ACTOR_H
