// Game.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Protag.h"
#include "Grapple.h"
#include "FileReader.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <SFML/Audio.hpp>
#include <iomanip>
#include <random>
using namespace std;

int **tile_map;

Grapple grapple;

double pi = 3.14159265358979323846;
double grap_fall_length = 0;
double damage_velocity = 0.0;

int correcting_count = 0;

bool should_fall_was = false;

sf::Sound fire_grapple_sound;
sf::SoundBuffer sound_fire_grapple;

sf::Sound release_grapple_sound;
sf::SoundBuffer sound_release_grapple;

sf::Sound pain_sound;
sf::SoundBuffer sound_pain_01;
sf::SoundBuffer sound_pain_02;
sf::SoundBuffer sound_pain_03;
sf::SoundBuffer sound_pain_04;
sf::SoundBuffer sound_pain_05;

vector< vector<double> > catch_points;

bool temp = false;

// play a sound effect
void Protag::protag_play_sound_effect(string sound_type) {

	if(sound_effects_on == true) {

		if(sound_type == "fire grapple") {
			fire_grapple_sound.setBuffer(sound_fire_grapple);
			fire_grapple_sound.setVolume(40);
			fire_grapple_sound.play();
		}
		else if(sound_type == "release grapple") {
			release_grapple_sound.setBuffer(sound_release_grapple);
			release_grapple_sound.setVolume(40);
			release_grapple_sound.play();
		}
		else if(sound_type == "pain") {

			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1,5); 
			float rand = dist6(rng);

			pain_sound.setVolume(40);

			if(rand == 1) {
				pain_sound.setBuffer(sound_pain_01);
				pain_sound.play();
			}
			else if(rand == 2) {
				pain_sound.setBuffer(sound_pain_02);
				pain_sound.play();
			}
			else if(rand == 3) {
				pain_sound.setBuffer(sound_pain_03);
				pain_sound.play();
			}
			else if(rand == 4) {
				pain_sound.setBuffer(sound_pain_04);
				pain_sound.play();
			}
			else if(rand == 5) {
				pain_sound.setBuffer(sound_pain_05);
				pain_sound.play();
			}
		}
	}
}

// constructor
Protag::Protag(int the_tileSize, int ** map) {
    
	vertical_velocity = 0;
	horizontal_velocity = 0;

	FileReader reader = FileReader();
	tile_map = reader.GetMap("map_01.txt");

	gravity_acceleration = the_tileSize * (-6.81 / 36);
	jump_acceleration	 = the_tileSize * (4.8 / 36);
	running_acceleration = the_tileSize * (5.6 / 36);
	running_deceleration = the_tileSize * (7.0 / 36);
	running_speed_max	 = the_tileSize * (8.5 / 36);
	minimum_speed_skid	 = the_tileSize * (2.0 / 36);
	grapple_fire_speed   = the_tileSize * (10.0 / 36);

	current_cell_x = 0;
	current_cell_y = 0;
	x_into_current_cell = 0;
	y_into_current_cell = 0;

	grap_angle = 0;
	grap_length = 0;
	grap_angle_acceleration = 0;
	grap_angle_velocity = 0;

	falling = false;
	grapple_active = false;
	grapple_hooked = false;
	need_to_deactivate_grapple = false;
	need_to_fire_grapple = false;
	is_jumping = false;
	recovery = false;
	sound_effects_on = true;

	tileSize = the_tileSize;
	new_tile_size = 0;

	// assign initial cell
	current_cell_x = 23;
	current_cell_y = 27;
	x = (tileSize * 23);
	y = (tileSize * 27);

	health = 1000;
	max_health = 1000;

	grapple = Grapple(tileSize);
}

// turn sound effects on or off
void Protag::set_sound_effects(bool onoff) {
	sound_effects_on = onoff;
	grapple.set_sound_effects(onoff);
}

