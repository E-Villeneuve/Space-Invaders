#include "game_render.h"

#include "software_renderer.h"

void game_render(Game &game, Buffer &buffer, uint32_t clear_colour)
{
    buffer_clear(&buffer, clear_colour);

    buffer_text_draw(&buffer, text_spritesheet, "SCORE", 4, game.height - text_spritesheet.height - 7, GREEN);
    buffer_number_draw(&buffer, number_spritesheet, game.score, 4 + 2 * number_spritesheet.width, game.height - 2 * number_spritesheet.height - 12, GREEN);
    for (size_t i = 0; i < game.width; ++i)
    {
        buffer.data[game.width * 16 + i] = GREEN;
    }
    buffer_text_draw(&buffer, text_spritesheet, "LIVES", 4, 5, GREEN);
    for (size_t i = 0; i + 1 < game.player.life; ++i)
    {
        buffer_sprite_draw(&buffer, player_sprite, 38 + i * (player_sprite.width + 1), text_spritesheet.height - 1, GREEN);
    }

    for (size_t ai = 0; ai < game.alien_count; ++ai)
    {
        if (!game.death_count[ai])
        {
            continue;
        }

        const Alien &alien = game.aliens[ai];
        if (alien.type == ALIEN_DEAD)
        {
            buffer_sprite_draw(&buffer, alien_death_sprite, alien.x, alien.y, DARK_RED);
        }
        else
        {
            const SpriteAnimation &animation = game.alien_animation[alien.type - 1];
            size_t curr_frame = animation.time / animation.frame_duration;
            const Sprite &sprite = *animation.frames[curr_frame];
            buffer_sprite_draw(&buffer, sprite, alien.x, alien.y, WHITE);
        }
    }

    for (size_t pi = 0; pi < game.projectile_count; ++pi)
    {
        const Projectile &projectile = game.projectiles[pi];
        uint32_t colour = projectile.from_alien ? CYAN : GREEN;
        buffer_sprite_draw(&buffer, projectile_sprite, projectile.x, projectile.y, colour);
    }

    if (game.player.life == 0)
    {
        if (game.player_death_count > 0)
        {
            buffer_sprite_draw(&buffer, alien_death_sprite, game.player.x, game.player.y, DARK_RED);
        }
    }
    else
    {
        buffer_sprite_draw(&buffer, player_sprite, game.player.x, game.player.y, GREEN);
    }

    for (size_t i = 0; i < 3; ++i)
    {
        SpriteAnimation &animation = game.alien_animation[i];
        ++animation.time;
        if (animation.time == animation.num_frames * animation.frame_duration)
        {
            animation.time = 0;
        }
    }
}