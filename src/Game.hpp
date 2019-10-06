#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>

#include "Tilemap.hpp"
//#include "Character.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

using namespace sf;

class Game {
private:
	RenderWindow 	window;
	Tilemap 		tilemap;
	View 			camera;
	Clock 			deltaClock;
	//Character 		character;

public:
	Game();

	void update();

	void renderGraphics();

	void processEvents();

	void run();

	~Game();
};