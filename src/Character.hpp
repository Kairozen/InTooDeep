#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;

class Character
{
public:
	Texture characterTexture;
	Sprite characterSprite;

	int lives;
	
	// Coordinates
	float x,y;

	// Animation
	int frameNum, frameMax, frameTimer;

	int state, direction;

	// Collision
	bool isOnGround;

	float nextX, nextY;

	Character();
	~Character();
	
};