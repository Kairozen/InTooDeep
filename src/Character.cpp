#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "Character.hpp"

using namespace std;

Character::Character()
{
    if(!texture.loadFromFile("graphics/character_spritesheet.png"))
        throw runtime_error("Erreur lors du chargement du sprite personnage");
    sprite.setTexture(texture);

    direction = RIGHT;
    lives = 3;
    
    position = Vector2f(0,0);
    movementVector = Vector2f(0,0);
    changeState(IDLE);
    isOnGround = false;
    isDead = false;
    deathTimer = 0;
}

void Character::initialize()
{
    lives = 3;
    direction = RIGHT;
    changeState(IDLE);
    isOnGround = false;
    position = Vector2f(0,0);
    movementVector = Vector2f(0,0);
    isJumping = false;
    isSlidingOnWall = false;
    wallLeft = false;
    wallRight = false;
    isWallJumpingLeft = false;
    isWallJumpingRight = false;
    sprite.setRotation(0);
    sprite.setPosition(0,0);
    sprite.setOrigin(0,0);
    isDead = false;
    deathTimer = 0;
    jumpTime = 0;
    collidingBox = FloatRect(position.x, position.y, SPRITE_WIDTH, SPRITE_HEIGHT);
}

void Character::draw(RenderWindow &window, Time &deltaTime)
{
    if(!isDead)
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

        // Take the rect to draw depending on direction
        if(direction == LEFT)
        {
            sprite.setTextureRect(IntRect((frameNum + 1) * SPRITE_WIDTH, state * SPRITE_HEIGHT, -SPRITE_WIDTH, SPRITE_HEIGHT));
        }
        else
        {
            sprite.setTextureRect(IntRect(frameNum * SPRITE_WIDTH, state * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT));
        }
    }
    else
    {
        if(sprite.getRotation() < 90)
        {
            sprite.rotate(90 * 3 * deltaTime.asSeconds());
        }
    }

    // Position of sprite
    sprite.setPosition(position);
    if(isDead)
        sprite.move(sprite.getRotation() / 1.5 * SPRITE_HEIGHT * deltaTime.asSeconds(), sprite.getRotation() / 4 * SPRITE_HEIGHT * deltaTime.asSeconds());
    
    // RectangleShape shape;
    // shape.setFillColor(Color::Blue);
    // shape.setPosition(Vector2f(collidingBox.left, collidingBox.top));
    // shape.setSize(Vector2f(collidingBox.width, collidingBox.height));
    // window.draw(shape);
    window.draw(sprite);
}

void Character::changeState(int state)
{
    this->state = state;
    frameNum = 0;
    frameTimer = 0;
    frameMax = ANIMATION_NB_FRAMES[state];
}

void Character::update(UserInput &input, Audio &audio, Tilemap& map, Time &deltaTime)
{
    movementVector.x = 0;
    movementVector.y += GRAVITY * deltaTime.asSeconds();
    if(!isDead)
    {
        // Slow down the falling if we're on a wall and going down
        if(state == WALL_SLIDE)
            movementVector.y *= 0.8;

        if(movementVector.y > MAX_FALLING_SPEED)
            movementVector.y = MAX_FALLING_SPEED;

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
                wallJumpLeft(deltaTime, audio);
            }
            else if(wallRight && state == WALL_SLIDE && movementVector.y > 0 && !isOnGround && !isWallJumpingRight)
            {
                wallJumpRight(deltaTime, audio);
            }
            else
            {
                jump(deltaTime, audio);
            }
            
        }
        if(!input.getButton().left && !input.getButton().right && isOnGround)
        {
            idle();
        }
        // Continue walljump if possible
        if(isWallJumpingLeft)
            wallJumpLeft(deltaTime, audio);
        if(isWallJumpingRight)
            wallJumpRight(deltaTime, audio);

        // If the player is not on the ground, it's in the air so switch to jump animation
        if(!isOnGround && state != JUMP)
        {
            changeState(JUMP);
        }

    }
    else
    {
        deathTimer -= deltaTime.asSeconds();
    }

    handleCollision(map);

    collidingBox.left = position.x;
    collidingBox.top = position.y;

    if(wallRight && input.getButton().right)
    {
        changeState(WALL_SLIDE);
    }
    if(wallLeft && input.getButton().left)
    {
        changeState(WALL_SLIDE);
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

    // Moving to the left
    if(movementVector.x < 0)
    {
        // Collision with colliding tiles
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
        // Collision with colliding tiles
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
        // Collision with colliding tiles
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
        // Collision with colliding tiles
        if(map.collidingTiles[yTop][xLeft] || map.collidingTiles[yTop][xRight])
        {
            position.y = (yTop + 1) * TILE_SIZE;
            movementVector.y = 0;
        }
    }

    position += movementVector;

    // New position
    xLeft = position.x / TILE_SIZE;
    xRight = (position.x + SPRITE_WIDTH) / TILE_SIZE;
    yTop = position.y / TILE_SIZE;
    yBottom = (position.y + SPRITE_HEIGHT) / TILE_SIZE;
    // Collision with spikes
    if(map.collectibleTiles[yTop][xLeft] >= SPIKES || 
        map.collectibleTiles[yTop][xRight] >= SPIKES ||
        map.collectibleTiles[yBottom][xLeft] >= SPIKES ||
        map.collectibleTiles[yBottom][xRight] >= SPIKES)
    {
        kill();
    }
}

