#ifndef GAME_CPP
#define GAME_CPP

#include "Game.hpp"

Game::Game() : window(VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Game") 
{
    window.setFramerateLimit(60);
    userInput = UserInput();
    tilemap.level = 1;
    tilemap.changeLevel();
    camera = window.getView();
    camera.zoom(0.25);
    character.initialize();
}

void Game::update(Time &deltaTime) 
{
    character.update(userInput, tilemap, deltaTime);
}

void Game::renderGraphics() 
{
    window.setView(camera);
    window.clear();
    tilemap.drawBackground(window);
    tilemap.drawMap(window);
    camera.setCenter(character.position);
    character.draw(window);
    window.display();
}

void Game::processEvents() 
{
    userInput.inputManagement(window);
}

void Game::run() 
{
    Time deltaTime;
    // Gameloop
    while (window.isOpen()) 
    {
        deltaTime = deltaClock.restart();
        std::cout << "FPS : " << 1.0 / deltaTime.asSeconds() << std::endl;
        processEvents();
        update(deltaTime);
        renderGraphics();
    }
}

Game::~Game() {}

#endif