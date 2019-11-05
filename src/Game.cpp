#ifndef GAME_CPP
#define GAME_CPP

#include "Game.hpp"
#include <cmath>

Game::Game() : window(VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Game") 
{
    window.setTitle("In Too Deep");
    window.setFramerateLimit(0);
    userInput = UserInput();
    tilemap.level = 0;
    monsters = std::vector<Monster*>();
    audio.playBackgroundMusic();
    nextLevel();
    font.loadFromFile("fonts/Pixeled.ttf");
    fpsText.setFont(font);
    fpsText.setPosition(window.getSize().x - 100, 10);
    fpsText.setFillColor(Color::White);
    fpsText.setCharacterSize(15);
    diamondText.setFont(font);
    diamondText.setPosition(20, 10);
    diamondText.setFillColor(Color::White);
    diamondText.setCharacterSize(15);
    camera = window.getView();
    camera.zoom(CAMERA_ZOOM);
    character.audio = &audio;
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
    if(character.goToNextLevel && tilemap.level < 3)
    {
        nextLevel();
    }
    // Update character
    character.update(userInput, audio, tilemap, deltaTime);
    if(character.isDead && character.deathTimer <= 0)
    {
        resetLevel();
    }
    // Update tilemap
    tilemap.update(deltaTime);
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
            character.kill();
        }
    }
    // Update text diamonds
    diamondText.setString(std::to_string(character.nbDiamonds) + " / 3");
}

void Game::renderGraphics(Time &deltaTime) 
{
    window.setView(camera);
    window.clear();
    tilemap.drawBackground(window);
    tilemap.drawMap(window, camera);
    moveCamera();
    
    for(auto& monster: monsters)
        monster->draw(window, deltaTime);
    character.draw(window, deltaTime);
    window.setView(window.getDefaultView());
    window.draw(fpsText);
    window.draw(diamondText);
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
        // Deltatime between this and last frame
        deltaTime = deltaClock.restart();

        clockManager.add(deltaTime.asSeconds());
        Time averageDeltaTime = seconds(clockManager.average());
        
        // Set fps text
        fpsText.setString(std::to_string((int)std::ceil(1.0/deltaTime.asSeconds())) + " fps");
        //cout << "av : " << averageDeltaTime.asSeconds() << " dt : " << deltaTime.asSeconds() << endl;
        processEvents();
        update(averageDeltaTime);
        renderGraphics(averageDeltaTime);
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
    tilemap.changeLevel();
    initializeMonsters();
    character.initialize();
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