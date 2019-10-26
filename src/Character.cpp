#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "Character.hpp"

using namespace std;

Character::Character()
{
    if(!characterTexture.loadFromFile("graphics/character_spritesheet.png"))
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
    isJumping = false;
    isSlidingOnWall = false;
    wallLeft = false;
    wallRight = false;
    isWallJumpingLeft = false;
    isWallJumpingRight = false;
    jumpTime = 0;
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
    
    // Slow down the falling if we're on a wall and going down
    if(state == WALL_SLIDE)
        movementVector.y *= 0.8;

    if(movementVector.y > MAX_FALLING_SPEED)
        movementVector.y = MAX_FALLING_SPEED;
    
    cout << "grav:" << movementVector.y << endl;

    if(isJumping && !input.getButton().jump)
        isJumping = false;

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
        if(wallLeft && state == WALL_SLIDE && movementVector.y > 0 && !isOnGround && !isWallJumpingLeft)
        {
            wallJumpLeft(deltaTime);
        }
        else if(wallRight && state == WALL_SLIDE && movementVector.y > 0 && !isOnGround && !isWallJumpingRight)
        {
            wallJumpRight(deltaTime);
        }
        else
        {
            jump(deltaTime);
        }
        
    }
    if(!input.getButton().left && !input.getButton().right && isOnGround)
    {
        idle();
    }
    // Continue walljump if possible
    if(isWallJumpingLeft)
        wallJumpLeft(deltaTime);
    if(isWallJumpingRight)
        wallJumpRight(deltaTime);

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

    if(wallRight && input.getButton().right)
    {
        state = WALL_SLIDE;
    }
    if(wallLeft && input.getButton().left)
    {
        state = WALL_SLIDE;
    }
}

void Character::handleCollision(Tilemap &map)
{
    int xLeft, xRight, yTop, yBottom;
    isOnGround = false;
    wallRight = false;
    wallLeft = false;

    // Horizontal collisions
    xLeft = (position.x + movementVector.x) / TILE_SIZE;
    xRight = (position.x + movementVector.x + SPRITE_WIDTH) / TILE_SIZE;
    yTop = position.y / TILE_SIZE;
    yBottom = (position.y + SPRITE_HEIGHT - 1) / TILE_SIZE;

    //cout << "x: " << position.x << " y: " << position.y << " move x: " << movementVector.x << " move y: " << movementVector.y << " xleft: " << xLeft << " xright: " << xRight << " ytop: " << yTop << " ybttom: " << yBottom << " ";

    // Moving to the left
    if(movementVector.x < 0)
    {
        if(map.collidingTiles[yTop][xLeft] || map.collidingTiles[yBottom][xLeft])
        {
            position.x = (xLeft + 1) * TILE_SIZE;
            movementVector.x = 0;
            wallLeft = true;
        }
    }
    // Moving to the right
    if(movementVector.x > 0)
    {    
        if(map.collidingTiles[yTop][xRight] || map.collidingTiles[yBottom][xRight])
        {
            position.x = xRight * TILE_SIZE;
            position.x -= SPRITE_WIDTH + 1;
            movementVector.x = 0;
            wallRight = true;
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

void Character::wallJumpLeft(Time &deltaTime)
{
    if(!isWallJumpingLeft)
    {
        wallJumpTime = 0;
        isWallJumpingLeft = true;
    }
    if(wallJumpTime < WALL_JUMP_LIMIT)
    {
        float tmpWallJumpTime;
        wallJumpTime += deltaTime.asSeconds();
        tmpWallJumpTime = wallJumpTime / 3;
        movementVector.x = RUNNING_SPEED * 1.5 * deltaTime.asSeconds();
        movementVector.y = -JUMP_HEIGHT * tmpWallJumpTime;
        state = JUMP;
        isSlidingOnWall = false;
    }
    else
    {
        isWallJumpingLeft = false;
    }
}

void Character::wallJumpRight(Time &deltaTime)
{
    if(!isWallJumpingRight)
    {
        wallJumpTime = 0;
        isWallJumpingRight = true;
    }
    if(wallJumpTime < WALL_JUMP_LIMIT)
    {
        float tmpWallJumpTime;
        wallJumpTime += deltaTime.asSeconds();
        tmpWallJumpTime = wallJumpTime / 3;
        movementVector.x = -RUNNING_SPEED * 1.5 * deltaTime.asSeconds();
        movementVector.y = -JUMP_HEIGHT * tmpWallJumpTime;
        state = JUMP;
        isSlidingOnWall = false;
    }
    else
    {
        isWallJumpingRight = false;
    }
}

void Character::jump(Time &deltaTime)
{
    if(isOnGround)
    {
        jumpTime = 0;
        movementVector.y = - 2 * JUMP_HEIGHT * deltaTime.asSeconds();
        isJumping = true;
        isOnGround = false;
    }
    else if(isJumping)
    {
        jumpTime += deltaTime.asSeconds();
        if(jumpTime < JUMP_LIMIT)
        {
            movementVector.y = -JUMP_HEIGHT * jumpTime + ((GRAVITY * jumpTime * jumpTime) / 2);
        }
        else
        {
            isJumping = false;
        }
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