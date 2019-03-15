//
//  ui.h
//  RayCaster
//
//  Copyright (c) 2019 David Kopec
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef DDUI_h
#define DDUI_h



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "c11threads.h"
#include "game.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 400

#define MAP_WIDTH 200
#define MAP_HEIGHT 200

#define PI 3.14159265358979323846

void event_loop(void);
void frame_ready(void);
void display_main_window(const char *title);
void draw_pixel(int x, int y, uint32_t palette_entry);
void ui_cleanup(void);

#endif
