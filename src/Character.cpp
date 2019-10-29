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
    nbDiamonds = 0;
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
    isDashing = false;
    canDash = true;
    goToNextLevel = false;
    collidingBox = FloatRect(position.x + ((SPRITE_WIDTH - CHARACTER_WIDTH) >> 1), position.y + SPRITE_HEIGHT - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT);
}

void Character::draw(RenderWindow &window, Time &deltaTime)
{
    if(isDashing)
    {
        Sprite copy = sprite;
        Color color = Color(0,0,0, 127);
        copy.setColor(color);
        window.draw(copy);
    }
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
        sprite.move(sprite.getRotation() / 5, sprite.getRotation() / 12);
    
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
    if(!isDashing)
    {
        movementVector.x = 0;
        movementVector.y += GRAVITY * deltaTime.asSeconds();
    }
    if(!isDead)
    {
        if(input.getButton().dash && !input.getLastButton().dash)
        {
            startDashing(input);
        }
        if(!isDashing)
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
                if(!input.getLastButton().jump && wallLeft && state == WALL_SLIDE && movementVector.y > 0 && !isOnGround && !isWallJumpingLeft)
                {
                    wallJumpLeft(deltaTime, audio);
                }
                else if(!input.getLastButton().jump && wallRight && state == WALL_SLIDE && movementVector.y > 0 && !isOnGround && !isWallJumpingRight)
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
            dashTime -= deltaTime.asSeconds();
            switch(dashDirection)
            {
                case UP:
                    movementVector.x = 0;
                    movementVector.y = -DASH_POWER * deltaTime.asSeconds();
                    break;
                case BOTTOM:
                    movementVector.x = 0;
                    movementVector.y = DASH_POWER * deltaTime.asSeconds();
                    break;
                case LEFT:
                    movementVector.x = -DASH_POWER * deltaTime.asSeconds();
                    movementVector.y = 0;
                    break;
                case RIGHT:
                    movementVector.x = DASH_POWER * deltaTime.asSeconds();
                    movementVector.y = 0;
                    break;
                case UP_LEFT:
                    movementVector.y = -DASH_POWER * deltaTime.asSeconds();
                    movementVector.x = -DASH_POWER * deltaTime.asSeconds();
                    break;
                case UP_RIGHT:
                    movementVector.y = -DASH_POWER * deltaTime.asSeconds();
                    movementVector.x = DASH_POWER * deltaTime.asSeconds();
                    break;
                case BOTTOM_LEFT:
                    movementVector.y = DASH_POWER * deltaTime.asSeconds();
                    movementVector.x = -DASH_POWER * deltaTime.asSeconds();
                    break;
                case BOTTOM_RIGHT:
                    movementVector.y = DASH_POWER * deltaTime.asSeconds();
                    movementVector.x = DASH_POWER * deltaTime.asSeconds();
                    break;
            }
            if(dashTime <= 0)
            {
                isDashing = false;
            }
        }
        
    }
    else
    {
        deathTimer -= deltaTime.asSeconds();
    }

    handleCollision(map);

    collidingBox.left = position.x + ((SPRITE_WIDTH - CHARACTER_WIDTH) >> 1);
    collidingBox.top = position.y + SPRITE_HEIGHT - CHARACTER_HEIGHT;

    if(wallRight && input.getButton().right)
    {
        changeState(WALL_SLIDE);
    }
    if(wallLeft && input.getButton().left)
    {
        changeState(WALL_SLIDE);
    }
    if(position.x < 0)
        position.x = 0;
    if(position.x + SPRITE_WIDTH > MAX_X * TILE_SIZE)
        position.x = MAX_X * TILE_SIZE;
    
}

