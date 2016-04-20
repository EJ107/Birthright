// Game.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Game.h"
#include "Protag.h"
#include "Obstacle.h"
#include "Battle.h"
#include "FileReader.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include <iomanip>
using namespace std;

int smallest_tile_size = 0;
int window_width = 0;
int window_height = 0;
int protag_offset_y = 0;
int protag_offset_x = 0;
int view_pane_start_x = 0;
int view_pane_start_y = 0;
int viewing_pane_into_first_tile_x = 0;
int viewing_pane_into_first_tile_y = 0;
int number_of_tiles_per_width = 26;
int tileSize = 0;
int main_menu_selector = 0;
int pause_menu_selector = 0;
int inventory_menu_selector = 1;
int game_over_menu_selector = 1;
int enemy_being_fought = 0;
int number_of_healing_potions = 3;

bool sound_effects_on = true;
bool music_on = true;

double _pi = 3.14159265358979323846;

const int FRAMES_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

int **the_tile_map;

Protag protag(0, the_tile_map);

float time_tracker = 0;

bool is_fullscreen = false;
bool battle_active = false;
bool main_menu = true;
bool pause_menu = false;
bool options_menu = false;
bool inventory_menu = false;
bool game_over = false;
bool fade_to_black = false;
bool fade_to_black_changed = false;
bool demo_won = false;
string fade_to_black_action = "";
double fade_to_black_count = 0;

Battle battle(0, 0, 0, 0, 0, "", "", "", "", "", "", true);

// array of enemies 
vector<Obstacle> enemies;
vector<Obstacle> lightning_columns;

sf::Font font;
sf::Texture morgan_texture;
sf::Texture enemy_texture;
sf::Texture morgan_icon_texture;
sf::Texture waterfall_texture;
sf::Texture backdrop_texture;
sf::Texture background_01_texture;
sf::Texture background_02_texture;
sf::Texture background_03_texture;
sf::Texture background_04_texture;
sf::Texture background_05_texture;
sf::Texture background_06_texture;
sf::Texture background_07_texture;
sf::Texture background_08_texture;
sf::Texture background_09_texture;
sf::Texture background_10_texture;
sf::Texture background_11_texture;
sf::Texture background_12_texture;
sf::Texture background_13_texture;

sf::Image icon;

sf::Sound run_sound;
sf::SoundBuffer sound_run_1;
sf::SoundBuffer sound_run_2;
sf::SoundBuffer sound_run_3;
sf::SoundBuffer sound_run_4;
sf::SoundBuffer sound_run_5;

sf::Sound button_sound;
sf::SoundBuffer sound_button;

sf::Sound back_sound;
sf::SoundBuffer sound_back;

sf::Sound potion_sound;
sf::SoundBuffer sound_potion;

sf::Music music_menu;
sf::Music game_music;
sf::Music battle_music;

int run_sound_iterator = 1;
int morgan_animation_frame = 1;
int morgan_animation_cap = 6;
int enemy_animation_frame = 1;
int enemy_animation_cap = 4;
bool facing_forwards = true;

double morgan_animation_count = 0;
double enemy_animation_count = 0;

bool morgan_sprite_change = true;
bool enemy_sprite_change = false;

double waterfall_animation_count = 0.0;
double waterfall_duration_count = 0.0; 
int waterfall_animation_frame = 1;
int waterfall_animation_cap = 4;

bool waterfalls_active = false;
bool waterfall_start = true;
bool waterfall_end = false;

// respawn variables
int last_checkpoint = 0;
int potions_at_last_checkpoint = 3;
int health_at_last_checkpoint = 1000;

string type_of_enemy = "";

// play a sound effect
void play_sound_effect(string sound_type) {

	if(sound_effects_on == true) {

		if(sound_type == "run") {

			switch(run_sound_iterator) {

				case 1: {	
					run_sound.setBuffer(sound_run_1);
					break;
				}
				case 2: {
					run_sound.setBuffer(sound_run_2);
					break;
				}
				case 3: {
					run_sound.setBuffer(sound_run_3);
					break;
				}
				case 4: {
					run_sound.setBuffer(sound_run_4);
					break;
				}
				case 5: {
					run_sound.setBuffer(sound_run_5);
					break;
				}
				default: 
					break;
			}

			run_sound.setVolume(50);
			run_sound_iterator++;
			run_sound.play();	

			if(run_sound_iterator > 5) {
				run_sound_iterator = 1;
			}
		}
		else if(sound_type == "button") {
			button_sound.setBuffer(sound_button);
			button_sound.setVolume(80);
			button_sound.play();	
		}
		else if(sound_type == "back") {
			back_sound.setBuffer(sound_back);
			back_sound.setVolume(80);
			back_sound.play();	
		}
		else if(sound_type == "potion") {
			potion_sound.setBuffer(sound_potion);
			potion_sound.setVolume(80);
			potion_sound.play();	
		}
	}
}

// return a sprite with the correct texture based on certain parameters
sf::Sprite getSprite(string character, string mode, string action, int frame) {

	sf::Sprite player;
	string filepath = "";

	if(character == "frostbeast") {

		filepath = ("Resources/Sprites/Frostbeast/idle_0" + to_string(static_cast<long long>(enemy_animation_frame)) + ".png");

		if(enemy_sprite_change) {
			// LOAD THE BASE ENEMY TEXTURE
			if (!enemy_texture.loadFromFile(filepath)) {
					// error...
			}
			enemy_sprite_change = false;
		}

		enemy_texture.setSmooth(true);
		player.setTexture(enemy_texture);
		player.setScale(sf::Vector2f((0.75f * ((double)window_width / 1920)), (0.75f * ((double)window_width / 1920))));
	}
	else {

		if(mode == "icon") {
			player.setColor(sf::Color(255, 255, 255, 200));
			player.setTexture(morgan_icon_texture);
			player.setScale(sf::Vector2f((0.75f * ((double)window_width / 1920)), (0.75f * ((double)window_width / 1920))));
		}
		else {

			string action = "";

			if(protag.get_grapple_hooked() != true) {
				if(protag.get_horizontal_velocity() != 0) {
					if(protag.get_horizontal_velocity() > 0) {
						facing_forwards = true;
					}
					else {
						facing_forwards = false;
					}
				}
			}
			if(true == protag.get_recovery()) {

				if(morgan_animation_cap != 2) {
					morgan_animation_cap = 2;
					morgan_animation_frame = 1;
				}

				filepath = ("Resources/Sprites/Morgan/morgan_recovery_0" + to_string(static_cast<long long>(morgan_animation_frame)) + ".png");
				morgan_sprite_change = true;
				action = "recovery";

				if(morgan_animation_frame == 2) {
					protag.set_recovery();
				}
			}
			else if(true == protag.get_is_jumping()) {

				if(morgan_animation_cap != 5) {
					morgan_animation_cap = 5;
					morgan_animation_frame = 1;
				}

				filepath = ("Resources/Sprites/Morgan/morgan_jumping_0" + to_string(static_cast<long long>(morgan_animation_frame)) + ".png");
				action = "jumping";

				if(morgan_animation_frame == 5) {
					protag.set_jumping();
				}
			}
			else if(true == protag.get_is_falling()) {

				if(morgan_animation_cap != 4) {
					morgan_animation_cap = 4;
					morgan_animation_frame = 1;
				}

				filepath = ("Resources/Sprites/Morgan/morgan_falling_0" + to_string(static_cast<long long>(morgan_animation_frame)) + ".png");
			}
			else if(protag.get_horizontal_velocity() != 0) {

				if(morgan_animation_cap != 12) {
					morgan_animation_cap = 12;
					morgan_animation_frame = 1;
				}

				action = "run";
				filepath = ("Resources/Sprites/Morgan/morgan_running_0" + to_string(static_cast<long long>(morgan_animation_frame)) + ".png");
			}
			else {

				if(morgan_animation_cap != 6) {
					morgan_animation_cap = 6;
					morgan_animation_frame = 1;
				}
				filepath = ("Resources/Sprites/Morgan/morgan_idle_0" + to_string(static_cast<long long>(morgan_animation_frame)) + ".png");
			}

			if(morgan_sprite_change) {
				// LOAD THE BASE PLAYER TEXTURE
				if (!morgan_texture.loadFromFile(filepath)){
					// error...
				}

				morgan_sprite_change = false;

				// PLAY RUN SOUND IF ON THE CORRECT SPRITE
				if((action == "run" && (morgan_animation_frame == 5 || morgan_animation_frame == 11)) || ((action == "recovery" || action == "jumping") && (morgan_animation_frame == 2))) {
					play_sound_effect("run");
				}
			}

			morgan_texture.setSmooth(true);
			player.setTexture(morgan_texture);

			if(facing_forwards) {
				player.setScale(sf::Vector2f((-0.75f * ((double)window_width / 1920)), (0.75f * ((double)window_width / 1920))));
			}
			else {
				player.setScale(sf::Vector2f((0.75f * ((double)window_width / 1920)), (0.75f * ((double)window_width / 1920))));
			}
		}
	}

	return player;
}

