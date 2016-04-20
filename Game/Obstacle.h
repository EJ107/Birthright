#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include "stdafx.h"

using namespace std;

class Obstacle {

	public:

		Obstacle(int tileSize, string type, int x, int y, int width, int height, int id);

		int get_x();
		int get_y();
		int get_width();
		int get_height();

		bool get_active() {
			return lightning_active;
		}

		bool get_visuals_active() {
			return visuals_active;
		}

		int get_id() {
			return id;
		}

		void update();

		void update_tile_size(int tileSize);

	private:

		int x;
		int y;
		int width;
		int height;
		int tileSize;
		int id;

		bool lightning_active;
		bool visuals_active;

		double active_count;

		string type;
};