void Character::handleCollision(Tilemap &map)
{
    int xLeft, xRight, yTop, yBottom;
    isOnGround = false;
    wallRight = false;
    wallLeft = false;

    // Horizontal collisions
    xLeft = (collidingBox.left + movementVector.x) / TILE_SIZE;
    xRight = (collidingBox.left + collidingBox.width + movementVector.x) / TILE_SIZE;
    yTop = collidingBox.top / TILE_SIZE;
    yBottom = (collidingBox.top + collidingBox.height - 1) / TILE_SIZE;

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
            position.x = (xRight * TILE_SIZE) - SPRITE_WIDTH;
            // position.x -= SPRITE_WIDTH + 1;
            movementVector.x = 0;
            wallRight = true;
        }
    }

    // Vertical collisions
    xLeft = collidingBox.left / TILE_SIZE;
    xRight = (collidingBox.left + collidingBox.width) / TILE_SIZE;
    yTop = (collidingBox.top + movementVector.y) / TILE_SIZE;
    yBottom = (collidingBox.top + movementVector.y + collidingBox.height) / TILE_SIZE;

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
            canDash = true;
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
    xLeft = collidingBox.left / TILE_SIZE;
    xRight = (collidingBox.left + collidingBox.width) / TILE_SIZE;
    yTop = collidingBox.top / TILE_SIZE;
    yBottom = (collidingBox.top + collidingBox.height) / TILE_SIZE;

    // Collision with other objects 
    if(map.collectibleTiles[yTop][xLeft] || 
        map.collectibleTiles[yTop][xRight] ||
        map.collectibleTiles[yBottom][xLeft] ||
        map.collectibleTiles[yBottom][xRight])
    {
        // Collision with spikes
        if(map.collectibleTiles[yTop][xLeft] >= SPIKES || 
            map.collectibleTiles[yTop][xRight] >= SPIKES ||
            map.collectibleTiles[yBottom][xLeft] >= SPIKES ||
            map.collectibleTiles[yBottom][xRight] >= SPIKES)
        {
            kill();
        }
        // Collision with diamond
        if(map.collectibleTiles[yTop][xLeft] == DIAMOND)
        {
            map.pickupDiamond(xLeft, yTop);
            audio->playDiamondSound();
            nbDiamonds++;
        }
        else if(map.collectibleTiles[yTop][xRight] == DIAMOND)
        {
            map.pickupDiamond(xRight, yTop);
            audio->playDiamondSound();
            nbDiamonds++;
        }
        else if(map.collectibleTiles[yBottom][xLeft] == DIAMOND)
        {
            map.pickupDiamond(xLeft, yBottom);
            audio->playDiamondSound();
            nbDiamonds++;
        }
        else if(map.collectibleTiles[yBottom][xRight] == DIAMOND)
        {
            map.pickupDiamond(xRight, yBottom);
            audio->playDiamondSound();
            nbDiamonds++;
        }
        // Collision with dash stone
        if(map.collectibleTiles[yTop][xLeft] == DASH_STONE)
        {
            map.pickupDashStone(xLeft, yTop);
            audio->playDashStoneSound();
            canDash = true;
        }
        else if(map.collectibleTiles[yTop][xRight] == DASH_STONE)
        {
            map.pickupDashStone(xRight, yTop);
            audio->playDashStoneSound();
            canDash = true;
        }
        else if(map.collectibleTiles[yBottom][xLeft] == DASH_STONE)
        {
            map.pickupDashStone(xLeft, yBottom);
            audio->playDashStoneSound();
            canDash = true;
        }
        else if(map.collectibleTiles[yBottom][xRight] == DASH_STONE)
        {
            map.pickupDashStone(xRight, yBottom);
            audio->playDashStoneSound();
            canDash = true;
        }
        // For going to next level
        if(map.collectibleTiles[yTop][xLeft] == NEXT_LEVEL || 
            map.collectibleTiles[yTop][xRight] == NEXT_LEVEL ||
            map.collectibleTiles[yBottom][xLeft] == NEXT_LEVEL ||
            map.collectibleTiles[yBottom][xRight] == NEXT_LEVEL)
        {
            goToNextLevel = true;
        }
    }
}

void Character::startDashing(UserInput &input)
{
    if(canDash && !isOnGround)
    {
        audio->playDashSound();
        changeState(JUMP);
        isDashing = true;
        canDash = false;
        if(input.getButton().up && input.getButton().left)
            dashDirection = UP_LEFT;
        else if(input.getButton().up && input.getButton().right)
            dashDirection = UP_RIGHT;
        else if(input.getButton().up)
            dashDirection = UP;
        else if(input.getButton().down && input.getButton().left)
            dashDirection = BOTTOM_LEFT;
        else if(input.getButton().down && input.getButton().right)
            dashDirection = BOTTOM_RIGHT;
        else if(input.getButton().down)
            dashDirection = BOTTOM;
        else if(input.getButton().left)
            dashDirection = LEFT;
        else if(input.getButton().right)
            dashDirection = RIGHT;
        else
            dashDirection = direction;
        if(dashDirection == UP_LEFT || dashDirection == UP_RIGHT || dashDirection == BOTTOM_RIGHT || dashDirection == BOTTOM_LEFT)
            dashTime = DASH_TIME / 1.5;
        else
            dashTime = DASH_TIME;
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
        isDashing = false;
        canDash = false;
        deathTimer =  DEATH_TIMER;
    }
}

Character::~Character() {}

#endif