// draws the backdrop
void draw_backdrop(sf::RenderWindow* window) {

	int new_x = protag.get_x();
	int new_y = protag.get_y();

	viewing_pane_into_first_tile_x = (new_x % tileSize);
	viewing_pane_into_first_tile_y = (new_y % tileSize);
	view_pane_start_x = new_x - ((window_width / 2) + viewing_pane_into_first_tile_x);      
	view_pane_start_y = new_y - ((window_height / 2) + viewing_pane_into_first_tile_y);        

	sf::Sprite backdrop;

	backdrop.setTexture(backdrop_texture);
	backdrop.setPosition((0 - ((view_pane_start_x + viewing_pane_into_first_tile_x) / 80)), (0 - ((view_pane_start_y + viewing_pane_into_first_tile_y)  / 20) - (tileSize * 3)));
	backdrop.setScale(sf::Vector2f((1.0f * ((double)(tileSize * number_of_tiles_per_width) / 1920)), (1.0f * ((double)(tileSize * number_of_tiles_per_width) / 1920))));
	window->draw(backdrop);
}

// draws the game platformer section
void draw_overworld(sf::RenderWindow* window) {

	int new_x = protag.get_x();
	int new_y = protag.get_y();

	viewing_pane_into_first_tile_x = (new_x % tileSize);
	viewing_pane_into_first_tile_y = (new_y % tileSize);
	view_pane_start_x = new_x - ((window_width / 2) + viewing_pane_into_first_tile_x);      
	view_pane_start_y = new_y - ((window_height / 2) + viewing_pane_into_first_tile_y);        

	// DRAW THE BACKGROUND
	sf::Sprite background;

	for(int i = 1; i <= 13; i++) {

		switch(i) {

			case 1:
				background.setTexture(background_01_texture);
				break;
			case 2:
				background.setTexture(background_02_texture);
				break;
			case 3:
				background.setTexture(background_03_texture);
				break;
			case 4:
				background.setTexture(background_04_texture);
				break;
			case 5:
				background.setTexture(background_05_texture);
				break;
			case 6:
				background.setTexture(background_06_texture);
				break;
			case 7:
				background.setTexture(background_07_texture);
				break;
			case 8:
				background.setTexture(background_08_texture);
				break;
			case 9:
				background.setTexture(background_09_texture);
				break;
			case 10:
				background.setTexture(background_10_texture);
				break;
			case 11:
				background.setTexture(background_11_texture);
				break;
			case 12:
				background.setTexture(background_12_texture);
				break;
			case 13:
				background.setTexture(background_13_texture);
				break;
			default:
				background.setTexture(background_01_texture);
				break;
		}
		
		if(is_fullscreen) {
			if(i >= 7) {
				background.setPosition((0 - tileSize - view_pane_start_x - viewing_pane_into_first_tile_x + ( (i - 1) * (tileSize * 50) ) ) - (tileSize * 1.25), (0 - view_pane_start_y - viewing_pane_into_first_tile_y + (tileSize * 0.65)));
			}
			else {
				background.setPosition((0 - view_pane_start_x - viewing_pane_into_first_tile_x + ( (i - 1) * (tileSize * 50) ) ) - (tileSize * 1.25), (0 - view_pane_start_y - viewing_pane_into_first_tile_y + (tileSize * 0.65)));
			}
		}
		else {
			if(i >= 7) {
				background.setPosition((0 - tileSize - view_pane_start_x - viewing_pane_into_first_tile_x + ( (i - 1) * (tileSize * 50) ) ) - (tileSize * 1.40), (0 - view_pane_start_y - viewing_pane_into_first_tile_y + (tileSize * 0.6)));
			}
			else {
				background.setPosition((0 - view_pane_start_x - viewing_pane_into_first_tile_x + ( (i - 1) * (tileSize * 50) ) ) - (tileSize * 1.40), (0 - view_pane_start_y - viewing_pane_into_first_tile_y + (tileSize * 0.6)));
			}
		}

		background.setScale(sf::Vector2f((0.985f * ((double)(tileSize * number_of_tiles_per_width) / 1920.0)), (0.98f * ((double)(tileSize * number_of_tiles_per_width) / 1920.0))));
		window->draw(background);
	}

	/**
	 * -------------------------------
	 * Draw the Lightning Columns:	 |
	 * -------------------------------
	 */
	for(int i = 0; i < lightning_columns.size(); i++) {
		if(true == lightning_columns[i].get_visuals_active()) {
			sf::Sprite waterfall;
			waterfall.setTexture(waterfall_texture);
			waterfall.setPosition(((lightning_columns[i].get_x()) - view_pane_start_x - viewing_pane_into_first_tile_x), (lightning_columns[i].get_y() - view_pane_start_y - viewing_pane_into_first_tile_y + (tileSize / 2)));
			waterfall.setScale(sf::Vector2f((1.5f * ((double)window_width / 1920)), (2.6f * ((double)window_width / 1920))));
			window->draw(waterfall);
		}
	}
	
	/*
	int number_of_rows_to_draw = window_height / tileSize + 2;
	int number_of_columns_to_draw = window_width / tileSize + 2;

	// sets the column offset (how many columns in to start drawing)
	int column_offset = ((view_pane_start_x + (tileSize / 2)) / tileSize);
		
	// sets the row offset (how many rows in to start drawing)
	int row_offset = ((view_pane_start_y ) / tileSize);
		
	// iterates from 1 to number of rows to draw
	int view_row = 0;

	// FOR EACH ROW ============================================================
	for(int row = row_offset; row < (row_offset + number_of_rows_to_draw); row++) {
			
		// iterates from 1 to number of columns to draw
		int view_column = 0;

		// FOR EACH COLUMN =================================================================================
		for(int column = column_offset; column < (column_offset + (number_of_columns_to_draw + 1)); column++) {

			// if the row/column iterator is greater than the size of the map array do nothing- an error must have occurred
			if(column > 633) {
				// do nothing
			}
			else if(row > 59) {
				// do nothing
			}
			else if(the_tile_map[row][column] == 1) {
					
				// draw solid tiles
				sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));

				rectangle.setFillColor(sf::Color(100, 100, 100));
				rectangle.setPosition(((view_column * tileSize) - tileSize - viewing_pane_into_first_tile_x), ((view_row * tileSize) - viewing_pane_into_first_tile_y));

				window->draw(rectangle);
			}
			else if(the_tile_map[row][column] == 2) {
					
				// draw solid tiles
				sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));

				rectangle.setFillColor(sf::Color(0, 0, 0));
				rectangle.setPosition(((view_column * tileSize) - tileSize - viewing_pane_into_first_tile_x), ((view_row * tileSize) - viewing_pane_into_first_tile_y));

				window->draw(rectangle);
			}
			else if(the_tile_map[row][column] == 3) {
					
				// draw solid tiles
				sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));

				rectangle.setFillColor(sf::Color(75, 0, 0));
				rectangle.setPosition(((view_column * tileSize) - tileSize - viewing_pane_into_first_tile_x), ((view_row * tileSize) - viewing_pane_into_first_tile_y));

				window->draw(rectangle);
			}
			else {
				// tile blank so do nothing
			}		
			view_column++;
		}
		view_row++;
	}	
	*/

	/**
	 * -----------------------------------
	 * Draw the Grapple:				 |
	 * -----------------------------------
	 */
	if(true == protag.grapple_is_active()) {

		vector< vector<double> > catch_points = protag.get_catch_points();

		if (protag.is_hooked() == false || catch_points.size() == 0) {

			// connect the final point to the player
			int before_pos_x = (protag.get_grapple_coords()[0] - view_pane_start_x - viewing_pane_into_first_tile_x);
			int before_pos_y = (protag.get_grapple_coords()[1] - view_pane_start_y - viewing_pane_into_first_tile_y);

			sf::Vertex grapple_cord[] = {
				sf::Vertex(sf::Vector2f(((window_width / 2) - protag_offset_x + (tileSize / 2)), ((window_height / 2) - protag_offset_y) + tileSize), sf::Color(0, 0, 0)),
				sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x), (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
			};
			sf::Vertex grapple_cord_2[] = {
				sf::Vertex(sf::Vector2f(((window_width / 2) - protag_offset_x + (tileSize / 2)) + 1, ((window_height / 2) - protag_offset_y) + tileSize), sf::Color(0, 0, 0)),
				sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x) + 1, (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
			};
			sf::Vertex grapple_cord_3[] = {
				sf::Vertex(sf::Vector2f(((window_width / 2) - protag_offset_x + (tileSize / 2)) - 1, ((window_height / 2) - protag_offset_y) + tileSize), sf::Color(0, 0, 0)),
				sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x) - 1, (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
			};

			window->draw(grapple_cord, 2, sf::Lines);
			window->draw(grapple_cord_2, 2, sf::Lines);
			window->draw(grapple_cord_3, 2, sf::Lines);
		}
		else {
			// for each point along the grapple connect it to its previous point
			for(int i = 0; i < catch_points.size(); i++){
			   if(i != 0){
					int before_pos_x = (catch_points[(i - 1)].front() - view_pane_start_x - viewing_pane_into_first_tile_x);
					int before_pos_y = (catch_points[(i - 1)].back() - view_pane_start_y - viewing_pane_into_first_tile_y);
					int after_pos_x = (catch_points[i].front() - view_pane_start_x - viewing_pane_into_first_tile_x);
					int after_pos_y = (catch_points[i].back() - view_pane_start_y - viewing_pane_into_first_tile_y);

					sf::Vertex grapple_cord[] = {
						sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x), (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0)),
						sf::Vertex(sf::Vector2f((after_pos_x - protag_offset_x), (after_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
					};
					sf::Vertex grapple_cord_2[] = {
						sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x) + 1, (before_pos_y - protag_offset_y) + 1), sf::Color(0, 0, 0)),
						sf::Vertex(sf::Vector2f((after_pos_x - protag_offset_x) + 1, (after_pos_y - protag_offset_y) + 1), sf::Color(0, 0, 0))
					};
					sf::Vertex grapple_cord_3[] = {
						sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x) - 1, (before_pos_y - protag_offset_y) - 1), sf::Color(0, 0, 0)),
						sf::Vertex(sf::Vector2f((after_pos_x - protag_offset_x) - 1, (after_pos_y - protag_offset_y) - 1), sf::Color(0, 0, 0))
					};

					window->draw(grapple_cord, 2, sf::Lines);
					window->draw(grapple_cord_2, 2, sf::Lines);
					window->draw(grapple_cord_3, 2, sf::Lines);
			   }
			}

			// connect the final point to the player
			int before_pos_x = (catch_points.back().front() - view_pane_start_x - viewing_pane_into_first_tile_x);
			int before_pos_y = (catch_points.back().back() - view_pane_start_y - viewing_pane_into_first_tile_y);

			sf::Vertex grapple_cord[] = {
				sf::Vertex(sf::Vector2f(((window_width / 2) - protag_offset_x + (tileSize / 2)), ((window_height / 2) - protag_offset_y) + tileSize), sf::Color(0, 0, 0)),
				sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x), (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
			};
			sf::Vertex grapple_cord_2[] = {
				sf::Vertex(sf::Vector2f(((window_width / 2) - protag_offset_x + (tileSize / 2)) + 1, ((window_height / 2) - protag_offset_y) + tileSize), sf::Color(0, 0, 0)),
				sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x) + 1, (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
			};
			sf::Vertex grapple_cord_3[] = {
				sf::Vertex(sf::Vector2f(((window_width / 2) - protag_offset_x + (tileSize / 2)) - 1, ((window_height / 2) - protag_offset_y) + tileSize), sf::Color(0, 0, 0)),
				sf::Vertex(sf::Vector2f((before_pos_x - protag_offset_x) - 1, (before_pos_y - protag_offset_y)), sf::Color(0, 0, 0))
			};

			window->draw(grapple_cord, 2, sf::Lines);
			window->draw(grapple_cord_2, 2, sf::Lines);
			window->draw(grapple_cord_3, 2, sf::Lines);
		}
	}

	/**
	 * -------------------------------
	 * Draw the Enemies:			 |
	 * -------------------------------
	 */
	for(int i = 0; i < enemies.size(); i++) {

		sf::Sprite enemy = getSprite("frostbeast", "", "", 0);
		
		if(enemies[i].get_id() == 3) {
			enemy.setScale(sf::Vector2f((1.3f * ((double)window_width / 1920)), (1.3f * ((double)window_width / 1920))));
			enemy.setPosition(((enemies[i].get_x()) - view_pane_start_x - viewing_pane_into_first_tile_x - (tileSize * 2.8)), (enemies[i].get_y() - view_pane_start_y - viewing_pane_into_first_tile_y - (tileSize * 2.5)));
		}
		else {
			enemy.setPosition(((enemies[i].get_x()) - view_pane_start_x - viewing_pane_into_first_tile_x - (tileSize * 2)), (enemies[i].get_y() - view_pane_start_y - viewing_pane_into_first_tile_y - (tileSize * 0.5)));
		}

		window->draw(enemy);
	}
	
	/**
	 * -----------------------------------
	 * Draw the Protagonist:			 |
	 * -----------------------------------
	 */
	sf::Sprite player = getSprite("", "", "", 0);

	if(facing_forwards) {
		player.setPosition(((window_width / 2) - protag_offset_x + (300 * 0.5 * ((double)window_width / 1920))), ((window_height / 2) - protag_offset_y - (50 * 0.5 * ((double)window_width / 1920))));
	}
	else {
		player.setPosition(((window_width / 2) - protag_offset_x - (320 * 0.25 * ((double)window_width / 1920))), ((window_height / 2) - protag_offset_y - (50 * 0.5 * ((double)window_width / 1920))));
	}

	if(protag.getIsDead() != 1) {
		window->draw(player);
	}

	/**
	 * -------------------------------
	 * Draw the UI:					 |
	 * -------------------------------
	 */
	// draw the player icon
	sf::Sprite player_icon = getSprite("", "icon", "", 0);
	player_icon.setPosition((window_width / 200), (window_width / 200));
	window->draw(player_icon);

	// draw the healthbar
	sf::RectangleShape health_outline(sf::Vector2f((tileSize * 3), (tileSize * 0.7)));

	health_outline.setOutlineThickness(4);
	health_outline.setOutlineColor(sf::Color(0, 0, 0, 200));
	health_outline.setPosition((window_width / 50) + (tileSize * 2.2), (window_width / 50) + (tileSize * 0.5));
	health_outline.setFillColor(sf::Color(0, 0, 0, 0));

	double health_percentage = ((double)protag.getHealth() / protag.getMaxHealth());

	sf::RectangleShape health(sf::Vector2f(((tileSize * 3) * health_percentage), (tileSize * 0.7)));
	health.setPosition((window_width / 50) + (tileSize * 2.2), (window_width / 50) + (tileSize * 0.5));
	health.setFillColor(sf::Color(200, 0, 0, 200));

	window->draw(health);
	window->draw(health_outline);
}

