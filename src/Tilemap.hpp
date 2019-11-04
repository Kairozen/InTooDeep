#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#define TILE_SIZE 16
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define MAX_X 100
#define MAX_Y 30

#define TILESET_COLUMN_SIZE 32
#define TILESET_LINE_SIZE 32

#define SPIKES 1021
#define DIAMOND 602
#define DASH_STONE 600
#define MONSTER_TILE 561
#define NEXT_LEVEL 81
#define CHECKPOINT 435
#define COLLECTIBLE_RESPAWN_TIME 1.5

using namespace sf;

class Character;

struct RespawnObject
{
    int x, y, type;
    float respawnTime;
};


class Tilemap 
{
    public:
        Texture tilesetTexture;
        Sprite tilesetSprite;
        Texture backgroundTexture;
        Sprite backgroundSprite;

        // Start of map
        Vector2<int> mapStart;

        // End of map
        Vector2<int> mapEnd;

        // Character starting position
        Vector2<int> characterBeginPosition;

        // Vector of objects that will respawn soon
        std::vector<RespawnObject> respawnObjects;

        // Tilemaps
        int backgroundTiles[MAX_Y][MAX_X];
        int collidingTiles[MAX_Y][MAX_X];
        int collectibleTiles[MAX_Y][MAX_X];

        int level;

        Tilemap();

        void drawBackground(RenderWindow &window);

        void loadLevel(std::string filename);

        void drawMap(RenderWindow &window, View &camera);

        void changeLevel();

        void pickupDiamond(int x, int y);

        void pickupDashStone(int x, int y);

        void update(Time &deltaTime);
};

#endif