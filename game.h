#pragma once

#include "game_types.h"

void game_initialize(Game &game, size_t width, size_t height);
void game_move_aliens(Game &game);
void game_update(Game &game, int move_dir, bool fire_pressed);
void game_cleanup(Game &game);