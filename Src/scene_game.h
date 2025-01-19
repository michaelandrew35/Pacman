// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"
#include "utility.h"
#include "allegro5/allegro.h"
//ALLEGRO_TIMER* speed_up_timer;
ALLEGRO_TIMER* power_up_timer;
ALLEGRO_TIMER* speed_up_timer;
ALLEGRO_TIMER* punchanim;
Scene scene_main_create(void);
Scene scene_lose_create(void);
Scene scene_win_create(void);

#endif
