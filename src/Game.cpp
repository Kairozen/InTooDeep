#ifndef GAME_CPP
#define GAME_CPP

#include "Game.hpp"

Game::Game() : window(VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Game") 
{
    window.setFramerateLimit(30);
    userInput = UserInput();
    tilemap.level = 1;
    tilemap.changeLevel();
    font.loadFromFile("fonts/Bubblegum.ttf");
    text.setFont(font);
    text.setFillColor(Color::Black);
    text.setCharacterSize(15);
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
        cameraPosition.y = MAX_Y * TILE_SIZE - (camera.getSize().y/2); 
    }
    if(character.position.x - (camera.getSize().x/2) < 0)
    {
        cameraPosition.x = camera.getSize().x/2;
    }
    else if(character.position.x + (camera.getSize().x/2) > (MAX_X * TILE_SIZE))
    {
        cameraPosition.x = MAX_X * TILE_SIZE - (camera.getSize().x/2);
    }    camera.setCenter(cameraPosition);
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
    window.setView(window.getDefaultView());
    window.draw(text);
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
        text.setString(std::to_string(1.0/deltaTime.asSeconds()));
        //std::cout << "FPS : " << 1.0 / deltaTime.asSeconds() << std::endl;
        processEvents();
        update(deltaTime);
        renderGraphics();
    }
}

Game::~Game() {}

#endif