// time = time in seconds
void Protag::update(double time) {
	
	if(temp == false) {
		if (!sound_fire_grapple.loadFromFile("Resources/Sounds/grapple_fire.wav")) {
			// error
		}
		if (!sound_release_grapple.loadFromFile("Resources/Sounds/grapple_release.wav")) {
			// error
		}
		if (!sound_pain_01.loadFromFile("Resources/Sounds/pain_01.wav")) {
			// error
		}
		if (!sound_pain_02.loadFromFile("Resources/Sounds/pain_02.wav")) {
			// error
		}
		if (!sound_pain_03.loadFromFile("Resources/Sounds/pain_03.wav")) {
			// error
		}
		if (!sound_pain_04.loadFromFile("Resources/Sounds/pain_04.wav")) {
			// error
		}
		if (!sound_pain_05.loadFromFile("Resources/Sounds/pain_05.wav")) {
			// error
		}

		temp = true;
	}

	damage_velocity = 12.0 * ((double)window_width / 1280);

	int protag_centre_x = (x + (tileSize / 2));
	int protag_centre_y = (y + (tileSize / 2));

	if(health <= 0) {
		is_dead = true;
		health = 0;
	}

	if(true == grapple.get_is_hooked() && false == grapple_hooked) {
		initialize_hooked_variables();
		grapple_hooked = true;
	}

	if(true == grapple.get_need_to_deactivate_grapple()) {
		need_to_deactivate_grapple = true;
		grapple.set_need_to_deactivate_grapple();
	}

	// Grapple
	if(true == need_to_deactivate_grapple) {
		should_fall_was = false;
		grapple.deactivate();
		grapple_hooked = false;
		grap_angle = 0;
		grap_length = 0;
		grap_angle_acceleration = 0;
		grap_angle_velocity = 0;
		grapple.set_max_grapple_length((tileSize * 8));
		catch_points.clear();
		need_to_deactivate_grapple = false;
	}
	else if(true == need_to_fire_grapple) {
		fire_grapple();
		need_to_fire_grapple = false;
	}

	bool should_fall = false;

	// if the player is grappled but should fall anyway
	if(grapple.get_y() > protag_centre_y && grap_angle_velocity < 10) {
		should_fall = true;
		should_fall_was = true;
	}
	// the player is transitioning from free-falling to swinging
	else if(true == should_fall_was && true == grapple.get_is_hooked() && true == falling) {
		
		initialize_hooked_variables();
		
		if(protag_centre_x - catch_points.back().front() > 0) {
			grap_angle_velocity = -6;
		}
		else {
			grap_angle_velocity = 6;
		}

		should_fall_was = false;
	}

	// SWING!
	if(true == grapple.get_is_hooked() && true == falling && false == should_fall) {

		// CALCULATE GRAPPLE X AND Y SCALARS ===========================================================
		double x_angle_scalar = 0;
		double y_angle_scalar = 0;

		// --------------------------------------------
		//	INCRESE/ DECREASE GRAPPLE LENGTH		  |
		// --------------------------------------------
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (grap_length - (0.5 * (36 / tileSize)) > (tileSize - 1))) {
			grap_length -= (2.5 * (tileSize / 36));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (grap_length + (2.5 * (36 / tileSize)) < (grapple.get_max_grapple_length()))) {
			grap_length += (3.0 * (tileSize / 36));
		}

		// work out to what scale the horizontal/vertical velocity should impact the current motion
		if(protag_centre_x > grapple.get_x()) {
			// BOTTOM RIGHT
			if(protag_centre_y > grapple.get_y()) {
				x_angle_scalar = 1 - (grap_angle - (pi / 2)) / (pi / 2);
			}
			// TOP RIGHT
			else {
				x_angle_scalar = (grap_angle - pi) / (pi / 2);
			}
		}
		else {
			// BOTTOM LEFT
			if(protag_centre_y > grapple.get_y()) {
				x_angle_scalar = grap_angle / (pi / 2);
			}
			// TOP LEFT
			else {
				x_angle_scalar = 1 - (grap_angle - (pi * 1.5)) / (pi / 2);
			}
		}

		y_angle_scalar = 1 - x_angle_scalar; 

		// Sort Out
		if(y_angle_scalar < 0) {
			y_angle_scalar = 0;
		} 
		if(y_angle_scalar > 1) {
			y_angle_scalar = 1;
		} 
		if(x_angle_scalar < 0) {
			x_angle_scalar = 0;
		} 
		if(y_angle_scalar < 1) {
			x_angle_scalar = 1;
		} 

		// SORT OUT Y AND X SCALARS THAT ARE COMPLETELY WHACK
		if(x_angle_scalar > 1.0 || x_angle_scalar < 0) {

			if(x_angle_scalar > 1.0) {
				y_angle_scalar = 0;
				x_angle_scalar = 1;
			}
			else {
				x_angle_scalar = 0;
				y_angle_scalar = 1;
			}

			if(grap_angle < (pi / 4)) {
				vertical_velocity = 0;
			}
		}

		// =============================================================================================

		// if we are in front of the grapple
		if(protag_centre_x > grapple.get_x()) {
			grap_angle_velocity -= ((grapple_fire_speed * y_angle_scalar) * 0.1);
		}
		// behind the gapple
		else {
			grap_angle_velocity += ((grapple_fire_speed * y_angle_scalar) * 0.1);
		}

		// correction count is used to stop the velocity permanantly reseting to 0 when at rest (thus preventing swinging from rest)

		// if the player is swinging
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && grap_angle_velocity >= -0.1 && grap_angle_velocity < (tileSize * 0.75)) {
			grap_angle_velocity += ((tileSize / 36) * 0.2);
			if(correcting_count == 0) {
				correcting_count += 2;
			}	
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && grap_angle_velocity <= 0.1 && grap_angle_velocity > -(tileSize * 0.75)) {
			grap_angle_velocity -= ((tileSize / 36) * 0.2);
			if(correcting_count == 0) {
				correcting_count += 2;
			}	
		}
		else if (grap_angle > ((pi / 2)-0.01) && grap_angle < ((pi / 2) + 0.01) && grap_angle_velocity <= 3 && grap_angle_velocity >= -3 && correcting_count == 0) {
			grap_angle_velocity = 0;
		}

		if(correcting_count > 0) {
			correcting_count -= 1;
		}

		grap_angle += ((grap_angle_velocity * (36 / tileSize)) * 0.001);

		// prevent the angle going below 0 or above 2 pi
		if(grap_angle > (pi * 2)) {
			grap_angle = 0 + fmod(grap_angle, (pi * 2));
		}
		else if(grap_angle < 0) {
			grap_angle = (2 * pi) + grap_angle;
		}

		// MAKE THE GRAPPLE SLOW DOWN OVER TIME (FRICTION / RESISTANCE)
		if(grap_angle_velocity > 0) {
			if(grap_angle_velocity - 3 < 0) {
				grap_angle_velocity = -3;
			}
			else {
				grap_angle_velocity -= ((tileSize / 36) * 0.1);
			}
		}
		else if(grap_angle_velocity < 0) {
			if(grap_angle_velocity + 3 > 0) {
				grap_angle_velocity = 3;
			}
			else {
				grap_angle_velocity += ((tileSize / 36) * 0.1);
			}
		}

		// set the horizontal and vertical velocity in case the player ungrapples
		horizontal_velocity = grap_angle_velocity * x_angle_scalar * 0.3;
		vertical_velocity = grap_angle_velocity * y_angle_scalar * 0.25;

		// work out whether the vertical velocity should be positive or negative
		if(protag_centre_x > grapple.get_x()) {

		}
		else {
			// BOTTOM LEFT
			if(protag_centre_y > grapple.get_y()) {
				vertical_velocity = -vertical_velocity;
			}
			// TOP LEFT
			else {
				vertical_velocity = -vertical_velocity;
			}
		}

		// CALCULATE PROTAGONISTS NEW X AND Y CO-ORDINATES -----------------

		// get the new horizontal distance from the grapple
		int x_distance_from_grapple = 0;

		// get the new horizontal distance from the grapple
		int y_distance_from_grapple = 0;

		// length * cos(angle) = y
		x_distance_from_grapple = (grap_length * cos(grap_angle));

		// length * sin(angle) = y
		y_distance_from_grapple = (grap_length * sin(grap_angle));

		//update the x and y co-ordinates of the protag
		x = (catch_points.back().front() - x_distance_from_grapple - (tileSize / 2));
		y = (catch_points.back().back() + y_distance_from_grapple - (tileSize / 2));

		// ------------------------------------------------------------------

		// COLLISION --------------------------------------------------------

		// HORIZONTAL

		// calculates the new x-grid cell from the new x coordinate
		int new_cell_x = (int)((x / tileSize) + 1);

		// if the new cell is solid
		if ((tile_map[current_cell_y][new_cell_x] > 0) || (tile_map[current_cell_y][new_cell_x + 1] > 0) || 
			(tile_map[current_cell_y - 1][new_cell_x] > 0) || (tile_map[current_cell_y - 1][new_cell_x + 1] > 0) ||	
			(tile_map[current_cell_y - 2][new_cell_x] > 0) || (tile_map[current_cell_y - 2][new_cell_x + 1] > 0)) {

			// if the new cell is solid
			if ((tile_map[current_cell_y][new_cell_x] == 3) || (tile_map[current_cell_y][new_cell_x + 1] == 3) || 
				(tile_map[current_cell_y - 1][new_cell_x] == 3) || (tile_map[current_cell_y - 1][new_cell_x + 1] == 3) ||	
				(tile_map[current_cell_y - 2][new_cell_x] == 3) || (tile_map[current_cell_y - 2][new_cell_x + 1] == 3)) {
					health = 0;
					protag_play_sound_effect("pain");
			}

			if(horizontal_velocity > damage_velocity || horizontal_velocity < -damage_velocity) {
				double scalar = 0;

				if(horizontal_velocity < 0) {
					scalar = -(horizontal_velocity + damage_velocity);
				}
				else {
					scalar = (horizontal_velocity - damage_velocity);
				}

				health = health - ((scalar * 2) * (max_health * 0.26));
				protag_play_sound_effect("pain");
			}

			// stop the players movement
			horizontal_velocity = 0;

			// prevent getting stuck when hitting a tile above you
			if(x_distance_from_grapple > 0) {
				grap_angle_velocity = 6;
			}
			else {
				grap_angle_velocity = -6;
			}

			// if the new cell is forwards...
			if(new_cell_x > current_cell_x) {
				x = (current_cell_x * (tileSize) -1);
			}
			//otherwise the new cell is backwards...
			else {
				x = ((current_cell_x - 1) * tileSize);
			}
		}
		// if the new cell isn't solid make it the current cell and update x
		else {
			current_cell_x = new_cell_x;
		}

		// VERTICAL

		// declares a variable new cell y
		int new_cell_y = 0;
			
		// sets new y grid coordinate based on whether the horizontal velocity is positive or negative
		if(vertical_velocity >= 0) {
			// calculates the new x-grid cell from the new x coordinate
			new_cell_y = (int)((y / tileSize) + 1);
		}
		else {
			// calculates the new x-grid cell from the new x coordinate
			new_cell_y = (int)(((y - 1) / tileSize) + 1);
		}

		// if the new cell is solid
		if ((tile_map[new_cell_y][current_cell_x] > 0) || (tile_map[new_cell_y][current_cell_x + 1] > 0) ||
			(tile_map[new_cell_y - 2][current_cell_x] > 0) || (tile_map[new_cell_y - 2][current_cell_x + 1]) > 0)	{

			if ((tile_map[new_cell_y][current_cell_x] == 3) || (tile_map[new_cell_y][current_cell_x + 1] == 3) ||
				(tile_map[new_cell_y - 2][current_cell_x] == 3) || (tile_map[new_cell_y - 2][current_cell_x + 1]) == 3)	{
					health = 0;
			}

			if(vertical_velocity > damage_velocity || vertical_velocity < -damage_velocity) {
				double scalar = 0;

				if(vertical_velocity < 0) {
					scalar = -(vertical_velocity + damage_velocity);
				}
				else {
					scalar = (vertical_velocity - damage_velocity);
				}

				health = health - ((scalar * 2) * (max_health * 0.26));
				protag_play_sound_effect("pain");
			}

			// stop the players vertical movement
			vertical_velocity = 0;

			// prevent getting stuck when hitting a tile above you
			if(x_distance_from_grapple > 0) {
				grap_angle_velocity = 6;
			}
			else {
				grap_angle_velocity = -6;
			}
			
			// if the new cell is downwards...
			if(new_cell_y > current_cell_y) {
				y = (current_cell_y * (tileSize));
				falling = false;
			}
			//otherwise the new cell is upwards...
			else {
				y = (new_cell_y * tileSize + 1);
			}
		}
		// if the new cell isn't solid make it the current cell and update x
		else {
			current_cell_y = new_cell_y;
		}

		/**
		 * ------------------------------
		 *	GRAPPLE CORD COLLISION:		|
		 * ------------------------------
		 */

		// UN-COLLISION (removing last catch point if it is no longer relevant -------------------------------------------------------
		if (catch_points.size() > 1) {

			int the_length_to_check = (tileSize / 5);

			int catch_points_size = catch_points.size();

			bool at_least_one_catch = false;
			bool should_break = false;

			// ITERATE ALONG GRAPPLE CORD
			while(should_break == false) {

				// work out x and y co-ordinates of this point on the grapple
				int points_x_distance_from_grapple = 0;
				int points_y_distance_from_grapple = 0;
				int points_x = 0;
				int points_y = 0;

				// length * cos(angle) = y
				points_x_distance_from_grapple = (the_length_to_check * cos(grap_angle)); 
				points_y_distance_from_grapple = (the_length_to_check * sin(grap_angle) - tileSize); 

				// work out the co-ordinates of this point on the grapple
				if(grapple.get_attach_point() == 'l') {
					points_x = (catch_points[(catch_points_size - 2)].front() - points_x_distance_from_grapple - 1);
				}
				else {
					points_x = (catch_points[(catch_points_size - 2)].front() - points_x_distance_from_grapple);
				}

				points_y = (catch_points[(catch_points_size - 2)].back() + points_y_distance_from_grapple);

				int points_cell_x = (int)((points_x / tileSize) + 1);
				int points_cell_y = (int)(points_y / tileSize);
			
				// if the point is inside a tile
				if (tile_map[points_cell_y][points_cell_x] > 0) {
					at_least_one_catch = true;
				}

				the_length_to_check += (tileSize / 10);

				if(the_length_to_check >= grap_length) {
					should_break = true;
				}
			}

			// CATCH
			if(at_least_one_catch) {
				// remain caught	
			}
			else {
				// restore the max length
				grapple.set_max_grapple_length(grapple.get_max_grapple_length() + (catch_points.back()[1]));
				//fix the shortening that will occur
				grap_length += catch_points.back()[1];

				catch_points.pop_back();
			}
		}

		// --------------------------------------------------------------------------------------------------------------------------

		// NEW COLLISIONS

		int the_grapple_starting_length = grap_length;
		int current_length_to_check = (tileSize / 5);

		double amount_to_add = 0;

		bool at_least_one_catch = false;
		bool should_break = false;
		int largest_catch = 0;
		int largest_points_x = 0;
		int largest_points_y = 0;

		// ITERATE ALONG GRAPPLE CORD
		while(should_break == false) {

			// work out x and y co-ordinates of this point on the grapple
			int points_x_distance_from_grapple = 0;
			int points_y_distance_from_grapple = 0;
			int points_x = 0;
			int points_y = 0;

			// length * cos(angle) = y
			points_x_distance_from_grapple = (current_length_to_check * cos(grap_angle)); 
			points_y_distance_from_grapple = (current_length_to_check * sin(grap_angle) - tileSize); 

			// work out the co-ordinates of this point on the grapple
			if(grapple.get_attach_point() == 'l') {
				points_x = (catch_points.back().front() - points_x_distance_from_grapple - 1);
			}
			else {
				points_x = (catch_points.back().front() - points_x_distance_from_grapple);
			}

			points_y = (catch_points.back().back() + points_y_distance_from_grapple);

			int points_cell_x = (int)((points_x / tileSize) + 1);
			int points_cell_y = (int)(points_y / tileSize);
			
			// if the point is inside a tile
			if (tile_map[points_cell_y][points_cell_x] > 0) {
				at_least_one_catch = true;
				largest_catch = current_length_to_check;
				largest_points_x = points_x;
				largest_points_y = points_y;
			}

			current_length_to_check += (tileSize / 10);

			if(current_length_to_check >= grap_length) {
				should_break = true;
			}
		}

		// CATCH
		if(at_least_one_catch) {
			int catch_x = 0;
			int catch_y = 0;

			int immediate_left_temp = (int)((x - 1) / tileSize);

			// fixes a bug where collisions would happen if grappled while directly next to a wall to the left 
			if(x_angle_scalar >= y_angle_scalar && (tile_map[current_cell_y][immediate_left_temp] > 0)) {
				// do nothing
			}
			else {

				// if there is an error with the scalars fix them
				if(x_angle_scalar + y_angle_scalar > 1) {
					x_angle_scalar = 1 - y_angle_scalar;
				}

				// fix an error where the angle scalars are incorrect
				if(x_angle_scalar == 1 && (x_distance_from_grapple > y_distance_from_grapple)) {
					x_angle_scalar = 0;
					y_angle_scalar = 1;
				}

				// catch furhter along vertically
				if(x_angle_scalar >= y_angle_scalar) {

					// work out the y co-ordinate catch point
					if((y - catch_points.back().back()) > 0) {
						catch_y = ((int)((largest_points_y / tileSize) + 2) * tileSize);
					}
					else {
						catch_y = (int)((largest_points_y / tileSize) * tileSize);
					}

					// work out the x co-ordinate catch point
					if((x - catch_points.back().front()) + (tileSize / 2) > 0) {

						// we are behind of the grapple
						int catch_x_iterator = 0;
						int temp = 0;
						while(temp == 0) {
							int a_x = (largest_points_x / tileSize) - catch_x_iterator;
							int a_y = (catch_y / tileSize); 
							if(tile_map[a_y][a_x] == 0) {
								catch_x = ((int)((largest_points_x / tileSize) - catch_x_iterator) * tileSize);
								temp = 1;
							}
							catch_x_iterator++;
						}
					}
					else {

						// we are in front of the grapple
						int catch_x_iterator = 0;
						int temp = 0;
						while(temp == 0) {
							int a_x = (largest_points_x / tileSize) + catch_x_iterator;
							int a_y = (catch_y / tileSize);
							if(tile_map[a_y][a_x] == 0) {
								catch_x = ((int)(((largest_points_x / tileSize) + 1) - catch_x_iterator) * tileSize);
								temp = 1;
							}
							catch_x_iterator++;
						}
					}
				}

				// catch further along horizontally
				else {

					// work out the x co-ordinate catch point
					if((x - catch_points.back().front()) > 0) {
						catch_x = ((int)((largest_points_x / tileSize) + 1) * tileSize);
					}
					else {
						catch_x = ((int)(largest_points_x / tileSize) * tileSize);
					}

					// work out the y co-ordinate catch point
					if(vertical_velocity <= 0) {

						// we are below the grapple
						int catch_y_iterator = 0;
						int temp = 0;
						while(temp == 0) {
							int a_y = (largest_points_y / tileSize) - catch_y_iterator;
							int a_x = (catch_x / tileSize);
							if(tile_map[a_y][a_x] == 0) {
								if((x - catch_points.back().front()) + (tileSize / 2) > 0) {
									catch_y = ((int)((largest_points_y / tileSize) - catch_y_iterator + 2) * tileSize);
									grapple.set_attach_point('r');
								}
								else {
									catch_y = ((int)((largest_points_y / tileSize) - catch_y_iterator + 1) * tileSize);
									grapple.set_attach_point('l');
								}
								temp = 1;
							}
							catch_y_iterator++;
						}
					}
					else {

						// we are above the grapple
						int catch_y_iterator = 0;
						int temp = 0;
						while(temp == 0) {
							int a_y = (largest_points_y / tileSize) + catch_y_iterator;
							int a_x = (catch_x / tileSize);
							if(tile_map[a_y][a_x] == 0) {
								if((x - catch_points.back().front()) + (tileSize / 2) > 0) {
									catch_y = ((int)((largest_points_y / tileSize) + catch_y_iterator + 2) * tileSize);
									grapple.set_attach_point('r');
								}
								else {
									catch_y = ((int)((largest_points_y / tileSize) + catch_y_iterator + 1) * tileSize);
									grapple.set_attach_point('l');
								}
								temp = 1;
							}
							catch_y_iterator++;
						}
					}
				}

				grapple.set_coords(catch_x, catch_y);
				initialize_hooked_variables();
			}
		}
	}

	// NOT SWINGING
	else {

		/**
		 * ----------------------
		 *	GRAPPLE:			|
		 * ----------------------
		 */
		int protag_centre_x = (x + (tileSize / 2));
		int protag_centre_y = (y + (tileSize / 2));

		if(grapple.check_is_active() == true) {
			grapple.update(0.016, protag_centre_x, protag_centre_y);
		}

		// jump
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if(falling) {
				// do nothing
			}
			else if(true == is_hooked() && catch_points.back().back() < protag_centre_y) {
				// do nothing
			}
			else {
				is_jumping = true;
				vertical_velocity += jump_acceleration;
				falling = true;
			}
		}

		/**
		 * -----------------------------------
		 * Horizontal velocity calculations: |
		 * -----------------------------------
		 */
	
		// if the player is currently holding down the "Move Right" button (D) :
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

			// if the acceleration is going to take the player over the maximum speed achievable (through running):
			if((horizontal_velocity <= running_speed_max) && (horizontal_velocity + (running_acceleration * time) > running_speed_max)) {
				// cap the speed 
				horizontal_velocity = running_speed_max;
			}
			// if the player is trying to shit from running to the left to running to the right (sliding):
			else if(horizontal_velocity < 0 && ((horizontal_velocity + (running_acceleration * time))) > -minimum_speed_skid) {
				// makes switching directions quicker
				horizontal_velocity = minimum_speed_skid;
			}
			// otherwise they are accelerating to the right
			else {
				horizontal_velocity += (running_acceleration * time);
			}	
		}

		// if the player is currently holding down the "Move Left" button (A) :
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			
			// if the acceleration is going to take the player over the maximum speed achievable (through running):
			if((horizontal_velocity >= (-running_speed_max)) && (horizontal_velocity - (running_acceleration * time) < (-running_speed_max))) {
				// cap the speed 
				horizontal_velocity = (-running_speed_max);
			}
			// if the player is trying to shit from running to the right to running to the left (sliding):
			else if(horizontal_velocity > 0 && ((horizontal_velocity - (running_acceleration * time))) < minimum_speed_skid) {
				// makes switching directions quicker
				horizontal_velocity = -minimum_speed_skid;
			}
			// otherwise they are accelerating to the left
			else {
				horizontal_velocity -= (running_acceleration * time);
			}	
		}
		// if they are not trying to move but still have speed, decelerate to 0
		else if (horizontal_velocity != 0) {
			
			// decelerates positive velocity
			if (horizontal_velocity > 0) {
				
				horizontal_velocity = (horizontal_velocity - (running_deceleration * time));
				
				if(horizontal_velocity < 0) {
					horizontal_velocity = 0;
				}
			}
			// decelerates negative velocity
			else {
				
				horizontal_velocity = (horizontal_velocity + (running_deceleration * time));
				
				if(horizontal_velocity > 0) {
					horizontal_velocity = 0;
				}
			}
		}
		
		/**
		 * -------------------------------------------------
		 * Vertical velocity and collision calculations:   |
		 * -------------------------------------------------
		 */
		
		// if the player is falling
		if(falling) {
					
			vertical_velocity = vertical_velocity + (gravity_acceleration * time);
		
			// calculates the would-be new y coordinate
			int new_y = (int) (y - vertical_velocity);
					
			// declares a variable new cell x
			int new_cell_y = 0;
			
			// sets new y grid coordinate based on whether the horizontal velocity is positive or negative
			if(vertical_velocity >= 0) {
				// calculates the new x-grid cell from the new x coordinate
				new_cell_y = (int)((new_y / tileSize) + 1);
			}
			else {
				// calculates the new x-grid cell from the new x coordinate
				new_cell_y = (int)(((new_y - 1) / tileSize) + 1);
			}

			// if the new cell is solid
			if ((tile_map[new_cell_y][current_cell_x] > 0) || (tile_map[new_cell_y][current_cell_x + 1] > 0) ||
				(tile_map[new_cell_y - 2][current_cell_x] > 0) || (tile_map[new_cell_y - 2][current_cell_x + 1]) > 0)	{

				if ((tile_map[new_cell_y][current_cell_x] == 3) || (tile_map[new_cell_y][current_cell_x + 1] == 3) ||
					(tile_map[new_cell_y - 2][current_cell_x] == 3) || (tile_map[new_cell_y - 2][current_cell_x + 1]) == 3)	{
						health = 0;
						protag_play_sound_effect("pain");
				}

				if(vertical_velocity > damage_velocity || vertical_velocity < -damage_velocity) {

					double scalar = 0;

					if(vertical_velocity < 0) {
						scalar = -(vertical_velocity + damage_velocity);
					}
					else {
						scalar = (vertical_velocity - damage_velocity);
					}

					health = health - ((scalar * 2) * (max_health * 0.26));
					protag_play_sound_effect("pain");
				}

				// stop the players vertical movement
				vertical_velocity = 0;
				
				// if the new cell is downwards...
				if(new_cell_y > current_cell_y) {
					y = (current_cell_y * (tileSize));
					falling = false;
					recovery = true;
				}
				//otherwise the new cell is upwards...
				else {
					y = (new_cell_y * tileSize + 1);
				}
			}
			// if the new cell isn't solid make it the current cell and update x
			else {
				current_cell_y = new_cell_y;
				y = new_y;
			}
		}
	
		/**
		* ---------------------------------------------------
		* Coordinate and collision variable initialisation: |
		* ---------------------------------------------------
		*/
		// calculates the new x coordinate based on the current horizontal velocity
		int new_x = (int) (x + (horizontal_velocity / 2));
		
		// calculates the new x-grid cell from the new x coordinate
		int new_cell_x = (int)((new_x / tileSize) + 1);
	
		/**
		 * ---------------------------------------------------
		 * Horizontal coordinate and collision calculations: |
		 * ---------------------------------------------------
		 */
		// if the new cell is solid
		if ((tile_map[current_cell_y][new_cell_x] > 0) || (tile_map[current_cell_y][new_cell_x + 1] > 0) || 
			(tile_map[current_cell_y - 1][new_cell_x] > 0) || (tile_map[current_cell_y - 1][new_cell_x + 1] > 0) ||	
			(tile_map[current_cell_y - 2][new_cell_x] > 0) || (tile_map[current_cell_y - 2][new_cell_x + 1] > 0)) {

			// if the new cell is deadly
			if ((tile_map[current_cell_y][new_cell_x] == 3) || (tile_map[current_cell_y][new_cell_x + 1] == 3) || 
				(tile_map[current_cell_y - 1][new_cell_x] == 3) || (tile_map[current_cell_y - 1][new_cell_x + 1] == 3) ||	
				(tile_map[current_cell_y - 2][new_cell_x] == 3) || (tile_map[current_cell_y - 2][new_cell_x + 1] == 3)) {
					health = 0;
					protag_play_sound_effect("pain");
			}

			if(horizontal_velocity > damage_velocity || horizontal_velocity < -damage_velocity) {
				double scalar = 0;

				if(horizontal_velocity < 0) {
					scalar = -(horizontal_velocity + damage_velocity);
				}
				else {
					scalar = (horizontal_velocity - damage_velocity);
				}

				health = health - ((scalar * 2) * (max_health * 0.26));
				protag_play_sound_effect("pain");
			}

			// stop the players movement
			horizontal_velocity = 0;
			
			// if the new cell is forwards...
			if(new_cell_x > current_cell_x) {
				x = (current_cell_x * (tileSize) -1);
			}
			//otherwise the new cell is backwards...
			else {
				x = ((current_cell_x - 1) * tileSize);
			}
		}
		// if the new cell isn't solid make it the current cell and update x
		else {
			current_cell_x = new_cell_x;
			x = new_x;
		}

		/**
		 * ---------------------------------------------------
		 * Falling Parameter assignment:                     |
		 * ---------------------------------------------------
		 */
		if(!falling) {
			if((tile_map[current_cell_y + 1][current_cell_x] == 0) && (tile_map[current_cell_y + 1][current_cell_x + 1] == 0)) {
				falling = true;
			}
		}
	}
}

