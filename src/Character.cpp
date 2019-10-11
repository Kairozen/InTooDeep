#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "Character.hpp"

using namespace std;

Character::Character()
{
    if(!characterTexture.loadFromFile("graphics/player_spritesheet.png"))
        throw runtime_error("Erreur lors du chargement du sprite personnage");
    characterSprite.setTexture(characterTexture);

    direction = RIGHT;
    lives = 3;
    
    position = Vector2f(0,0);
    movementVector = Vector2f(0,0);
    state = IDLE;
    frameNum = frameTimer = frameMax = 0;
    isOnGround = false;
}

void Character::initialize()
{
    lives = 3;
    direction = RIGHT;
    state = IDLE;
    frameMax = NB_FRAMES_IDLE;
    frameNum = 0;
    frameTimer = ANIMATION_SPEED;
    isOnGround = false;
    position = Vector2f(0,0);
    movementVector = Vector2f(0,0);
}

void Character::draw(RenderWindow &window)
{
    // Handling animation
    if(frameTimer <= 0)
    {
        frameTimer = ANIMATION_SPEED;
        ++frameNum;
        if(frameNum >= frameMax)
            frameNum = 0;
    }
    else
    {
        --frameTimer;
    }

    // Position of sprite
    characterSprite.setPosition(position);

    // Take the rect to draw depending on direction
    if(direction == LEFT)
    {
        characterSprite.setTextureRect(IntRect((frameNum + 1) * SPRITE_WIDTH, state * SPRITE_HEIGHT, -SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    else
    {
        characterSprite.setTextureRect(IntRect(frameNum * SPRITE_WIDTH, state * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT));
    }

    window.draw(characterSprite);
}


void Character::update(UserInput &input, Tilemap& map, Time &deltaTime)
{
    movementVector.x = 0;
    movementVector.y += GRAVITY * deltaTime.asSeconds();

    if(movementVector.y > MAX_FALLING_SPEED)
        movementVector.y = MAX_FALLING_SPEED;

    if(input.getButton().left)
    {
        moveLeft(deltaTime);
    }
    if(input.getButton().right)
    {
        moveRight(deltaTime);
    }
    if(input.getButton().jump)
    {
        jump(deltaTime);
    }
    if(!input.getButton().left && !input.getButton().right)
    {
        idle();
    }
    // If the player is not on the ground, it's in the air so switch to jump animation
    if(!isOnGround)
    {
        if(state != JUMP)
        {
            state = JUMP;
            frameNum = 0;
            frameTimer = ANIMATION_SPEED;
            frameMax = NB_FRAMES_JUMP;
        }
    }

    handleCollision(map);
}

void Character::handleCollision(Tilemap &map)
{
    isOnGround = false;    
    int xLeft, xRight, yTop, yBottom;

    cout << "x: " << position.x << " y: " << position.y << " move x: " << movementVector.x << " move y: " << movementVector.y << " ";

    xLeft = (position.x + movementVector.x) / TILE_SIZE;
    xRight = (position.x + movementVector.x + SPRITE_WIDTH) / TILE_SIZE;
    yTop = (position.y + movementVector.y) / TILE_SIZE;
    yBottom = (position.y + movementVector.y + SPRITE_HEIGHT) / TILE_SIZE;

    if(xLeft >= 0 && xRight < MAX_X && yTop >= 0)
    {
        // Moving to the left
        if(movementVector.x < 0)
        {
            if(map.collidingTiles[yTop][xLeft]/* || map.collidingTiles[yBottom][xLeft]*/)
            {
                cout << "gauche is nope" << endl;
                position.x = xLeft * TILE_SIZE;
                position.x -= SPRITE_WIDTH + 1;
                movementVector.x = 0;
            }
        }
        // Moving to the right
        if(movementVector.x > 0)
        {    
            if(map.collidingTiles[yTop][xRight] /*|| map.collidingTiles[yBottom][xRight]*/)
            {
                cout << "noooon pas à droite" << endl;
                position.x = (xRight + 1) * TILE_SIZE;
                movementVector.x = 0;
            }
        }
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
                cout << "BONK LE PLAFOND" << endl;
                position.y = (yTop + 1) * TILE_SIZE;
                movementVector.y = 0;
            }
        }

        position += movementVector;

        if(position.y > MAX_Y * TILE_SIZE)
        {
            // We should die here
            cout << "DEAD BOI" << endl;
        }
    }
}

void Character::jump(Time &deltaTime)
{
    if(isOnGround)
    {
        movementVector.y = -JUMP_HEIGHT /** deltaTime.asSeconds()*/;
        isOnGround = false;
    }
}

void Character::moveRight(Time &deltaTime)
{
    direction = RIGHT;
    movementVector.x += RUNNING_SPEED * deltaTime.asSeconds();
    if(state != RUN && isOnGround)
    {
        state = RUN;
        frameNum = 0;
        frameTimer = ANIMATION_SPEED;
        frameMax = NB_FRAMES_RUN;
    }
}

void Character::moveLeft(Time &deltaTime)
{    
    direction = LEFT;
    movementVector.x -= RUNNING_SPEED * deltaTime.asSeconds();
    if(state != RUN && isOnGround)
    {
        state = RUN;
        frameNum = 0;
        frameTimer = ANIMATION_SPEED;
        frameMax = NB_FRAMES_RUN;
    }
}

void Character::idle()
{
    if(state != IDLE)
    {
        state = IDLE;
        frameNum = 0;
        frameTimer = ANIMATION_SPEED;
        frameMax = NB_FRAMES_IDLE;
    }
}

Character::~Character() {}

#endif