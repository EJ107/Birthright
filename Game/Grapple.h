#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "stdafx.h"
#include "Protag.h"

using namespace std;

class Grapple {

	public:

		Grapple(){};
		Grapple(int tileSize);

		void update(double time, int pro_cen_x, int pro_cen_y);
		void initialize(int x, int y, double horizontal_velocity, double vertical_velocity);
		void update_tile_size(int the_new_tile_size, int win_height, int win_width);
		void deactivate();
		void hook();
		void set_sound_effects(bool onoff);
		void grapple_play_sound_effect(string name);

		void set_max_grapple_length(double new_max_length);
		double get_max_grapple_length();

		void set_need_to_deactivate_grapple();
		bool get_need_to_deactivate_grapple();

		void set_coords(int x, int y);

		bool check_is_active();
		bool get_is_hooked();
		char get_attach_point();
		void set_attach_point(char new_attatch_point);

		double get_x();
		double get_y();

	private:

		double x;
		double y; 
		double horizontal_velocity;
		double vertical_velocity;

		double tileSize;
		double new_tile_size;

		bool is_active;
		bool is_hooked;
		bool need_to_deactivate_grapple;
		bool sound_effects_on;

		char attatch_point;
};