// when the protag collides with the lightning columns
void Protag::lightning_collision() {

	vertical_velocity = (-10 * (window_width / 1280));

	int new_health = (getHealth() - 30);

	if(new_health < 0) {
		new_health = 0;
		protag_play_sound_effect("pain");
	}

	in_lightning = true;

	setHealth(new_health);
}

void Protag::set_lightning_false() {
	in_lightning = false;
}

void Protag::respawn(int checkpoint, int new_health) {

	health = new_health;
	is_dead = false;
	horizontal_velocity = 0;
	vertical_velocity = 0;

	if(true == grapple_active) {
		need_to_deactivate_grapple = true;
		grapple.set_need_to_deactivate_grapple();
	}

	switch(checkpoint) {

		case 0:
			current_cell_x = 23;
			current_cell_y = 27;
			x = (tileSize * 23);
			y = (tileSize * 27);
			break;

		case 1:
			current_cell_x = 140;
			current_cell_y = 30;
			x = (tileSize * 140);
			y = (tileSize * 30);
			break;

		case 2:
			current_cell_x = 280;
			current_cell_y = 34;
			x = (tileSize * 280);
			y = (tileSize * 34);
			break;

		case 3:
			current_cell_x = 387;
			current_cell_y = 34;
			x = (tileSize * 382);
			y = (tileSize * 34);
			break;

		case 4:
			current_cell_x = 530;
			current_cell_y = 38;
			x = (tileSize * 530);
			y = (tileSize * 38);
			break;

		default:
			break;

	}
}

