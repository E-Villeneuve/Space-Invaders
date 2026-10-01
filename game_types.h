#pragma once

#include <cstddef>
#include <cstdint>

#include "sprites.h"

constexpr size_t GAME_MAX_PROJ = 128;

struct Buffer
{
    size_t width, height;
    uint32_t *data;
};

struct Alien
{
    size_t x, y;
    uint8_t type;
};

struct Player
{
    size_t x, y;
    size_t life;
};

struct Projectile
{
    size_t x, y;
    int dir;
    bool from_alien;
};

struct SpriteAnimation
{
    bool loop;
    size_t num_frames;
    size_t frame_duration;
    size_t time;
    Sprite *frames[2];
};

enum AlienType : uint8_t
{
    ALIEN_DEAD = 0,
    ALIEN_TYPE_A = 1,
    ALIEN_TYPE_B = 2,
    ALIEN_TYPE_C = 3
};

struct Game
{
    size_t width;
    size_t height;
    size_t alien_count;
    size_t projectile_count;
    Alien *aliens;
    Player player;
    size_t player_death_count;
    Projectile projectiles[GAME_MAX_PROJ];
    SpriteAnimation alien_animation[3];
    uint8_t *death_count;
    int alien_move_dir;
    int alien_move_countdown;
    size_t alien_fire_countdown;
    size_t alien_fire_cursor;
    size_t score;
};