// Game.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Grapple.h"
#include "Protag.h"
#include "FileReader.h"
#include <stdio.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <iomanip>
#include <random>
using namespace std;

int **grapple_tile_map;
int gravity_acceleration = 0;
double maximum_grapple_length = 0;
bool atemp = false;

sf::Sound impact_sound;
sf::SoundBuffer sound_impact_01;
sf::SoundBuffer sound_impact_02;
sf::SoundBuffer sound_impact_03;
sf::SoundBuffer sound_impact_04;
sf::SoundBuffer sound_null_impact;

// play a sound effect
void Grapple::grapple_play_sound_effect(string sound_type) {

	if(sound_effects_on == true) {

		if(sound_type == "impact") {

			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1,4); 
			float rand = dist6(rng);

			impact_sound.setVolume(60);

			if(rand == 1) {
				impact_sound.setBuffer(sound_impact_01);
				impact_sound.play();
			}
			else if(rand == 2) {
				impact_sound.setBuffer(sound_impact_02);
				impact_sound.play();
			}
			else if(rand == 3) {
				impact_sound.setBuffer(sound_impact_03);
				impact_sound.play();
			}
			else {
				impact_sound.setBuffer(sound_impact_04);
				impact_sound.play();
			}
		}
		else if(sound_type == "null impact") {
			impact_sound.setVolume(40);
			impact_sound.setBuffer(sound_null_impact);
			impact_sound.play();
		}
	}
}

// constructor
Grapple::Grapple(int the_tileSize) {
    
	x = 0;
	y = 0;
	vertical_velocity = 0;
	horizontal_velocity = 0;

	gravity_acceleration = the_tileSize * (8.81 / 36);

	tileSize = the_tileSize;
	new_tile_size = the_tileSize;
	maximum_grapple_length = (tileSize * 8.0); 

	is_active = false;
	is_hooked = false;
	sound_effects_on = true;

	FileReader reader = FileReader();
	grapple_tile_map = reader.GetMap("map_01.txt");
}

// initialization/re-initialization
void Grapple::initialize(int new_x, int new_y, double new_horizontal_velocity, double new_vertical_velocity) {
    
	x = new_x;
	y = new_y;
	vertical_velocity = new_vertical_velocity;
	horizontal_velocity = new_horizontal_velocity;

	is_active = true;
}

// turn sound effects on or off
void Grapple::set_sound_effects(bool onoff) {
	sound_effects_on = onoff;
}