void Character::wallJumpLeft(Time &deltaTime, Audio &audio)
{
    if(!isWallJumpingLeft)
    {
        wallJumpTime = 0;
        isWallJumpingLeft = true;
        audio.playWalljumpSound();
    }
    if(wallJumpTime < WALL_JUMP_LIMIT)
    {
        float tmpWallJumpTime;
        wallJumpTime += deltaTime.asSeconds();
        tmpWallJumpTime = wallJumpTime / 3;
        movementVector.x = RUNNING_SPEED * 1.5 * deltaTime.asSeconds();
        movementVector.y = (-JUMP_HEIGHT * tmpWallJumpTime + (tmpWallJumpTime * tmpWallJumpTime / 2)) * deltaTime.asSeconds() * DELTATIME_MULTIPLICATOR;
        changeState(JUMP);
        isSlidingOnWall = false;
    }
    else
    {
        isWallJumpingLeft = false;
    }
}

void Character::wallJumpRight(Time &deltaTime, Audio &audio)
{
    if(!isWallJumpingRight)
    {
        wallJumpTime = 0;
        isWallJumpingRight = true;
        audio.playWalljumpSound();
    }
    if(wallJumpTime < WALL_JUMP_LIMIT)
    {
        float tmpWallJumpTime;
        wallJumpTime += deltaTime.asSeconds();
        tmpWallJumpTime = wallJumpTime / 3;
        movementVector.x = -RUNNING_SPEED * 1.5 * deltaTime.asSeconds();
        movementVector.y = (-JUMP_HEIGHT * tmpWallJumpTime + (tmpWallJumpTime * tmpWallJumpTime / 2)) * deltaTime.asSeconds() * DELTATIME_MULTIPLICATOR;
        changeState(JUMP);
        isSlidingOnWall = false;
    }
    else
    {
        isWallJumpingRight = false;
    }
}

void Character::jump(Time &deltaTime, Audio &audio)
{
    if(isOnGround)
    {
        audio.playJumpSound();
        jumpTime = 0;
        movementVector.y = - 2 * JUMP_HEIGHT * deltaTime.asSeconds();
        isJumping = true;
        isOnGround = false;
    }
    else if(isJumping)
    {
        jumpTime += deltaTime.asSeconds();
        if(jumpTime <= JUMP_LIMIT)
        {
            movementVector.y = (-JUMP_HEIGHT * jumpTime + ((GRAVITY * jumpTime * jumpTime) / 2)) * deltaTime.asSeconds() * DELTATIME_MULTIPLICATOR;
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
        changeState(RUN);
    }
}

void Character::moveLeft(Time &deltaTime)
{    
    direction = LEFT;
    movementVector.x -= RUNNING_SPEED * deltaTime.asSeconds();
    if(state != RUN && isOnGround)
    {
        changeState(RUN);
    }
}

void Character::idle()
{
    if(state != IDLE)
    {
        changeState(IDLE);
    }
}

void Character::kill()
{
    if(!isDead)
    {
        if(audio)
            audio->playKillSound();
        isDead = true;
        deathTimer =  DEATH_TIMER;
    }
}

Character::~Character() {}

#endif