// draws the main menu
void draw_main_menu(sf::RenderWindow* window) {

	// DRAW THE BACKDROP
	sf::Sprite backdrop;

	backdrop.setTexture(backdrop_texture);
	backdrop.setPosition(0, 0);
	backdrop.setScale(sf::Vector2f((0.9f * ((double)window_width / 1920)), (0.9f * ((double)window_width / 1920))));
	window->draw(backdrop);

	// DRAW THE MENU BUTTONS
	// button 1
	sf::RectangleShape button(sf::Vector2f(window_width / 4 * 0.6, tileSize * 1.5 * 0.6));
	button.setPosition(window_width / 2 - ((window_width / 8) * 0.6), window_height * (2/3) + (tileSize * 0.6 * 2) + (tileSize/1.3) + (tileSize * 2));
	
	if(main_menu_selector == 0) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}
	
	window->draw(button);

	// button 2 
	button.setPosition(window_width / 2 - (window_width / 8) * 0.6, window_height * (2/3) + (tileSize * 0.6 * 4) + (tileSize/1.3) + (tileSize * 2));
	
	if(main_menu_selector == 1) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}

	window->draw(button);

	// button 3 
	button.setPosition(window_width / 2 - (window_width / 8) * 0.6, window_height * (2/3) + (tileSize * 0.6 * 6) + (tileSize/1.3) + (tileSize * 2));
	
	if(main_menu_selector == 2) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}

	window->draw(button);

	// DRAW THE MENU TEXT
	sf::Text text;
	text.setFont(font); 
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(tileSize * 1.3); 
	text.setColor(sf::Color::Black);
	text.setString("Birthright");
	text.setPosition((window_width / 2) - (tileSize * 2.6), window_height * 0.08);
	window->draw(text);
	text.setCharacterSize(tileSize * 0.5); 

	// text 1
	text.setString("Start Game");
	if(main_menu_selector == 0) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition((window_width / 2) - (tileSize * 1.4), window_height * (2/3) + (tileSize / 3.3) + (tileSize * 0.6 * 3) + (tileSize * 2));
	window->draw(text);

	// text 2
	text.setString("Options");
	if(main_menu_selector == 1) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition((window_width / 2) - (tileSize * 0.95), window_height * (2/3) + (tileSize / 3.3) + (tileSize * 0.6 * 5) + (tileSize * 2));
	window->draw(text);

	// text 3
	text.setString("Exit Game");
	if(main_menu_selector == 2) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition((window_width / 2) - (tileSize * 1.25), window_height * (2/3) + (tileSize / 3.3) + (tileSize * 0.6 * 7) + (tileSize * 2));
	window->draw(text);
}

