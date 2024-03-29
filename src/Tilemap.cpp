#ifndef TILEMAP_CPP
#define TILEMAP_CPP

#include "Tilemap.hpp"

using namespace std;

Tilemap::Tilemap() 
{
    // Loading background
    if(!backgroundTexture.loadFromFile("graphics/background.png"))
    {
        throw runtime_error("Problème lors du chargement du background");
    }
    else
    {
        backgroundSprite.setTexture(backgroundTexture);
    }
    // Loading tileset
    if(!tilesetTexture.loadFromFile("graphics/platformer-tileset.png"))
    {
        throw runtime_error("Problème lors du chargement du tileset");
    }
    else
    {
        tilesetSprite.setTexture(tilesetTexture);
    }

    mapStart = Vector2<int>(0,0);
    mapEnd = Vector2<int>(MAX_X,MAX_Y);
    level = 1;

    for (int j = 0; j < MAX_Y; ++j)
        for(int i = 0; i < MAX_X; ++i)
            backgroundTiles[j][i] = collidingTiles[j][i] = 0;   
}

void Tilemap::loadLevel(string filename) 
{
    fstream file;

    string mapLine, mapNum;

    // Vector for lines of file
    vector<vector<int>> mapLines;

    vector<int> tmp;

    // Open map file
    file.open(filename.c_str(), fstream::in);
    if(!file.is_open())
    {
        throw runtime_error("Erreur de chargement de la map");
    }

    // Read map file
    while(!file.eof())
    {
        stringstream strstream;
        // Get a line of numbers from file
        getline(file,mapLine);
        if(mapLine.size() == 0)
            continue;
        // Clear stream and copy new line into it
        strstream.clear();
        strstream.str(mapLine);
        // Clear previous vector of int
        tmp.clear();

        while(strstream.good())
        {
            // Get all numbers of tiles for the map
            getline(strstream, mapNum, ',');
            tmp.push_back(atoi(mapNum.c_str()));
            if(tmp.size() == MAX_X) {
                mapLines.push_back(tmp);
                tmp.clear();
            }
        }

        if(tmp.size() != 0)
            mapLines.push_back(tmp);
    }

    file.close();

    // Load tiles for all layers
    for(int y = 0; y < MAX_Y; ++y)
    {
        for(int x = 0; x < MAX_X; ++x)
        {
            collectibleTiles[y][x] = mapLines[y][x];
            collidingTiles[y][x] = mapLines[y + MAX_Y][x];
            backgroundTiles[y][x] = mapLines[y + MAX_Y + MAX_Y][x];
        }
    }
    respawnObjects.clear();
}

void Tilemap::drawBackground(RenderWindow &window)
{
    window.draw(backgroundSprite);
}

