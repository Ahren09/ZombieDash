#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include <cmath>
#include <vector>

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
//bool Actor::checkOverlap(Actor* other, int A, int C)
//{
//    int B=A+SPRITE_WIDTH-1, D=C+SPRITE_HEIGHT-1;
//    int A_=other->getX(), B_=other->getX()+SPRITE_WIDTH-1, C_=other->getY(), D_=other->getY()+SPRITE_HEIGHT-1;
//
//
//    return ((A <= A_ && A_ <= B && C <= C_ && C_ < D) ||
//            (A <= B_ && B_ <= B && C <= C_ && C_ < D) ||
//            (A <= A_ && A_ <= B && C <= D_ && D_ < D) ||
//            (A <= B_ && B_ <= B && C <= D_ && D_ < D));
//}

//Returns true if Euclidean distance is less than 10
//bool Actor::checkOverlapAnother(Actor *other)
//{
//    int d_x=abs(getX()-other->getX());
//    int d_y=abs(getY()-other->getY());
//    
//    return d_x * d_x + d_y * d_y <=100;
//}



//Mark: Wall

Wall::Wall(StudentWorld* gw, double startX, double startY)
:Actor(gw,IID_WALL, startX, startY, right, 0)
{}

void Wall::doSomething(){};



//##########################
// MARK: - Activating Objects
//##########################



ActivatingObject::ActivatingObject(StudentWorld* gw, int id, double startX, double startY, Direction dir, int depth)
:Actor(gw, id, startX, startY, dir, depth)
{
    
    
}

Exit::Exit(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_EXIT, startX, startY, right, 1)
{}

void Exit::doSomething()
{
    //TODO: get info about a citizen's successful escape
    if(/* */)
    {
        getWorld()->increaseScore(500);
        
        getWorld()->playSound(SOUND_CITIZEN_SAVED);
    }
    
    //TODO: all citizen saved and penelope escape
    if(getWorld()->getCitizenCount()==0 && checkOverlapAnother(getWorld()->getPenelope()) )
    {
        getWorld()->
        
    }
}

Pit::Pit(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_PIT, startX, startY, right, 0)
{}

void Pit::doSomething()
{
    
    
}


void Pit::kill()
{
    if(checkOverlapAnother(<#Actor *other#>))
}

Flame::Flame(StudentWorld* gw, double startX, double startY, Direction dir)
:ActivatingObject(gw, IID_FLAME, startX, startY, dir, 0),
active_count(2)
{}

void Flame::doSomething()
{
    if(getCount()>0)
        decCount();
    
    else if
    
    else setDead();
}

Vomit::Vomit(StudentWorld* gw, double startX, double startY, Direction dir)
:ActivatingObject(gw, IID_VOMIT, startX, startY, dir, 0),
active_count(2)
{}

void Vomit::doSomething()
{
    if(getCount()>0)
        decCount();
    
    else if
        
    else setDead();
}

Landmine::Landmine(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_LANDMINE, startX, startY, right, 1),
activation_count(30),activation_status(false)
{}

void Landmine::doSomething()
{
    //IF Landmine is activated
    if(activationCountDown())
    {
        
        
    }
}

void Landmine::explode()
{
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
}







//##########################
// MARK: - Goodies
//##########################

Goodie::Goodie(StudentWorld* gw, int imageID, double startX, double startY)
:ActivatingObject(gw, imageID, startX, startY, right, 1)
{}

void VaccineGoodie::doSomething()
{
    if(checkOverlapAnother(getWorld()->getPenelope()))
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPenelope()->increaseVaccines();
        
    }
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
:Agent(gw, imageID, startX, startY),
infectedStatus(false),infectionCount(0)
{}

void Human::doSomething()
{
    if(getInfectedStatus())
    {
        infectionCount++;
        if(getInfectionCount()>=500)
            setDead();
    }
}

Citizen::Citizen(StudentWorld* gw, double startX, double startY)
:Human(gw, IID_CITIZEN, startX, startY)
{
    
    
}

Penelope::Penelope(StudentWorld* gw, double startX, double startY)
:Human(gw,IID_PLAYER, startX, startY),
flame_count(0),mine_count(0),vaccine_count(0)
{}

void Penelope::doSomething()
{
    //If infection Count reach the threshold
    Human::doSomething();

    
    
    
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
            case KEY_PRESS_TAB:
                //TODO:
                 break;
                
            case KEY_PRESS_SPACE:
                //TODO:
                break;
                
            case KEY_PRESS_ENTER:
                //TODO:
                break;
                
                
        }
    }
}

void Penelope::useVaccine()
{
    if(vaccine_count>0)
    {
        vaccine_count--;
        clearInfectedStatus();
    }
}

void Penelope::useFlame()
{
    double flame_x, flame_y;
    double prev_x=getX(), prev_y=getY();
    for(int i=1;i<=3;i++)
    {
        //IF position of the flame is valid
        if(getNewPositionWithDir(getDirection(), prev_x, prev_y, flame_x, flame_y))
        {
            //No Exits or Walls block the flame
            if(!getWorld()->isFlameBlockedAt(flame_x, flame_y))
            {
                getWorld()->addActor(new Flame(getWorld(),flame_x,flame_y,getDirection()));
                prev_x=flame_x;
                prev_y=flame_y;
            }
        }
        //There exist objects blocking the flame
        else break;
    }
}

void Penelope::useLandmine()
{
    getWorld()->addActor(new Landmine(getWorld(),getX(),getY()));
}

void Citizen::setDead()
{
    Human::setDead();
    getWorld()->increaseScore(-1000);
}

void Citizen::doSomething()
{
    Human::doSomething();
    if(!isAlive())
    {
        
    }
}

//Citizen die when infection count reaches 500
void Citizen::dieOfInfection()
{
    setDead();
    getWorld()->playSound(SOUND_ZOMBIE_BORN);
    getWorld()->addActor(new Zombie(getX(),getY()));
    
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
