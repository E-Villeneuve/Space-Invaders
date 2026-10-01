#!/bin/bash
g++ -o main main.cpp game.cpp game_render.cpp input.cpp software_renderer.cpp sprites.cpp -lglfw -lGLEW -lGL -lGLU -lm
