// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "shared.h"
#include "game.h"
#include "utility.h"
#include "pacman_obj.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
static Button homebtn;
static Button sfxupbtn;
static Button sfxdownbtn;
static Button musicupbtn;
static Button musicdownbtn;
static Button masterupbtn;
static Button masterdownbtn;
static Button nextbtn;
static Button prevbtn;
//static Button exitbtn;
static ALLEGRO_SAMPLE_ID settingBGM;
static ALLEGRO_BITMAP* setpic = NULL;
static int setpicW;
static int setpicH;
static int anim = 0;
static int aniX = 400;
static int aniY = 500;
static int aniSize = 60;
extern int skin;
static ALLEGRO_BITMAP* skin1;
static ALLEGRO_BITMAP* skin2;
static ALLEGRO_BITMAP* skin3;
ALLEGRO_TIMER* setting_timer;

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init()
{
	homebtn = button_create(SCREEN_W - 120, 15, 100, 100, "Assets/sunny_settings.png", "Assets/sunny_settings_hover.png");
	sfxupbtn = button_create(SCREEN_W - 62, SCREEN_H - 415, 50, 50, "Assets/vol_up.png", "Assets/vol_up_hover.png");
	sfxdownbtn = button_create(SCREEN_W - 200, SCREEN_H - 415, 50, 50, "Assets/vol_down.png", "Assets/vol_down_hover.png");
	musicupbtn = button_create(SCREEN_W - 650, SCREEN_H - 415, 50, 50, "Assets/vol_up.png", "Assets/vol_up_hover.png");
	musicdownbtn = button_create(SCREEN_W - 800, SCREEN_H - 415, 50, 50, "Assets/vol_down.png", "ASsets/vol_down_hover.png");
	masterupbtn = button_create(SCREEN_W / 1.65, SCREEN_H - 635, 50, 50, "Assets/vol_up.png", "Assets/vol_up_hover.png");
	masterdownbtn = button_create(SCREEN_W / 3, SCREEN_H - 635, 50, 50, "Assets/vol_down.png", "Assets/vol_down_hover.png");
	nextbtn = button_create(SCREEN_W / 1.65, SCREEN_H - 150, 100, 100, "Assets/arrow_next.png", "Assets/arrow_next_hover.png");
	prevbtn = button_create(SCREEN_W / 4, SCREEN_H - 150, 100, 100, "Assets/arrow_prev.png", "Assets/arrow_prev_hover.png");
	//exitbtn = button_create(SCREEN_W - 750, SCREEN_H - 175, 100, 140, "Assets/exit.png", "Assets/exit_hover.png");

	skin1 = load_bitmap("Assets/luffy_dacat.png");
	skin2 = load_bitmap("Assets/zoro_sprite.png");
	skin3 = load_bitmap("Assets/sanji_sprite.png");
	
	stop_bgm(settingBGM);
	settingBGM = play_bgm(settingsMusic, music_volume);

	setpic = load_bitmap("Assets/compass_settings.png");
	setpicW = al_get_bitmap_width(setpic);
	setpicH = al_get_bitmap_height(setpic);

	setting_timer = al_create_timer(1.0f);
	if (!setting_timer)
		game_abort("Error creating setting timer\n");
	al_start_timer(setting_timer);
}

static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 725, ALLEGRO_ALIGN_CENTER, "SETTINGS");
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 700, SCREEN_H - 460, ALLEGRO_ALIGN_CENTER, "MUSIC");

	char str1[10];
	sprintf(str1, "%d", (int)(20 * music_volume));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 700, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, str1);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 100, SCREEN_H - 460, ALLEGRO_ALIGN_CENTER, "SFX");

	char str2[10];
	sprintf(str2, "%d", (int)(20 * effect_volume));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 100, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, str2);

	char str3[10];
	sprintf(str3, "%d", (int)(20 * master_volume));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 625, ALLEGRO_ALIGN_CENTER, str3);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 675, ALLEGRO_ALIGN_CENTER, "MASTER VOL.");

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * setpicW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * setpicH;

	al_draw_scaled_bitmap(setpic, 0, 0, setpicW, setpicH, offset_w, offset_h, setpicW * scale, setpicH * scale, 0);

	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 200, ALLEGRO_ALIGN_CENTER, "CHARACTER");
	
	drawButton(homebtn);
	drawButton(sfxupbtn);
	drawButton(sfxdownbtn);
	drawButton(musicupbtn);
	drawButton(musicdownbtn);
	drawButton(masterupbtn);
	drawButton(masterdownbtn);
	drawButton(nextbtn);
	drawButton(prevbtn);
	//drawButton(exitbtn);

	//al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 700, SCREEN_H - 200, ALLEGRO_ALIGN_CENTER, "EXIT");
	anim = (al_get_timer_count(setting_timer) % 2) ? 0 : 16;
	switch (skin)
	{
	case 1:
		al_draw_scaled_bitmap(skin1, 0 + anim, 0, 16, 16, aniX - 30, aniY + 160, aniSize, aniSize, 0);
		al_draw_text(menuFont, al_map_rgb(255, 255, 255), aniX, SCREEN_H-50, ALLEGRO_ALIGN_CENTER, "LUFFY");
		break;
	case 2:
		al_draw_scaled_bitmap(skin2, 0 + anim, 0, 16, 16, aniX - 30, aniY + 160, aniSize, aniSize, 0);
		al_draw_text(menuFont, al_map_rgb(255, 255, 255), aniX, SCREEN_H-50, ALLEGRO_ALIGN_CENTER, "ZORO");
		break;
	case 3:
		al_draw_scaled_bitmap(skin3, 0 + anim, 0, 16, 16, aniX - 30, aniY + 160, aniSize, aniSize, 0);
		al_draw_text(menuFont, al_map_rgb(255, 255, 255), aniX, SCREEN_H-50, ALLEGRO_ALIGN_CENTER, "SANJI");
		break;
	default:
		al_draw_scaled_bitmap(skin1, 0 + anim, 0, 16, 16, aniX - 30, aniY + 160, aniSize, aniSize, 0);
		al_draw_text(menuFont, al_map_rgb(255, 255, 255), aniX, SCREEN_H-50, ALLEGRO_ALIGN_CENTER, "LUFFY");
		break;

	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f)
{
	homebtn.hovered = buttonHover(homebtn, mouse_x, mouse_y);
	sfxupbtn.hovered = buttonHover(sfxupbtn, mouse_x, mouse_y);
	sfxdownbtn.hovered = buttonHover(sfxdownbtn, mouse_x, mouse_y);
	musicupbtn.hovered = buttonHover(musicupbtn, mouse_x, mouse_y);
	musicdownbtn.hovered = buttonHover(musicdownbtn, mouse_x, mouse_y);
	masterupbtn.hovered = buttonHover(masterupbtn, mouse_x, mouse_y);
	masterdownbtn.hovered = buttonHover(masterdownbtn, mouse_x, mouse_y);
	prevbtn.hovered = buttonHover(prevbtn, mouse_x, mouse_y);
	nextbtn.hovered = buttonHover(nextbtn, mouse_x, mouse_y);
	//exitbtn.hovered = buttonHover(exitbtn, mouse_x, mouse_y);
}

