#define _CRT_SECURE_NO_WARNINGS
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "scene_settings.h"
#include "shared.h"
#include "win.h"
#include "lose.h"


// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
int game_high_Score = 0;
extern int mapsoption;
extern int mapshover;
bool game_over = false;
bool game_win = false;


/* Internal variables*/
extern ALLEGRO_TIMER* power_up_timer;
extern ALLEGRO_TIMER* speed_up_timer;
extern ALLEGRO_TIMER* punchanim;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
static ALLEGRO_SAMPLE_ID PACMAN_POWERBEAN2_SOUND;
static ALLEGRO_SAMPLE_ID PACMAN_PUNCH_SOUND;
static ALLEGRO_SAMPLE_ID gameBGM;
extern FILE* highscore = NULL;
extern char highest[9];
extern char current[9];
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);

static ALLEGRO_BITMAP* op1 = NULL;
static ALLEGRO_BITMAP* op2 = NULL;
static ALLEGRO_BITMAP* op3 = NULL;
static ALLEGRO_BITMAP* op4 = NULL;
static ALLEGRO_BITMAP* op5 = NULL;
static ALLEGRO_BITMAP* op6 = NULL;
static ALLEGRO_BITMAP* op7 = NULL;
static ALLEGRO_BITMAP* op8 = NULL;
static ALLEGRO_BITMAP* op9 = NULL;
static ALLEGRO_BITMAP* op10 = NULL;
static ALLEGRO_BITMAP* op11 = NULL;
static ALLEGRO_BITMAP* op12  = NULL;

static void init(void) {
	game_over = false; 
	game_win = false;
	game_main_Score = 0;
	// create map
	//basic_map = create_map(NULL);
	// [TODO]
	// Create map from .txt file and design your own map !!
	basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}

	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	
	ghosts = (Ghost*)malloc(sizeof(Ghost) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {

			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");

		}
	}
	GAME_TICK = 0;
	stop_bgm(gameBGM);
	gameBGM = play_bgm(gameMusic, music_volume);

	op1 = load_bitmap("Assets/op1.png");
	op2 = load_bitmap("Assets/op2.png");
	op3 = load_bitmap("Assets/op3.png");
	op4 = load_bitmap("Assets/op4.png");
	op5 = load_bitmap("Assets/op5.png");
	op6 = load_bitmap("Assets/op6.png");
	op7 = load_bitmap("Assets/op7.png");
	op8 = load_bitmap("Assets/op8.png");
	op9 = load_bitmap("Assets/op9.png");
	op10 = load_bitmap("Assets/op10.png");
	op11 = load_bitmap("Assets/op11.png");
	op12 = load_bitmap("Assets/op12.png");

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	speed_up_timer = al_create_timer(1.0f);
	if (!speed_up_timer)
		game_abort("Error on creating Speed up timer\n");
	punchanim = al_create_timer(1.0f);

	highscore = fopen("Assets/highscore.txt", "r");
	fgets(highest, 9, highscore);
	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->beansNum--;
		//game_log("%d", basic_map->beansNum);
		game_main_Score += 10;
		basic_map->map[Grid_y][Grid_x] = ' ';
		if (basic_map->beansNum == 0)
		{
			game_log("Win");
			game_win = true;
			break;
		}
		break;
	case 'P':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]); 
		game_main_Score += 100;
		basic_map->map[Grid_y][Grid_x] = ' ';
		if (al_get_timer_started(power_up_timer))
		{
			al_set_timer_count(power_up_timer, 0);
			stop_bgm(PACMAN_POWERBEAN2_SOUND);
		}
		else
		{
			stop_bgm(gameBGM);
			al_start_timer(power_up_timer);
		}
		PACMAN_POWERBEAN2_SOUND = play_audio(PACMAN_POWERBEAN_SOUND, effect_volume);
		for (int i = 0; i < GHOST_NUM; i++)
		{
			ghost_toggle_FLEE(ghosts[i], true);
		}
		break;
	case 'S':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		game_main_Score += 100;
		basic_map->map[Grid_y][Grid_x] = " ";
		pman->speed = 4;
		if (al_get_timer_started(speed_up_timer)) {
			al_set_timer_count(speed_up_timer, 0);
		}
		else { al_start_timer(speed_up_timer); }
		break;
	case'K':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		game_main_Score += 100;
		basic_map->map[Grid_y][Grid_x] = " ";
		for (int i = 0; i < GHOST_NUM; i++) {
			ghosts[i]->status = GO_IN;
			ghosts[i]->speed = 10;
		}
		break;
	default:
		break;
	}
	
	// [HACKATHON 1-4]
	// erase the item you eat from map
	// be careful no erasing the wall block.
	/*
		basic_map->map...;
	*/
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		
		if(!cheat_mode && RecAreaOverlap(getDrawArea(pman->objData,GAME_TICK_CD),getDrawArea(ghosts[i]->objData,GAME_TICK_CD)))
		{
			if (ghosts[i]->status == FLEE)
			{
				al_start_timer(punchanim);
				stop_bgm(PACMAN_PUNCH_SOUND);
				PACMAN_PUNCH_SOUND = play_bgm(PACMAN_KILL_SOUND, effect_volume);
				ghost_collided(ghosts[i]); 
				game_main_Score += 100;
			}
			else
			{
				game_log("collide with ghost\n");
				al_rest(1.0);
				pacman_die();
				game_over = true;
				//al_start_timer(pman->death_anim_counter);
				break;
			}
		}
		/*if (basic_map->beansNum == 0)
		{
			game_log("Win");
			game_win = true;
			break;
		}*/
	}
}

