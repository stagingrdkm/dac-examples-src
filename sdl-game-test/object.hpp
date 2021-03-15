#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <SDL.h> 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h> 

class Object
{
public:
    struct Coordinates
    {
        int x;
        int y;
        int w;
        int h;
    };

    Object() = default;
    virtual ~Object() = default;

    void setSrc(const Coordinates& c);
    void setDest(const Coordinates& c);
    SDL_Rect getSrc() const;
    SDL_Rect getDest() const;
    SDL_Texture* getTex() const;
    void setImage(const std::string& imageName, SDL_Renderer* rend);

private:
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Texture* tex;

};


#endif // OBJECT_HPP
