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
    nextPosition = Vector2f(0,0);
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
    nextPosition = Vector2f(0,0);
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

Character::~Character() {}