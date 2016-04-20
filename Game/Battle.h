#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "stdafx.h"
#include "Grapple.h"
#include "Combatant.h"
#include <string>
#include <vector>

using namespace std;

class Battle {

	public:

		Battle(int tileSize, int window_height, int window_width, double starting_health, int number_of_healing_potions, string ally_01, string ally_02, string ally_03, string enemy_01, string enemy_02, string enemy_03, bool onoff);
	
		void update();
		void draw(sf::RenderWindow* window);
		void set_need_to_update_tile_size(double new_tile_size);
		void set_screen_parameters(int width, int height, sf::RenderWindow * window);
		void register_input(char key);
		void cast_ability(int i);
		void enemy_cast_ability(int i);
		void assign_enemy_ability(int i);
		void battle_play_sound_effect(string sound_type);
		void set_sound_effects(bool onoff);

		sf::Sprite getSprite(string character, string mode, string action, int j, int frame);

		int get_morgans_health();
		int get_number_of_healing_potions() {
			return number_of_healing_potions;
		}

		bool get_battle_finished() {
			return battle_finished;
		}
		bool get_game_over() {
			return game_over;
		}

		string get_type() {
			return the_type;
		}

		sf::Font font;

	private:

		bool need_to_issue_command;
		bool need_to_select_ability;
		bool need_to_select_potion;
		bool need_to_select_enemy;
		bool need_to_update_tile_size;
		bool victory_screen;
		bool battle_finished;
		bool game_over;
		bool sound_effects_on;

		vector <Combatant> allies;
		vector <Combatant> enemies;

		int window_width;
		int window_height;
		int tileSize;
		int new_tile_size;
		int ally_to_give_command;
		int action_currently_selected;
		int ability_currently_selected;
		int potion_currently_selected;
		int enemy_currently_selected;
		int number_of_healing_potions;

		string the_type;

		double cooldown_on_change_ability;
		double victory_screen_count;
};
