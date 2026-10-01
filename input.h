#pragma once

#include <GLFW/glfw3.h>

struct InputState
{
    bool running;
    bool fire_pressed;
    int move_dir;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);