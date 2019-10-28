#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "UserInput.hpp"
#include "Tilemap.hpp"
#include "Entity.hpp"
#include "Audio.hpp"

#define DELTATIME_MULTIPLICATOR 60
#define DEATH_TIMER 2
#define SPIKES 402
using namespace sf;

class Character : public Entity
{
public:
	const int SPRITE_HEIGHT = 16;
	const int SPRITE_WIDTH = 11;

	const int IDLE = 0;
	const int RUN = 1;
	const int JUMP = 2;
	const int WALL_SLIDE = 3;

	const int ANIMATION_NB_FRAMES[4] = {3, 6, 1, 1};

	const int JUMP_HEIGHT = 50;
	const float JUMP_LIMIT = 0.1f;
	const float WALL_JUMP_LIMIT = 0.2f;
	const int RUNNING_SPEED = 100;
	const int MAX_FALLING_SPEED = 9;

	int lives;

	float jumpTime;
	bool isJumping;

	float wallJumpTime;
	bool isWallJumpingLeft;
	bool isWallJumpingRight;

	bool isSlidingOnWall;
	bool wallLeft, wallRight;

	bool isDead;
	float deathTimer;

	int state;

	Character();
	~Character();
	
	void initialize();

	void draw(RenderWindow &window, Time &deltaTime);

	void changeState(int state);

	void update(UserInput &input, Audio &audio, Tilemap &map, Time &deltaTime);

	void handleCollision(Tilemap &map);

	void wallJumpLeft(Time &deltaTime, Audio &audio);
	
	void wallJumpRight(Time &deltaTime, Audio &audio);

	void jump(Time &deltaTime, Audio &audio);

	void moveRight(Time &deltaTime);

	void moveLeft(Time &deltaTime);

	void idle();

	void kill();

	void kill(Audio &audio);
};

#endif