void Protag::fire_grapple() {

	int protag_centre_x = (x + (tileSize / 2));
	int protag_centre_y = (y + (tileSize / 2));

	// get the position of the mouse
	sf::Vector2i position = sf::Mouse::getPosition(*window);

	//fire the grapple if it is not already active
	if(false == grapple.check_is_active()) {

		protag_play_sound_effect("fire grapple");

		double x_difference = position.x - ((window_width / 2) - protag_offset_x + (tileSize / 2));
		double y_difference = -((position.y - (tileSize / 2)) - ((window_height / 2) - protag_offset_y + (tileSize / 2)));

		// initialize the horizontal and vertical velocity of the grapple shot
		double horizontal_vel = 0;
		double vertical_vel = 0;

		// initialize the scale values
		double scale_x = 0;
		double scale_y = 0;

		double pos_x_diff = fabs(x_difference);
		double pos_y_dif = fabs(y_difference);

		if(pos_x_diff == pos_y_dif) {
			scale_x = 1;
			scale_y = 1;
		}
		else if(pos_x_diff == 0) {
			scale_y = 1;
			scale_x = 0;
		}
		else if(pos_y_dif == 0) {
			scale_x = 1;
			scale_y = 0;
		}
		else if(pos_x_diff > pos_y_dif) {
			scale_x = 1;
			scale_y = pos_y_dif / pos_x_diff;
		}
		else if(pos_x_diff < pos_y_dif) {
			scale_y = 1;
			scale_x = pos_x_diff / pos_y_dif;
		}

		// make the scale factor negative if it should be negative
		if(x_difference < 0) {
			scale_x = -(scale_x);
		}
		if (y_difference < 0) {
			scale_y = -(scale_y);
		}

		horizontal_vel = scale_x * grapple_fire_speed;
		vertical_vel = scale_y * grapple_fire_speed;

		// initialize the grapple
		grapple.initialize(protag_centre_x, protag_centre_y, horizontal_vel, vertical_vel);
		grapple_active = true;
	}
}

