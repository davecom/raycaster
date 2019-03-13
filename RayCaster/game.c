//
//  game.c
//  RayCaster
//
//  Created by David Kopec on 3/12/19.
//  Copyright © 2019 David Kopec. All rights reserved.
//

#include "game.h"

Object *player;
Map *map;
mtx_t player_mutex;

void initialize() {
    map = malloc(sizeof(Map));
    map->width = 8;
    map->height = 8;
    map->grid = malloc(sizeof(int) * map->width * map->height);
    memcpy(map->grid,
           (int[]) {80, 0, 0, 0, 0, 80, 80, 80,
                    80, 0, 0, 0, 0, 0, 0, 80,
                    80, 0, 0, 0, 0, 0, 0, 80,
                    80, 0, 0, 0, 0, 0, 0, 80,
                    80, 0, 0, 0, 0, 0, 0, 80,
                    80, 0, 0, 0, 0, 0, 0, 80,
                    80, 0, 0, 0, 0, 0, 0, 80,
                    80, 80, 80, 80, 80, 80, 80, 80
           },
           sizeof(int) * map->width * map->height);
    
    player = malloc(sizeof(Object));
    player->x = TILE_LENGTH * map->width / 2;
    player->y = TILE_LENGTH * map->height / 2;
    player->direction = 0;
    player->type = 0;
    mtx_init(&player_mutex, mtx_plain);
}

static float get_direction() {
    mtx_lock(&player_mutex);
    float direction = player->direction;
    mtx_unlock(&player_mutex);
    return direction;
}

int map_query(int x, int y) {
    int tx = x / TILE_LENGTH;
    int ty = y / TILE_LENGTH;
    if (tx > map->width || ty > map->height || tx < 0 || ty < 0) { return 0; }
    return map->grid[ty * 8 + tx];
}

// x is x, h is height
void draw_wall(int x, int h) {
    int on_each_side = 0;
    if (h < SCREEN_HEIGHT && h > 0) {
        on_each_side = (SCREEN_HEIGHT - (int)h) / 2;
    }
    //printf("height: %d, on_each_side: %d", h, on_each_side);
    for (int y = 0; y < on_each_side; y++) { // draw sky
        draw_pixel(x, y, 0xFFFFFFFF); // white
    }
    for (int y = on_each_side; y < (SCREEN_HEIGHT - on_each_side); y++) {
        draw_pixel(x, y, 0x0000FFFF); // blue
    }
    for (int y = SCREEN_HEIGHT - on_each_side; y < (SCREEN_HEIGHT - 1); y++) { // draw ground
        draw_pixel(x, y, 0x000000FF); // black
    }
}

int raycast(void *data) {
    initialize();
    float f = 250.0; // focal length
    while(1) {
        float direction = get_direction();
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float a = atan2f(x, f); // angle of column
            float ar = direction + a; // angle of ray
            // step along the ray
            float wh = 0.0; // wall height
            float sx = 0.0;
            float sy = 0.0;
            float cs = cos(ar);
            float sn = sin(ar);
            float dx = 0.0;
            float dy = 0.0;
            for (int s = 0; s < f; s++) {
                dx = s * cs;
                dy = s * sn;
                sx = dx + player->x;
                sy = dy + player->y;
                wh = map_query(sx, sy);
                if (wh > 0) { break; } // done on first wall
            }
            //printf("wh pre scale: %f", wh);
            // calculate wall height based on distance
            float d = sqrtf(sx * sx + sy * sy); // d is distance
            //float z = dx * cos(a - direction) + dy * sin(a - direction);
            float z = d * cs;
            wh = SCREEN_HEIGHT * wh / z;
            //printf("wh post scale: %f", wh);
            draw_wall(x, wh);
        }
    }
}


void turn_left(float amount) {
    mtx_lock(&player_mutex);
    player->direction -= amount;
    mtx_unlock(&player_mutex);
}

void turn_right(float amount) {
    mtx_lock(&player_mutex);
    player->direction += amount;
    mtx_unlock(&player_mutex);
}

Map *get_map(void) {
    return map;
}

Object get_player(void) {
    Object temp;
    mtx_lock(&player_mutex);
    memcpy(&temp, player, sizeof(Object));
    mtx_unlock(&player_mutex);
    return temp;
}
