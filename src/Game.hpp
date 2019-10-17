#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>

#include "Character.hpp"
#include "Tilemap.hpp"
#include "UserInput.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

#define CAMERA_ZOOM 0.35

using namespace sf;

class Game {
private:
	RenderWindow 	window;
	Tilemap 		tilemap;
	View 			camera;
	Clock 			deltaClock;
	Character 		character;
	UserInput		userInput;

public:
	Game();

	void moveCamera();

	void update(Time &deltaTime);

	void renderGraphics();

	void processEvents();

	void run();

	~Game();
};

#endif