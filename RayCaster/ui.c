//
//  DDUI.c
//  DDNES
//
//  Created by David Kopec on 7/10/18.
//  Copyright © 2018 David Kopec. All rights reserved.
//

#include "ui.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

SDL_Window *nametable_window;
SDL_Renderer *nametable_renderer;
SDL_Texture *nametable_texture;

//typedef struct pixel_list {
//    int x;
//    int y;
//    Uint8 r;
//    Uint8 g;
//    Uint8 b;
//    Uint8 a;
//    struct pixel_list *next;
//} pixel_list;
//
//pixel_list * head_pixel = NULL;
mtx_t pixel_list_mutex;
mtx_t frame_ready_mutex;
cnd_t frame_ready_condition;
mtx_t nametable_debug_mutex;
uint32_t *nametable_debug_pixels = NULL;
uint32_t pixels[(SCREEN_HEIGHT * SCREEN_WIDTH)]; // pixel buffer


void frame_ready() {
    cnd_signal(&frame_ready_condition);
}

//void start_stop_nametable_debug() {
//    if (nametable_debug) { // start
//        nametable_debug_pixels = malloc(sizeof(uint32_t) * NES_WIDTH * 2 * NES_HEIGHT * 2);
//        mtx_init(&nametable_debug_mutex, mtx_plain);
//
//        nametable_window = SDL_CreateWindow("Nametables", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NES_WIDTH * 2, NES_HEIGHT * 2, SDL_WINDOW_ALLOW_HIGHDPI);
//        if (nametable_window == NULL) {
//            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
//            return;
//        }
//        nametable_renderer = SDL_CreateRenderer(nametable_window, -1, SDL_RENDERER_ACCELERATED);
//        if (nametable_renderer == NULL) {
//            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
//            return;
//        }
//        nametable_texture = SDL_CreateTexture(nametable_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, NES_WIDTH * 2, NES_HEIGHT * 2);
//        SDL_SetRenderTarget(nametable_renderer, nametable_texture);
//        SDL_SetRenderDrawColor(nametable_renderer, 0x00, 255, 0x00, 0x00);
//        SDL_RenderClear(nametable_renderer);
//    } else { // stop
//        SDL_DestroyRenderer(nametable_renderer);
//        SDL_DestroyWindow(nametable_window);
//        SDL_DestroyTexture(nametable_texture);
//        free(nametable_debug_pixels);
//    }
//}

void event_loop() {
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
                            //joypad1.a = true;
                            break;
                        case SDLK_z:
                            //joypad1.b = true;
                            break;
                        case SDLK_s:
                            //printf("start pressed");
                            //joypad1.start = true;
                            //printf("%x", joypad1.start);
                            break;
                        case SDLK_a:
                            //joypad1.select = true;
                            break;
                        case SDLK_UP:
                            //joypad1.up = true;
                            break;
                        case SDLK_DOWN:
                            //joypad1.down = true;
                            break;
                        case SDLK_LEFT:
                            //joypad1.left = true;
                            break;
                        case SDLK_RIGHT:
                            //joypad1.right = true;
                            break;
                        case SDLK_d:
                            //debug = !debug;
                            break;
                        case SDLK_n:
                            //nametable_debug = !nametable_debug;
                            //start_stop_nametable_debug();
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym) {
                        case SDLK_x:
                            //joypad1.a = false;
                            break;
                        case SDLK_z:
                           // joypad1.b = false;
                            break;
                        case SDLK_s:
                            //joypad1.start = false;
                            break;
                        case SDLK_a:
                            //joypad1.select = false;
                            break;
                        case SDLK_UP:
                            //joypad1.up = false;
                            break;
                        case SDLK_DOWN:
                            //joypad1.down = false;
                            break;
                        case SDLK_LEFT:
                            //joypad1.left = false;
                            break;
                        case SDLK_RIGHT:
                            //joypad1.right = false;
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
        // is there a pixel to draw?
//        pixel_list *pixel = pop_pixel();
//        while (pixel != NULL) {
//            SDL_SetRenderTarget(renderer, texture);
//            SDL_SetRenderDrawColor(renderer, pixel->r, pixel->g, pixel->b, pixel->a);
//            SDL_RenderDrawPoint(renderer, pixel->x, pixel->y);
//            free(pixel); // responsible for freeing
//            pixel = pop_pixel();
//        }
        //cnd_wait(&frame_ready_condition, &frame_ready_mutex);
        mtx_lock(&pixel_list_mutex);
        //SDL_SetRenderTarget(renderer, texture);
        SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * 4);
        mtx_unlock(&pixel_list_mutex);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL); // blit texture
        SDL_RenderPresent(renderer);
        
//        if (nametable_debug) {
//            SDL_SetRenderDrawColor(nametable_renderer, 0, 0, 0, 255); // black
//            SDL_RenderClear(nametable_renderer);
//            mtx_lock(&nametable_debug_mutex);
//            //SDL_SetRenderTarget(renderer, texture);
//            SDL_UpdateTexture(nametable_texture, NULL, nametable_debug_pixels, NES_WIDTH * 4 * 2);
//            mtx_unlock(&nametable_debug_mutex);
//            SDL_SetRenderTarget(nametable_renderer, NULL);
//            SDL_RenderCopy(nametable_renderer, nametable_texture, NULL, NULL); // blit texture
//            SDL_RenderPresent(nametable_renderer);
//        }
        
        //SDL_Delay(16);
        //printf("end drawing loop");
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

//void draw_nametables_pixel(int x, int y, byte palette_entry) {
//    if (nametable_debug) {
//        mtx_lock(&nametable_debug_mutex);
//        nametable_debug_pixels[(x + y * NES_WIDTH * 2)] = nes_palette[palette_entry];
//        mtx_unlock(&nametable_debug_mutex);
//    }
//}

void draw_pixel(int x, int y, uint32_t color) {
    //printf("%d\n", palette_entry);
    mtx_lock(&pixel_list_mutex);
    pixels[(x + y * SCREEN_WIDTH)] = color;
    mtx_unlock(&pixel_list_mutex);
}

//void draw_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
////    pixel_list *pixel = malloc(sizeof(pixel_list));
////    pixel->x = x;
////    pixel->y = y;
////    pixel->r = r;
////    pixel->g = g;
////    pixel->b = b;
////    pixel->a = 255;
////    pixel->next = NULL;
//    mtx_lock(&pixel_list_mutex);
//    uint32_t temp = 0;
//    temp |= (r << 24);
//    temp |= (g << 16);
//    temp |= (b << 8);
//    temp |= 255;
//    pixels[(x + y * NES_WIDTH)] = temp;
//    mtx_unlock(&pixel_list_mutex);
////    append_pixel(pixel);
//}

void ui_cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}
