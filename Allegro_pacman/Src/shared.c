// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_KILL_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_POWERBEAN_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_BEAN_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_SAMPLE* settingsMusic = NULL;
ALLEGRO_SAMPLE* gameMusic = NULL;
ALLEGRO_SAMPLE* winMusic = NULL;
ALLEGRO_SAMPLE* loseMusic = NULL;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
float master_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/Overtaken.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/Eat.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/Death.ogg");
	PACMAN_KILL_SOUND = load_audio("Assets/Music/Punch.ogg");
	PACMAN_POWERBEAN_SOUND = load_audio("Assets/Music/TVVS.ogg");
	PACMAN_BEAN_SOUND = load_audio("Assets/Music/Yahoo.ogg");
	settingsMusic = load_audio("Assets/Music/Prepare.ogg");
	gameMusic = load_audio("Assets/Music/One_Piece_Game_BGM.ogg");
	winMusic = load_audio("Assets/Music/Victory.ogg");
	loseMusic = load_audio("Assets/Music/Defeat.ogg");
}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_sample(PACMAN_KILL_SOUND);
	al_destroy_sample(PACMAN_POWERBEAN_SOUND);
	al_destroy_sample(PACMAN_BEAN_SOUND);
	al_destroy_sample(settingsMusic);
	al_destroy_sample(gameMusic);
	al_destroy_sample(winMusic);
	al_destroy_sample(loseMusic);
}