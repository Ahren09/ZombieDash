#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include <cmath>
#include <vector>
#include <algorithm>

using Direction=int;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* gw, int imageID, double startX, double startY,
             Direction dir, int depth)
:GraphObject(imageID, startX, startY, dir, depth, 1),
m_world(gw),aliveStatus(true)
{};

Direction Actor::getDirectionByNum(int n)
{
    switch (n) {
        case 1:
            return right;
            break;
        case 2:
            return up;
            break;
        case 3:
            return left;
            break;
        case 4:
            return down;
            break;
        default:
            return -1;
        
    }
    
}

bool Actor::determineNewPosition(Direction dir, double& x, double& y, double distance)
{
    
    switch(dir)
    {
        case left:
            x-=distance;
            break;
        case right:
            x+=distance;
            break;
        case up:
            y+=distance;
            break;
        case down:
            y-=distance;
            break;
    }
    bool X_OutOfBound = x<0 || x >= VIEW_WIDTH;
    bool Y_OutOfBound = y<0 || y >= VIEW_HEIGHT;
    
    return !X_OutOfBound && !Y_OutOfBound;
    
}

//Check whether the new location is still in StudentWorld
//Returns false if x y coordinates are out of bound, with x,y remaining unchanged.
//Returns true otherwise, and set x,y to new values
bool Actor::getNewPositionWithDir(Direction dir, double& x, double& y)
{
    if( dir==left || dir == right )
        return determineNewPosition(dir, x, y, SPRITE_WIDTH);
    else return determineNewPosition(dir, x, y, SPRITE_HEIGHT);
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
{}

Exit::Exit(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_EXIT, startX, startY, right, 1)
{}



void Exit::doSomething()
{
    getWorld()->useExit(this);
}


Pit::Pit(StudentWorld* gw, double startX, double startY)
:ActivatingObject(gw, IID_PIT, startX, startY, right, 0)
{}

void Pit::doSomething()
{
    getWorld()->activateOnAppropriateActors(this);
}

void Pit::activateIfAppropriate(Actor* a)
{
    if(a->isAlive() && a->canKillByFlameAndPit() && getWorld()->checkOverlapByTwoObjects(this, a))
    {
        a->dieByFallOrBurnIfAppropriate();
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
        getWorld()->activateOnAppropriateActors(this);
    }
    
    else setDead();
}

//Set Agents that overlap with Flame to dead
void Flame::activateIfAppropriate(Actor* a)
{
    if(a->isAlive() && a->canKillByFlameAndPit() && getWorld()-> checkOverlapByTwoObjects(this, a))
    {
        a->dieByFallOrBurnIfAppropriate();
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
        getWorld()->activateOnAppropriateActors(this);
    }
    else setDead();
}

//Set Agents that overlap with Flame to dead
void Vomit::activateIfAppropriate(Actor *a)
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
    if(isAlive() && activationCountDown())
    {
        getWorld()->triggerLandmineIfAppropriate(this);
        
    }
}

void Landmine::activateIfAppropriate(Actor* a)
{
    if(this!=a && getWorld()->checkOverlapByTwoObjects(this, a))
    {
        dieByFallOrBurnIfAppropriate();
    }
}


//Introduce surrounding flames and create pit
void Landmine::dieByFallOrBurnIfAppropriate()
{
    //Set status to dead
    setDead();
    
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    int x=getX();
    int y=getY();
    
    
    //Introduce surrounding flame objects
    getWorld()->introduceFlameIfAppropriate(x, y);
    getWorld()->introduceFlameIfAppropriate(x+SPRITE_WIDTH, y);
    getWorld()->introduceFlameIfAppropriate(x-SPRITE_WIDTH, y);
    getWorld()->introduceFlameIfAppropriate(x, y+SPRITE_HEIGHT);
    getWorld()->introduceFlameIfAppropriate(x+SPRITE_WIDTH, y+SPRITE_HEIGHT);
    getWorld()->introduceFlameIfAppropriate(x-SPRITE_WIDTH, y+SPRITE_HEIGHT);
    getWorld()->introduceFlameIfAppropriate(x, y-SPRITE_HEIGHT);
    getWorld()->introduceFlameIfAppropriate(x+SPRITE_WIDTH, y-SPRITE_HEIGHT);
    getWorld()->introduceFlameIfAppropriate(x-SPRITE_WIDTH, y-SPRITE_HEIGHT);
    
    //Introduce a pit at location of Landmine;
    getWorld()->addActor(new Pit(getWorld(),x,y));
    
}



