#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Tilemap.hpp"

using namespace sf;

class Entity
{
public:

	const int ANIMATION_NB_FRAMES[4] = {3, 6, 1, 1};
	const int NB_FRAMES_IDLE = 3;
	const int NB_FRAMES_RUN = 6;
	const int NB_FRAMES_JUMP = 1;
	const int NB_FRAMES_WALL_SLIDE = 1;

	const float ANIMATION_SPEED = 0.1;

	const int RIGHT = 1;
	const int LEFT = 2;

	const float GRAVITY = 15.0f;

	Texture texture;
	Sprite sprite;

	// Coordinates
	Vector2f position;

	// Animation
	int frameNum, frameMax;
	float frameTimer;
	int direction;

	// Collision
	bool isOnGround;
    FloatRect collidingBox;

	Vector2f movementVector;

    virtual ~Entity() = default;

	virtual void draw(RenderWindow &window, Time &deltaTime) = 0;

	virtual void handleCollision(Tilemap &map) = 0;
};

#endif