// time = time in seconds
void Grapple::update(double time, int pro_cen_x, int pro_cen_y) {
		
	if(atemp == false) {
		if (!sound_impact_01.loadFromFile("Resources/Sounds/impact_01.wav")) {
			// error
		}
		if (!sound_impact_02.loadFromFile("Resources/Sounds/impact_02.wav")) {
				// error
		}
		if (!sound_impact_03.loadFromFile("Resources/Sounds/impact_03.wav")) {
			// error
		}
		if (!sound_impact_04.loadFromFile("Resources/Sounds/impact_04.wav")) {
			// error
		}
		if (!sound_null_impact.loadFromFile("Resources/Sounds/grapple_release.wav")) {
			// error
		}

		atemp = true;
	}

	if(true == is_hooked) {

	}
	else {
		// get the current x cell and y cell
		int current_cell_x = (int)((x / tileSize) + 1);
		int current_cell_y = (int)(y / tileSize);

		/**
		 * -------------------------------------------------
		 * Vertical velocity and collision calculations:   |
		 * -------------------------------------------------
		 */

		vertical_velocity = vertical_velocity - ((gravity_acceleration * time) * 0.75);
		
		// calculates the would-be new y coordinate
		int new_y = (int) (y - vertical_velocity);

		// declares a variable new cell x
		int new_cell_y = 0;

		// calculates the new x-grid cell from the new x coordinate
		new_cell_y = (int)((new_y / tileSize) - 1);

		// if the new cell is solid
		if (grapple_tile_map[new_cell_y][current_cell_x] == 1) {
	
			// if the new cell is downwards...
			if((new_cell_y + 1) > current_cell_y) {
				y = ((new_cell_y + 1) * tileSize);
				attatch_point = 't';
			}
			//otherwise the new cell is upwards...
			else {
				y = ((current_cell_y) * tileSize);
				attatch_point = 'b';
			}

			// stop the grapples movement
			vertical_velocity = 0;
			horizontal_velocity = 0;
			hook();

			grapple_play_sound_effect("impact");
		}
		// if the new cell is solid but ungrappleable
		else if (grapple_tile_map[new_cell_y][current_cell_x] == 2) {

			// stop the grapples movement
			vertical_velocity = 0;
			horizontal_velocity = 0;

			is_active = false;
			is_hooked = false;

			need_to_deactivate_grapple = true;
			grapple_play_sound_effect("null impact");
		}
		// if the new cell isn't solid make it the current cell and update x
		else {
			current_cell_y = new_cell_y;
			y = new_y;

			/**
			 * -----------------------------------
			 * Horizontal velocity calculations: |
			 * -----------------------------------
			 */

			// calculate the new horizontal velocity
			if(horizontal_velocity > 0) {
				horizontal_velocity = horizontal_velocity - (horizontal_velocity / 10000);
			}
			else if(horizontal_velocity < 0) {
				horizontal_velocity = horizontal_velocity + (horizontal_velocity / 10000);
			}

			// calculates the new x coordinate based on the current horizontal velocity
			int new_x = (int) (x + horizontal_velocity);
		
			// calculates the new x-grid cell from the new x coordinate
			int new_cell_x = (int)((new_x / tileSize) + 1);

			// if the new cell is solid
			if (grapple_tile_map[new_cell_y][new_cell_x] == 1) {

				// stop the grapples movement
				horizontal_velocity = 0;
				vertical_velocity = 0;
				hook();
				grapple_play_sound_effect("impact");

				// if the new cell is forwards...
				if(new_cell_x > current_cell_x) {
					x = (current_cell_x * tileSize);
					attatch_point = 'l';
				}
				//otherwise the new cell is backwards...
				else {
					x = (new_cell_x * tileSize);
					attatch_point = 'r';
				}
			}
			// if the new cell is solid but ungrappleable
			else if (grapple_tile_map[new_cell_y][current_cell_x] == 2) {

				// stop the grapples movement
				vertical_velocity = 0;
				horizontal_velocity = 0;

				is_active = false;
				is_hooked = false;

				need_to_deactivate_grapple = true;
				grapple_play_sound_effect("null impact");
			}
			// if the new cell isn't solid make it the current cell and update x
			else {
				current_cell_x = new_cell_x;
				x = new_x;
			}
		}
	}

	/**
	 * -----------------------------------
	 *	Maximum Length Checker:			 |
	 * -----------------------------------
	 */

	int x_difference = (x - pro_cen_x);
	int y_difference = (y - pro_cen_y);

	int vector_length = (sqrt(((double)x_difference * (double)x_difference) + ((double)y_difference * (double)y_difference)));

	// if the grapple length is greater than the maximum length 'snap' it
	if(vector_length > maximum_grapple_length) {

		x = 0;
		y = 0;
		vertical_velocity = 0;
		horizontal_velocity = 0;

		is_active = false;
		is_hooked = false;

		need_to_deactivate_grapple = true;
	}
}

// updates the tile size when the window changes size
void Grapple::update_tile_size(int the_new_tile_size, int win_height, int win_width) {

	// scales up/down the x and y co-ordinates
	x = x * (the_new_tile_size / tileSize);
	y = y * (the_new_tile_size / tileSize);

	horizontal_velocity = horizontal_velocity		* (the_new_tile_size / tileSize);
	vertical_velocity = vertical_velocity			* (the_new_tile_size / tileSize);
	gravity_acceleration = gravity_acceleration		* (the_new_tile_size / tileSize);
	maximum_grapple_length = maximum_grapple_length * (the_new_tile_size / tileSize);

	tileSize = the_new_tile_size;
}

// return is_active
bool Grapple::check_is_active() {
	return is_active;
}

double Grapple::get_x() {
	return x;
}

double Grapple::get_y() {
	return y;
}

bool Grapple::get_is_hooked() {
	return is_hooked;
}

void Grapple::deactivate() {
	x = 0;
	y = 0;
	vertical_velocity = 0;
	horizontal_velocity = 0;
	is_hooked = false;
	is_active = false;
}

void Grapple::hook() {
	is_hooked = true;
}

char Grapple::get_attach_point() {
	return attatch_point;
}

void Grapple::set_attach_point(char new_attatch_point) {
	attatch_point = new_attatch_point;
}

void Grapple::set_max_grapple_length(double new_max_length) {
	maximum_grapple_length = new_max_length;
}

double Grapple::get_max_grapple_length() {
	return maximum_grapple_length;
}

void Grapple::set_coords(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}

void Grapple::set_need_to_deactivate_grapple() {
	need_to_deactivate_grapple = false;
}

bool Grapple::get_need_to_deactivate_grapple() {
	return need_to_deactivate_grapple;
}
