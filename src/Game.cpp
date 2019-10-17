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
    camera.zoom(CAMERA_ZOOM);
    character.initialize();
}

void Game::moveCamera()
{
    Vector2f cameraPosition = character.position;
    if(character.position.y - (camera.getSize().y/2) < 0)
    {
        cameraPosition.y = camera.getSize().y/2;
    }
    else if(character.position.y + (camera.getSize().y/2) > (MAX_Y * TILE_SIZE))
    {
        cameraPosition.y = WINDOW_HEIGHT - (camera.getSize().y/2); 
    }
    if(character.position.x - (camera.getSize().x/2) < 0)
    {
        cameraPosition.x = camera.getSize().x/2;
    }
    else if(character.position.x + (camera.getSize().x/2) > (MAX_X * TILE_SIZE))
    {
        cameraPosition.x = WINDOW_WIDTH - (camera.getSize().x/2);
    } 

    camera.setCenter(cameraPosition);
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
    moveCamera();
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