// draws the pause menu
void draw_pause_menu(sf::RenderWindow* window) {

	// DRAW THE backdrop
	sf::RectangleShape backdrop(sf::Vector2f(window_width / 4, window_height * 0.7));
	backdrop.setFillColor(sf::Color(200, 200, 200, 200));
	backdrop.setPosition((window_width / 2 - (window_width / 8)), 0 + (window_height * 0.1));
	window->draw(backdrop);

	// DRAW THE MENU BUTTONS
	// button 1
	sf::RectangleShape button(sf::Vector2f(window_width / 4 * 0.6, tileSize * 1.5 * 0.6));
	button.setPosition(window_width / 2 - ((window_width / 8) * 0.6), window_height * (2/3) + (tileSize * 0.6 * 2) + (tileSize/1.3));
	
	if(pause_menu_selector == 0) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}
	
	window->draw(button);

	// button 2 
	button.setPosition(window_width / 2 - (window_width / 8) * 0.6, window_height * (2/3) + (tileSize * 0.6 * 4) + (tileSize/1.3));
	
	if(pause_menu_selector == 1) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}

	window->draw(button);

	// button 3 
	button.setPosition(window_width / 2 - (window_width / 8) * 0.6, window_height * (2/3) + (tileSize * 0.6 * 6) + (tileSize/1.3));
	
	if(pause_menu_selector == 2) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}

	window->draw(button);

	// DRAW THE MENU TEXT
	sf::Text text;
	text.setFont(font); 
	text.setCharacterSize(tileSize * 0.5); 
	text.setStyle(sf::Text::Bold);

	// text 1
	text.setString("Inventory");
	if(pause_menu_selector == 0) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition((window_width / 2) - (tileSize * 1.1), window_height * (2/3) + (tileSize / 3.3) + (tileSize * 0.6 * 3));
	window->draw(text);

	// text 2
	text.setString("Options");
	if(pause_menu_selector == 1) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition((window_width / 2) - (tileSize * 0.95), window_height * (2/3) + (tileSize / 3.3) + (tileSize * 0.6 * 5));
	window->draw(text);
	
	// text 3
	text.setString("Quit");
	if(pause_menu_selector == 2) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition((window_width / 2) - (tileSize * 0.58), window_height * (2/3) + (tileSize / 3.3) + (tileSize * 0.6 * 7));
	window->draw(text);
}

// draws the options menu
void draw_options_menu(sf::RenderWindow* window) {

	//draw the backdrop
	sf::RectangleShape backdrop(sf::Vector2f(window_width / 2.5, window_height * 0.7));
	backdrop.setFillColor(sf::Color(200, 200, 200, 200));
	backdrop.setPosition((window_width / 2 - (window_width / 5)), 0 + (window_height * 0.1));
	window->draw(backdrop);

	// Draw the options title text
	sf::Text text;
	text.setFont(font); 
	text.setCharacterSize(tileSize * 0.7); 
	text.setStyle(sf::Text::Bold);
	text.setString("Options");
	text.setColor(sf::Color::Black);
	text.setPosition((window_width / 2) - (tileSize * 1.15), 0 + (window_height * 0.12));
	window->draw(text);

	// ----------------------------------
	//	FULLSCREEN CHECKBOX				|
	// ----------------------------------
	// draw the 'fullscreen mode' text
	text.setCharacterSize(tileSize * 0.5); 
	text.setStyle(sf::Text::Regular);
	text.setString("Fullscreen Mode:");
	text.setPosition((window_width / 2 - (window_width / 5) + (tileSize / 2)), 0 + (window_height * 0.225));
	window->draw(text);

	//draw the fullscreen mode checkbox
	sf::RectangleShape checkbox(sf::Vector2f((tileSize / 2), (tileSize / 2)));
	checkbox.setOutlineColor(sf::Color(0, 0, 0));
	checkbox.setOutlineThickness(tileSize / 15);
	checkbox.setPosition((window_width / 2 - (window_width / 5) + (tileSize * 4.5)), 0 + (window_height * 0.232));
	window->draw(checkbox);

	// mark the checkbox if fullscreen mode is active 
	if(true == is_fullscreen) {
		text.setCharacterSize(tileSize * 0.8); 
		text.setStyle(sf::Text::Bold);
		text.setString("X");
		text.setPosition((window_width / 2 - (window_width / 5) + (tileSize * 4.47)), 0 + (window_height * 0.216));
		window->draw(text);
	}

	// ----------------------------------
	//	SOUND EFFECTS CHECKBOX			|
	// ----------------------------------
	// draw the 'sound effects' text
	text.setCharacterSize(tileSize * 0.5); 
	text.setStyle(sf::Text::Regular);
	text.setString("Sound Effects:");
	text.setPosition((window_width / 2 - (window_width / 5) + (tileSize / 2)), 0 + (window_height * 0.225) + (tileSize * 1.1));
	window->draw(text);

	//draw the fullscreen mode checkbox
	checkbox.setPosition((window_width / 2 - (window_width / 5) + (tileSize * 4.5)), 0 + (window_height * 0.232) + (tileSize * 1.1));
	window->draw(checkbox);

	// mark the checkbox if fullscreen mode is active 
	if(true == sound_effects_on) {
		text.setCharacterSize(tileSize * 0.8); 
		text.setStyle(sf::Text::Bold);
		text.setString("X");
		text.setPosition((window_width / 2 - (window_width / 5) + (tileSize * 4.47)), 0 + (window_height * 0.216) + (tileSize * 1.1));
		window->draw(text);
	}

	// --------------------------
	//	MUSIC CHECKBOX			|
	// --------------------------
	// draw the 'sound effects' text
	text.setCharacterSize(tileSize * 0.5); 
	text.setStyle(sf::Text::Regular);
	text.setString("Music:");
	text.setPosition((window_width / 2 - (window_width / 5) + (tileSize / 2)), 0 + (window_height * 0.225) + (tileSize * 2.2));
	window->draw(text);

	//draw the fullscreen mode checkbox
	checkbox.setPosition((window_width / 2 - (window_width / 5) + (tileSize * 4.5)), 0 + (window_height * 0.232) + (tileSize * 2.2));
	window->draw(checkbox);

	// mark the checkbox if fullscreen mode is active 
	if(true == music_on) {
		text.setCharacterSize(tileSize * 0.8); 
		text.setStyle(sf::Text::Bold);
		text.setString("X");
		text.setPosition((window_width / 2 - (window_width / 5) + (tileSize * 4.47)), 0 + (window_height * 0.216) + (tileSize * 2.2));
		window->draw(text);
	}
}

// draws the inventory menu
void draw_inventory_menu(sf::RenderWindow* window) {

	//draw the backdrop
	sf::RectangleShape backdrop(sf::Vector2f(window_width / 2.5, window_height * 0.7));
	backdrop.setFillColor(sf::Color(200, 200, 200, 200));
	backdrop.setPosition((window_width / 2 - (window_width / 5)), 0 + (window_height * 0.1));
	window->draw(backdrop);

	// draw the potion icon
	sf::Sprite potion_icon;
	sf::Texture potion_icon_texture;
	if (!potion_icon_texture.loadFromFile("Resources/Sprites/Icons/icon_healing_potion.png")) {
		// error...
	}
	potion_icon.setScale(sf::Vector2f((1.5f * ((double)window_width / 1920)), (1.5f * ((double)window_width / 1920))));
	potion_icon_texture.setSmooth(true);
	potion_icon.setTexture(potion_icon_texture);
	potion_icon.setPosition((window_width / 3.15), (window_width / 14));
	window->draw(potion_icon);

	// draw the potion text
	sf::Text text;
	text.setFont(font); 
	text.setCharacterSize(tileSize * 0.7); 
	text.setStyle(sf::Text::Bold);
	text.setString("Healing Potion x " + to_string(static_cast<long long>(number_of_healing_potions)));
	text.setColor(sf::Color::Black);
	text.setPosition((window_width / 2) - (tileSize * 2.5), 0 + (window_height * 0.165));
	window->draw(text);

	// draw the 'use' button
	sf::RectangleShape button(sf::Vector2f(window_width / 4 * 0.25, tileSize * 1.5 * 0.5));
	button.setPosition(window_width / 2 - (tileSize * 4.55), window_height * (2/3) + (tileSize * 4));
	
	if(inventory_menu_selector == 0 || true == battle_active || number_of_healing_potions == 0 || (protag.getHealth() == protag.getMaxHealth())) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}
	
	window->draw(button);

	// draw the 'use' button text 
	text.setCharacterSize(tileSize * 0.5); 

	// text 1
	text.setString("Use");
	if(inventory_menu_selector == 0 || true == battle_active || number_of_healing_potions == 0|| (protag.getHealth() == protag.getMaxHealth())) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}
	text.setPosition(window_width / 2 - (tileSize * 4.23), window_height * (2/3) + (tileSize * 4.05));
	window->draw(text);
}

// draws a battle scenario
void draw_battle(sf::RenderWindow* window) {
	battle.draw(window);
}

