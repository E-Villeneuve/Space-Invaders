#include "game.h"

namespace
{
bool sprite_overlap_check(const Sprite &sprite_a, size_t x_a, size_t y_a, const Sprite &sprite_b, size_t x_b, size_t y_b)
{
    return x_a < x_b + sprite_b.width && x_a + sprite_a.width > x_b &&
           y_a < y_b + sprite_b.height && y_a + sprite_a.height > y_b;
}
}

void game_initialize(Game &game, size_t width, size_t height)
{
    game.width = width;
    game.height = height;
    game.alien_count = 55;
    game.projectile_count = 0;
    game.aliens = new Alien[game.alien_count];
    game.death_count = new uint8_t[game.alien_count];
    game.alien_move_dir = -1;
    game.alien_move_countdown = 15;
    game.alien_fire_countdown = 90;
    game.alien_fire_cursor = 0;
    game.score = 0;

    for (size_t i = 0; i < 3; ++i)
    {
        SpriteAnimation &animation = game.alien_animation[i];
        animation.loop = true;
        animation.num_frames = 2;
        animation.frame_duration = 10;
        animation.time = 0;
        animation.frames[0] = &alien_sprites[2 * i];
        animation.frames[1] = &alien_sprites[2 * i + 1];
    }

    game.player.x = 100;
    game.player.y = 32;
    game.player.life = 3;
    game.player_death_count = 0;

    for (size_t y = 0; y < 5; y++)
    {
        for (size_t x = 0; x < 11; x++)
        {
            Alien &alien = game.aliens[y * 11 + x];
            alien.type = (5 - y) / 2 + 1;
            const Sprite &sprite = alien_sprites[2 * (alien.type - 1)];
            alien.x = 16 * x + 20 + (alien_death_sprite.width - sprite.width) / 2;
            alien.y = 17 * y + 128;
        }
    }

    for (size_t i = 0; i < game.alien_count; ++i)
    {
        game.death_count[i] = 10;
    }
}

void game_move_aliens(Game &game)
{
    if (game.alien_move_countdown == 0)
    {
        size_t left_edge = game.width;
        size_t right_edge = 0;

        for (size_t ai = 0; ai < game.alien_count; ++ai)
        {
            const Alien &alien = game.aliens[ai];
            if (alien.type == ALIEN_DEAD)
                continue;

            const Sprite &sprite = alien_sprites[2 * (alien.type - 1)];
            if (alien.x < left_edge)
                left_edge = alien.x;
            if (alien.x + sprite.width > right_edge)
            {
                right_edge = alien.x + sprite.width;
            }
        }

        const bool hit_edge =
            (game.alien_move_dir < 0 && left_edge == 0) ||
            (game.alien_move_dir > 0 && right_edge >= game.width);

        if (hit_edge)
        {
            game.alien_move_dir *= -1;
        }

        for (size_t ai = 0; ai < game.alien_count; ++ai)
        {
            Alien &alien = game.aliens[ai];
            if (alien.type == ALIEN_DEAD)
                continue;

            if (hit_edge)
            {
                alien.y -= 4;
            }
            else
            {
                alien.x += game.alien_move_dir;
            }
        }

        game.alien_move_countdown = 15;
    }
    else
    {
        --game.alien_move_countdown;
    }
}

void game_update(Game &game, int move_dir, bool fire_pressed)
{
    for (size_t ai = 0; ai < game.alien_count; ++ai)
    {
        const Alien &alien = game.aliens[ai];
        if (alien.type == ALIEN_DEAD && game.death_count[ai])
        {
            --game.death_count[ai];
        }
    }
    if (game.player.life == 0 && game.player_death_count > 0)
    {
        --game.player_death_count;
    }

    for (size_t pi = 0; pi < game.projectile_count;)
    {
        game.projectiles[pi].y += game.projectiles[pi].dir;
        bool remove_projectile = false;
        if (game.projectiles[pi].y >= game.height || game.projectiles[pi].y < 16)
        {
            remove_projectile = true;
        }
        else if (game.projectiles[pi].from_alien)
        {
            if (sprite_overlap_check(projectile_sprite, game.projectiles[pi].x, game.projectiles[pi].y,
                                     player_sprite, game.player.x, game.player.y))
            {
                if (game.player.life > 0)
                {
                    --game.player.life;
                    if (game.player.life == 0)
                    {
                        game.player_death_count = 10;
                    }
                }
                remove_projectile = true;
            }
        }
        else
        {
            for (size_t ai = 0; ai < game.alien_count; ++ai)
            {
                const Alien &alien = game.aliens[ai];
                if (alien.type == ALIEN_DEAD)
                {
                    continue;
                }

                const SpriteAnimation &animation = game.alien_animation[alien.type - 1];
                size_t curr_frame = animation.time / animation.frame_duration;
                const Sprite &sprite = *animation.frames[curr_frame];
                if (!sprite_overlap_check(projectile_sprite, game.projectiles[pi].x, game.projectiles[pi].y,
                                          sprite, alien.x, alien.y))
                {
                    continue;
                }

                game.score += 10 * (4 - game.aliens[ai].type);
                game.aliens[ai].type = ALIEN_DEAD;
                game.aliens[ai].x -= (alien_death_sprite.width - sprite.width) / 2;
                remove_projectile = true;
                break;
            }
        }

        if (remove_projectile)
        {
            game.projectiles[pi] = game.projectiles[game.projectile_count - 1];
            --game.projectile_count;
        }
        else
        {
            ++pi;
        }
    }

    if (game.player.life > 0)
    {
        int player_move_dir = 2 * move_dir;

        if (game.player.x + player_sprite.width + player_move_dir >= game.width)
        {
            game.player.x = game.width - player_sprite.width - player_move_dir;
            player_move_dir *= -1;
        }
        else if ((int)game.player.x + player_move_dir <= 0)
        {
            game.player.x = 0;
            player_move_dir *= -1;
        }
        else
        {
            game.player.x += player_move_dir;
        }

        if (fire_pressed && game.projectile_count < GAME_MAX_PROJ)
        {
            game.projectiles[game.projectile_count].x = game.player.x + player_sprite.width / 2;
            game.projectiles[game.projectile_count].y = game.player.y + player_sprite.height;
            game.projectiles[game.projectile_count].dir = 2;
            game.projectiles[game.projectile_count].from_alien = false;
            ++game.projectile_count;
        }
    }

    if (game.alien_fire_countdown > 0)
    {
        --game.alien_fire_countdown;
    }
    if (game.alien_fire_countdown == 0)
    {
        if (game.projectile_count < GAME_MAX_PROJ)
        {
            for (size_t offset = 0; offset < game.alien_count; ++offset)
            {
                size_t ai = (game.alien_fire_cursor + offset) % game.alien_count;
                const Alien &alien = game.aliens[ai];
                if (alien.type == ALIEN_DEAD)
                {
                    continue;
                }

                Projectile &projectile = game.projectiles[game.projectile_count++];
                projectile.x = alien.x + alien_sprites[2 * (alien.type - 1)].width / 2;
                projectile.y = alien.y - projectile_sprite.height;
                projectile.dir = -2;
                projectile.from_alien = true;
                game.alien_fire_cursor = (ai + 1) % game.alien_count;
                break;
            }
        }
        game.alien_fire_countdown = 90;
    }
}

void game_cleanup(Game &game)
{
    delete[] game.death_count;
    delete[] game.aliens;
    game.death_count = nullptr;
    game.aliens = nullptr;
}