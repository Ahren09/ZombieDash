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

void Pit::killByPitIfAppropriate(Actor* a)
{
    if(a->canKillByPit() && getWorld()->checkOverlapByTwoObjects(this, a) )
    {
        a->setDead();
    }
    
}


Flame::Flame(StudentWorld* gw, double startX, double startY, Direction dir)
:ActivatingObject(gw, IID_FLAME, startX, startY, dir, 0),
active_count(2)
{}

void Flame::doSomething()
{
    if(getCount()>0)
    {
        decCount();
        getWorld()->killByFlameIfAppropriate(this);
    }
    
    else setDead();
}

//Set Agents that overlap with Flame to dead
void Flame::killByFlameIfAppropriate(Actor* a)
{
    if(a->canKillByFlame() && getWorld()-> checkOverlapByTwoObjects(this, a))
    {
        a->setDead();
    }
}


Vomit::Vomit(StudentWorld* gw, double startX, double startY, Direction dir)
:ActivatingObject(gw, IID_VOMIT, startX, startY, dir, 0),
active_count(2)
{}

void Vomit::doSomething()
{
    if(getCount()>0)
        
    {
        decCount();
        getWorld()->infectByVomitIfAppropriate(this);
    }
    else setDead();
}

//Set Agents that overlap with Flame to dead
void Vomit::infectByVomitIfAppropriate(Actor *a)
{
    if(a->canInfectByVomit() && getWorld()->checkOverlapByTwoObjects(this, a))
    {
        a->setInfectionStatus();
    }
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

void Landmine::activateIfAppropriate(Actor* a)
{
    
    if(getWorld()->checkOverlapByTwoObjects(this, a))
    {
        //TODO: generate flame and pit
    }
    
}


//Introduce surrounding flames and create pit
void Landmine::explode()
{
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    int x=getX();
    int y=getY();
    
    
    //Introduce surrounding flame objects
    getWorld()->introduceFlameIfAppropriate(this, x, y, up);
    getWorld()->introduceFlameIfAppropriate(this, x+SPRITE_WIDTH, y, up);
    getWorld()->introduceFlameIfAppropriate(this, x-SPRITE_WIDTH, y, up);
    getWorld()->introduceFlameIfAppropriate(this, x, y+SPRITE_HEIGHT, up);
    getWorld()->introduceFlameIfAppropriate(this, x+SPRITE_WIDTH, y+SPRITE_HEIGHT, up);
    getWorld()->introduceFlameIfAppropriate(this, x-SPRITE_WIDTH, y+SPRITE_HEIGHT, up);
    getWorld()->introduceFlameIfAppropriate(this, x, y-SPRITE_HEIGHT, up);
    getWorld()->introduceFlameIfAppropriate(this, x+SPRITE_WIDTH, y-SPRITE_HEIGHT, up);
    getWorld()->introduceFlameIfAppropriate(this, x-SPRITE_WIDTH, y-SPRITE_HEIGHT, up);
    
    //Introduce a pit at location of Landmine;
    getWorld()->introducePitIfAppropriate(this, x, y);
    
    //Set status to dead
    setDead();
}








//##########################
// MARK: - Goodies
//##########################

Goodie::Goodie(StudentWorld* gw, int imageID, double startX, double startY)
:ActivatingObject(gw, imageID, startX, startY, right, 1)
{}

void Goodie::activateIfAppropriate(Actor *a)
{
    
    
}

void Goodie::pickUp(Penelope *p)
{
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    
}

void Goodie::doSomething()
{
    Penelope* p = getWorld()->getPenelope();
    if(getWorld()->checkOverlapByTwoObjects(this, p))
    {
        pickUp(p);
    }
}

VaccineGoodie::VaccineGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_VACCINE_GOODIE, startX, startY)
{}

void VaccineGoodie::pickUp(Penelope *p)
{
    Goodie::pickUp(p);
    getWorld()->getPenelope()->increaseVaccines();
}

void VaccineGoodie::doSomething()
{
    Goodie::doSomething();
    getWorld()->getPenelope()->increaseVaccines();
}

GasCanGoodie::GasCanGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_GAS_CAN_GOODIE, startX, startY)
{}

void GasCanGoodie::pickUp(Penelope *p)
{
    Goodie::pickUp(p);
    getWorld()->getPenelope()->increaseFlameCharges();
}

void GasCanGoodie::doSomething()
{
    Goodie::doSomething();
    getWorld()->getPenelope()->increaseFlameCharges();
}

LandmineGoodie::LandmineGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_LANDMINE_GOODIE, startX, startY)
{}

void LandmineGoodie::pickUp(Penelope *p)
{
    Goodie::pickUp(p);
    getWorld()->getPenelope()->increaseLandmines();
}
void LandmineGoodie::doSomething()
{
    Goodie::doSomething();
    
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

void Human::useExitIfAppropriate(Exit* exit)
{
    if(getWorld()->checkOverlapByTwoObjects(this, exit))
    { setDead(); }
}

Citizen::Citizen(StudentWorld* gw, double startX, double startY)
:Human(gw, IID_CITIZEN, startX, startY)
{}

void Citizen::useExitIfAppropriate(Exit* exit)
{
    Human::useExitIfAppropriate(exit);
    getWorld()->increaseScore(500);
    getWorld()->playSound(SOUND_CITIZEN_SAVED);
    getWorld()->recordCitizenGone();
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
        clearInfection();
    }
}

void Penelope::useGasCan()
{
    double flame_x=getX(), flame_y=getY();
    Direction dir=getDirection();
    for(int i=0;i<3;i++)
    {
        //IF position of the flame is valid
        if(getWorld()->getNewPositionWithDir(dir, flame_x, flame_y))
        {
            //No Exits or Walls block the flame
            if(!getWorld()->isFlameBlockedAt(flame_x, flame_y))
            {
                getWorld()->addActor(new Flame(getWorld(),flame_x,flame_y,up));
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

void Penelope::useExitIfAppropriate(Exit* exit)
{
    if(getWorld()->checkOverlapByTwoObjects(this, exit))
    {
        if(getWorld()->getCitizenCount() == 0)
        {
            getWorld()->recordLevelFinishedIfAllCitizensGone();
        }
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