// draws fade to black
void draw_fade_to_black(sf::RenderWindow* window) {

	// clear the window
	double add_me = 0.0;

	if(fade_to_black_count < 1.0) {
		add_me = (fade_to_black_count / 1.0) * 255;
	}
	else {
		add_me = 255 - (((fade_to_black_count - 1.0) / 1.0) * 255);
	}

	// darken the screen
	sf::RectangleShape darken(sf::Vector2f(window_width, window_height));
	darken.setFillColor(sf::Color(0, 0, 0, (int)add_me));
	darken.setPosition(0, 0);
	window->draw(darken);
	
	// draw the loading text
	if(add_me > 240 && fade_to_black_action == "start game") {
		sf::Text text;
		text.setFont(font); 
		text.setStyle(sf::Text::Bold);
		text.setCharacterSize(tileSize * 0.8); 
		text.setColor(sf::Color::White);
		text.setString("Loading...");
		text.setPosition((window_width / 2) - (tileSize * 1.5), window_height * 0.4);
		window->draw(text);
	}
}

// draws game over
void draw_game_over(sf::RenderWindow* window) {

	// darken the screen
	sf::RectangleShape darken(sf::Vector2f(window_width, window_height));
	darken.setFillColor(sf::Color(0, 0, 0, 100));
	darken.setPosition(0, 0);
	window->draw(darken);

	//draw the backdrop
	sf::RectangleShape backdrop(sf::Vector2f(window_width / 4, window_height * 0.25));
	backdrop.setFillColor(sf::Color(220, 220, 220));
	backdrop.setPosition((window_width / 2 - (window_width / 8)), ((window_height / 2) - (window_height * 0.2)));
	window->draw(backdrop);

	// Draw the options title text
	sf::Text text;
	text.setFont(font); 
	text.setCharacterSize(tileSize * 0.5); 
	text.setStyle(sf::Text::Bold);

	if(demo_won == true) {
		text.setString("Demo Complete!");
	}
	else {
		text.setString("Your journey has ended");
	}
	
	text.setColor(sf::Color::Black);

	if(demo_won == true) {
		text.setPosition((window_width / 2 - (window_width / 9.2) + (tileSize * 0.8)), ((window_height / 2) - (window_height * 0.19)));
	}
	else {
		text.setPosition((window_width / 2 - (window_width / 9.2)), ((window_height / 2) - (window_height * 0.19)));
	}

	window->draw(text);

	// draw the retry button
	sf::RectangleShape button(sf::Vector2f(window_width / 4 * 0.6, tileSize * 1.5 * 0.6));
	button.setPosition(window_width / 2 - ((window_width / 8) * 0.62), ((window_height / 2) - (window_height * 0.12)));
	
	if(game_over_menu_selector == 0) {
		button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		button.setFillColor(sf::Color(100, 100, 100));
	}
	
	window->draw(button);

	// draw the retry text
	if(demo_won == true) {
		text.setString("Play Again");
	}
	else {
		text.setString("Try Again");
	}
	if(game_over_menu_selector == 0) {
		text.setColor(sf::Color::White);
	}

	text.setPosition((window_width / 2) - (tileSize * 1.2), ((window_height / 2) - (window_height * 0.11)));
	window->draw(text);
	
	// draw the quit button
	sf::RectangleShape quit_button(sf::Vector2f(window_width / 4 * 0.6, tileSize * 1.5 * 0.6));
	quit_button.setPosition(window_width / 2 - ((window_width / 8) * 0.62), ((window_height / 2) - (window_height * 0.04)));
	
	if(game_over_menu_selector == 1) {
		quit_button.setFillColor(sf::Color(200, 200, 200));
	}
	else {
		quit_button.setFillColor(sf::Color(100, 100, 100));
	}
	
	window->draw(quit_button);

	// draw the quit text
	text.setString("Quit");
	if(game_over_menu_selector == 1) {
		text.setColor(sf::Color::White);
	}
	else {
		text.setColor(sf::Color::Black);
	}

	text.setPosition((window_width / 2) - (tileSize * 0.6), ((window_height / 2) - (window_height * 0.033)));
	window->draw(text);
}

// starts the game
void start_game() {

	// LOAD THE MAP
	FileReader reader = FileReader();
	the_tile_map = reader.GetMap("Resources/map_01.txt");

	// LOAD THE BACKGROUND TEXTURES ----------------------------------------------------------
	if (!background_01_texture.loadFromFile("Resources/background_01.png")) {
		// error...
	}
	background_01_texture.setSmooth(true);

	if (!background_01_texture.loadFromFile("Resources/background_01.png")) {
		// error...
	}
	background_01_texture.setSmooth(true);

	if (!background_02_texture.loadFromFile("Resources/background_02.png")) {
		// error...
	}
	background_02_texture.setSmooth(true);

	if (!background_03_texture.loadFromFile("Resources/background_03.png")) {
		// error...
	}
	background_03_texture.setSmooth(true);

	if (!background_04_texture.loadFromFile("Resources/background_04.png")) {
		// error...
	}
	background_04_texture.setSmooth(true);

	if (!background_05_texture.loadFromFile("Resources/background_05.png")) {
		// error...
	}
	background_05_texture.setSmooth(true);

	if (!background_06_texture.loadFromFile("Resources/background_06.png")) {
		// error...
	}
	background_06_texture.setSmooth(true);

	if (!background_07_texture.loadFromFile("Resources/background_07.png")) {
		// error...
	}
	background_07_texture.setSmooth(true);

	if (!background_08_texture.loadFromFile("Resources/background_08.png")) {
		// error...
	}
	background_08_texture.setSmooth(true);

	if (!background_09_texture.loadFromFile("Resources/background_09.png")) {
		// error...
	}
	background_09_texture.setSmooth(true);

	if (!background_10_texture.loadFromFile("Resources/background_10.png")) {
		// error...
	}
	background_10_texture.setSmooth(true);

	if (!background_11_texture.loadFromFile("Resources/background_11.png")) {
		// error...
	}
	background_11_texture.setSmooth(true);

	if (!background_12_texture.loadFromFile("Resources/background_12.png")) {
		// error...
	}
	background_12_texture.setSmooth(true);

	if (!background_13_texture.loadFromFile("Resources/background_13.png")) {
		// error...
	}
	background_13_texture.setSmooth(true);

	// LOAD THE SOUND EFFECTS -----------------------------------------------------
	if (!sound_run_1.loadFromFile("Resources/Sounds/run_01.wav")) {
		// error
	}
	if (!sound_run_2.loadFromFile("Resources/Sounds/run_02.wav")) {
		// error
	}
	if (!sound_run_3.loadFromFile("Resources/Sounds/run_03.wav")) {
		// error
	}
	if (!sound_run_4.loadFromFile("Resources/Sounds/run_04.wav")) {
		// error
	}
	if (!sound_run_5.loadFromFile("Resources/Sounds/run_05.wav")) {
		// error
	}
	if (!sound_potion.loadFromFile("Resources/Sounds/potion.wav")) {
		// error
	}
	if (!game_music.openFromFile("Resources/Sounds/music_game.wav")) {
		// error
	}
	if (!battle_music.openFromFile("Resources/Sounds/music_battle.wav")) {
		// error
	}
	game_music.setVolume(50);
	game_music.setLoop(true);
	battle_music.setVolume(50);
	battle_music.setLoop(true);

	// CREATE THE ENEMIES FOR THIS LEVEL ----------------------------------------------------------------------------------------------------------
	enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 122), (tileSize * 25), (2 * tileSize), (2 * tileSize), 1));
	enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 233), (tileSize * 34), (2 * tileSize), (2 * tileSize), 2));
	enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 264), (tileSize * 29), (2 * tileSize), (2 * tileSize), 1));
	enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 598), (tileSize * 31), (2 * tileSize), (2 * tileSize), 3));

	// first group of columns
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 287), (tileSize * 24), (3 * tileSize), (16 * tileSize), 1));
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 311), (tileSize * 24), (3 * tileSize), (16 * tileSize), 1));
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 326), (tileSize * 24), (3 * tileSize), (16 * tileSize), 1));
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 353), (tileSize * 24), (3 * tileSize), (16 * tileSize), 1));

	// second group of columns
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 406), (tileSize * 22), (3 * tileSize), (21 * tileSize), 1));
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 447), (tileSize * 22), (3 * tileSize), (21 * tileSize), 1));
	lightning_columns.push_back(* new Obstacle(tileSize, "Lightning", (tileSize * 485), (tileSize * 28), (3 * tileSize), (15 * tileSize), 1));

	main_menu = false;
	music_menu.stop();

	if(true == music_on) {
		game_music.play();
	}
}

