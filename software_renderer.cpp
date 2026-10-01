#include "software_renderer.h"

const uint32_t GREEN = rgb_to_uint32(57, 255, 20);
const uint32_t DARK_RED = rgb_to_uint32(128, 0, 0);
const uint32_t CYAN = rgb_to_uint32(0, 200, 255);
const uint32_t BLACK = rgb_to_uint32(0, 0, 0);
const uint32_t WHITE = rgb_to_uint32(255, 255, 255);

void buffer_sprite_draw(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t colour)
{
    for (size_t xi = 0; xi < sprite.width; ++xi)
    {
        for (size_t yi = 0; yi < sprite.height; ++yi)
        {
            size_t sy = sprite.height - 1 + y - yi;
            size_t sx = x + xi;

            if (sprite.data[yi * sprite.width + xi] && sy < buffer->height && sx < buffer->width)
            {
                buffer->data[sy * buffer->width + sx] = colour;
            }
        }
    }
}

void buffer_text_draw(Buffer *buffer, const Sprite &text_spritesheet, const char *text, size_t x, size_t y, uint32_t colour)
{
    size_t xp = x;
    size_t stride = text_spritesheet.width * text_spritesheet.height;
    Sprite sprite = text_spritesheet;
    for (const char *charp = text; *charp != '\0'; ++charp)
    {
        char character = *charp - 32;
        if (character < 0 || character >= 65)
            continue;

        sprite.data = text_spritesheet.data + character * stride;
        buffer_sprite_draw(buffer, sprite, xp, y, colour);
        xp += sprite.width + 1;
    }
}

void buffer_number_draw(Buffer *buffer, const Sprite &number_spritesheet, size_t number, size_t x, size_t y, uint32_t colour)
{
    uint8_t digits[64];
    size_t num_digits = 0;

    size_t current_number = number;
    do
    {
        digits[num_digits++] = current_number % 10;
        current_number /= 10;
    } while (current_number > 0);

    size_t xp = x;
    size_t stride = number_spritesheet.width * number_spritesheet.height;
    Sprite sprite = number_spritesheet;
    for (size_t i = 0; i < num_digits; ++i)
    {
        uint8_t digit = digits[num_digits - i - 1];
        sprite.data = number_spritesheet.data + digit * stride;
        buffer_sprite_draw(buffer, sprite, xp, y, colour);
        xp += sprite.width + 1;
    }
}

uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

void buffer_clear(Buffer *buffer, uint32_t colour)
{
    for (size_t i = 0; i < buffer->width * buffer->height; i++)
    {
        buffer->data[i] = colour;
    }
}