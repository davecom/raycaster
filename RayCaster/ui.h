//
//  DDUI.h
//  DDNES
//
//  Created by David Kopec on 7/10/18.
//  Copyright © 2018 David Kopec. All rights reserved.
//

#ifndef DDUI_h
#define DDUI_h



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "c11threads.h"
#include "game.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

#define MAP_WIDTH 200
#define MAP_HEIGHT 200

#define PI 3.14159265358979323846

void event_loop(void);
void frame_ready(void);
void display_main_window(const char *title);
//void draw_nametables_pixel(int x, int y, byte palette_entry);
void draw_pixel(int x, int y, uint32_t palette_entry); //Uint8 r, Uint8 g, Uint8 b);
void ui_cleanup(void);

#endif /* DDUI_h */
