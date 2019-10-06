#include "Game.hpp"

Game::Game() : window(VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Game") 
{
    window.setFramerateLimit(60);
    tilemap.level = 1;
    tilemap.changeLevel();
    camera = window.getView();
    camera.zoom(0.25);
    character.initialize();
}

void Game::update() 
{

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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        camera.zoom(1.05f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        camera.move(0.f,-10.f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        camera.zoom(0.95f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        camera.move(-10.f,0.f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        camera.move(0.f,10.f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        camera.move(10.f,0.f);
}

void Game::run() 
{
    Time deltaTime;
    // Gameloop
    while (window.isOpen()) 
    {
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type==sf::Event::Closed)
                window.close();

        deltaTime = deltaClock.restart();
        std::cout << "FPS : " << 1.0 / deltaTime.asSeconds() << std::endl;
        processEvents();
        update();
        renderGraphics();
    }
}

Game::~Game() {}