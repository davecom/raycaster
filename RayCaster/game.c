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

void initialize() {
    map = malloc(sizeof(Map));
    map->width = 8;
    map->height = 8;
    map->grid = malloc(sizeof(int) * map->width * map->height);
    memcpy(map->grid,
           (int[]) {80, 80, 80, 80, 80, 80, 80, 80,
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
}

int map_query(int x, int y) {
    int tx = x / TILE_LENGTH;
    int ty = y / TILE_LENGTH;
    if (tx > map->width || ty > map->height) { return 0; }
    return map->grid[ty * 8 + tx];
}

// x is x, h is height
void draw_wall(int x, int h) {
    int on_each_side = 0;
    if (h < SCREEN_HEIGHT) {
        on_each_side = (SCREEN_HEIGHT - (int)h) / 2;
    }
    //printf("height: %d, on_each_side: %d", h, on_each_side);
    for (int y = 0; y < on_each_side; y++) { // draw sky
        draw_pixel(x, y, 0xFFFFFFFF);
    }
    for (int y = on_each_side; y < (SCREEN_HEIGHT - on_each_side); y++) {
        draw_pixel(x, y, 0x0000FFFF);
    }
    for (int y = SCREEN_HEIGHT - on_each_side; y < (SCREEN_HEIGHT - 1); y++) { // draw ground
        draw_pixel(x, y, 0x000000FF);
    }
}

int raycast(void *data) {
    initialize();
    float f = 250.0; // focal length
    while(1) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float a = player->direction + atan2(f, x); // angle of ray
            // step along the ray
            float wh = 0.0; // wall height
            float sx = 0.0;
            float sy = 0.0;
            for (int s = 0; s < f; s++) {
                sx = s * cos(a) + player->x;
                sy = s * sin(a) + player->y;
                wh = map_query(sx, sy);
                if (wh > 0) { break; } // done on first wall
            }
            //printf("wh pre scale: %f", wh);
            // calculate wall height based on distance
            float d = sqrtf(sx * sx + sy * sy); // d is distance
            float z = d;
            wh = SCREEN_HEIGHT * wh / z;
            //printf("wh post scale: %f", wh);
            draw_wall(x, wh);
        }
    }
}
