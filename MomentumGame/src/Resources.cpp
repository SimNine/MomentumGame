#include "Standard.h"
#include "Resources.h"

#include "Global.h"
#include "ResourceLoader.h"

// titleScreen resources
std::string _title_title;
std::string _title_sub;
std::string _title_urfco;

// mainScreen resources
SDL_Texture* _main_bkgdata;
SDL_Texture* _main_bkgsplotch;
SDL_Texture* _main_bkgsplotch2;
std::string _main_campaign_classic;
std::string _main_campaign_nightfall;
std::string _main_campaign_procedural;
std::string _main_campaign_bkg;

// SDL_Mixer music
Mix_Music* _music_title;
Mix_Music* _music_map_ambient;
Mix_Music* _music_game1;
Mix_Music* _music_game2;
Mix_Music* _music_game3;
Mix_Music* _music_game4;

// SDL_Mixer sounds
Mix_Chunk* _sound_move_player;
Mix_Chunk* _sound_move_computer;
Mix_Chunk* _sound_action_attack;
Mix_Chunk* _sound_action_speed;
Mix_Chunk* _sound_action_heal;
Mix_Chunk* _sound_action_grid_damage;
Mix_Chunk* _sound_action_grid_fix;
Mix_Chunk* _sound_select;
Mix_Chunk* _sound_pickup_credit;
Mix_Chunk* _sound_pickup_files;

// colors
SDL_Color _color_bkg_standard;
SDL_Color _color_clear;
SDL_Color _color_black;
SDL_Color _color_white;

// animations
SDL_Texture* _particle_basic;
SDL_Texture* _particle_selectring;

// achievements
SDL_Texture* _achievement_badge;
SDL_Texture* _achievement_badge_large;
SDL_Texture* _achievement_texture[ACHIEVEMENT_NUM_ACHIEVEMENTS];
SDL_Texture* _achievement_locked;
std::string _achievement_name[ACHIEVEMENT_NUM_ACHIEVEMENTS];
std::string _achievement_description[ACHIEVEMENT_NUM_ACHIEVEMENTS];

// initialize all data
void initResources() {
	// titleScreen resources
	_title_title = "resources/title_main.png";
	_title_sub = "resources/title_subtitle.png";
	_title_urfco = "resources/title/title_urfco_alt3.png";

	// mainScreen resources
	_main_bkgdata = loadTexture("resources/main/menu_bkgdata.png");
	_main_bkgsplotch = loadTexture("resources/main/menu_bkgsplotch.png");
	_main_bkgsplotch2 = loadTexture("resources/main/menu_bkgsplotch2.png");
	_main_campaign_classic = "resources/main/campaign_classic_icon.png";
	_main_campaign_nightfall = "resources/main/campaign_nightfall_icon.png";
	_main_campaign_procedural = "resources/main/campaign_procedural_icon.png";
	_main_campaign_bkg = "resources/main/campaign_icon.png";

	// SDL_Mixer music
	_music_title = loadMusic("resources/sounds/title_music.flac");
	_music_map_ambient = loadMusic("resources/sounds/map_ambient.flac");
	_music_game1 = loadMusic("resources/sounds/e1.flac");
	_music_game2 = loadMusic("resources/sounds/e3.flac");
	_music_game3 = loadMusic("resources/sounds/e5.flac");
	_music_game4 = loadMusic("resources/sounds/e8.flac");

	// SDL_Mixer sounds
	_sound_move_player = loadSound("resources/sounds/move_player.flac");
	_sound_move_computer = loadSound("resources/sounds/move_computer.flac");
	_sound_action_attack = loadSound("resources/sounds/action_attack.flac");
	_sound_action_grid_damage = loadSound("resources/sounds/action_damage_grid.flac");
	_sound_action_grid_fix = loadSound("resources/sounds/action_fix_grid.flac");
	_sound_action_heal = loadSound("resources/sounds/action_size.flac");
	_sound_action_speed = loadSound("resources/sounds/action_speed.flac");
	_sound_pickup_credit = loadSound("resources/sounds/game_credit.flac");
	_sound_pickup_files = loadSound("resources/sounds/win_flag.flac");

	// colors
	_color_bkg_standard = { 120, 120, 120, 140 };
	_color_clear = { 0, 0, 0, 0 };
	_color_black = { 0, 0, 0, 255 };
	_color_white = { 255, 255, 255, 255 };

	// gameboard animations
	_particle_basic = loadTexture("resources/game/anims/particle.png");
	_particle_selectring = loadTexture("resources/game/anims/particle_selectring.png");

	// achievements
	_achievement_badge = loadTexture("resources/achievements/plaque.png");
	_achievement_badge_large = loadTexture("resources/achievements/plaque_large.png");

	_achievement_locked = loadTexture("resources/achievements/achievement_locked.png");
	_achievement_texture[ACHIEVEMENT_PLACEHOLDER1] = loadTexture("resources/achievements/game_started.png");
	_achievement_texture[ACHIEVEMENT_PLACEHOLDER2] = loadTexture("resources/achievements/game_finished.png");
	_achievement_texture[ACHIEVEMENT_PLACEHOLDER3] = loadTexture("resources/achievements/game_won.png");

	_achievement_description[ACHIEVEMENT_PLACEHOLDER1] = "Start a campaign game";
	_achievement_description[ACHIEVEMENT_PLACEHOLDER2] = "Finish a campaign game";
	_achievement_description[ACHIEVEMENT_PLACEHOLDER3] = "Win a campaign game";
}