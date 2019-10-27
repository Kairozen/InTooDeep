#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "Character.hpp"
#include "Tilemap.hpp"
#include "Monster.hpp"
#include "UserInput.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

#define CAMERA_ZOOM 0.35

#define MONSTER_TILE 561
#define MONSTER_REBOUND 4

using namespace sf;

class Game {
private:
	RenderWindow 	window;
	Tilemap 		tilemap;
	View 			camera;
	Clock 			deltaClock;
	Character 		character;
	UserInput		userInput;
	Text			text;
	Font 			font;
	std::vector<Monster*> monsters;
public:
	Game();

	void moveCamera();

	void update(Time &deltaTime);

	void renderGraphics(Time &deltaTime);

	void processEvents();

	void run();

	void nextLevel();

	void initializeMonsters();

	~Game();
};

#endif