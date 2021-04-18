#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "object.hpp"
#include <vector>

class Entity : public Object
{
public:
    int  createAnimation(int row, int w, int h, int amount, int speed);
    void setCurAnimation(int ca);
    void updateAnimation();
    int getCurAnimation() const;
private:
    struct Cycle
    {
        int row;
        int w;
        int h;
        int amount;
        int speed;
        int tick;

    };
    std::vector<Cycle> animations;
    unsigned int currAnim;
    unsigned int currentAnimationIterator;
};

#endif //ENTITY_HPP