//##########################
// MARK: - Goodies
//##########################

Goodie::Goodie(StudentWorld* gw, int imageID, double startX, double startY)
:ActivatingObject(gw, imageID, startX, startY, right, 1)
{}

//Goodies can only be accessed by Penelope
void Goodie::activateIfAppropriate(Actor *a)
{}

void Goodie::pickUp(Penelope *p)
{
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(50);
    
}

void Goodie::doSomething()
{
    //getWorld()->activateOnAppropriateActors(this);
    Penelope* p = getWorld()->getPenelope();
    if(getWorld()->checkOverlapByTwoObjects(this, p))
    {
        pickUp(p);
    }
}

void Goodie::dieByFallOrBurnIfAppropriate()
{
    setDead();
}

VaccineGoodie::VaccineGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_VACCINE_GOODIE, startX, startY)
{}

void VaccineGoodie::pickUp(Penelope *p)
{
    Goodie::pickUp(p);
    p->increaseVaccines();
}

GasCanGoodie::GasCanGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_GAS_CAN_GOODIE, startX, startY)
{}

void GasCanGoodie::pickUp(Penelope *p)
{
    Goodie::pickUp(p);
    p->increaseFlameCharges();
}

LandmineGoodie::LandmineGoodie(StudentWorld* gw, double startX, double startY)
:Goodie(gw, IID_LANDMINE_GOODIE, startX, startY)
{}

void LandmineGoodie::pickUp(Penelope *p)
{
    Goodie::pickUp(p);
    p->increaseLandmines();
}

//##########################
// MARK: - Agent
//##########################

Agent::Agent(StudentWorld* gw, int imageID, double startX, double startY)
:Actor(gw, imageID, startX, startY, right, 0)
{}

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
    }
}

void Human::useExitIfAppropriate(Exit* exit)
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
    if(getInfectionCount()>=500)
    {
        setDead();
        return;
    }
    
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch){
            //move actions
            case KEY_PRESS_UP:
                moveToPosition(getX(), getY(), up);
                break;
            case KEY_PRESS_DOWN:
                moveToPosition(getX(), getY(), down);
                break;
                
            case KEY_PRESS_LEFT:
                moveToPosition(getX(), getY(), left);
                break;
                
            case KEY_PRESS_RIGHT:
                moveToPosition(getX(), getY(), right);
                break;
            
            //Perform actions
            case KEY_PRESS_TAB:
                useLandmine();
                break;
                
            case KEY_PRESS_SPACE:
                fireGasCan();
                break;
                
            case KEY_PRESS_ENTER:
                useVaccine();
                break;
        }
    }
}

