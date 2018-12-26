#pragma once

#include "Standard.h"

// initializing function
void initResources();

// titleScreen resources
extern std::string _title_title;
extern std::string _title_sub;
extern std::string _title_urfco;

// mainScreen resources
extern SDL_Texture* _main_bkgdata;
extern SDL_Texture* _main_bkgsplotch;
extern SDL_Texture* _main_bkgsplotch2;

// SDL_Mixer music
extern Mix_Music* _music_title;
extern Mix_Music* _music_map_ambient;
extern Mix_Music* _music_game1;
extern Mix_Music* _music_game2;
extern Mix_Music* _music_game3;
extern Mix_Music* _music_game4;

// SDL_Mixer sounds
extern Mix_Chunk* _sound_move_player;
extern Mix_Chunk* _sound_move_computer;
extern Mix_Chunk* _sound_action_attack;
extern Mix_Chunk* _sound_action_speed;
extern Mix_Chunk* _sound_action_heal;
extern Mix_Chunk* _sound_action_grid_damage;
extern Mix_Chunk* _sound_action_grid_fix;
extern Mix_Chunk* _sound_select;
extern Mix_Chunk* _sound_pickup_credit;
extern Mix_Chunk* _sound_pickup_files;

// colors
extern SDL_Color _color_bkg_standard;
extern SDL_Color _color_clear;
extern SDL_Color _color_black;
extern SDL_Color _color_white;

// animations
extern SDL_Texture* _particle_basic;
extern SDL_Texture* _particle_selectring;

// achievements
extern SDL_Texture* _achievement_badge;
extern SDL_Texture* _achievement_badge_large;
extern SDL_Texture* _achievement_texture[ACHIEVEMENT_NUM_ACHIEVEMENTS];
extern SDL_Texture* _achievement_locked;
extern std::string _achievement_name[ACHIEVEMENT_NUM_ACHIEVEMENTS];
extern std::string _achievement_description[ACHIEVEMENT_NUM_ACHIEVEMENTS];