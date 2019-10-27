#ifndef MONSTER_CPP
#define MONSTER_CPP

#include "Monster.hpp"

using namespace std;

Monster::Monster(Vector2f position)
{
    if(!texture.loadFromFile("graphics/slimes.png"))
        throw runtime_error("Erreur lors du chargement du sprite monstre");
    sprite.setTexture(texture);

    direction = RIGHT;
    
    this->position = position;
    movementVector = Vector2f(0,0);
    frameNum = 0;
    frameTimer = 0;
    frameMax = ANIMATION_NB_FRAMES;
    isOnGround = false;
}

void Monster::draw(RenderWindow &window, Time &deltaTime)
{
    // Handling animation
    if(frameTimer >= ANIMATION_SPEED)
    {
        frameTimer = 0;
        ++frameNum;
        if(frameNum >= frameMax)
            frameNum = 0;
    }
    else
    {
        frameTimer += deltaTime.asSeconds();
    }
    // Position of sprite
    sprite.setPosition(position);
    
    // Take the rect to draw depending on direction
    if(direction == LEFT)
    {
        sprite.setTextureRect(IntRect((frameNum + 1) * SPRITE_WIDTH, color * SPRITE_HEIGHT, -SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    else
    {
        sprite.setTextureRect(IntRect(frameNum * SPRITE_WIDTH, color * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT));
    }

    // RectangleShape shape;
    // shape.setFillColor(Color::Blue);
    // shape.setPosition(Vector2f(collidingBox.left, collidingBox.top));
    // shape.setSize(Vector2f(collidingBox.width, collidingBox.height));
    // window.draw(shape);

    window.draw(sprite);
}

void Monster::update(Tilemap &map, Time &deltaTime)
{
    movementVector.x = 0;
    movementVector.y += GRAVITY * deltaTime.asSeconds();

    if(direction == LEFT)
        moveLeft(deltaTime);
    else
        moveRight(deltaTime);

    handleCollision(map);

    collidingBox = FloatRect(position.x + (SPRITE_WIDTH - MONSTER_WIDTH), position.y + (SPRITE_HEIGHT - MONSTER_HEIGHT), MONSTER_WIDTH, MONSTER_HEIGHT);
}

/* Returns 0 if no collision with player, 1 if the player kills the monster, 2 if the monster kills the player */
int Monster::collisionWithPlayer(Character &character)
{
    if(collidingBox.intersects(character.collidingBox))
    {
        if(character.collidingBox.top + character.collidingBox.height <= collidingBox.top + collidingBox.height / 2)
            return 1;
        return 2;
    }
    else
        return 0;
}

void Monster::handleCollision(Tilemap &map)
{
    int xLeft, xRight, yTop, yBottom;
    isOnGround = false;

    // Horizontal collisions
    xLeft = (position.x + movementVector.x) / TILE_SIZE;
    xRight = (position.x + movementVector.x + SPRITE_WIDTH) / TILE_SIZE;
    yTop = position.y / TILE_SIZE;
    yBottom = (position.y + SPRITE_HEIGHT - 1) / TILE_SIZE;

    // Moving to the left
    if(movementVector.x < 0)
    {
        // If colliding on a wall, change direction
        if(map.collidingTiles[yTop][xLeft] || map.collidingTiles[yBottom][xLeft])
        {
            position.x = (xLeft + 1) * TILE_SIZE;
            movementVector.x = 0;
            direction = RIGHT;
        }
    }
    // Moving to the right
    if(movementVector.x > 0)
    {    
        // If colliding on a wall, change direction
        if(map.collidingTiles[yTop][xRight] || map.collidingTiles[yBottom][xRight])
        {
            position.x = xRight * TILE_SIZE;
            position.x -= SPRITE_WIDTH + 1;
            movementVector.x = 0;
            direction = LEFT;
        }
    }

        // Vertical collisions
    xLeft = position.x / TILE_SIZE;
    xRight = (position.x + SPRITE_WIDTH) / TILE_SIZE;
    yTop = (position.y + movementVector.y) / TILE_SIZE;
    yBottom = (position.y + movementVector.y + SPRITE_HEIGHT) / TILE_SIZE;

    // Moving towards bottom
    if(movementVector.y > 0)
    {
        if(map.collidingTiles[yBottom][xLeft] || map.collidingTiles[yBottom][xRight])
        {
            position.y = yBottom * TILE_SIZE;
            position.y -= SPRITE_HEIGHT;
            movementVector.y = 0;
            isOnGround = true;
        }
    }
    // Moving towards top
    if(movementVector.y < 0)
    {
        if(map.collidingTiles[yTop][xLeft] || map.collidingTiles[yTop][xRight])
        {
            position.y = (yTop + 1) * TILE_SIZE;
            movementVector.y = 0;
        }
    }

    position += movementVector;
}

void Monster::moveRight(Time &deltaTime)
{
    direction = RIGHT;
    movementVector.x += RUNNING_SPEED * deltaTime.asSeconds();
}

void Monster::moveLeft(Time &deltaTime)
{    
    direction = LEFT;
    movementVector.x -= RUNNING_SPEED * deltaTime.asSeconds();
}

#endif