void Protag::set_need_to_update_tile_size(double the_new_tile_size) {
	need_to_update_tile_size = true;
	new_tile_size = the_new_tile_size;

	grapple.update_tile_size(new_tile_size, window_height, window_width);

	// scales up/down the x and y co-ordinates
	x = x * (new_tile_size / tileSize);
	y = y * (new_tile_size / tileSize);

	// scales up/down the constants
	gravity_acceleration = gravity_acceleration * (new_tile_size / tileSize);
	jump_acceleration = jump_acceleration * (new_tile_size / tileSize);
	running_acceleration = running_acceleration * (new_tile_size / tileSize);
	running_speed_max = running_speed_max * (new_tile_size / tileSize);
	minimum_speed_skid = minimum_speed_skid * (new_tile_size / tileSize);
	grapple_fire_speed = grapple_fire_speed * (new_tile_size / tileSize);

	damage_velocity = damage_velocity * (new_tile_size / tileSize);
	minimum_speed_skid = minimum_speed_skid * (new_tile_size / tileSize);

	grap_length = grap_length * (new_tile_size / tileSize);

	horizontal_velocity = horizontal_velocity * (new_tile_size / tileSize);
	vertical_velocity = vertical_velocity * (new_tile_size / tileSize);
	grap_angle_velocity =  grap_angle_velocity * (new_tile_size / tileSize);

	// scale all catch points
	for(int i = 0; i < catch_points.size(); i++) {
		catch_points[i].front() = catch_points[i].front() * (new_tile_size / tileSize);
		catch_points[i].back() = catch_points[i].back() * (new_tile_size / tileSize);
	}

	tileSize = new_tile_size;
}

