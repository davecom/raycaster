//
//  main.c
//  RayCaster
//
//  Created by David Kopec on 3/12/19.
//  Copyright © 2019 David Kopec. All rights reserved.
//

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
