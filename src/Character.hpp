#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;

class Character
{
public:
	const int SPRITE_HEIGHT = 16;
	const int SPRITE_WIDTH = 16;

	const int CHARACTER_HEIGHT = 13;
	const int CHARACTER_WIDTH = 10;

	const int IDLE = 0;
	const int RUN = 1;
	const int JUMP = 2;
	const int ATTACK = 4;
	const int CLIMB = 5;

	const int NB_FRAMES_IDLE = 4;
	const int NB_FRAMES_RUN = 6;
	const int NB_FRAMES_JUMP = 1;
	const int NB_FRAMES_ATTACK = 3;
	const int NB_FRAMES_CLIMB = 3;

	const int ANIMATION_SPEED = 5;

	const int RIGHT = 1;
	const int LEFT = 2;

	Texture characterTexture;
	Sprite characterSprite;

	int lives;
	
	// Coordinates
	Vector2f position;

	// Animation
	int frameNum, frameMax, frameTimer;

	int state, direction;

	// Collision
	bool isOnGround;

	Vector2f nextPosition;

	Character();
	~Character();
	
	void initialize();

	void draw(RenderWindow &window);

};