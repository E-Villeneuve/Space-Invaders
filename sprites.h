#pragma once
#include <cstddef>
#include <cstdint>

struct Sprite {
    size_t width, height;
    uint8_t* data;
};

extern Sprite player_sprite;
extern Sprite alien_sprite0;
extern Sprite alien_sprite1;
extern Sprite projectile_sprite;
extern Sprite alien_sprites[6];
extern Sprite alien_death_sprite;
extern Sprite text_spritesheet;
extern Sprite number_spritesheet;