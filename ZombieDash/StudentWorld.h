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

private:
    std::list<Actor*> m_actors;
    int citizen_count;
    Penelope* pene;
    
    
    
};

#endif // STUDENTWORLD_H_
