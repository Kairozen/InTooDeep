#ifndef GAME_CPP
#define GAME_CPP

#include "Game.hpp"

Game::Game() : window(VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Game") 
{
    window.setFramerateLimit(60);
    userInput = UserInput();
    tilemap.level = 0;
    monsters = std::vector<Monster*>();
    audio.playBackgroundMusic();
    nextLevel();
    font.loadFromFile("fonts/Bubblegum.ttf");
    text.setFont(font);
    text.setFillColor(Color::Black);
    text.setCharacterSize(15);
    camera = window.getView();
    camera.zoom(CAMERA_ZOOM);
    character.initialize();
}

Game::~Game()
{
    for(auto& monster : monsters)
        delete monster;
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
    // Update character
    character.update(userInput, audio, tilemap, deltaTime);
    if(character.isDead && character.deathTimer <= 0)
    {
        resetLevel();
    }
    // Update monsters and collision with player
    for(size_t i = 0; i < monsters.size(); ++i)
    {
        monsters[i]->update(tilemap, deltaTime);
        int collisionStatus = monsters[i]->collisionWithPlayer(character);
        if(collisionStatus == 1)
        {
            Monster* tmp = monsters[i];
            monsters.erase(monsters.begin() + i);
            delete tmp;
            character.movementVector.y = - MONSTER_REBOUND - character.GRAVITY * deltaTime.asSeconds();
            audio.playStompSound();
        }
        else if(collisionStatus == 2)
        {
            character.kill(audio);
        }
    }
}

void Game::renderGraphics(Time &deltaTime) 
{
    window.setView(camera);
    window.clear();
    tilemap.drawBackground(window);
    tilemap.drawMap(window);
    moveCamera();
    
    for(auto& monster: monsters)
        monster->draw(window, deltaTime);
    character.draw(window, deltaTime);
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
        renderGraphics(deltaTime);
    }
}


void Game::nextLevel()
{
    tilemap.level++;
    tilemap.changeLevel();
    initializeMonsters();
    character.initialize();
}

void Game::resetLevel()
{
    character.initialize();
    tilemap.changeLevel();
    initializeMonsters();
}

void Game::initializeMonsters()
{
    srand(time(nullptr));
    for(auto& monster: monsters)
        delete monster;
    monsters.clear();
    for(int y = 0; y < MAX_Y; ++y)
        for (int x = 0; x < MAX_X; ++x)
            if(tilemap.collidingTiles[y][x] == MONSTER_TILE)
            {
                tilemap.collidingTiles[y][x] = 0;
                Monster *monster = new Monster(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                monster->color = rand() % 3;
                monsters.push_back(monster);
            }
}

#endif