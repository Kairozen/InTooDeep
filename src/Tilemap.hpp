#define TILE_SIZE 16
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define MAX_X 100
#define MAX_Y 30

#define TILESET_COLUMN_SIZE 9
#define TILESET_LINE_SIZE 10

using namespace sf;

class Character;

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

        // Tilemaps
        int backgroundTiles[MAX_Y][MAX_X];
        int collidingTiles[MAX_Y][MAX_X];

        int level;

        Tilemap();

        void drawBackground(RenderWindow &window);

        void loadLevel(std::string filename);

        void drawMap(RenderWindow &window);

        void changeLevel();
};