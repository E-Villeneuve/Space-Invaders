#pragma once

#include "game_types.h"

extern const uint32_t GREEN;
extern const uint32_t DARK_RED;
extern const uint32_t CYAN;
extern const uint32_t BLACK;
extern const uint32_t WHITE;

void buffer_sprite_draw(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t colour);
void buffer_text_draw(Buffer *buffer, const Sprite &text_spritesheet, const char *text, size_t x, size_t y, uint32_t colour);
void buffer_number_draw(Buffer *buffer, const Sprite &number_spritesheet, size_t number, size_t x, size_t y, uint32_t colour);
uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b);
void buffer_clear(Buffer *buffer, uint32_t colour);