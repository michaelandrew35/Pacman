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
#include "game.h"
#include "lose.h"
#include "win.h"
#include "pacman_obj.h"

static ALLEGRO_SAMPLE_ID loseBGM;
static Button settingsbtn;
static Button homebtn;
static Button restartbtn;
static ALLEGRO_BITMAP* losepic = NULL;
static int losepicW;
static int losepicH;

FILE* highscore;
char highest[9];
char current[9];

static void init()
{
	settingsbtn = button_create(SCREEN_W - 120, 15, 100, 100, "Assets/sunny_settings.png", "Assets/sunny_settings_hover.png");
	homebtn = button_create(SCREEN_W - 300, SCREEN_H - 250, 100, 140, "Assets/exit.png", "Assets/exit_hover.png");
	restartbtn = button_create(SCREEN_W - 600, SCREEN_H - 225, 100, 100, "Assets/restart.png", "Assets/restart_hover.png");

	stop_bgm(loseBGM);
	loseBGM = play_bgm(loseMusic, music_volume);

	losepic = load_bitmap("Assets/losescene.png");
	losepicW = al_get_bitmap_width(losepic);
	losepicH = al_get_bitmap_height(losepic);

}

static void draw()
{
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 675, ALLEGRO_ALIGN_CENTER, "DEFEAT!");
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 250, SCREEN_H - 275, ALLEGRO_ALIGN_CENTER, "HOME");
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 550, SCREEN_H - 275, ALLEGRO_ALIGN_CENTER, "PLAY AGAIN");

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.75 * scale * losepicW;
	const float offset_h = (SCREEN_H >> 1) - scale * losepicH;

	al_draw_scaled_bitmap(losepic, 0, 0, losepicW, losepicH, offset_w, offset_h, losepicW, losepicH, 0);

	drawButton(settingsbtn);
	drawButton(homebtn);
	drawButton(restartbtn);

	/*char scoree[30];
	sprintf(scoree, 30, "HIGHEST SCORE: %d", highscore);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 200, ALLEGRO_ALIGN_CENTER, scoree);*/

	al_draw_textf(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 110, ALLEGRO_ALIGN_CENTER, "HIGHEST SCORE: %s", highest);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f)
{
	settingsbtn.hovered = buttonHover(settingsbtn, mouse_x, mouse_y);
	homebtn.hovered = buttonHover(homebtn, mouse_x, mouse_y);
	restartbtn.hovered = buttonHover(restartbtn, mouse_x, mouse_y);
}

static void on_mouse_down()
{
	if (homebtn.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		game_change_scene(scene_menu_create());
	}
	if (settingsbtn.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		game_change_scene(scene_settings_create());
	}
	if (restartbtn.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		game_change_scene(scene_main_create());
	}
}

static void destroy()
{
	stop_bgm(loseBGM);
	al_destroy_bitmap(settingsbtn.default_img);
	al_destroy_bitmap(settingsbtn.hovered_img);
	al_destroy_bitmap(homebtn.default_img);
	al_destroy_bitmap(homebtn.hovered_img);
	al_destroy_bitmap(restartbtn.default_img);
	al_destroy_bitmap(restartbtn.hovered_img);
}

static void on_key_down(int keycode)
{
	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		game_change_scene(scene_main_create());
		break;
	default:
		break;
	}
}

Scene scene_lose_create(void) {

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
	game_log("Lose scene created");
	return scene;
}