double Protag::get_x() {
	return x;
}

double Protag::get_y() {
	return y;
}

bool Protag::is_grappled() {
	return grapple_active;
}

bool Protag::is_hooked() {
	return grapple.get_is_hooked();
}

bool Protag::grapple_is_active() {
	return grapple.check_is_active();
}

int * Protag::get_grapple_coords() {
	int x_y[2];
	x_y[0] = grapple.get_x();
	x_y[1] = grapple.get_y();
	return x_y;
}

void Protag::set_screen_parameters(int width, int height, int pro_offset_x, int pro_offset_y, sf::RenderWindow * the_window) {
	window = the_window;
	window_width = width;
	window_height = height;
	protag_offset_y = pro_offset_y;
	protag_offset_x = pro_offset_x;
}

void Protag::set_need_to_fire_grapple() {

	if(true == grapple.get_is_hooked()) {
		need_to_deactivate_grapple = true;
		protag_play_sound_effect("release grapple");
	}
	else if(true == grapple.check_is_active()) {
		// do nothing
	}
	else {
		need_to_fire_grapple = true;
	}
}

void Protag::initialize_hooked_variables() {

	int protag_centre_x = (x + (tileSize / 2));
	int protag_centre_y = (y + (tileSize / 2));

	int x_difference = (grapple.get_x() - protag_centre_x);
	int y_difference = (grapple.get_y() - protag_centre_y);

	// Assign the first 'catch point' aka the grapples starting location

	double temp_01 = grapple.get_x();
	double temp_02 = grapple.get_y();

	double * co_ord_array;

	vector<double> vec_01;

	vec_01.push_back(temp_01);

	if (catch_points.size() >= 1) {
		// work out length of this new section
		int the_previous_x = catch_points.back().front();
		int the_previous_y = catch_points.back().back();
		int length_calculator_x_diff = grapple.get_x() - the_previous_x;
		int length_calculator_y_diff = grapple.get_y() - the_previous_y;

		double catch_length = (sqrt(((double)length_calculator_x_diff * (double)length_calculator_x_diff) + ((double)length_calculator_y_diff * (double)length_calculator_y_diff)));
		vec_01.push_back(catch_length);

		grapple.set_max_grapple_length(grapple.get_max_grapple_length() - catch_length);
	}
	else {
		vec_01.push_back(0);
	}

	vec_01.push_back(temp_02);
	catch_points.push_back(vec_01);

	// calculate the length of the cord
	grap_length = (sqrt(((double)x_difference * (double)x_difference) + ((double)y_difference * (double)y_difference)));

	float temp = 0;

	double x_angle_scalar = 0;
	double y_angle_scalar = 0;

	// if y is incredibly small
	if(y_difference < 1 && y_difference > -1) {
		y_angle_scalar = 1;

		if(x_difference > 0) {
			grap_angle = 0;
		}
		else {
			grap_angle = pi;
		}
	}
	else {
		// calculate the initial angle
		if(y_difference > 0) {
			if(x_difference > 0) {
				temp = (float)x_difference /(float)-y_difference;
			}
			else {
				temp = (float)-y_difference / (float)x_difference;
			}
		}
		else {
			if(x_difference > 0) {
				temp = (float)-y_difference / (float)x_difference;
			}
			else {
				temp = (float)x_difference /(float)-y_difference;
			}
		}

		grap_angle = atan (temp); 

		if(grap_angle < 0) {
			grap_angle = -grap_angle;
		}

		// fix the starting angle
		if(x_difference > 0) {
			if(y_difference < 0) {
				// it's already correct
			}
			else {
				grap_angle = (grap_angle + (pi * 1.5));
			}
		}
		else {
			if(y_difference < 0) {
				grap_angle = (grap_angle + (pi * 0.5));
			}
			else {
				grap_angle = (grap_angle + (pi));
			}
		}

		// CALCULATE GRAPPLE X AND Y SCALARS ---------------------------------------------------------

		if(grap_angle > (pi * 2)) {
			grap_angle = 0 + fmod(grap_angle, (pi * 2));
		}

		// work out to what scale the horizontal/vertical velocity should impact the current motion
		if(protag_centre_x > grapple.get_x()) {
			// BOTTOM RIGHT
			if(protag_centre_y > grapple.get_y()) {
				x_angle_scalar = 1 - (grap_angle - (pi / 2)) / (pi / 2);
			}
			// TOP RIGHT
			else {
				x_angle_scalar = (grap_angle - pi) / (pi / 2);
			}
		}
		else {
			// BOTTOM LEFT
			if(protag_centre_y > grapple.get_y()) {
				x_angle_scalar = grap_angle / (pi / 2);
			}
			// TOP LEFT
			else {
				x_angle_scalar = 1 - (grap_angle - (pi * 1.5)) / (pi / 2);
			}
		}

		y_angle_scalar = 1 - x_angle_scalar; 
		// ---------------------------------------------------------------------------------------------
	}

	// if this is a catch do not change the grapple angle velocity, otherwise set it
	if (catch_points.size() > 1) {
		// do nothing
	}
	else {

		if(x_difference > 0) {
			// work out what speed the swing should be based on the current velocity of the protag
			grap_angle_velocity = (-vertical_velocity * y_angle_scalar * 4) + (horizontal_velocity * x_angle_scalar + 0.5) * 3;
		}
		else {
			// work out what speed the swing should be based on the current velocity of the protag
			grap_angle_velocity = (vertical_velocity * y_angle_scalar * 4) + (horizontal_velocity * (x_angle_scalar + 0.5) * 3);
		}

		if (y_difference > 0 && horizontal_velocity > 0) {
			grap_angle_velocity = -grap_angle_velocity;
		}
	}
}

const vector< vector<double> > Protag::get_catch_points() {
	return catch_points;	
};

void Protag::set_jumping() {
	is_jumping = false;
}

void Protag::set_recovery() {
	recovery = false;
}

void Protag::set_velocity_0() {
	horizontal_velocity = 0;
}