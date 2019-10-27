#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Character.hpp"
#include "Tilemap.hpp"
#include "Entity.hpp"

using namespace sf;

class Monster : public Entity
{
public:
    const int SPRITE_HEIGHT = 16;
	const int SPRITE_WIDTH = 16;

    const int MONSTER_HEIGHT = 10;
    const int MONSTER_WIDTH = 14;

	const int ANIMATION_NB_FRAMES = 6;
    
    const int RUNNING_SPEED = 80;

    int color = 0;

    Monster(Vector2f position);

    void initialize();

    void draw(RenderWindow &window, Time &deltaTime);

    void update(Tilemap &map, Time &deltaTime);

    void handleCollision(Tilemap &map);

    void moveRight(Time &deltaTime);

    void moveLeft(Time &deltaTime);

    int collisionWithPlayer(Character &character);
};


#endif