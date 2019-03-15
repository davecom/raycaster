//
//  ui.c
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

#include "ui.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture = NULL;

SDL_Window *map_window;
SDL_Renderer *map_renderer;
SDL_Texture *player_texture = NULL;

mtx_t pixel_list_mutex;
mtx_t frame_ready_mutex;
cnd_t frame_ready_condition;
mtx_t map_mutex;
uint32_t pixels[(SCREEN_HEIGHT * SCREEN_WIDTH)]; // pixel buffer

bool map_open = false;


void frame_ready() {
    cnd_signal(&frame_ready_condition);
}

void start_stop_map_window() {
    if (map_open) { // start
        mtx_init(&map_mutex, mtx_plain);

        map_window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAP_WIDTH, MAP_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
        if (map_window == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
            return;
        }
        map_renderer = SDL_CreateRenderer(map_window, -1, SDL_RENDERER_ACCELERATED);
        if (map_renderer == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
            return;
        }
        SDL_Texture* newTexture = NULL;
        
        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load( "arrow.png" );
        if( loadedSurface == NULL )
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", "arrow.png", IMG_GetError() );
        }
        else
        {
            //Color key image
            SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
            
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface( map_renderer, loadedSurface );
            if( newTexture == NULL )
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", "arrow.png", SDL_GetError() );
            }
            
            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }
        
        player_texture = newTexture;
        SDL_SetRenderDrawColor(map_renderer, 0x00, 255, 0x00, 0x00);
        SDL_RenderClear(map_renderer);
    } else { // stop
        SDL_DestroyRenderer(map_renderer);
        SDL_DestroyWindow(map_window);
        SDL_DestroyTexture(player_texture);
        //free(map_pixels);
    }
}

#define FPS_INTERVAL 1.0 //seconds.


void event_loop() {
    Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.
    Uint32 fps_current; //the current FPS.
    Uint32 fps_frames = 0; //frames passed since the last recorded fps.
    
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_x:
                            break;
                        case SDLK_z:
                            break;
                        case SDLK_s:
                            break;
                        case SDLK_a:
                            break;
                        case SDLK_UP:
                            move_player(2.0);
                            break;
                        case SDLK_DOWN:
                            move_player(-2.0);
                            break;
                        case SDLK_LEFT:
                            turn_player(-0.05);
                            break;
                        case SDLK_RIGHT:
                            turn_player(0.05);
                            break;
                        case SDLK_d:
                            break;
                        case SDLK_n:
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym) {
                        case SDLK_x:
                            break;
                        case SDLK_z:
                            break;
                        case SDLK_s:
                            break;
                        case SDLK_a:
                            break;
                        case SDLK_UP:
                            break;
                        case SDLK_DOWN:
                            break;
                        case SDLK_LEFT:
                            break;
                        case SDLK_RIGHT:
                            break;
                        case SDLK_m:
                            map_open = !map_open;
                            start_stop_map_window();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // clear renderer
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
        SDL_RenderClear(renderer);
        mtx_lock(&pixel_list_mutex);
        //SDL_SetRenderTarget(renderer, texture);
        SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * 4);
        mtx_unlock(&pixel_list_mutex);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL); // blit texture
        SDL_RenderPresent(renderer);
        
        if (map_open) {
            SDL_SetRenderDrawColor(map_renderer, 255, 255, 255, 255); // white
            SDL_RenderClear(map_renderer);
            mtx_lock(&map_mutex);
            mtx_unlock(&map_mutex);
            SDL_SetRenderTarget(map_renderer, NULL);
            
            // draw the map
            Map *map = get_map();
            int sl = MAP_WIDTH / map->width;
            float playfield_length = map->width * TILE_LENGTH;
            for (int x = 0; x < map->width; x++) {
                for (int y = 0; y < map->height; y++) {
                    SDL_Rect rect;
                    rect.h = sl;
                    rect.w = sl;
                    rect.x = x * sl;
                    rect.y = y * sl;
                    Wall wall = get_wall(map->grid[x + map->width * y]);
                    if (wall.height > 0) {
                        SDL_SetRenderDrawColor(map_renderer, (wall.color >> 24 & 0xFF), (wall.color >> 16 & 0xFF), (wall.color >> 8 & 0xFF), (wall.color & 0xFF)); // fill with wall color
                        SDL_RenderFillRect(map_renderer, &rect);
                    }
                    SDL_SetRenderDrawColor(map_renderer, 0, 0, 0, 255); // black
                    SDL_RenderDrawRect(map_renderer, &rect);
                }
            }
            Object player = get_player();
            SDL_Rect player_rect;
            player_rect.h = 10;
            player_rect.w = 20;
            int playerx = player.x * (MAP_WIDTH / playfield_length);
            int playery = player.y * (MAP_HEIGHT / playfield_length);
            player_rect.x = playerx - 10;
            player_rect.y = playery - 5;
            SDL_RenderCopyEx(map_renderer, player_texture, NULL, &player_rect, player.direction * 180 / PI, NULL, SDL_FLIP_NONE);
            
            // draw sweep lines for 0 and 255
            SDL_SetRenderDrawColor(map_renderer, 0, 255, 0, 255); // black
            float f = 500.0;
            float a = atan2f(SCREEN_WIDTH/2 - 0, f); // angle of column
            float ar = player.direction - a; // angle of ray
            // step along the ray
            int sx = f * cos(ar) + player.x;
            int sy = f * sin(ar) + player.y;
            SDL_RenderDrawLine(map_renderer, playerx, playery, sx, sy);
            a = atan2f(SCREEN_WIDTH/2 - SCREEN_WIDTH, f); // angle of column
            ar = player.direction - a; // angle of ray
            // step along the ray
            sx = f * cos(ar) + player.x;
            sy = f * sin(ar) + player.y;
            SDL_RenderDrawLine(map_renderer, playerx, playery, sx, sy);
            SDL_RenderPresent(map_renderer);
        }
        
        //SDL_Delay(16);
        //printf("end drawing loop");
        
        // FPS Counting
        fps_frames++;
        if (fps_lasttime < SDL_GetTicks() - FPS_INTERVAL*1000)
        {
            fps_lasttime = SDL_GetTicks();
            fps_current = fps_frames;
            fps_frames = 0;
            printf("FPS: %d\n", fps_current);
        }
    }
    ui_cleanup();
}

void display_main_window(const char *title) {
    mtx_init(&pixel_list_mutex, mtx_plain);
    mtx_init(&frame_ready_mutex, mtx_plain);
    cnd_init(&frame_ready_condition);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
        return;
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 255, 0x00, 0x00);
    SDL_RenderClear(renderer);
}

void draw_pixel(int x, int y, uint32_t color) {
    //printf("%d\n", palette_entry);
    mtx_lock(&pixel_list_mutex);
    pixels[(x + y * SCREEN_WIDTH)] = color;
    mtx_unlock(&pixel_list_mutex);
}

void ui_cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}
