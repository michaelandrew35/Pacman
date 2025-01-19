#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW ;
static int gameTitleH ;
static Button mapbtn;
static Button schoolbtn;
static Button opmapbtn;
int mapshover = 0;
int mapsoption = 0;
extern int game_high_Score;
extern FILE* highscore;
FILE* prevhigh;
extern char highest[9];
extern char current[9];
char prevhighest[9];

// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first. 

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below
 static Button btnSettings;

static void init() {

	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	btnSettings = button_create(680, 20, 100, 100, "Assets/sunny_settings.png", "Assets/sunny_settings_hover.png");

	mapbtn = button_create(20, 20, 100, 100, "Assets/map.png", "Assets/map_hover.png");
	opmapbtn = button_create(150, 10, 100, 140, "Assets/exit.png", "Assets/exit_hover.png");
	schoolbtn = button_create(275, 20, 100, 100, "Assets/nthumap.png", "Assets/nthumap_hover.png");

	gameTitle = load_bitmap("Assets/final_title2.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);

	prevhigh = fopen("Assets/highscore.txt", "r");
	fgets(prevhighest, 9, prevhigh);
	return;
}


static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0)); 

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	//draw title image
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255), 
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);

	al_draw_textf(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 110, ALLEGRO_ALIGN_CENTER, "HIGHEST SCORE: %s", prevhighest);
		// [HACKATHON 3-3]
		// TODO: Draw button
		// Uncomment and fill the code below
	drawButton(btnSettings);
	drawButton(mapbtn);
	if (mapshover == 1)
	{
		drawButton(opmapbtn);
		drawButton(schoolbtn);
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
		 btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
		 mapbtn.hovered = buttonHover(mapbtn, mouse_x, mouse_y);
		 opmapbtn.hovered = buttonHover(opmapbtn, mouse_x, mouse_y);
		 schoolbtn.hovered = buttonHover(schoolbtn, mouse_x, mouse_y);
}


//	[HACKATHON 3-8]
//	TODO: When btnSettings clicked, switch to settings scene
//  `game_change_scene` is defined in `game.h`.
//  You can check line 121 `scene_menu.c` to see how to use this function.
// 	Utilize the member defined in struct `Button`.
//  The logic here is 
//  `if clicked the mouse` && `mouse position is in the area of button`
// 	      `Enter the setting scene`
//	Uncomment and fill the code below

static void on_mouse_down() {
	if (btnSettings.hovered)
		game_change_scene(scene_settings_create());
	if (mapbtn.hovered)
	{
		if (mapshover == 0)
		{
			mapshover = 1;
		}
		else mapshover = 0;
	}
	if (opmapbtn.hovered)
	{
		mapsoption = 1;
	}
	if (schoolbtn.hovered)
	{
		mapsoption = 2;
	}
}


static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(gameTitle);
	//	[HACKATHON 3-10]
	//	TODO: Destroy button images
	//	Uncomment and fill the code below
	
	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
	al_destroy_bitmap(mapbtn.default_img);
	al_destroy_bitmap(mapbtn.hovered_img);
	al_destroy_bitmap(opmapbtn.default_img);
	al_destroy_bitmap(opmapbtn.hovered_img);
	al_destroy_bitmap(schoolbtn.default_img);
	al_destroy_bitmap(schoolbtn.hovered_img);
}

static void on_key_down(int keycode) {

	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_main_create());
			break;
		default:
			break;
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Menu";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	
	scene.on_mouse_down = &on_mouse_down;
	
	// -------------------------------------



	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Menu scene created");
	return scene;
}