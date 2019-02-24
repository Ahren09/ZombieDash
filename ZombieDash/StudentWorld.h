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
    void setGameFinished()
    {
        gameFinished=true;
    }
    int getCitizenCount() const
    {
        return citizen_count;
    }
    Penelope* getPenelope() const
    {
        return pene;
    }
    
    void addActor(Actor* actor)
    {
        m_actors.push_back(actor);
    }
    
    const std::list<Actor*> &getAllActors()
    {
        return m_actors;
    }
    
    

private:
    std::list<Actor*> m_actors;
    int citizen_count;
    Penelope* pene;
    
    bool gameFinished;
    
    
};

#endif // STUDENTWORLD_H_
