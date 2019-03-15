//
//  main.c
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

#include <stdio.h>
#include "ui.h"
#include "game.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    display_main_window(argv[1]);
    // start separate thread for raycasting
    thrd_t emuthr;
    thrd_create(&emuthr, raycast, NULL);
    // start ui event loop
    event_loop();
    
    return 0;
}
