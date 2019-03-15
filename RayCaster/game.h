//
//  game.h
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

#ifndef game_h
#define game_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ui.h"

#define TILE_LENGTH 32

typedef struct {
    float x;
    float y;
    float direction;
    int type;
} Object;

typedef struct {
    int width;
    int height;
    int *grid;
} Map;

typedef struct {
    int height;
    uint32_t color;
} Wall;

void initialize(void);
int raycast(void *data);
Wall map_query(int x, int y);
void turn_player(float amount);
void move_player(float amount);
Map *get_map(void);
Object get_player(void);
Wall get_wall(int index);

#endif /* game_h */