static void update(void) {

	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		stop_bgm(gameBGM); 
		stop_bgm(PACMAN_POWERBEAN2_SOUND);
		al_start_timer(pman->death_anim_counter);
		if (atoi(highest) <= atoi(current))
		{
			highscore = fopen("Assets/highscore.txt", "w");
			fputs(current, highscore);
			fclose(highscore);
		}
		if (al_get_timer_count(pman->death_anim_counter) > 50)
		{
			game_change_scene(scene_lose_create());
		}
		return;
	}
	else if (game_win)
	{
		stop_bgm(gameBGM); 
		game_change_scene(scene_win_create());
		if (atoi(highest) <= atoi(current))
		{
			highscore = fopen("Assets/highscore.txt", "w");
			fputs(current, highscore);
			fclose(highscore);
		}
		game_win = false;
		return;
	}
	if (al_get_timer_count(power_up_timer) > 10) {
		al_stop_timer(power_up_timer);
		stop_bgm(PACMAN_POWERBEAN2_SOUND);
		gameBGM = play_audio(gameMusic, music_volume);
		al_set_timer_count(power_up_timer, 0);
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], false);
		}
	}
	if (al_get_timer_count(speed_up_timer) > 10) {
		al_stop_timer(speed_up_timer);
		al_set_timer_count(speed_up_timer, 0);
		pman->speed = 2;
	}
	if (al_get_timer_count(punchanim) > 0.0001) {
		al_stop_timer(punchanim);
		al_set_timer_count(punchanim, 0);
		stop_bgm(PACMAN_PUNCH_SOUND);
	}
	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0)); 

	
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	/*
		al_draw_text(...);
	*/

	char scoreboard[20];
	sprintf_s(scoreboard, 20, "SCORE: %d", game_main_Score);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 25, 10, ALLEGRO_ALIGN_RIGHT, scoreboard);

	sprintf_s(current, 9,"%d",game_main_Score);

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

	if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 12)
	{
		al_draw_bitmap(op1, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 6)
	{
		al_draw_bitmap(op2, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 4)
	{
		al_draw_bitmap(op3, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 3)
	{
		al_draw_bitmap(op4, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 2.4)
	{
		al_draw_bitmap(op5, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 2)
	{
		al_draw_bitmap(op6, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 1.7)
	{
		al_draw_bitmap(op7, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 1.5)
	{
		al_draw_bitmap(op8, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 1.33)
	{
		al_draw_bitmap(op9, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 1.2)
	{
		al_draw_bitmap(op10, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount / 1.1)
	{
		al_draw_bitmap(op11, 100, 690, 0);
	}
	else if (basic_map->beansNum > basic_map->beansCount - (int)basic_map->beansCount)
	{
		al_draw_bitmap(op12, 100, 690, 0);
	}
	
}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	al_destroy_bitmap(op1);
	al_destroy_bitmap(op2);
	al_destroy_bitmap(op3);
	al_destroy_bitmap(op4);
	al_destroy_bitmap(op5);
	al_destroy_bitmap(op6);
	al_destroy_bitmap(op7);
	al_destroy_bitmap(op8);
	al_destroy_bitmap(op9);
	al_destroy_bitmap(op10);
	al_destroy_bitmap(op11);
	al_destroy_bitmap(op12);
	delete_map(basic_map);
	pacman_destory(pman);
	for (int i = 0; i < GHOST_NUM; i++)
	{
		ghost_destory(ghosts[i]);
	}
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
		case ALLEGRO_KEY_P:
			stop_bgm(gameBGM);
			stop_bgm(PACMAN_POWERBEAN2_SOUND);
			basic_map->beansNum = 0;
			game_change_scene(scene_win_create());
			break;
		
	default:
		break;
	}
	 // TODO
}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0)); 

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0), 
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