static void on_mouse_down()
{
	if (homebtn.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		game_change_scene(scene_menu_create());
	}
	if (sfxupbtn.hovered && effect_volume < 5)
	{
		effect_volume += (float)0.5;
		stop_bgm(settingBGM);
		settingBGM = play_bgm(settingsMusic, music_volume);
	}
	if (sfxdownbtn.hovered && effect_volume > 0)
	{
		effect_volume -= (float)0.5;
		stop_bgm(settingBGM);
		settingBGM = play_bgm(settingsMusic, music_volume);

	}
	if (musicupbtn.hovered && music_volume < 5)
	{
		music_volume += (float)0.5;
		stop_bgm(settingBGM);
		settingBGM = play_bgm(settingsMusic, music_volume);

	}
	if (musicdownbtn.hovered && music_volume > 0)
	{
		music_volume -= (float)0.5;
		stop_bgm(settingBGM);
		settingBGM = play_bgm(settingsMusic, music_volume);

	}
	if (masterupbtn.hovered && effect_volume < 5 && music_volume < 5)
	{
		effect_volume += (float)0.5;
		music_volume += (float)0.5;
		master_volume += (float)0.5;
		stop_bgm(settingBGM);
		settingBGM = play_bgm(settingsMusic, music_volume);

	}
	if (masterdownbtn.hovered && effect_volume > 0 && music_volume > 0)
	{

		effect_volume -= (float)0.5;
		music_volume -= (float)0.5;
		master_volume -= (float)0.5;
		stop_bgm(settingBGM);
		settingBGM = play_bgm(settingsMusic, music_volume);
	}
	if (prevbtn.hovered)
	{
		if (skin == 1) skin = 3;
		else skin--;
	}
	if (nextbtn.hovered)
	{
		if (skin == 3) skin = 1;
		else skin++;
	}
	/*if (exitbtn.hovered);
	{
		exit(1);
	}*/
}

static void destroy()
{
	stop_bgm(settingBGM);
	al_destroy_bitmap(homebtn.default_img);
	al_destroy_bitmap(homebtn.hovered_img);
	al_destroy_bitmap(sfxupbtn.default_img);
	al_destroy_bitmap(sfxupbtn.hovered_img);
	al_destroy_bitmap(sfxdownbtn.default_img);
	al_destroy_bitmap(sfxdownbtn.hovered_img);
	al_destroy_bitmap(musicupbtn.default_img);
	al_destroy_bitmap(musicupbtn.hovered_img);
	al_destroy_bitmap(musicdownbtn.default_img);
	al_destroy_bitmap(musicdownbtn.hovered_img);
	al_destroy_bitmap(masterupbtn.default_img);
	al_destroy_bitmap(masterupbtn.hovered_img);
	al_destroy_bitmap(masterdownbtn.default_img);
	al_destroy_bitmap(masterdownbtn.hovered_img);
	al_destroy_bitmap(prevbtn.default_img);
	al_destroy_bitmap(prevbtn.hovered_img);
	al_destroy_bitmap(nextbtn.default_img);
	al_destroy_bitmap(nextbtn.hovered_img);
	al_destroy_bitmap(skin1);
	al_destroy_bitmap(skin2);
	al_destroy_bitmap(skin3);
	al_destroy_timer(setting_timer);
	//al_destroy_bitmap(exitbtn.default_img);
	//al_destroy_bitmap(exitbtn.hovered_img);
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

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}