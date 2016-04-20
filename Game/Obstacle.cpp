#include "stdafx.h"
#include "Obstacle.h"
#include <iostream>

// constructor
Obstacle::Obstacle(int the_tileSize, string the_type, int the_x, int the_y, int the_width, int the_height, int the_id) {
    
	tileSize = the_tileSize;
	type = the_type;
	id = the_id;

	x = the_x;
	y = the_y;

	active_count = 0.0;
	lightning_active = false;
	visuals_active = false;

	width = the_width;
	height = the_height;
}

void Obstacle::update() {

	active_count += 0.016;

	if(active_count > 1.6 && visuals_active == false && lightning_active == false) {
		visuals_active = true;
	}

	if(active_count > 0.4 && active_count < 1.6 && visuals_active == true && lightning_active == false) {
		visuals_active = false;
	}

	if(active_count > 2.0 && lightning_active == false) {
		lightning_active = true;
		active_count = (0 + (active_count - 2.0));
	}

	if(active_count > 1.5 && lightning_active == true) {
		lightning_active = false;
		active_count = (0 + (active_count - 1.5));
	}
}

int Obstacle::get_x() {
	return x;
}

int Obstacle::get_y() {
	return y;
}

int Obstacle::get_width() {
	return width;
}

int Obstacle::get_height() {
	return height;
}

void Obstacle::update_tile_size(int new_tile_size) {

	x = x * ((float)new_tile_size / tileSize);
	y = y * ((float)new_tile_size / tileSize);
	width = width * ((float)new_tile_size / tileSize);
	height = height * ((float)new_tile_size / tileSize);

	tileSize = new_tile_size;
}