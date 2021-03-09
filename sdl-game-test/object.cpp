#include "object.hpp"
#include <iostream>

using std::cout;
using std::endl;


void Object::setSrc(const Coordinates& c)
{
    src.x = c.x;
    src.y = c.y;
    src.w = c.w;
    src.h = c.h;

}

void Object::setDest(const Coordinates& c)
{
    dest.x = c.x;
    dest.y = c.y;
    dest.w = c.w;
    dest.h = c.h;
}

SDL_Rect Object::getSrc() const
{
    return src;
}

SDL_Rect Object::getDest() const
{
    return dest;
}

SDL_Texture* Object::getTex() const
{
    return tex;
}

void Object::setImage(const std::string& imageName, SDL_Renderer* rend)
{
    SDL_Surface* surf = IMG_Load(imageName.c_str());

    if(surf == nullptr)
    {
        cout << "IMG_Load failed." << endl;
        return;
    }

    tex = SDL_CreateTextureFromSurface(rend, surf);
    if(tex == nullptr)
    {
        cout << "SDL_CreateTextureFromSurface failed." << endl;
    }
}