void Tilemap::drawMap(RenderWindow &window, View &camera)
{
    int centerX = camera.getCenter().x / TILE_SIZE;
    int centerY = camera.getCenter().y / TILE_SIZE;
    int horizontalLengthToDraw = (camera.getSize().x / TILE_SIZE);
    int verticalLengthToDraw = (camera.getSize().y / TILE_SIZE); 
    int mapStartX = centerX - ((horizontalLengthToDraw >> 1) + 2); 
    int mapEndX = centerX + ((horizontalLengthToDraw >> 1) + 2); 
    int mapStartY = centerY - ((verticalLengthToDraw >> 1) + 2); 
    int mapEndY = centerY + ((verticalLengthToDraw >> 1) + 2);

    for(int y = mapStartY; y < mapEndY; ++y)
    {
        for(int x = mapStartX; x < mapEndX; ++x)
        {
            int tileNumCollectible = collectibleTiles[y][x] - 1;
            int tileNumBackground = backgroundTiles[y][x] - 1;
            int tileNumColliding = collidingTiles[y][x] - 1;
            if(tileNumBackground > -1)
            {
                int horizontalPositionOnSprite = (tileNumBackground % TILESET_LINE_SIZE) * TILE_SIZE;
                int verticalPositionOnSprite = (tileNumBackground / TILESET_LINE_SIZE) * TILE_SIZE;
                tilesetSprite.setPosition(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                tilesetSprite.setTextureRect(IntRect(horizontalPositionOnSprite, verticalPositionOnSprite, TILE_SIZE, TILE_SIZE));
                window.draw(tilesetSprite);
            }
            if(tileNumColliding > -1)
            {
                int horizontalPositionOnSprite = (tileNumColliding % TILESET_LINE_SIZE) * TILE_SIZE;
                int verticalPositionOnSprite = (tileNumColliding / TILESET_LINE_SIZE) * TILE_SIZE;
                tilesetSprite.setPosition(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                tilesetSprite.setTextureRect(IntRect(horizontalPositionOnSprite, verticalPositionOnSprite, TILE_SIZE, TILE_SIZE));
                window.draw(tilesetSprite);
            }            
            if(tileNumCollectible > -1)
            {
                int horizontalPositionOnSprite = (tileNumCollectible % TILESET_LINE_SIZE) * TILE_SIZE;
                int verticalPositionOnSprite = (tileNumCollectible / TILESET_LINE_SIZE) * TILE_SIZE;
                tilesetSprite.setPosition(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                tilesetSprite.setTextureRect(IntRect(horizontalPositionOnSprite, verticalPositionOnSprite, TILE_SIZE, TILE_SIZE));
                window.draw(tilesetSprite);
            }
        }

    // for(int y = mapStart.y; y < mapEnd.y; ++y)
    // {
    //     for(int x = mapStart.x; x < mapEnd.x; ++x)
    //     {
    //         int tileNumCollectible = collectibleTiles[y][x] - 1;
    //         int tileNumBackground = backgroundTiles[y][x] - 1;
    //         int tileNumColliding = collidingTiles[y][x] - 1;
    //         if(tileNumBackground > -1)
    //         {
    //             int horizontalPositionOnSprite = (tileNumBackground % TILESET_LINE_SIZE) * TILE_SIZE;
    //             int verticalPositionOnSprite = (tileNumBackground / TILESET_LINE_SIZE) * TILE_SIZE;
    //             tilesetSprite.setPosition(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
    //             tilesetSprite.setTextureRect(IntRect(horizontalPositionOnSprite, verticalPositionOnSprite, TILE_SIZE, TILE_SIZE));
    //             window.draw(tilesetSprite);
    //         }
    //         if(tileNumColliding > -1)
    //         {
    //             int horizontalPositionOnSprite = (tileNumColliding % TILESET_LINE_SIZE) * TILE_SIZE;
    //             int verticalPositionOnSprite = (tileNumColliding / TILESET_LINE_SIZE) * TILE_SIZE;
    //             tilesetSprite.setPosition(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
    //             tilesetSprite.setTextureRect(IntRect(horizontalPositionOnSprite, verticalPositionOnSprite, TILE_SIZE, TILE_SIZE));
    //             window.draw(tilesetSprite);
    //         }            
    //         if(tileNumCollectible > -1)
    //         {
    //             int horizontalPositionOnSprite = (tileNumCollectible % TILESET_LINE_SIZE) * TILE_SIZE;
    //             int verticalPositionOnSprite = (tileNumCollectible / TILESET_LINE_SIZE) * TILE_SIZE;
    //             tilesetSprite.setPosition(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
    //             tilesetSprite.setTextureRect(IntRect(horizontalPositionOnSprite, verticalPositionOnSprite, TILE_SIZE, TILE_SIZE));
    //             window.draw(tilesetSprite);
    //         }
    //     }
        //cout << endl;
    }
}

void Tilemap::changeLevel() 
{
    loadLevel("maps/level" + to_string(level) + ".csv");
}


void Tilemap::pickupDiamond(int x, int y)
{
    collectibleTiles[y][x] = 0;
}

void Tilemap::pickupDashStone(int x, int y)
{
    collectibleTiles[y][x] = 0;
    RespawnObject ro = {x, y, DASH_STONE, COLLECTIBLE_RESPAWN_TIME};
    respawnObjects.push_back(ro);
}

void Tilemap::update(Time &deltaTime)
{
    for (size_t i = 0; i < respawnObjects.size(); i++)
    {
        respawnObjects[i].respawnTime -= deltaTime.asSeconds();
        if(respawnObjects[i].respawnTime <= 0)
        {
            collectibleTiles[respawnObjects[i].y][respawnObjects[i].x] = respawnObjects[i].type;
            respawnObjects.erase(respawnObjects.begin() + i);
        }
    }
    
}

#endif