// update the platforming section of the game
void update(double time) {

	protag.update(time);

	if(last_checkpoint < 4) {

		int tile_x = protag.get_x() / tileSize;
		int tile_y = protag.get_y() / tileSize;

		if(last_checkpoint < 1 && (tile_x > 138) && (tile_y > 25)) {
			last_checkpoint = 1;
			potions_at_last_checkpoint = number_of_healing_potions;
			health_at_last_checkpoint = protag.getHealth();
		}

		if(last_checkpoint < 2 && (tile_x > 274) && (tile_y > 29)) {
			last_checkpoint = 2;
			potions_at_last_checkpoint = number_of_healing_potions;
			health_at_last_checkpoint = protag.getHealth();
		}

		if(last_checkpoint < 3 && (tile_x > 376)) {
			last_checkpoint = 3;
			potions_at_last_checkpoint = number_of_healing_potions;
			health_at_last_checkpoint = protag.getHealth();
		}

		if(last_checkpoint < 4 && (tile_x > 523) && (tile_y > 34)) {
			last_checkpoint = 3;
			potions_at_last_checkpoint = number_of_healing_potions;
			health_at_last_checkpoint = protag.getHealth();
		}
	}

	if(protag.getIsDead() == true) {
		game_over = true;
	}
	else {

		int protag_y  = protag.get_y();
		int protag_x  = protag.get_x();

		// CHECK ENEMY COLLISIONS
		for(int i = 0; i < enemies.size(); i++) {

			int enemy_x = enemies[i].get_x();
			int enemy_y = enemies[i].get_y();
			int enemy_width = enemies[i].get_width();
			int enemy_height = enemies[i].get_height() + (tileSize);

			if(protag_x < (enemy_x + enemy_width) &&
			   protag_x + tileSize > enemy_x  &&
			   protag_y > (enemy_y - enemy_height) &&
			   protag_y - (tileSize * 2) < enemy_y) {

				   //----------------------------
				   // initiate a battle			|
				   //----------------------------
				   // reduce velocity to 0 
				   protag.set_velocity_0();
				   enemy_being_fought = i;

				   if(enemies[i].get_id() == 2) {
					   type_of_enemy = "double";
				   }
				   else if(enemies[i].get_id() == 3) {
					   type_of_enemy = "boss";
				   }
				   else {
					   type_of_enemy = "";
				   }

				   fade_to_black = true;
				   fade_to_black_action = "battle";

				   if(true == music_on) {
					   game_music.pause();
					   battle_music.play();
				   }
			}
		}

		bool lightning_collision = false;

		// CHECK Lightning COLLISIONS
		for(int i = 0; i < lightning_columns.size(); i++) {

			int enemy_x = lightning_columns[i].get_x();
			int enemy_y = lightning_columns[i].get_y();
			int enemy_width = lightning_columns[i].get_width();
			int enemy_height = lightning_columns[i].get_height();

			if(protag_x < (enemy_x + enemy_width) &&
			   protag_x + tileSize > enemy_x  &&
			   protag_y < (enemy_y + enemy_height) &&
			   protag_y + (tileSize * 2) > enemy_y) {

				//--------------------------------------------
				// protag is damaged and knocked back		|
				//--------------------------------------------
				if(true == lightning_columns[i].get_active()) {
					protag.lightning_collision();
					lightning_collision = true;
				}
			}
		}

		if(false == lightning_collision) {
			protag.set_lightning_false();
		}
	}
}

// update  battle
void update_battle(double time) {

	if(battle.get_game_over() == true) {
		game_over = true;
	}
	else {
		if(battle.get_battle_finished() == true) {
			fade_to_black = true;
			fade_to_black_action = "battle finished";
			battle_music.stop();

			if(battle.get_type() == "boss") {
				demo_won = true;
			}
			else {
				if(true == music_on) {
					game_music.play();
				}
			}
		}
		else {
			battle.update();
		}
	}
}

// update the options menu
void update_options_menu(double time) {
	// do nothing
}

// the game thread
void renderingThread(sf::RenderWindow* window) {

	int time = 0;
	DWORD last_update_finished = 0;

    // the rendering loop
    while (window->isOpen()) {

		DWORD next_game_tick = GetTickCount();

		// calculate the time since the last update and update the game objects
		time = GetTickCount() - last_update_finished;

		// TIME TRACKER ----------------------------------
		time_tracker += time;
		
		if(time_tracker >= 16.67) {

			time_tracker = 0 + (time_tracker - 16.67);

			if(true == fade_to_black) {

				fade_to_black_count += 0.016;

				if(fade_to_black_count > 1.0 && fade_to_black_changed == false) {

					fade_to_black_changed = true;

					if(fade_to_black_action == "start game") {
						start_game();
					}
					else if(fade_to_black_action == "battle") {

						if(type_of_enemy == "double") {
							battle = * new Battle(tileSize, window_width, window_height, ((double)protag.getHealth() / protag.getMaxHealth()), number_of_healing_potions, "", "Morgan", "", "", "Enemy 1", "double", sound_effects_on);
						}
						else if(type_of_enemy == "boss") {
							battle = * new Battle(tileSize, window_width, window_height, ((double)protag.getHealth() / protag.getMaxHealth()), number_of_healing_potions, "", "Morgan", "", "", "Enemy 1", "boss", sound_effects_on);
						}
						else {
							battle = * new Battle(tileSize, window_width, window_height, ((double)protag.getHealth() / protag.getMaxHealth()), number_of_healing_potions, "", "Morgan", "", "", "Enemy 1", "single", sound_effects_on);
						}

						battle_active = true;
					}
					else if(fade_to_black_action == "battle finished") {
						number_of_healing_potions = (battle.get_number_of_healing_potions() + 1);
						protag.setHealth(((double)battle.get_morgans_health() / 920) * 1000);
						battle_active = false;
						enemies.erase(enemies.begin() + enemy_being_fought);
					}
					else if(fade_to_black_action == "respawn") {

						if(battle_active) {
							battle_active = false;
						}
						if(demo_won) {
							demo_won = false;
						}

						protag.respawn(last_checkpoint, health_at_last_checkpoint);
						number_of_healing_potions = potions_at_last_checkpoint;
						game_over = false;
					}
				}
				else if(fade_to_black_count > 2.0) {
					fade_to_black = false;
					fade_to_black_changed = false;
					fade_to_black_count = 0;
					fade_to_black_action = "";
				}
			}

			window->clear(sf::Color::White);

			if(true == game_over) {
				
				draw_backdrop(window);

				if(true == battle_active) {
					draw_battle(window);
				}
				else {
					draw_overworld(window);
				}

				draw_game_over(window);

				if(true == fade_to_black) {
					draw_fade_to_black(window);
				}

				window->display();
			}
			else if(true == main_menu) {
				// do nothing
				draw_main_menu(window);

				if(true == options_menu) {
					draw_options_menu(window);
				}

				if(true == fade_to_black) {
					draw_fade_to_black(window);
				}

				window->display();
			}
			// if a battle is active draw the battle
			else if(true == battle_active) {

				if(false == pause_menu) {
					update_battle(0.016);
				}
				
				draw_backdrop(window);
				draw_battle(window);

				if(true == pause_menu) {
					// do nothing
					draw_pause_menu(window);

					if(true == options_menu) {
						draw_options_menu(window);
					}
					else if(true == inventory_menu) {
						draw_inventory_menu(window);
					}
				}

				if(true == fade_to_black) {
					draw_fade_to_black(window);
				}

				window->display();
			}
			// otherwise draw the overworld
			else {

				draw_backdrop(window);

				if(false == pause_menu) {

					// WATERFALLS
					if(true == lightning_columns[0].get_active() && waterfalls_active == false) {
						waterfalls_active = true;
						waterfall_animation_frame = 0;
						waterfall_animation_count = 0;
					}
					if(true == waterfalls_active) {
						waterfall_duration_count += 0.016;
					}
					if(waterfall_duration_count > 0.3 && waterfall_start == true) {
						waterfall_start = false;
						waterfall_animation_cap = 6;
					}
					if(waterfall_duration_count > 1.4 && waterfall_end == false) {
						waterfall_end = true;
						waterfall_animation_cap = 4;
						waterfall_animation_frame = 1;
						waterfall_animation_count = 0;
					}
					if(waterfall_duration_count > 1.9 && waterfall_end == true) {
						waterfall_end = false;
						waterfall_start = true;
						waterfall_duration_count = 0;
						waterfalls_active = false;

						if (!waterfall_texture.loadFromFile("Resources/Sprites/beam_blank.png")) {
							// error...
						}
					}

					// update the lightning columns
					for(int i = 0; i < lightning_columns.size(); i++) {
						lightning_columns[i].update();
					}

					if(false == pause_menu) {
						update(0.016);
					}
				}

				draw_overworld(window);

				if(true == pause_menu) {
					// do nothing
					draw_pause_menu(window);

					if(true == options_menu) {
						draw_options_menu(window);
					}
					else if(true == inventory_menu) {
						draw_inventory_menu(window);
					}
				}

				if(true == fade_to_black) {
					draw_fade_to_black(window);
				}
				
				window->display();
			}

			// ANIMATION
			if(false == pause_menu && false == main_menu && false == game_over) {

				// Waterfalls ------------------------------------------------------------------------------------------
				if(true == waterfalls_active) {
					waterfall_animation_count += 0.03;
				}

				if(waterfall_animation_count > 0.25) {

					waterfall_animation_frame += 1;
					waterfall_animation_count = 0;

					if(waterfall_animation_frame > waterfall_animation_cap) {
						waterfall_animation_frame = 1;
					}
				
					string filepath = "";

					// change the texture
					if(true == waterfall_start) {

						filepath = ("Resources/Sprites/beam_start_0" + to_string(static_cast<long long>(waterfall_animation_frame)) + ".png");
					}
					else if(true == waterfall_end) {

						filepath = ("Resources/Sprites/beam_end_0" + to_string(static_cast<long long>(waterfall_animation_frame)) + ".png");
					}
					else {

						filepath = ("Resources/Sprites/beam_run_0" + to_string(static_cast<long long>(waterfall_animation_frame)) + ".png");
					}

					// load the texture
					if (!waterfall_texture.loadFromFile(filepath)) {
						// error...
					}
				}

				// Enemies ---------------------------------------------------------------------------------------------
				enemy_animation_count += 0.02;

				if(enemy_animation_count > 0.3) {

					enemy_sprite_change = true;

					// reset the animation
					enemy_animation_count = 0 + (enemy_animation_count - 0.3);

					enemy_animation_frame++;

					if(enemy_animation_frame > enemy_animation_cap) {
						enemy_animation_frame = 1;
					}
				}

				//Morgan
				if(true == protag.get_is_jumping()) {
					morgan_animation_count += 0.04;
				}
				else if(true == protag.get_is_falling() || true == protag.get_recovery()) {
					morgan_animation_count += 0.02;
				}
				else if(protag.get_horizontal_velocity() == 0) {

					morgan_animation_count += 0.016;
				}
				else {

					if(protag.get_horizontal_velocity() > 0) {
						morgan_animation_count += (0.01 + (protag.get_horizontal_velocity() / tileSize / 4));
					}
					else {
						morgan_animation_count += (0.01 + (-protag.get_horizontal_velocity() / tileSize / 4));
					}
				}

				if(morgan_animation_count > 0.3) {

					morgan_sprite_change = true;

					// reset the animation
					morgan_animation_count = 0 + (morgan_animation_count - 0.3);

					morgan_animation_frame++;
					if(morgan_animation_frame > morgan_animation_cap) {
						morgan_animation_frame = 1;
					}
				}
			}
		}
		// -----------------------------------------------

		last_update_finished = GetTickCount();
		next_game_tick += SKIP_TICKS;
    }
}

