#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "UserInput.hpp"
#include "Tilemap.hpp"

using namespace sf;

class Character
{
public:
	const int SPRITE_HEIGHT = 16;
	const int SPRITE_WIDTH = 11;

	const int CHARACTER_HEIGHT = 13;
	const int CHARACTER_WIDTH = 11;

	const int IDLE = 0;
	const int RUN = 1;
	const int JUMP = 2;
	const int WALL_SLIDE = 3;

	const int NB_FRAMES_IDLE = 3;
	const int NB_FRAMES_RUN = 6;
	const int NB_FRAMES_JUMP = 1;
	const int NB_FRAMES_WALL_SLIDE = 1;

	const int ANIMATION_SPEED = 5;

	const int RIGHT = 1;
	const int LEFT = 2;

	const float GRAVITY = 15.0f;
	const int JUMP_HEIGHT = 50;
	const float JUMP_LIMIT = 0.1f;
	const float WALL_JUMP_LIMIT = 0.2f;
	const int RUNNING_SPEED = 100;
	const int MAX_FALLING_SPEED = 15;
	const float MAX_HORIZONTAL_SPEED = 0.5f;

	Texture characterTexture;
	Sprite characterSprite;

	int lives;

	float jumpTime;
	bool isJumping;

	float wallJumpTime;
	bool isWallJumpingLeft;
	bool isWallJumpingRight;

	bool isSlidingOnWall;
	bool wallLeft, wallRight;

	// Coordinates
	Vector2f position;

	// Animation
	int frameNum, frameMax, frameTimer;

	int state;
	int direction;

	// Collision
	bool isOnGround;

	Vector2f movementVector;

	Character();
	~Character();
	
	void initialize();

	void draw(RenderWindow &window);

	void update(UserInput &input, Tilemap &map, Time &deltaTime);

	void handleCollision(Tilemap &map);

	void wallJumpLeft(Time &deltaTime);
	
	void wallJumpRight(Time &deltaTime);

	void jump(Time &deltaTime);

	void moveRight(Time &deltaTime);

	void moveLeft(Time &deltaTime);

	void idle();
};

#endif