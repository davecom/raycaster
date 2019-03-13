//
//  game.h
//  RayCaster
//
//  Created by David Kopec on 3/12/19.
//  Copyright © 2019 David Kopec. All rights reserved.
//
// Originally based on http://www.playfuljs.com/a-first-person-engine-in-265-lines/

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

void initialize(void);
int raycast(void *data);
int map_query(int x, int y);
void turn_player(float amount);
void move_player(float amount);
Map *get_map(void);
Object get_player(void);

#endif /* game_h */
