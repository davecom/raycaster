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
Wall walls[] = {
    {0, 0},
    {80, 0x0000FFFF},
    {80, 0x00AA11FF}
};


void initialize() {
    map = malloc(sizeof(Map));
    map->width = 8;
    map->height = 8;
    map->grid = malloc(sizeof(int) * map->width * map->height);
    memcpy(map->grid,
           (int[]) {1, 0, 0, 0, 0, 2, 1, 1,
                    1, 0, 0, 2, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 1,
                    1, 0, 0, 0, 0, 0, 0, 2,
                    1, 0, 0, 0, 0, 0, 0, 2,
                    1, 0, 0, 0, 0, 0, 0, 2,
                    1, 0, 0, 0, 0, 0, 0, 1,
                    1, 1, 1, 1, 1, 1, 1, 1
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

Wall map_query(int x, int y) {
    int tx = x / TILE_LENGTH;
    int ty = y / TILE_LENGTH;
    if (tx > map->width || ty > map->height || tx < 0 || ty < 0) { return walls[0]; }
    int wall_index = map->grid[ty * 8 + tx];
    return walls[wall_index];
}

uint32_t brighten(uint32_t color, float factor) {
    Uint32 alpha = color & 0x000000FF;
    Uint32 red = (color & 0xFF000000) * factor;
    Uint32 green = (color & 0x00FF0000) * factor;
    Uint32 blue = (color & 0x0000FF00) * factor;
    
    return (alpha | (red & 0xFF000000) | (green & 0x00FF0000) | (blue & 0x0000FF00));
}

// x is x, h is height
void draw_column(int x, Wall wall, float scale) {
    int on_each_side = 0;
    float h = wall.height * scale; // scaled wall height
    if (h < SCREEN_HEIGHT && h >= 0) {
        on_each_side = (SCREEN_HEIGHT - (int)h) / 2;
    }
    //printf("height: %d, on_each_side: %d", h, on_each_side);
    for (int y = 0; y < on_each_side; y++) { // draw sky
        draw_pixel(x, y, 0xFFFFFFFF); // white
    }
    // calculate scaled color
    scale = scale <= 1.0 ? scale : 1.0; // don't lighten things, just darken them
    uint32_t color = brighten(wall.color, scale);
    for (int y = on_each_side; y < (SCREEN_HEIGHT - on_each_side); y++) {
        draw_pixel(x, y, color); // blue
    }
    for (int y = SCREEN_HEIGHT - on_each_side; y < (SCREEN_HEIGHT - 1); y++) { // draw ground
        draw_pixel(x, y, 0x000000FF); // black
    }
}

int raycast(void *data) {
    initialize();
    float f = 500.0; // focal length
    while(1) {
        float direction = get_direction();
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float a = atan2f(SCREEN_WIDTH/2 - x, f); // angle of column
            float ar = direction - a; // angle of ray
            // step along the ray
            Wall wall = {0, 0xFF000000}; // wall height
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
                wall = map_query(sx, sy);
                if (wall.height > 0) { break; } // done on first wall
            }
            
            //printf("wh pre scale: %f", wh);
            // calculate wall height based on distance
            float d = sqrtf(dx * dx + dy * dy); // d is distance
            //float z = dx * cos(a - direction) + dy * sin(a - direction);
            float z = d * cos(a);
            //printf("wh post scale: %f", wh);
            draw_column(x, wall, wall.height / z);
//            if (x == 100) {
//                printf("wall_height: %f final_height: %f a: %f ar: %f d: %d z: %d sx: %f sy: %f", wh, fh, a, ar, d, z, sx, sy);
//            }
        }
    }
}

void turn_player(float amount) {
    mtx_lock(&player_mutex);
    player->direction += amount;
    mtx_unlock(&player_mutex);
}

void move_player(float amount) {
    mtx_lock(&player_mutex);
    int newx = cos(player->direction) * amount + player->x;
    int newy = sin(player->direction) * amount + player->y;
    if (!map_query(newx, newy).height) {
        player->x = newx;
        player->y = newy;
    }
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

Wall get_wall(int index) {
    return walls[index];
}
