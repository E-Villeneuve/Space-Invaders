#include "input.h"

void key_callback(GLFWwindow *window, int key, int, int action, int)
{
    InputState *input = static_cast<InputState *>(glfwGetWindowUserPointer(window));
    if (!input)
    {
        return;
    }

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
        {
            input->running = false;
            break;
        }
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS)
        {
            input->move_dir += 1;
        }
        else if (action == GLFW_RELEASE)
        {
            input->move_dir -= 1;
        }
        break;
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS)
        {
            input->move_dir -= 1;
        }
        else if (action == GLFW_RELEASE)
        {
            input->move_dir += 1;
        }
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_RELEASE)
        {
            input->fire_pressed = true;
        }
        break;
    default:
        break;
    }
}