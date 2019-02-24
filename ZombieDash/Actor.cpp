#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* gw, int imageID, double startX, double startY,
             Direction dir, int depth)
:GraphObject(imageID, startX, startY, dir, depth, 1),
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



//Mark: Wall

Wall::Wall(StudentWorld* gw, double startX, double startY)
:Actor(gw,IID_WALL, startX, startY, right, 0)
{}

void Wall::doSomething(){};
    
//Does nothing. Overriden to prevent accidental call
void Wall::setDead(){};



//##########################
// MARK: - Activating Objects
//##########################



ActivatingObject::ActivatingObject(StudentWorld* gw, int id, double startX, double startY, Direction dir, int depth)
:Actor(gw, id, startX, startY, dir, depth)
{
    
    
}

Exit::Exit(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_EXIT, startX, startY, right, 1)
{
    
    
}

Pit::Pit(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_PIT, startX, startY, right, 0)
{
    
    
}

Flame::Flame(StudentWorld* gw, double startX, double startY, Direction dir)
:ActivatingObject(gw, IID_FLAME, startX, startY, dir, 0)
{
    
    
}

Vomit::Vomit(StudentWorld* gw, double startX, double startY, Direction dir)
:ActivatingObject(gw, IID_VOMIT, startX, startY, dir, 0)
{
    
    
}

Landmine::Landmine(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_LANDMINE, startX, startY, right, 1)
{
    
    
}







//##########################
// MARK: - Goodies
//##########################

Goodie::Goodie(StudentWorld* gw, int imageID, double startX, double startY)
:ActivatingObject(gw, imageID, startX, startY, right, 1)
{
    
    
}

VaccineGoodie::VaccineGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_VACCINE_GOODIE, startX, startY)
{
    
    
}

GasCanGoodie::GasCanGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_GAS_CAN_GOODIE, startX, startY)
{
    
    
}

LandmineGoodie::LandmineGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_LANDMINE_GOODIE, startX, startY)
{
    
    
}


//##########################
// MARK: - Agent
//##########################

Agent::Agent(StudentWorld* gw, int imageID, double startX, double startY)
:Actor(gw, imageID, startX, startY, right, 0)
{
    
    
}

//*************
// MARK: Human
Human::Human(StudentWorld* gw, int imageID, double startX, double startY)
:Agent(gw, imageID, startX, startY)
{
    
    
}

Citizen::Citizen(StudentWorld* gw, double startX, double startY)
:Human(gw, IID_CITIZEN, startX, startY)
{
    
    
}

Penelope::Penelope(StudentWorld* gw, double startX, double startY)
:Human(gw,IID_PLAYER, startX, startY),
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



//*************
// MARK: Zombie
Zombie::Zombie(StudentWorld* gw, double startX, double startY)
:Agent(gw, IID_ZOMBIE, startX, startY)
{
    
    
}

DumbZombie::DumbZombie(StudentWorld* gw, double startX, double startY)
:Zombie(gw, startX, startY)
{
    
    
}

SmartZombie::SmartZombie(StudentWorld* gw, double startX, double startY)
:Zombie(gw, startX, startY)
{
    
    
}
