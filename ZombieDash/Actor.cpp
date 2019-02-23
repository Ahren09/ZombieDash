#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* gw, int imageID, double startX, double startY,
             Direction dir, int depth, double size)
:GraphObject(imageID, startX, startY, dir, depth, size),
m_world(gw),aliveStatus(true)
{};



//Move to 4 directions
void Actor::moveUp()
{
    setDirection(up);
    if(!getWorld()->checkAllOverlap(this,getX(),getY()+4))
        moveTo(getX(),getY()+4);
    
}

void Actor::moveDown()
{
    setDirection(down);
    if(!getWorld()->checkAllOverlap(this,getX(),getY()-4))
        moveTo(getX(),getY()-4);
    
}

void Actor::moveLeft()
{
    setDirection(left);
    if(!getWorld()->checkAllOverlap(this,getX()-4,getY()))
        moveTo(getX()-4,getY());
    
}

void Actor::moveRight()
{
    setDirection(right);
    if(!getWorld()->checkAllOverlap(this,getX()+4,getY()))
        moveTo(getX()+4,getY());
    
}

//To check whether two Actors overlap, we simply check whether vertices of the other Actor is contained
//in scope of this Actor
bool Actor::checkOverlap(Actor* other, int A, int C)
{
    int B=A+SPRITE_WIDTH-1, D=C+SPRITE_HEIGHT-1;
    int A_=other->getX(), B_=other->getX()+SPRITE_WIDTH-1, C_=other->getY(), D_=other->getY()+SPRITE_HEIGHT-1;
    
    return ((A <= A_ && A_ <= B && C <= C_ && C_ < D) ||
            (A <= B_ && B_ <= B && C <= C_ && C_ < D) ||
            (A <= A_ && A_ <= B && C <= D_ && D_ < D) ||
            (A <= B_ && B_ <= B && C <= D_ && D_ < D));
}


//Mark: - Penelope
//Pass size here?
Penelope::Penelope(StudentWorld* gw, double startX, double startY)
:Actor(gw,IID_PLAYER, startX, startY, right, 0, 1),
flamethower_count(0),mine_count(0),vaccine_count(0),infectedStatus(false),infectionCount(0)
{}

void Penelope::doSomething()
{
    //If infection Count reach the threshold
    if(infectionCount >= 500)
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    
    
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch){
                
                //move actions
            case KEY_PRESS_UP:
                moveUp();
                break;
            case KEY_PRESS_DOWN:
                moveDown();
                break;
                
            case KEY_PRESS_LEFT:
                moveLeft();
                break;
                
            case KEY_PRESS_RIGHT:
                moveRight();
                break;
                
            
                
        }
    }
}


//Mark: Wall

Wall::Wall(StudentWorld* gw, double startX, double startY)
:Actor(gw,IID_WALL, startX, startY, right, 0, 1)
{}

void Wall::doSomething(){};
    
//Does nothing. Overriden to prevent accidental call
void Wall::setDead(){};
    