void Penelope::moveToPosition(double x, double y, Direction dir)
{
    setDirection(dir);
    //IF new position is valid, i.e. within StudentWorld
    if(determineNewPosition(dir, x, y, 4))
    {
        if(!getWorld()->isAgentMovementBlockedAt(this, x, y))
        {
            moveTo(x, y);
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

void Penelope::fireGasCan()
{
    if(flame_count>0)
    {
        flame_count--;
        double flame_x=getX(), flame_y=getY();
        Direction dir=getDirection();
        bool flag=false;
        for(int i=0;i<3;i++)
        {
            //IF position of the flame is valid
            if(getNewPositionWithDir(dir, flame_x, flame_y))
            {
                //No Exits or Walls block the flame
                if(!getWorld()->isFlameBlockedAt(flame_x, flame_y))
                {
                    flag=true;
                    getWorld()->addActor(new Flame(getWorld(),flame_x,flame_y,up));
                }
            }
            //There exist objects blocking the flame
            else break;
        }
        if(flag) getWorld()->playSound(SOUND_PLAYER_FIRE);
        
    }
}

void Penelope::useLandmine()
{
    if(mine_count>0)
    {
        getWorld()->addActor(new Landmine(getWorld(),getX(),getY()));
        mine_count--;
    }
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
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
    setDead();
}

Citizen::Citizen(StudentWorld* gw, double startX, double startY)
:Human(gw, IID_CITIZEN, startX, startY)
{}

void Citizen::useExitIfAppropriate(Exit* exit)
{
    if(getWorld()->checkOverlapByTwoObjects(this, exit))
    {
        setDead();
        getWorld()->decreaseCitizenCount();
        getWorld()->increaseScore(500);
        getWorld()->playSound(SOUND_CITIZEN_SAVED);
    }
}

void Citizen::dieByFallOrBurnIfAppropriate()
{
    setDead();
    getWorld()->playSound(SOUND_CITIZEN_DIE);
    getWorld()->increaseScore(-1000);
    getWorld()->decreaseCitizenCount();
}

void Citizen::doSomething()
{
    if(getWorld()->checkTick())
        return;
    
    Human::doSomething();
    if(!isAlive())
    { return; }
    
    if(getInfectionCount()>=500)
    {
        turnIntoZombie();
        return;
    }
    
    double zombie_x, zombie_y, dist_z, dist_p;
    double p_x, p_y;
    
    //Calculate distance from Penelope & Nearest Zombie
    getWorld()->locateNearestCitizenThreat(getX(), getY(), zombie_x, zombie_y, dist_z);
    getWorld()->getPenelopeDist(getX(), getY(), p_x, p_y, dist_p);
    
    //
    //IF Penelope is within 80px, AND Penelope is closer to the nearest zombie OR there's no Zombie
    if((getWorld()->getZombieCount()==0 || dist_p<=dist_z) && dist_p<=6400 )
    {
        //IF Citizen is on the same row or col as Penelope
        if(moveToPenelope(p_x,p_y)) return;
    }
    
    //ELSE try to escape nearest Zombie
    else if(dist_z<=6400)
    {
        moveAwayFromZombie(zombie_x, zombie_y, dist_z);
    }
    
}

bool Citizen::moveToPenelope(double p_x, double p_y)
{
    bool successfulMove=false;
    
    std::vector<int> direction_pool;
    double new_x=getX(), new_y=getY();
    pickDirection(getX(), getY(), p_x, p_y, direction_pool);
    Direction d;
    
    //Penelop is on the same row/col, ONE direction available
    //Otherwise, Citizen is blocked. First Direction fails
    //Penelop is NOT on the same row/col, TWO directions available
    for(int i=0;i<direction_pool.size();i++)
    {
        d=getDirectionByNum(direction_pool[i]);
        determineNewPosition(d, new_x, new_y, 2);
        //No blocking agent
        successfulMove=!getWorld()->isAgentMovementBlockedAt(this, new_x, new_y);
        if(successfulMove)
        {
            setDirection(d);
            moveTo(new_x,new_y);
            break;
        }
    }
    
    return successfulMove;
}

bool Citizen::moveAwayFromZombie(double zombie_x, double zombie_y, double dist_z)
{
    bool successfulMove=false;
    std::vector<int> direction_pool;
    std::vector<double> dist_pool;
    
    //If All directions are either blocked or closer to the nearest zombie, return false
    if(!pickEscapeDirection(getX(), getY(), zombie_x, zombie_y, direction_pool, dist_pool, dist_z))
        return false;
    
    //Represents Direction and X,Y Coordinates of the new move
    Direction new_dir;
    double new_x,new_y;
    double new_dist_z=dist_z;
    
    //dist_z: distance to nearest Zombie before move
    //new_dist_z: distance to nearest Zombie after move
    //new_temp_dist_z: distance to nearest Zombie after move IN EACH ITERATION
    
    //Attempt each unblocked escape direction given by pickEscapeDirection()
    for(int i=0;i<direction_pool.size();i++)
    {
        double temp_new_x=getX(), temp_new_y=getY();
        double new_zombie_x=zombie_x, new_zombie_y=zombie_y, new_temp_dist_z=dist_z;
        Direction d=getDirectionByNum(direction_pool[i]);
        
        //Determine new position of Citizen
        determineNewPosition(d, temp_new_x, temp_new_y, 2);
        
        //Determine position of new nearest zombie and new distance to nearest Zombie
        getWorld()->locateNearestCitizenThreat(temp_new_x, temp_new_y, new_zombie_x, new_zombie_y, new_temp_dist_z);
        
        //If there's no zombie nearer than dist_z after the move
        if(new_temp_dist_z>dist_z)
        {
            successfulMove=true;
            if(new_temp_dist_z>new_dist_z)
            {
                new_dist_z=new_temp_dist_z;
                new_dir=direction_pool[i];
                new_x=temp_new_x;
                new_y=temp_new_y;
            }
        }
    }
    
    if(successfulMove)
    {
        setDirection(new_dir);
        moveTo(new_x,new_y);
    }
    
    return successfulMove;
    
}

bool Citizen::pickEscapeDirection(double x, double y, double OtherX, double OtherY,std::vector<int>& direction_pool, std::vector<double>& dist_pool, double current_dist_z)
{
    bool hasFurther=false;
    double dist;
    if(!getWorld()->isAgentMovementBlockedAt(this, x+2, y))
    {
        dist=(x+2-OtherX)*(x+2-OtherX)+(y-OtherY)*(y-OtherY);
        if(dist>current_dist_z)
        {
            hasFurther=true;
            direction_pool.push_back(1);
            dist_pool.push_back(dist);
        }
        
    }
    else if(!getWorld()->isAgentMovementBlockedAt(this, x, y+2))
    {
        
        dist=(x-OtherX)*(x-OtherX)+(y+2-OtherY)*(y+2-OtherY);
        if(dist>current_dist_z)
        {
            hasFurther=true;
            direction_pool.push_back(2);
            dist_pool.push_back(dist);
        }
    }
    else if(!getWorld()->isAgentMovementBlockedAt(this, x-2, y))
    {
        dist=(x-2-OtherX)*(x-2-OtherX)+(y-OtherY)*(y-OtherY);
        if(dist>current_dist_z)
        {
            hasFurther=true;
            direction_pool.push_back(3);
            dist_pool.push_back(dist);
        }
    }
    else if(!getWorld()->isAgentMovementBlockedAt(this, x, y-2))
    {
        dist=(x-OtherX)*(x-OtherX)+(y-2-OtherY)*(y-2-OtherY);
        if(dist>current_dist_z)
        {
            hasFurther=true;
            direction_pool.push_back(4);
            dist_pool.push_back(dist);
        }
    }
    
    return hasFurther;
}

bool Citizen::pickDirection(double x, double y, double OtherX, double OtherY,std::vector<int>& direction_pool)
{
    double d_x=OtherX-x;
    double d_y=OtherY-y;
    
    if(d_x>0)
    {
        direction_pool.push_back(1);
    }
    
    else if(d_x<0)
    {
        direction_pool.push_back(3);
    }
    
    if(d_y>0)
    {
        direction_pool.push_back(2);
    }
    else if(d_y<0)
    {
        direction_pool.push_back(4);
    }
    
    return direction_pool.size()==1;
    
}


//Citizen die when infection count reaches 500
void Citizen::turnIntoZombie()
{
    StudentWorld* gw=getWorld();
    setDead();
    gw->increaseScore(-1000);
    gw->playSound(SOUND_ZOMBIE_BORN);
    int randnum=randInt(1, 10);
    if(randnum>0 && randnum<=10)
        gw->addActor(new SmartZombie(gw, getX(),getY()));
    else getWorld()->addActor(new DumbZombie(gw, getX(),getY()));
}
    
    
    


//*************
// MARK: Zombie
Zombie::Zombie(StudentWorld* gw, double startX, double startY)
:Agent(gw, IID_ZOMBIE, startX, startY)
{
    gw->increaseZombieCount();
    moves=0;
}

void Zombie::computeVomitPosition(double& x,double& y)
{
    Direction dir = getDirection();
    switch(dir)
    {
        case left:
            x-=SPRITE_WIDTH;
        case right:
            x+=SPRITE_WIDTH;
        case up:
            y+=SPRITE_HEIGHT;
        case down:
            y-=SPRITE_HEIGHT;
    }
}

//return true if zombie vomits, false otherwise
bool Zombie::vomitIfAppropriate(const double& x, const double& y)
{
    if(randInt(1, 3) == 3)
    {
        getWorld()->addActor(new Vomit(getWorld(),x,y,getDirection()));
        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
        return true;
    }
    return false;
}

//New direction already determined, move along this new position
void Zombie::moveToNewPosition()
{
    Direction dir=getDirection();
    double x=getX(), y=getY();
    
    //if the new position is valid
    if(determineNewPosition(dir, x, y, 1))
    {
        if(!getWorld()->isAgentMovementBlockedAt(this,x,y))
        {
            moveTo(x,y);
            decreaseMoves();
        }
        else setMoves(0);
    }
}

void Zombie::dieByFallOrBurnIfAppropriate()
{
    setDead();
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
    
}

void Zombie::setNewDirection()
{
    int dir=randInt(1,4);
    switch(dir)
    {
        case 1:
            setDirection(right);
            break;
        case 2:
            setDirection(up);
            break;
        case 3:
            setDirection(left);
            break;
        case 4:
            setDirection(down);
            break;
    }
}

void Zombie::doSomething()
{
    //Zombie is dead OR Number of ticks is even
    if(!isAlive() || getWorld()->checkTick() ) return;
    
    //Vomit if appropriate
    double vomit_x=getX(), vomit_y=getY();
    computeVomitPosition(vomit_x, vomit_y);
    double distance;
    Actor* target;
    
    if(getWorld()->locateNearestVomitTrigger(vomit_x, vomit_y, target, distance))
    {
        //If zombie vomits
        if(vomitIfAppropriate(vomit_x, vomit_y))
            return;
        //ELSE attempt move
    }
    
    if(getMoves() == 0)
    {
        setNewMoves();
    }
    
    setNewDirection();
    moveToNewPosition();
    
}

DumbZombie::DumbZombie(StudentWorld* gw, double startX, double startY)
:Zombie(gw, startX, startY)
{}

void DumbZombie::dropVaccineByChance(const double x, const double y)
{
    if(randInt(1, 10)==10)
    {
        getWorld()->addActor(new VaccineGoodie(getWorld(),x,y));
    }
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
    Zombie::dieByFallOrBurnIfAppropriate();
    getWorld()->increaseScore(1000);
    dropVaccineByChance(getX(), getY());
}


SmartZombie::SmartZombie(StudentWorld* gw, double startX, double startY)
:Zombie(gw, startX, startY)
{}

void SmartZombie::setNewDirection()
{
    double distance=INT_MAX;
    Actor* target;
    bool isThreat = false;
    double target_x=INT_MAX, target_y=INT_MAX;
    
    //IF nearest target is less than/equal to 80px
    if(getWorld()->locateNearestCitizenTrigger(getX(), getY(), target_x, target_y, target, distance, isThreat))
    {
        setDirection(pickDirection(getX(),getY(),target_x,target_y));
    }
    //ELSE nearest target is more than 80px, choose random position among the 4
    else Zombie::setNewDirection();
}

Direction SmartZombie::pickDirection(double x, double y, double target_x, double target_y)
{
    double d_x=target_x-x;
    double d_y=target_y-y;
    std::vector<int> direction_pool;
    
    if(d_x>0)
    {
        direction_pool.push_back(1);
    }
    
    else if(d_x<0)
    {
        direction_pool.push_back(3);
    }
    
    if(d_y>0)
    {
        direction_pool.push_back(2);
    }
    else if(d_y<0)
    {
        direction_pool.push_back(4);
    }
    
    if(direction_pool.size()==1)
    {
        return getDirectionByNum(direction_pool[0]);
    }
    else return getDirectionByNum(direction_pool[randInt(0,1)]);
    
}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
    Zombie::dieByFallOrBurnIfAppropriate();
    getWorld()->increaseScore(2000);
}
