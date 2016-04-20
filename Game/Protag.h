#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "stdafx.h"
#include "Grapple.h"

using namespace std;

class Protag {

	public:

		Protag(int tileSize, int **level_map);
		void update(double time);
		double get_x();
		double get_y();

		void set_need_to_update_tile_size(double new_tile_size);
		void set_screen_parameters(int width, int height, int pro_off_x, int pro_off_y, sf::RenderWindow * window);
		void fire_grapple() ;
		void set_need_to_fire_grapple();
		void initialize_hooked_variables();
		void set_jumping();
		void set_recovery();
		void set_velocity_0();
		void lightning_collision();
		void set_lightning_false();

		void setHealth(int newHealth) {
			health = newHealth;
		}

		double get_grap_angle() {
			return grap_angle;
		}

		void respawn(int checkpoint, int health);

		void protag_play_sound_effect(string name);

		void set_sound_effects(bool onoff);

		bool is_grappled();
		bool is_hooked();
		bool grapple_is_active();

		int * get_grapple_coords();

		int getHealth() {
			return health;
		}
		int getMaxHealth() {
			return max_health;
		}
		bool getIsDead() {
			return is_dead;
		}
		bool get_is_falling() {
			return falling;
		}
		double get_horizontal_velocity() {
			return horizontal_velocity;
		}
		bool get_is_jumping() {
			return is_jumping;
		}
		bool get_recovery() {
			return recovery;
		}
		bool get_grapple_hooked() {
			return grapple_hooked;
		}

		const vector< vector<double> > get_catch_points();

	private:

		sf::RenderWindow * window;

		int health;
		int max_health;

		double x;
		double y; 
	
		double tileSize;
		double new_tile_size;

		double gravity_acceleration; //-9.81
		double jump_acceleration; //7.1
		double running_acceleration; //6.0
		double running_deceleration; //6.0
		double running_speed_max; //8.0
		double minimum_speed_skid; //1.5
	
		double grapple_fire_speed;

		double vertical_velocity;
		double horizontal_velocity;

		bool falling;
		bool right;
		bool left;
		bool need_to_update_tile_size;
		bool grapple_active;
		bool grapple_hooked;
		bool need_to_fire_grapple;
		bool need_to_deactivate_grapple;
		bool is_dead;
		bool is_jumping;
		bool recovery;
		bool in_lightning;

		bool sound_effects_on;

		int **level_map;

		int window_width;
		int window_height;
		int protag_offset_y;
		int protag_offset_x;

		// grapple movement variables
		double grap_angle;
		int grap_length;
		double grap_angle_acceleration;
		double grap_angle_velocity;

		/** the x-coordinate of the current cell on the map array */
		int current_cell_x;
		/** the y-coordinate of the current cell on the map array */
		int current_cell_y;
		/** how far horizontally into said tile the player is */
		int x_into_current_cell;
		/** how far vertically into said tile the player is */
		int y_into_current_cell;
};