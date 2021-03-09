#include "entity.hpp"

int Entity::createAnimation(int row, int w, int h, int amount, int speed)
{
    Cycle tmp;
    tmp.row = row - 1;
    tmp.w = w;
    tmp.h = h;
    tmp.amount = amount;
    tmp.speed = speed;
    tmp.tick = 0;

    animations.push_back(tmp);
    return animations.size() - 1;
}

void Entity::setCurAnimation(int ca)
{
    currentAnimationIterator = 0;
    currAnim = ca;
}


void Entity::updateAnimation()
{
    setSrc(Object::Coordinates{animations[currAnim].w * animations[currAnim].tick,
                               animations[currAnim].h * animations[currAnim].row,
                               animations[currAnim].w,
                               animations[currAnim].h});

    if(currentAnimationIterator > animations[currAnim].speed)
    {
        animations[currAnim].tick++;
        currentAnimationIterator = 0;
    }

    currentAnimationIterator++;

    if(animations[currAnim].tick >= animations[currAnim].amount)
    {
        animations[currAnim].tick = 0;
    }
}

int Entity::getCurAnimation() const
{
    return currAnim;
}