// main method
int main() {

	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	sf::VideoMode mode2 = sf::VideoMode((mode.width / 2), (mode.height / 2));

	window_height = mode2.height;
	window_width = mode2.width;

	// SET THE TILE SIZE
	tileSize = (mode.width/number_of_tiles_per_width/2);
	protag_offset_y = ((window_height / 2) % tileSize);
	protag_offset_x = ((window_width / 2) % tileSize);

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(mode2, "Birthright", sf::Style::Close);

	protag = Protag(((window_width * 2)/number_of_tiles_per_width/2), the_tile_map);

	// set the window width and height for the protag
	protag.set_screen_parameters(window_width, window_height, protag_offset_x, protag_offset_y, &window);

	font.loadFromFile("arial.ttf");

	// load the icon
	if (!icon.loadFromFile("Resources/Sprites/birthright_icon.png")) {
		// error
	}

	window.setVerticalSyncEnabled(true);
	window.setIcon(32, 32, icon.getPixelsPtr());

	// LOAD THE BACKDROP TEXTURE
	if (!backdrop_texture.loadFromFile("Resources/backdrop.png")) {
		// error...
	}
	backdrop_texture.setSmooth(true);

	// LOAD THE INITIAL SOUND EFFECTS
	if (!sound_button.loadFromFile("Resources/Sounds/button.wav")) {
		// error
	}
	if (!sound_back.loadFromFile("Resources/Sounds/back.wav")) {
		// error
	}

	// LOAD THE INITIAL MUSIC
	if (!music_menu.openFromFile("Resources/Sounds/menu_music.wav")) {
		// error
	}
	music_menu.setVolume(50);
	music_menu.setLoop(true);

    // deactivate its OpenGL context
    window.setActive(false);

	// LOAD THE ICON TEXTURE
	if (!morgan_icon_texture.loadFromFile("Resources/Sprites/Morgan/morgan_icon_large.png")) {
		// error...
	}
	morgan_icon_texture.setSmooth(true);

    // launch the rendering thread
    sf::Thread thread(&renderingThread, &window);
    thread.launch();

	// START THE MUSIC
	if(true == music_on) {
		music_menu.play();
	}

    // the logic loop
    while (window.isOpen()) {

		if((false == fade_to_black) && (true == demo_won)) {
			game_over = true;
		}

		if(true == game_over) {
			sf::Vector2i position = sf::Mouse::getPosition(window);

			// check button
			if(position.x < (window_width / 2 - ((window_width / 8) * 0.6) + ((window_width / 4) * 0.6)) &&
			   position.x > (window_width / 2 - (window_width / 8) * 0.6))  {
				   
				   // button 1:
				   if (position.y < (((window_height / 2) - (window_height * 0.12)) + (tileSize * 1.5 * 0.6)) &&
				   position.y > ((window_height / 2) - (window_height * 0.12))) {
					   game_over_menu_selector = 0;
				   }
				   else if (position.y < (((window_height / 2) - (window_height * 0.04)) + (tileSize * 1.5 * 0.6)) &&
				   position.y > ((window_height / 2) - (window_height * 0.04))) {
					   game_over_menu_selector = 1;
				   }
				   else {
						game_over_menu_selector = 2;
				   }
			}
		}
		// inventory menu mouse hover check
		else if(true == inventory_menu) {

			sf::Vector2i position = sf::Mouse::getPosition(window);

			if(position.x < ( window_width / 2 - (tileSize * 4.55) + ((window_width / 4) * 0.25)) &&
				position.x > window_width / 2 - (tileSize * 4.55))  {

				if (position.y < (window_height * (2/3) + (tileSize * 4) + (tileSize * 0.75)) && 
				position.y > (window_height * (2/3) + (tileSize * 4))) {
					inventory_menu_selector = 0;
				}
				else {
					inventory_menu_selector = 1;
				}
			}
			else {
				inventory_menu_selector = 1;
			}
		}
		// main menu mouse hover check
		else if(true == main_menu && false == options_menu && false == inventory_menu) {
			sf::Vector2i position = sf::Mouse::getPosition(window);

			// check button
			if(position.x < (window_width / 2 - ((window_width / 8) * 0.6) + ((window_width / 4) * 0.6)) &&
			   position.x > (window_width / 2 - (window_width / 8) * 0.6))  {
				   
				   // button 1:
				   if (position.y < ((window_height * (2/3) + (tileSize * 0.6 *2) + (tileSize/1.3)) + (tileSize * 1.5 * 0.6) + (tileSize * 2)) &&
				   position.y > (window_height * (2/3) + (tileSize * 0.6 * 2) + (tileSize * 2) + (tileSize/1.3))) {
					   main_menu_selector = 0;
				   }

				   // button 2:
				   else if (position.y < ((window_height * (2/3) + (tileSize * 0.6 * 4) + (tileSize/1.3)) + (tileSize * 1.5 * 0.6) + (tileSize * 2)) &&
				   position.y > (window_height * (2/3) + (tileSize * 0.6 * 4) + (tileSize * 2) + (tileSize/1.3))) {
					   main_menu_selector = 1;
				   }

				   // button 3:
				   else if (position.y < ((window_height * (2/3) + (tileSize * 0.6 * 6) + (tileSize/1.3)) + (tileSize * 1.5 * 0.6) + (tileSize * 2)) &&
				   position.y > (window_height * (2/3) + (tileSize * 0.6 * 6) + (tileSize * 2) + (tileSize/1.3))) {
					   main_menu_selector = 2;
				   }
				   else {
						main_menu_selector = 3;
				   }
			}
			else {
				main_menu_selector = 3;
			}
		}
		// main menu mouse hover check
		else if(true == pause_menu && false == options_menu && false == inventory_menu) {
			sf::Vector2i position = sf::Mouse::getPosition(window);

			// check button
			if(position.x < (window_width / 2 - ((window_width / 8) * 0.6) + ((window_width / 4) * 0.6)) &&
			   position.x > (window_width / 2 - (window_width / 8) * 0.6))  {
				   
				   // button 1:
				   if (position.y < ((window_height * (2/3) + (tileSize * 0.6 *2) + (tileSize/1.3)) + (tileSize * 1.5 * 0.6)) &&
				   position.y > (window_height * (2/3) + (tileSize * 0.6 * 2) + (tileSize/1.3))) {
					   pause_menu_selector = 0;
				   }

				   // button 2:
				   else if (position.y < ((window_height * (2/3) + (tileSize * 0.6 * 4) + (tileSize/1.3)) + (tileSize * 1.5 * 0.6)) &&
				   position.y > (window_height * (2/3) + (tileSize * 0.6 * 4) + (tileSize/1.3))) {
					   pause_menu_selector = 1;
				   }

				   // button 3:
				   else if (position.y < ((window_height * (2/3) + (tileSize * 0.6 * 6) + (tileSize/1.3)) + (tileSize * 1.5 * 0.6)) &&
				   position.y > (window_height * (2/3) + (tileSize * 0.6 * 6) + (tileSize/1.3))) {
					   pause_menu_selector = 2;
				   }
				   else {
						pause_menu_selector = 3;
				   }
			}
			else {
				pause_menu_selector = 3;
			}
		}

		sf::Event event;

		// while there are pending events...
		while (window.pollEvent(event)) {
			// check the type of the event...
			switch (event.type) {

				// window closed
				case sf::Event::Closed:
					window.close();
					break;

				// key pressed
				case sf::Event::KeyPressed:

					switch (event.key.code) {

						case sf::Keyboard::W:

							if(true == battle_active) {
								battle.register_input('w');
							}
							break;

						case sf::Keyboard::S:

							if(true == battle_active) {
								battle.register_input('s');
							}
							break;

						case sf::Keyboard::Return:

							if(true == battle_active) {
								battle.register_input('1');
							}
							break;

						case sf::Keyboard::BackSpace:

							if(true == battle_active) {
								battle.register_input('2');
							}
							break;

						case sf::Keyboard::Escape:
							
							if(true == options_menu) {
								options_menu = false;
								play_sound_effect("back");
							}
							else if(true == inventory_menu) {
								inventory_menu = false;
								play_sound_effect("back");
							}
							else if(false == main_menu && false == pause_menu) {
								pause_menu = true;
								play_sound_effect("back");
							}
							else if(true == pause_menu) {
								pause_menu = false;
								play_sound_effect("back");
							}

							break;
					}

					break;

				case sf::Event::MouseButtonPressed:

					if (event.mouseButton.button == sf::Mouse::Left) {

						if(true == fade_to_black) {
							// do nothing
						}
						else if(true == game_over) {

							// try again
							if(game_over_menu_selector == 0) {
								fade_to_black = true;
								fade_to_black_action = "respawn";
								play_sound_effect("button");
								battle_music.stop();
								game_music.stop();
								game_music.play();

								// if the player has won and is playing again, reset the game
								if(demo_won == true) {

									enemies.clear();
									enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 122), (tileSize * 25), (2 * tileSize), (2 * tileSize), 1));
									enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 233), (tileSize * 34), (2 * tileSize), (2 * tileSize), 2));
									enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 264), (tileSize * 29), (2 * tileSize), (2 * tileSize), 1));
									enemies.push_back(* new Obstacle(tileSize, "Enemy", (tileSize * 598), (tileSize * 31), (2 * tileSize), (2 * tileSize), 3));

									// respawn variables
									last_checkpoint = 0;
									potions_at_last_checkpoint = 3;
									health_at_last_checkpoint = 1000;
								}
							}
							// quit
							else if(game_over_menu_selector == 1) {
								thread.terminate();
								window.close();
							}
						}
						else if(true == options_menu) {

							sf::Vector2i position = sf::Mouse::getPosition(window);

							if(position.x < ((window_width / 2 - (window_width / 5) + (tileSize * 4.5)) + (tileSize / 2)) &&
							position.x > (window_width / 2 - (window_width / 5) + (tileSize * 4.5)))  {

								if (position.y < ((0 + (window_height * 0.232)) + (tileSize / 2)) && 
									position.y > (0 + (window_height * 0.232))) {
								   
									if(true == is_fullscreen) {

										// FULLSCREEN -> WINDOWED
										window.create(mode2, "Birthright", sf::Style::Close);
										is_fullscreen = false;
										window.setVerticalSyncEnabled(true);

										window_width = mode2.width;
										window_height = mode2.height;
										tileSize = (mode2.width / number_of_tiles_per_width);
										protag_offset_y = ((window_height / 2) % tileSize);
										protag_offset_x = ((window_width / 2) % tileSize);
										protag.set_need_to_update_tile_size(mode2.width / number_of_tiles_per_width);
										battle.set_need_to_update_tile_size(mode2.width / number_of_tiles_per_width);

										// update the enemies
										for(int i = 0; i < enemies.size(); i++) {
											enemies[i].update_tile_size(mode2.width / number_of_tiles_per_width);
										}

										// update the lightning columns
										for(int i = 0; i < lightning_columns.size(); i++) {
											lightning_columns[i].update_tile_size(mode2.width / number_of_tiles_per_width);
										}

										// set the window width and height for the protag
										protag.set_screen_parameters(mode2.width, mode2.height, protag_offset_x, protag_offset_y, &window);
										battle.set_screen_parameters(mode2.width, mode2.height, &window);

										play_sound_effect("button");

										// deactivate its OpenGL context
										window.setActive(false);
										thread.launch();
									}
									else {
										// WINDOWED -> FULLSCREEN
										window.create(mode, "Birthright", sf::Style::Fullscreen);
										is_fullscreen = true;
										window.setVerticalSyncEnabled(true);

										window_width = mode.width;
										window_height = mode.height;
										tileSize = (mode.width / number_of_tiles_per_width);
										protag_offset_y = ((window_height / 2) % tileSize);
										protag.set_need_to_update_tile_size(mode.width / number_of_tiles_per_width);
										battle.set_need_to_update_tile_size(mode.width / number_of_tiles_per_width);

										// update the enemies
										for(int i = 0; i < enemies.size(); i++) {
											enemies[i].update_tile_size(mode.width / number_of_tiles_per_width);
										}

										// update the lightning columns
										for(int i = 0; i < lightning_columns.size(); i++) {
											lightning_columns[i].update_tile_size(mode.width / number_of_tiles_per_width);
										}

										// set the window width and height for the protag
										protag.set_screen_parameters(mode.width, mode.height, protag_offset_x, protag_offset_y, &window);
										battle.set_screen_parameters(mode.width, mode.height, &window);

										play_sound_effect("button");

										// deactivate its OpenGL context
										window.setActive(false);
										thread.launch();
									}
								}
								else if (position.y < ((0 + (window_height * 0.232) + (tileSize * 1.1)) + (tileSize / 2)) && 
									position.y > (0 + (window_height * 0.232) + (tileSize * 1.1))) {

									play_sound_effect("button");

									if(sound_effects_on == true) {
										sound_effects_on = false;
										protag.set_sound_effects(false);
										battle.set_sound_effects(false);
									}
									else {
										sound_effects_on = true;
										protag.set_sound_effects(true);
										battle.set_sound_effects(true);
									}
								}
								else if (position.y < ((0 + (window_height * 0.232) + (tileSize * 2.2)) + (tileSize / 2)) && 
									position.y > (0 + (window_height * 0.232) + (tileSize * 2.2))) {

									play_sound_effect("button");

									if(music_on == true) {
										music_on = false;
										music_menu.stop();
										battle_music.stop();
										game_music.stop();
									}
									else {
										music_on = true;

										// resume the music
										if(true == main_menu) {
											music_menu.play();
										}
										else if(true == battle_active) {
											battle_music.play();
										}
										else {
											game_music.play();
										}
									}
								}
							}
						}
						else if(true == inventory_menu) {

							// use a potion
							if(inventory_menu_selector == 0) {
								
								if(false == battle_active && number_of_healing_potions > 0 && (protag.getHealth() < protag.getMaxHealth())) {

									int new_health = 0;
									new_health = protag.getHealth() + 326;

									if(new_health > protag.getMaxHealth()) {
										new_health = protag.getMaxHealth();
									}

									protag.setHealth(new_health);
									number_of_healing_potions -= 1;
									play_sound_effect("potion");
								}
							}
						}
						else if(true == main_menu) {
							switch(main_menu_selector) {

								case 0:
									fade_to_black = true;
									play_sound_effect("button");
									fade_to_black_action = "start game";
									break;
								case 1:
									options_menu = true;
									play_sound_effect("button");
									break;
								case 2:
									thread.terminate();
									window.close();
									break;
								default: 
									break;
							}
						}
						else if(true == pause_menu) {

							switch(pause_menu_selector) {

								case 0:
									inventory_menu = true;
									play_sound_effect("button");
									break;
								case 1:
									options_menu = true;
									play_sound_effect("button");
									break;
								case 2:
									thread.terminate();
									window.close();
									break;
								default: 
									break;
							}
						}
						else if(false == battle_active) {
							protag.set_need_to_fire_grapple();
							play_sound_effect("fire grapple");
						}
					}

					break;

				// resize event
				case sf::Event::Resized:
					break;

				default:
					break;
			}
		}
    }

    return 0;
}