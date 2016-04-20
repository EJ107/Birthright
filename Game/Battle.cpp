#include "stdafx.h"
#include "Battle.h"
#include "Combatant.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <iomanip>
#include <random>

sf::Texture combatant_texture;
sf::Texture combatant_icon_texture;
sf::Texture the_enemy_texture;
sf::Texture enemy_attack_texture;
sf::Texture icon_texture;
sf::Texture action_icon_texture;
sf::Texture morgan_icon_battle_texture;
sf::Texture enemy_icon_texture;
sf::Texture slowed_texture;
sf::Texture skull_texture;
sf::Texture battle_podium_texture;

sf::Sound battle_button_sound;
sf::SoundBuffer sound_battle_button;

sf::Sound battle_back_sound;
sf::SoundBuffer sound_battle_back;

sf::Sound slash_sound;
sf::SoundBuffer sound_slash;

sf::Sound slow_sound;
sf::SoundBuffer sound_slow;

sf::Sound battle_potion_sound;
sf::SoundBuffer sound_battle_potion;

sf::Sound wolf_injured_sound;
sf::SoundBuffer sound_wolf_injured_01;
sf::SoundBuffer sound_wolf_injured_02;

sf::Sound wolf_attack_sound;
sf::SoundBuffer sound_wolf_attack_01;
sf::SoundBuffer sound_wolf_attack_02;

sf::Sound battle_pain_sound;
sf::SoundBuffer sound_battle_pain_01;
sf::SoundBuffer sound_battle_pain_02;
sf::SoundBuffer sound_battle_pain_03;
sf::SoundBuffer sound_battle_pain_04;
sf::SoundBuffer sound_battle_pain_05;

int combatant_animation_frame = 1;
int combatant_animation_cap = 6;
int the_enemy_animation_frame = 1;
int the_enemy_animation_cap = 4;
int enemy_attack_animation_frame = 0; 

double combatant_animation_count = 0;
double the_enemy_animation_count = 0;
double enemy_attack_animation_count = 0;

bool attack_temp = false;
bool ally_sprite_change = true;
bool the_enemy_sprite_change = true;
bool enemy_attack_sprite_change = false;

string tmp_ability_selected;

vector <string> actions;
vector <string> potions;

// play a sound effect
void Battle::battle_play_sound_effect(string sound_type) {

	if(sound_effects_on == true) {

		if(sound_type == "button") {
			battle_button_sound.setBuffer(sound_battle_button);
			battle_button_sound.setVolume(80);
			battle_button_sound.play();	
		}
		else if(sound_type == "back") {
			battle_back_sound.setBuffer(sound_battle_back);
			battle_back_sound.setVolume(80);
			battle_back_sound.play();	
		}
		else if(sound_type == "slash") {
			slash_sound.setBuffer(sound_slash);
			slash_sound.setVolume(40);
			slash_sound.play();	
		}
		else if(sound_type == "slow") {
			slow_sound.setBuffer(sound_slow);
			slow_sound.setVolume(40);
			slow_sound.play();	
		}
		else if(sound_type == "potion") {
			battle_potion_sound.setBuffer(sound_battle_potion);
			battle_potion_sound.setVolume(60);
			battle_potion_sound.play();	
		}
		else if(sound_type == "wolf injured") {
			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1,2); 
			float rand = dist6(rng);

			wolf_injured_sound.setVolume(40);

			if(rand == 1) {
				wolf_injured_sound.setBuffer(sound_wolf_injured_01);
				wolf_injured_sound.play();
			}
			else if(rand == 2) {
				wolf_injured_sound.setBuffer(sound_wolf_injured_02);
				wolf_injured_sound.play();
			}
		}
		else if(sound_type == "wolf attack") {
			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1,2); 
			float rand = dist6(rng);

			wolf_attack_sound.setVolume(40);

			if(rand == 1) {
				wolf_attack_sound.setBuffer(sound_wolf_attack_01);
				wolf_attack_sound.play();
			}
			else if(rand == 2) {
				wolf_attack_sound.setBuffer(sound_wolf_attack_02);
				wolf_attack_sound.play();
			}
		}
		else if(sound_type == "morgan injured") {
			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1,5); 
			float rand = dist6(rng);

			battle_pain_sound.setVolume(40);

			if(rand == 1) {
				battle_pain_sound.setBuffer(sound_battle_pain_01);
				battle_pain_sound.play();
			}
			else if(rand == 2) {
				battle_pain_sound.setBuffer(sound_battle_pain_02);
				battle_pain_sound.play();
			}
			else if(rand == 3) {
				battle_pain_sound.setBuffer(sound_battle_pain_03);
				battle_pain_sound.play();
			}
			else if(rand == 4) {
				battle_pain_sound.setBuffer(sound_battle_pain_04);
				battle_pain_sound.play();
			}
			else if(rand == 5) {
				battle_pain_sound.setBuffer(sound_battle_pain_05);
				battle_pain_sound.play();
			}
		}
	}
}

// return a textured sprite
sf::Sprite Battle::getSprite(string character, string mode, string action, int j, int frame) {
	sf::Sprite player;
	string filepath = "";

	if(character == "Frostbeast") {

		// ENEMY ATTACK
		if(action == "attack") {

			if(enemy_attack_animation_frame == 7) {
				enemies[j].set_attacking(false);
				enemies[j].set_cast(0.0);
				attack_temp = false;
				assign_enemy_ability(j);
			}
			else if(enemy_attack_animation_frame == 5) {

				for(int i = 0; i < 3; i++) {

					if(enemies[i].get_attacking() == true && attack_temp == false) {

						if(enemies[i].getAbilitySelected() == "Quick Attack") {

							if(the_type == "boss") {

								if(j == 1) {
									allies[enemies[i].getEnemySelected()].damage(100);
								}
								else {
									allies[enemies[i].getEnemySelected()].damage(60);
								}
							}
							else {
								allies[enemies[i].getEnemySelected()].damage(100);
							}
						}
						else if(enemies[i].getAbilitySelected() == "Slow Attack") {
							allies[enemies[i].getEnemySelected()].damage(270);
						}
						
						battle_play_sound_effect("morgan injured");
						attack_temp = true;
					}
				}	
			}

			filepath = ("Resources/Sprites/Frostbeast/attack_0" + to_string(static_cast<long long>(enemy_attack_animation_frame)) + ".png");

			if(enemy_attack_sprite_change == true) {

				// LOAD THE BASE ENEMY TEXTURE
				if (!enemy_attack_texture.loadFromFile(filepath)) {
					// error...
				}
				enemy_attack_texture.setSmooth(true);
				enemy_attack_sprite_change = false;
			}

			player.setTexture(enemy_attack_texture);

			if(the_type == "boss") {

				if(j == 1) {
					player.setScale(sf::Vector2f((1.3f * ((double)window_width / 1920)), (1.3f * ((double)window_width / 1920))));
				}
				else {
					player.setScale(sf::Vector2f((0.7f * ((double)window_width / 1920)), (0.7f * ((double)window_width / 1920))));
				}
			}
			else {
				player.setScale(sf::Vector2f((0.9f * ((double)window_width / 1920)), (0.9f * ((double)window_width / 1920))));
			}
		}
		// ENEMY IDLE
		else {
			int the_enemy_animation_cap = 4;
			filepath = ("Resources/Sprites/Frostbeast/idle_0" + to_string(static_cast<long long>(the_enemy_animation_frame)) + ".png");

			if(the_enemy_sprite_change == true) {

				// LOAD THE BASE ENEMY TEXTURE
				if (!the_enemy_texture.loadFromFile(filepath)) {
					// error...
				}
				the_enemy_texture.setSmooth(true);
				the_enemy_sprite_change = false;
			}

			if(action == "attack") {
				player.setTexture(enemy_attack_texture);
			}

			player.setTexture(the_enemy_texture);

			if(the_type == "boss") {

				if(j == 1) {
					player.setScale(sf::Vector2f((1.3f * ((double)window_width / 1920)), (1.3f * ((double)window_width / 1920))));
				}
				else {
					player.setScale(sf::Vector2f((0.7f * ((double)window_width / 1920)), (0.7f * ((double)window_width / 1920))));
				}
			}
			else {
				player.setScale(sf::Vector2f((0.9f * ((double)window_width / 1920)), (0.9f * ((double)window_width / 1920))));
			}
		}
	}
	else if(character == "Morgan") {

		if(true == victory_screen) {
			filepath = ("Resources/Sprites/Morgan/morgan_idle_01.png");
			ally_sprite_change = true;
		}
		else if(action == "slash") {

			if(combatant_animation_frame == 8) {
				for(int i = 0; i < 3; i++) {
					allies[i].set_attacking(false);
					allies[i].set_cast(0.0);
					need_to_issue_command = true;
				}

				combatant_animation_cap = 6;
				combatant_animation_frame = 1;
				attack_temp = false;
			}
			else if(combatant_animation_frame == 4) {

				for(int i = 0; i < 3; i++) {

					if(allies[i].get_attacking() == true && attack_temp == false) {

						// will the attack crit?
						std::mt19937 rng;
						rng.seed(std::random_device()());
						std::uniform_int_distribution<std::mt19937::result_type> dist6(0,3); 

						float rand = dist6(rng);

						// crit
						if(rand == 0) {
							enemies[allies[i].getEnemySelected()].damage(330);
							enemies[allies[i].getEnemySelected()].set_being_crit(true);
						}
						// normal
						else {
							enemies[allies[i].getEnemySelected()].damage(220);
						}

						battle_play_sound_effect("wolf injured");
						attack_temp = true;
					}
				}	
			}

			if(false == allies[1].get_attacking()) {
				filepath = ("Resources/Sprites/Morgan/morgan_idle_0" + to_string(static_cast<long long>(combatant_animation_frame)) + ".png");
			}
			else {
				filepath = ("Resources/Sprites/Morgan/morgan_slash_0" + to_string(static_cast<long long>(combatant_animation_frame)) + ".png");
			}
		}
		else {
			int combatant_animation_cap = 6;
			filepath = ("Resources/Sprites/Morgan/morgan_idle_0" + to_string(static_cast<long long>(combatant_animation_frame)) + ".png");
		}

		if(ally_sprite_change == true) {

			// LOAD THE BASE PLAYER TEXTURE
			if (!combatant_texture.loadFromFile(filepath)) {
				// error...
			}

			ally_sprite_change = false;
		}

		combatant_texture.setSmooth(true);
		player.setTexture(combatant_texture);
		player.setScale(sf::Vector2f((-0.9f * ((double)window_width / 1920)), (0.9f * ((double)window_width / 1920))));
	}

	return player;
}

// turn sound effects on or off
void Battle::set_sound_effects(bool onoff) {
	sound_effects_on = onoff;
}

// constructor
Battle::Battle(int the_tile_size, int width, int height, double starting_health, int starting_number_of_healing_potions, string ally_01, string ally_02, string ally_03, string enemy_01, string enemy_02, string type, bool onoff) {
	tileSize = the_tile_size;
	battle_finished = false;
	the_type = type;

	allies.push_back(*(new Combatant("", 1000, 1.0, 1000, 3.2, true, true)));
	allies.push_back(*(new Combatant("Morgan", 920, starting_health, 870, 1.4, true, false)));
	allies.push_back(*(new Combatant("", 1000, 1.0, 1000, 1.6, true, true)));

	if(type == "single") {
		enemies.push_back(*(new Combatant("", 1000, 1.0, 1000, 1.8, false, true)));
		enemies.push_back(*(new Combatant("Enemy", 760, 1.0, 690, 1, false, false)));
		enemies.push_back(*(new Combatant("", 1000, 1.0, 1000, 0.2, false, true)));
	}
	else if(type == "double") {
		enemies.push_back(*(new Combatant("", 1000, 1.0, 1000, 1.8, false, true)));
		enemies.push_back(*(new Combatant("Enemy", 680, 1.0, 540, 1, false, false)));
		enemies.push_back(*(new Combatant("Enemy", 680, 1.0, 540, 0.6, false, false)));
	}
	else if(type == "boss") {
		enemies.push_back(*(new Combatant("Enemy", 220, 1.0, 1000, 1.8, false, false)));
		enemies.push_back(*(new Combatant("Enemy", 980, 1.0, 540, 1, false, false)));
		enemies.push_back(*(new Combatant("Enemy", 220, 1.0, 540, 0.6, false, false)));
	}

	window_width = width;
	window_height = height;

	need_to_issue_command = false;
	need_to_select_enemy = false;
	game_over = false;
	victory_screen = false;
	sound_effects_on = onoff;

	ally_to_give_command = 0;
	action_currently_selected = 0;
	ability_currently_selected = 0;
	potion_currently_selected = 0;
	enemy_currently_selected = 1;
	cooldown_on_change_ability = 0;
	victory_screen_count = 0;

	number_of_healing_potions = starting_number_of_healing_potions;

	font.loadFromFile("arial.ttf");

	actions.push_back("Attack");
	actions.push_back("Potions");
	actions.push_back("Defend");
	potions.push_back("Healing Potion");

	if (!enemy_icon_texture.loadFromFile("Resources/Sprites/Frostbeast/frostbeast_icon_small.png")) {
		// error...
	}
	if (!morgan_icon_battle_texture.loadFromFile("Resources/Sprites/Morgan/morgan_icon_small.png")) {
		// error...
	}
	if (!slowed_texture.loadFromFile("Resources/Sprites/Icons/icon_slowed.png")) {
		// error...
	}
	if (!skull_texture.loadFromFile("Resources/Sprites/skull_01.png")) {
		// error...
	}
	if (!battle_podium_texture.loadFromFile("Resources/Sprites/battle_podium.png")) {
		// error...
	}

	// LOAD THE SOUND EFFECTS ------------------------------------------------------------------------
	if (!sound_battle_button.loadFromFile("Resources/Sounds/button.wav")) {
		// error
	}
	if (!sound_battle_back.loadFromFile("Resources/Sounds/back.wav")) {
		// error
	}
	if (!sound_slash.loadFromFile("Resources/Sounds/slash.wav")) {
		// error
	}
	if (!sound_slow.loadFromFile("Resources/Sounds/slow.wav")) {
		// error
	}
	if (!sound_battle_potion.loadFromFile("Resources/Sounds/potion.wav")) {
		// error
	}
	if (!sound_wolf_injured_01.loadFromFile("Resources/Sounds/wolf_injured_01.wav")) {
		// error
	}
	if (!sound_wolf_injured_02.loadFromFile("Resources/Sounds/wolf_injured_02.wav")) {
		// error
	}
	if (!sound_wolf_attack_01.loadFromFile("Resources/Sounds/wolf_attack_01.wav")) {
		// error
	}
	if (!sound_wolf_attack_02.loadFromFile("Resources/Sounds/wolf_attack_02.wav")) {
		// error
	}
	if (!sound_battle_pain_01.loadFromFile("Resources/Sounds/pain_01.wav")) {
		// error
	}
	if (!sound_battle_pain_02.loadFromFile("Resources/Sounds/pain_02.wav")) {
		// error
	}
	if (!sound_battle_pain_03.loadFromFile("Resources/Sounds/pain_03.wav")) {
		// error
	}
	if (!sound_battle_pain_04.loadFromFile("Resources/Sounds/pain_04.wav")) {
		// error
	}
	if (!sound_battle_pain_05.loadFromFile("Resources/Sounds/pain_05.wav")) {
		// error
	}
}

// Update the battle 
void Battle::update() {

	// end the battle if all enemies have been defeated
	bool all_knocked_out = true;

	for(int i = 0; i < enemies.size(); i++) {
		
		// HANDLE CRIT ANIMATION
		if(enemies[i].get_being_crit() == true) {

			// update the count
			enemies[i].set_being_crit_count(enemies[i].get_crit_count() + 0.016);

			if(enemies[i].get_crit_count() > 0.5) {
				enemies[i].set_being_crit_count(0);
				enemies[i].set_being_crit(false);
			}
		}

		if(enemies[i].get_knocked_out() == false) {
			all_knocked_out = false;
		}
	}

	if(true == victory_screen) {
		victory_screen_count += 0.016;

		if(victory_screen_count > 3.0) {

			combatant_animation_frame = 1;
			combatant_animation_cap = 6;
			the_enemy_animation_frame = 1;
			the_enemy_animation_cap = 4;
			enemy_attack_animation_frame = 0; 
			attack_temp = false;

			victory_screen_count = 0;
			battle_finished = true;
		}
	} 
	else if(true == all_knocked_out) {
		victory_screen = true;
		allies[1].set_attacking(false);
	}
	else {
		// if the enemy selected is knocked out select the next enemy
		while(enemies[enemy_currently_selected].get_knocked_out() == true) {

			enemy_currently_selected -= 1;

			if(enemy_currently_selected < 0) {
				enemy_currently_selected = enemies.size() - 1;
			}
		}

		// make it so that you can't change the ability at an insane speed
		if(cooldown_on_change_ability > 0) {
			cooldown_on_change_ability -= 0.016;
			if(cooldown_on_change_ability < 0) {
				cooldown_on_change_ability = 0;
			}
		}

		bool tmp2 = false;

		for(int i = 0; i < allies.size(); i++) {
			if(allies[i].get_attacking() == true) {
				tmp2 = true;
			}
		}
		for(int i = 0; i < enemies.size(); i++) {
			if(enemies[i].get_attacking() == true) {
				tmp2 = true;
			}
		}

		if(true == need_to_issue_command || tmp2 == true) {

		}
		else {

			for(int i = 0; i < 3; i++) {

				if(i < allies.size()) {
					if(false == allies[i].get_knocked_out()) {
						allies[i].update();
					}
				}
				if(i < enemies.size()) {
					if(false == enemies[i].get_knocked_out()) {
						enemies[i].update();
					}
				}
			}

			// check if an ally is ready to cast an ability
			for(int i = 0; i < 3; i++) {

				if(i < allies.size()) {

					if(true == allies[i].get_need_to_cast()) {

						allies[i].set_defend(false);
						ally_to_give_command = i;
						
						// CAST THE ABILITY
						cast_ability(i);

						break;
					}
				}
			}

			// check if an enemy is ready to cast an ability
			for(int i = 0; i < 3; i++) {
				if(i < enemies.size()) {

					// HANDLE SLOW
					if(enemies[i].get_slowed() == true) {

						// update the count
						enemies[i].set_slowed_count(enemies[i].get_slowed_count() + 0.016);

						if(enemies[i].get_slowed_count() > 5.5) {
							enemies[i].set_slowed_count(0);
							enemies[i].set_slowed(false);
						}
					}

					if(true == enemies[i].get_need_to_cast()) {

						enemies[i].set_defend(false);
						enemies[i].set_need_to_cast();

						// CAST THE ABILITY
						enemy_cast_ability(i);

						break;
					}
				}
			}
		}

		// end the game if all allies have been defeated
		bool all_allies_knocked_out = true;

		for(int i = 0; i < allies.size(); i++) {
			if(allies[i].get_knocked_out() == false) {
				all_allies_knocked_out = false;
			}
		}

		if(true == all_allies_knocked_out) {
			game_over = true;
		}

		// ANIMATION ------------------------------------

		// ALLIES
		bool tmp = false;

		for(int i = 0; i < allies.size(); i++) {
			if(allies[i].get_attacking() == true) {
				tmp = true;
			}
		}

		if(tmp) {
			combatant_animation_count += 0.04;
		}
		else {
			combatant_animation_count += 0.02;
		}

		if(combatant_animation_count > 0.3) {

			ally_sprite_change = true;

			// reset the animation
			combatant_animation_count = 0 + (combatant_animation_count - 0.3);

			combatant_animation_frame ++;

			if(combatant_animation_frame > combatant_animation_cap) {
				if(!tmp) {
					combatant_animation_frame = 1;
				}		
			}
		}

		// ENEMIES
		enemy_attack_animation_count += 0.06;
		the_enemy_animation_count += 0.02;

		if(the_enemy_animation_count > 0.3) {

			the_enemy_sprite_change = true;

			// reset the animation
			the_enemy_animation_count = 0 + (the_enemy_animation_count - 0.3);
			the_enemy_animation_frame ++;

			if(the_enemy_animation_frame > 4) {
				the_enemy_animation_frame = 1;
			}
		}
		if(enemy_attack_animation_count > 0.3) {

			enemy_attack_sprite_change = true;

			enemy_attack_animation_count = 0 + (the_enemy_animation_count - 0.3);
			enemy_attack_animation_frame++;

			if(enemy_attack_animation_frame > 7) {
				enemy_attack_animation_frame = 0;
			}
		}
	}
}

void Battle::draw(sf::RenderWindow* window) {

	// ---------------------------
	//	   DRAW ENEMY PODIUMS	 |
	// ---------------------------
	for(int i = 0; i < 3; i++) {
		if(i < enemies.size()) {
			if(false == enemies[i].get_knocked_out()) {
				int enemy_x = 0;
				int enemy_y = 0;
				int enemy_position_x = 0;
				int enemy_position_y = 0;

				switch(i) {
					case 0:
						
						if(the_type == "boss") {
							enemy_position_x = window_width - (window_width / 3.3) * 0.7;
							enemy_x =  window_width - (window_width / 8.5);
						}
						else {
							enemy_position_x = window_width - (window_width / 3.3) * 1.05;
							enemy_x = window_width - (window_width / 10);
						}

						enemy_position_y = ((window_height / 10) * 1.5) + (tileSize / 5);
						enemy_y = (window_height / 10) * 1.5;				
						break;
						
					case 1:

						enemy_position_y = ((window_height / 10) * 3.5) + (tileSize / 5);

						if(the_type == "double") {
							enemy_x = window_width - (window_width / 10) * 3;
							enemy_position_x = window_width - (window_width / 3.3) * 1.4;	
						}

						else if(the_type == "boss") {
							enemy_x = window_width - (window_width / 10) * 3;
							enemy_position_x = window_width - (window_width / 3.3) * 1.6;	
							enemy_position_y = ((window_height / 10) * 3.5) - (tileSize);
						}

						else {
							enemy_x = window_width - (window_width / 10) * 2;
							enemy_position_x = window_width - (window_width / 3.3) * 1.05;
						}

						enemy_y = (window_height / 10) * 3.5;
						break;

					case 2:

						if(the_type == "double") {
							enemy_x = window_width - (window_width / 10) * 1.6;
							enemy_position_x = window_width - (window_width / 3.3) * 0.94;	
						}
						else if(the_type == "boss") {
							enemy_position_x = window_width - (window_width / 3.3) * 0.7;
							enemy_x =  window_width - (window_width / 8.5);
						}
						else {
							enemy_x = window_width - (window_width / 10);
							enemy_position_x = window_width - (window_width / 3.3) * 1.05;
						}

						enemy_position_y = ((window_height / 10) * 5.5) + (tileSize / 5);
						enemy_y = (window_height / 10) * 5.5;
						break;
				}

				// DRAW THE PODIUM
				sf::Sprite podium;
				podium.setTexture(battle_podium_texture);

				if(the_type == "boss") {

					if(i == 1) {
						podium.setPosition(enemy_x - (tileSize * 2.8), enemy_y + (tileSize * 3.4));
						podium.setScale(sf::Vector2f((1.5f * ((double)window_width / 1920)), (1.2f * ((double)window_width / 1920))));	
					}
					else {
						podium.setPosition(enemy_x - (tileSize * 2.0), enemy_y + (tileSize * 2.5));
						podium.setScale(sf::Vector2f((1.0f * ((double)window_width / 1920)), (0.8f * ((double)window_width / 1920))));	
					}
				}
				else {
					podium.setPosition(enemy_x - (tileSize * 2.0), enemy_y + (tileSize * 3.0));
					podium.setScale(sf::Vector2f((1.1f * ((double)window_width / 1920)), (0.8f * ((double)window_width / 1920))));	
				}

				window->draw(podium);
			}
		}
	}

	// ---------------------------
	//	   DRAW THE CAST BAR	 |
	// ---------------------------
	sf::RectangleShape cast_bar(sf::Vector2f((window_width / 3), (window_height / 20)));
	cast_bar.setFillColor(sf::Color(255,255,102));

	cast_bar.setOutlineThickness(3);
	cast_bar.setOutlineColor(sf::Color(0, 0, 0));

	cast_bar.setPosition((window_width / 3), window_height - (window_height / 5));
	window->draw(cast_bar);

	for(int i = 0; i < 3; i++) {
		
		// ---------------------------------
		//          draw the ally		   |
		// ---------------------------------
		if(i < allies.size()) {

			if(i == 1) {

				sf::Sprite ally;
				double multiplier = 2.5;

				// draw the ally
				if(true == allies[i].get_attacking()) {
					ally = getSprite("Morgan", "", "slash", 0, 0);

					if(combatant_animation_frame == 1) {

					}
					else {
						multiplier = 3.6;
					}
				}
				else {
					ally = getSprite("Morgan", "", "", 0, 0);
				}

				int ally_x = 0;
				int ally_y = 0;

				switch(i) {
					case 0:
						ally_x = (window_width / 10);
						ally_y = (window_height / 10) * 1.5;				
						break;

					case 1:
						ally_x = ((window_width / 10) * 2);
						ally_y = (window_height / 10) * 3.5;
						break;

					case 2:
						ally_x = (window_width / 10);
						ally_y = (window_height / 10) * 5.5;
						break;
				}

				// DRAW THE PODIUM
				sf::Sprite podium;
				podium.setTexture(battle_podium_texture);
				podium.setPosition(ally_x - (tileSize * 1.6), ally_y + (tileSize * 3.0));
				podium.setScale(sf::Vector2f((1.0f * ((double)window_width / 1920)), (0.8f * ((double)window_width / 1920))));
				window->draw(podium);

				if(false == allies[i].get_knocked_out()) {

					ally.setPosition((ally_x + (tileSize * multiplier)), ally_y);
					window->draw(ally);

					// -----------------------------------------------------
					// draw the 'defend' bubble if the ally is defending   |
					// -----------------------------------------------------
					if(true == allies[i].get_is_defending()) {

						sf::CircleShape defend_bubble(tileSize * 1.9);
						defend_bubble.setFillColor(sf::Color(0,0,60,50));

						defend_bubble.setOutlineThickness(tileSize / 15);
						defend_bubble.setOutlineColor(sf::Color(50, 50, 100, 100));

						defend_bubble.setPosition(ally_x - (tileSize * 1.3), ally_y + (tileSize * 0.1));
						window->draw(defend_bubble);
					}
				}

				else {
					sf::Sprite skull;
					skull_texture.setSmooth(true);
					skull.setTexture(skull_texture);
					skull.setScale(sf::Vector2f((0.7f * ((double)window_width / 1920)), (0.7f * ((double)window_width / 1920))));
					skull.setPosition((((window_width / 10) * 2) - (tileSize * 0.95)), ((window_height / 10) * 2) + (tileSize * 2.5));
					window->draw(skull);
				}

				// ----------------------------------------------------
				// draw the health bar and energy bar of this ally    |
				// ----------------------------------------------------

				// draw the outlines of the boxes:
				sf::RectangleShape ally_health_outline(sf::Vector2f((tileSize * 2), (tileSize / 3)));

				ally_health_outline.setPosition((ally_x - (tileSize / 2)), (ally_y - tileSize));
				ally_health_outline.setOutlineThickness(tileSize / 20);
				ally_health_outline.setOutlineColor(sf::Color(0, 0, 0));

				sf::RectangleShape ally_energy_outline(sf::Vector2f((tileSize * 2), (tileSize / 3)));

				ally_energy_outline.setPosition((ally_x - (tileSize / 2)), (ally_y - (tileSize / 2)));
				ally_energy_outline.setOutlineThickness(tileSize / 20);
				ally_energy_outline.setOutlineColor(sf::Color(0, 0, 0));
				
				// draw the amount of health/energy:
				double health_percentage = ((double)allies[i].getHealth() / allies[i].getMaxHealth());
				double energy_percentage = ((double)allies[i].getEnergy() / allies[i].getMaxEnergy());

				sf::RectangleShape ally_health(sf::Vector2f(((tileSize * 2) * health_percentage), (tileSize / 3)));

				ally_health.setPosition((ally_x - (tileSize / 2)), (ally_y - tileSize));
				ally_health.setFillColor(sf::Color(200, 0, 0));

				sf::RectangleShape ally_energy(sf::Vector2f((tileSize * 2) * energy_percentage, (tileSize / 3)));

				ally_energy.setPosition((ally_x - (tileSize / 2)), (ally_y - (tileSize / 2)));
				ally_energy.setFillColor(sf::Color(100, 100, 250));
				
				window->draw(ally_health_outline);
				window->draw(ally_energy_outline);

				window->draw(ally_health);
				window->draw(ally_energy);

				// draw the health/energy text
				sf::Text text;
				text.setFont(font); 
				text.setColor(sf::Color::Black);
				text.setCharacterSize(tileSize * 0.25); 
				text.setStyle(sf::Text::Regular);

				string health_string = to_string(static_cast<long long>(allies[i].getHealth())) + " / " + to_string(static_cast<long long>(allies[i].getMaxHealth()));
				string energy_string = to_string(static_cast<long long>(allies[i].getEnergy())) + " / " + to_string(static_cast<long long>(allies[i].getMaxEnergy()));

				text.setString(health_string);
				text.setPosition((ally_x - (tileSize / 30)), (ally_y - tileSize));
				window->draw(text);

				text.setString(energy_string);
				text.setPosition((ally_x - (tileSize / 30)), (ally_y - (tileSize / 2)));
				window->draw(text);
				
				// draw the marker on the cast bar
				double cast_percentage = (allies[i].getCast() / allies[i].getMaxCast());
	
				// draw the line:
				sf::RectangleShape cast_line(sf::Vector2f((tileSize / 15), (window_height / 10)));
				cast_line.setFillColor(sf::Color(0,0,0));
				cast_line.setPosition((window_width / 3) + ((window_width / 3) * cast_percentage) - (tileSize / 30), window_height - (window_height / 5) - (window_height / 20));
				window->draw(cast_line);

				// draw the icon:
				if(allies[i].getName() == "Morgan") {

					sf::Sprite icon;
					morgan_icon_battle_texture.setSmooth(true);
					icon.setTexture(morgan_icon_battle_texture);
					icon.setScale(sf::Vector2f((1.0f * ((double)window_width / 1920)), (1.0f * ((double)window_width / 1920))));
					icon.setPosition((window_width / 3) + ((window_width / 3) * cast_percentage) - (tileSize / 1.4) + 1.5, window_height - (window_height / 5) - (window_height / 10));
					window->draw(icon);
				}
				else {
					sf::CircleShape icon(tileSize / 2);
					icon.setFillColor(sf::Color(100,100,100));
					icon.setPosition((window_width / 3) + ((window_width / 3) * cast_percentage) - (tileSize / 2) + 1.5, window_height - (window_height / 5) - (window_height / 10));
					window->draw(icon);
				}
			}
		}

		// -----------------------
		// draw the enemy		 |
		// -----------------------
		if(i < enemies.size()) {

			if(false == enemies[i].get_knocked_out()) {

				// draw the enemy
				sf::Sprite enemy;

				if(true == enemies[i].get_attacking()) {
					enemy = getSprite("Frostbeast", "", "attack", i, 0);
				}
				else {
					enemy = getSprite("Frostbeast", "", "", i, 0);
				}

				int enemy_x = 0;
				int enemy_y = 0;
				int enemy_position_x = 0;
				int enemy_position_y = 0;

				switch(i) {
					case 0:
						
						if(the_type == "boss") {
							enemy_position_x = window_width - (window_width / 3.3) * 0.7;
							enemy_x =  window_width - (window_width / 8.5);
						}
						else {
							enemy_position_x = window_width - (window_width / 3.3) * 1.05;
							enemy_x = window_width - (window_width / 10);
						}

						enemy_position_y = ((window_height / 10) * 1.5) + (tileSize / 5);
						enemy_y = (window_height / 10) * 1.5;				
						break;
						
					case 1:

						enemy_position_y = ((window_height / 10) * 3.5) + (tileSize / 5);

						if(the_type == "double") {
							enemy_x = window_width - (window_width / 10) * 3;
							enemy_position_x = window_width - (window_width / 3.3) * 1.4;	
						}

						else if(the_type == "boss") {
							enemy_x = window_width - (window_width / 10) * 3;
							enemy_position_x = window_width - (window_width / 3.3) * 1.6;	
							enemy_position_y = ((window_height / 10) * 3.5) - (tileSize);
						}

						else {
							enemy_x = window_width - (window_width / 10) * 2;
							enemy_position_x = window_width - (window_width / 3.3) * 1.05;
						}

						enemy_y = (window_height / 10) * 3.5;
						break;

					case 2:

						if(the_type == "double") {
							enemy_x = window_width - (window_width / 10) * 1.6;
							enemy_position_x = window_width - (window_width / 3.3) * 0.94;	
						}
						else if(the_type == "boss") {
							enemy_position_x = window_width - (window_width / 3.3) * 0.7;
							enemy_x =  window_width - (window_width / 8.5);
						}
						else {
							enemy_x = window_width - (window_width / 10);
							enemy_position_x = window_width - (window_width / 3.3) * 1.05;
						}

						enemy_position_y = ((window_height / 10) * 5.5) + (tileSize / 5);
						enemy_y = (window_height / 10) * 5.5;
						break;
				}

				enemy.setPosition(enemy_position_x, enemy_position_y);
				window->draw(enemy);

				// -----------------------------------------------------
				// draw the 'defend' bubble if the enemy is defending  |
				// -----------------------------------------------------
				if(true == enemies[i].get_is_defending()) {

					sf::CircleShape defend_bubble(tileSize * 2.2);

					if(the_type == "boss") {

						if(i == 1) {
							defend_bubble.setRadius(tileSize * 3.0);
							defend_bubble.setPosition(enemy_x - (tileSize * 2.6), enemy_y - (tileSize * 0.75));
						}
						else {
							defend_bubble.setRadius(tileSize * 1.7);
							defend_bubble.setPosition(enemy_x - (tileSize * 1.4), enemy_y + (tileSize * 0.125));
						}
					}
					else {
						defend_bubble.setPosition(enemy_x - (tileSize * 1.775), enemy_y + (tileSize * 0.125));
					}

					defend_bubble.setFillColor(sf::Color(0,0,60,50));
					defend_bubble.setOutlineThickness(tileSize / 15);
					defend_bubble.setOutlineColor(sf::Color(50, 50, 100, 100));
					window->draw(defend_bubble);
				}

				// -----------------------------------------------------
				// draw the health bar and energy bar of this enemy    |
				// -----------------------------------------------------

				// draw the outlines of the boxes:
				sf::RectangleShape enemy_health_outline(sf::Vector2f((tileSize * 2), (tileSize / 3)));

				enemy_health_outline.setPosition((enemy_x - (tileSize / 2)), (enemy_y - tileSize));
				enemy_health_outline.setOutlineThickness(tileSize / 20);
				enemy_health_outline.setOutlineColor(sf::Color(0, 0, 0));

				sf::RectangleShape enemy_energy_outline(sf::Vector2f((tileSize * 2), (tileSize / 3)));

				enemy_energy_outline.setPosition((enemy_x - (tileSize / 2)), (enemy_y - (tileSize / 2)));
				enemy_energy_outline.setOutlineThickness(tileSize / 20);
				enemy_energy_outline.setOutlineColor(sf::Color(0, 0, 0));

				// draw the amount of health/energy:
				double enemy_health_percentage = ((double)enemies[i].getHealth() / enemies[i].getMaxHealth());
				double enemy_energy_percentage = ((double)enemies[i].getEnergy() / enemies[i].getMaxEnergy());

				sf::RectangleShape enemy_health(sf::Vector2f(((tileSize * 2) * enemy_health_percentage), (tileSize / 3)));

				enemy_health.setPosition((enemy_x - (tileSize / 2)), (enemy_y - tileSize));

				// IF BEING CRIT CHANGE THE COLOUR
				int extra = 0;
				if(enemies[i].get_being_crit() == true) {

					double crit_count = enemies[i].get_crit_count();
					if(crit_count < 0.25) {
						extra = crit_count * 200;
					}
					else {
						extra = (0.5 - crit_count) * 200;
					}
				}

				enemy_health.setFillColor(sf::Color((200 + extra), (extra * 2.5), (extra * 2.5)));

				sf::RectangleShape enemy_energy(sf::Vector2f((tileSize * 2) * enemy_energy_percentage, (tileSize / 3)));

				enemy_energy.setPosition((enemy_x - (tileSize / 2)), (enemy_y - (tileSize / 2)));
				enemy_energy.setFillColor(sf::Color(100, 100, 250));

				window->draw(enemy_health_outline);
				window->draw(enemy_energy_outline);

				window->draw(enemy_health);
				window->draw(enemy_energy);
				
				// draw the health/energy text
				sf::Text text;
				text.setFont(font); 
				text.setColor(sf::Color::Black);
				text.setCharacterSize(tileSize * 0.25); 
				text.setStyle(sf::Text::Regular);

				string health_string = to_string(static_cast<long long>(enemies[i].getHealth())) + " / " + to_string(static_cast<long long>(enemies[i].getMaxHealth()));
				string energy_string = to_string(static_cast<long long>(enemies[i].getEnergy())) + " / " + to_string(static_cast<long long>(enemies[i].getMaxEnergy()));

				text.setString(health_string);
				text.setPosition((enemy_x - (tileSize / 30)), (enemy_y - tileSize));
				window->draw(text);

				text.setString(energy_string);
				text.setPosition((enemy_x - (tileSize / 30)), (enemy_y - (tileSize / 2)));
				window->draw(text);

				// draw the marker on the cast bar
				double enemy_cast_percentage = (enemies[i].getCast() / enemies[i].getMaxCast());

				// draw the line
				sf::RectangleShape enemy_cast_line(sf::Vector2f((tileSize / 15), (window_height / 10)));
				enemy_cast_line.setFillColor(sf::Color(0,0,0));
				enemy_cast_line.setPosition((window_width / 3) + ((window_width / 3) * enemy_cast_percentage) - (tileSize / 30), window_height - (window_height / 5));
				window->draw(enemy_cast_line);

				// draw the icon
				sf::Sprite enemy_icon;

				enemy_icon_texture.setSmooth(true);
				enemy_icon.setTexture(enemy_icon_texture);
				enemy_icon.setScale(sf::Vector2f((1.0f * ((double)window_width / 1920)), (1.0f * ((double)window_width / 1920))));
				enemy_icon.setPosition((window_width / 3) + ((window_width / 3) * enemy_cast_percentage) - (tileSize / 1.4) + 1.5, window_height - (window_height / 5) + (window_height / 18));

				window->draw(enemy_icon);

				// SLOWED
				if(enemies[i].get_slowed() == true) {

					sf::Sprite slowed;
					
					slowed_texture.setSmooth(true);
					slowed.setTexture(slowed_texture);
					slowed.setScale(sf::Vector2f((0.4f * ((double)window_width / 1920)), (0.4f * ((double)window_width / 1920))));
					slowed.setPosition((window_width / 3) + ((window_width / 3) * enemy_cast_percentage) - (tileSize / 3) + 1.5, window_height - (window_height / 5) + (window_height / 6.8));
					window->draw(slowed);
				}
			}
		}
	}

	// COMMAND MENU
	if(true == need_to_issue_command) {

		int caster_x = 0;
		int caster_y = 0;

		switch(ally_to_give_command) {
			case 0:
				caster_x = (window_width / 10);
				caster_y = (window_height / 10) * 1.5;				
				break;

			case 1:
				caster_x = (window_width / 10) * 2;
				caster_y = (window_height / 10) * 3.5;
				break;

			case 2:
				caster_x = (window_width / 10);
				caster_y = (window_height / 10) * 5.5;
				break;
		}
		
		// draw the enemy selection items
		if(true == need_to_select_enemy) {

			switch(enemy_currently_selected) {
				case 0:

					if(the_type == "boss") {
						caster_x =  window_width - (window_width / 8.5);
					}
					else {
						caster_x = window_width - (window_width / 10);
					}

					caster_y = (window_height / 10) * 1.5;				
					break;

				case 1:

					if(the_type == "double") {
						caster_x = window_width - (window_width / 10) * 3;
					}
					else if(the_type == "boss") {
						caster_x = window_width - (window_width / 10) * 3;
					}
					else {
						caster_x = window_width - (window_width / 10) * 2;
					}

					caster_y = (window_height / 10) * 3.5;
					break;

				case 2:

					if(the_type == "double") {
						caster_x = window_width - (window_width / 10) * 1.6;
					}
					else if(the_type == "boss") {
						caster_x =  window_width - (window_width / 8.5);
					}
					else {
						caster_x = window_width - (window_width / 10);
					}
					
					caster_y = (window_height / 10) * 5.5;
					break;
			}

			// draw a triangle above the selected enemies head
			sf::CircleShape triangle((tileSize / 2), 3);
			triangle.setFillColor(sf::Color(200,50,50));
			triangle.rotate(180);
			triangle.setPosition(caster_x + tileSize, caster_y - (tileSize * 1.25));
			window->draw(triangle);
		}

		// draw the potion selection items
		else if(true == need_to_select_potion) {

			// Draw the Potion icon
			sf::Sprite icon;

			if (!icon_texture.loadFromFile("Resources/Sprites/Icons/icon_healing_potion.png")) {
				// error...
			}
			
			icon_texture.setSmooth(true);
			icon.setTexture(icon_texture);
			icon.setScale(sf::Vector2f((0.7f * ((double)window_width / 1920)), (0.7f * ((double)window_width / 1920))));
			icon.setPosition(caster_x + (tileSize * 2), caster_y);

			window->draw(icon);
			
			sf::Text text;
			text.setFont(font); 
			text.setCharacterSize(tileSize * 0.5); 
			text.setStyle(sf::Text::Bold);

			// draw the potion text
			text.setColor(sf::Color::Black);
			text.setString(potions[potion_currently_selected] + " x " + to_string(static_cast<long long>(number_of_healing_potions)));
			text.setPosition(caster_x + (tileSize * 3.2), caster_y + (tileSize / 8.2));
			window->draw(text);
		}

		// draw the ability selection items
		else if(true == need_to_select_ability) {

			// Draw the ability icon
			sf::Sprite icon;

			switch(ability_currently_selected) {

				case 0:
					if (!icon_texture.loadFromFile("Resources/Sprites/Icons/icon_slash.png")) {
						// error...
					}
					break;

				case 1:
					if (!icon_texture.loadFromFile("Resources/Sprites/Icons/icon_slow.png")) {
						// error...
					}
					break;

				default: 
					break;
			}

			icon_texture.setSmooth(true);
			icon.setTexture(icon_texture);
			icon.setScale(sf::Vector2f((0.7f * ((double)window_width / 1920)), (0.7f * ((double)window_width / 1920))));
			icon.setPosition(caster_x + (tileSize * 2), caster_y);

			window->draw(icon);

			// draw the energy cost
			sf::Text text;
			text.setFont(font); 
			text.setCharacterSize(tileSize * 0.5); 
			text.setStyle(sf::Text::Bold);

			sf::Sprite energy_icon;
			
			// get the cost of the current ability
			int cost_of_selected_ability = 0;

			if(ability_currently_selected == 0) {
				cost_of_selected_ability = 100;
				text.setPosition(caster_x + (tileSize * 5.3), caster_y + (tileSize / 8.2));
				energy_icon.setPosition(caster_x + (tileSize * 4.7), caster_y + (tileSize / 9));
			}
			else {
				cost_of_selected_ability = 250;
				text.setPosition(caster_x + (tileSize * 5.15), caster_y + (tileSize / 8.2));
				energy_icon.setPosition(caster_x + (tileSize * 4.5), caster_y + (tileSize / 9));
			}

			text.setString(to_string(static_cast<long long>(cost_of_selected_ability)));

			if(allies[ally_to_give_command].getEnergy() >= cost_of_selected_ability) {

				// blue icon, blue text
				text.setColor(sf::Color::Blue);
				
				if (!icon_texture.loadFromFile("Resources/Sprites/Icons/energy_icon_blue.png")) {
					// error...
				}
			}
			else {

				// red icon, red text
				text.setColor(sf::Color::Red);

				if (!icon_texture.loadFromFile("Resources/Sprites/Icons/energy_icon_red.png")) {
					// error...
				}
			}

			energy_icon.setTexture(icon_texture);
			energy_icon.setScale(sf::Vector2f((0.5f * ((double)window_width / 1920)), (0.5f * ((double)window_width / 1920))));

			window->draw(text);
			window->draw(energy_icon);

			// draw the ability text
			text.setColor(sf::Color::Black);
			text.setString(allies[ally_to_give_command].getAbilities()[ability_currently_selected]);
			text.setPosition(caster_x + (tileSize * 3.2), caster_y + (tileSize / 8.2));
			window->draw(text);
		}

		// draw the menu selection items
		else {

			// draw the action icon
			sf::Sprite action_icon;

			switch(action_currently_selected) {

				case 0:
					if (!action_icon_texture.loadFromFile("Resources/Sprites/Icons/icon_attack.png")) {
						// error...
					}
					break;

				case 1:
					if (!action_icon_texture.loadFromFile("Resources/Sprites/Icons/icon_potions.png")) {
						// error...
					}
					break;

				case 2:
					if (!action_icon_texture.loadFromFile("Resources/Sprites/Icons/icon_defend.png")) {
						// error...
					}
					break;

				default: 
					break;
			}

			action_icon_texture.setSmooth(true);
			action_icon.setTexture(action_icon_texture);
			action_icon.setScale(sf::Vector2f((0.7f * ((double)window_width / 1920)), (0.7f * ((double)window_width / 1920))));
			action_icon.setPosition(caster_x + (tileSize * 2), caster_y);

			window->draw(action_icon);

			// draw the action text
			sf::Text text;
			text.setFont(font); 
			text.setColor(sf::Color::Black);
			text.setString(actions[action_currently_selected]);
			text.setCharacterSize(tileSize * 0.5); 
			text.setStyle(sf::Text::Bold);
			text.setPosition(caster_x + (tileSize * 3.2), caster_y + (tileSize / 8.2));
			window->draw(text);
		}
	}

	// draw the victory text
	if(true == victory_screen) {

		// Draw the vectory text
		sf::Sprite victory_text;

		if (!icon_texture.loadFromFile("Resources/Sprites/Icons/victory_text.png")) {
			// error...
		}
			
		icon_texture.setSmooth(true);
		victory_text.setTexture(icon_texture);
		victory_text.setScale(sf::Vector2f((0.6f * ((double)window_width / 1920)), (0.6f * ((double)window_width / 1920))));
		victory_text.setPosition((window_width / 2) - (tileSize * 2.0), (window_height / 7));

		window->draw(victory_text);

		// Draw the potion icon
		sf::Sprite potion_icon;

		if (!icon_texture.loadFromFile("Resources/Sprites/Icons/icon_potion.png")) {
			// error...
		}
			
		icon_texture.setSmooth(true);
		potion_icon.setTexture(icon_texture);
		potion_icon.setScale(sf::Vector2f((0.8f * ((double)window_width / 1920)), (0.8f * ((double)window_width / 1920))));
		potion_icon.setPosition((window_width / 2) - (tileSize * 1.0), (window_height / 4.7));

		window->draw(potion_icon);

		// draw the text
		sf::Text text;
		text.setFont(font); 
		text.setColor(sf::Color::Black);
		text.setString(" + 1");
		text.setCharacterSize(tileSize * 0.4); 
		text.setStyle(sf::Text::Bold);
		text.setPosition((window_width / 2) - (tileSize * 0.2), (window_height / 4.7) + (tileSize * 0.32));
		window->draw(text);
	}
}

void Battle::register_input(char key) {

	if(true == need_to_issue_command) {

		// 'w' key -> move up in lists/menus
		if(key == 'w') {

			// change enemy selected
			if(true == need_to_select_enemy) {

				enemy_currently_selected -= 1;

				if(enemy_currently_selected < 0) {
					enemy_currently_selected = enemies.size() - 1;
				}

				while(enemies[enemy_currently_selected].get_knocked_out() == true) {

					enemy_currently_selected -= 1;

					if(enemy_currently_selected < 0) {
						enemy_currently_selected = enemies.size() - 1;
					}
				}
			}

			else if(true == need_to_select_ability) {

				ability_currently_selected -= 1;

				if(ability_currently_selected < 0) {
					ability_currently_selected = allies[ally_to_give_command].getAbilities().size() - 1;
				}
			}

			else if(true == need_to_select_potion) {

			}

			// change ability selected
			else {

				action_currently_selected += 1;

				if(action_currently_selected > 2) {
					action_currently_selected = 0;
				}
			}
		}

		// 's' key -> move up in lists/menus
		else if(key == 's') {

			// change enemy selected
			if(true == need_to_select_enemy) {

				enemy_currently_selected += 1;

				if(enemy_currently_selected > (enemies.size() - 1)) {
					enemy_currently_selected = 0;
				}

				while(enemies[enemy_currently_selected].get_knocked_out() == true) {

					enemy_currently_selected += 1;

					if(enemy_currently_selected > (enemies.size() - 1)) {
						enemy_currently_selected = 0;
					}
				}
			}
			
			else if(true == need_to_select_ability) {

				ability_currently_selected += 1;

				if(ability_currently_selected > (allies[ally_to_give_command].getAbilities().size() - 1)) {
					ability_currently_selected = 0;
				}
			}

			else if(true == need_to_select_potion) {

			}

			// change ability selected
			else {

				action_currently_selected -= 1;

				if(action_currently_selected < 0) {
					action_currently_selected = 2;
				}
			}
		}

		// SELECT
		else if(key == '1') {
			
			if(true == need_to_select_enemy) {

				allies[ally_to_give_command].set_need_to_cast();
				allies[ally_to_give_command].set_ability_being_cast(tmp_ability_selected);
				allies[ally_to_give_command].set_enemy_being_targeted(enemy_currently_selected);
				need_to_issue_command = false;
				need_to_select_enemy = false;
				need_to_select_potion = false;
				need_to_select_ability = false;

				if(allies[ally_to_give_command].getAbilities()[ability_currently_selected] == "Slash") {
					allies[ally_to_give_command].expend_energy(100);
				}
				else if(allies[ally_to_give_command].getAbilities()[ability_currently_selected] == "Slow") {
					allies[ally_to_give_command].expend_energy(250);
				}

				action_currently_selected = 0;
				enemy_currently_selected = 1;
				battle_play_sound_effect("button");
			}

			// POTIONS
			else if(true == need_to_select_potion && number_of_healing_potions > 0) {

				allies[ally_to_give_command].set_need_to_cast();
				allies[ally_to_give_command].set_ability_being_cast("Healing Potion");
				allies[ally_to_give_command].set_maximum_cast(1.5);
				need_to_issue_command = false;
				need_to_select_enemy = false;
				need_to_select_potion = false;
				need_to_select_ability = false;

				number_of_healing_potions -= 1;
				action_currently_selected = 0;
				enemy_currently_selected = 0;
				battle_play_sound_effect("button");
			}

			// ABILITIES
			else if(true == need_to_select_ability) {

				// SLASH
				if(allies[ally_to_give_command].getAbilities()[ability_currently_selected] == "Slash" && allies[ally_to_give_command].getEnergy() >= 100) {
					need_to_select_enemy = true;
					allies[ally_to_give_command].set_maximum_cast(2.5);
					tmp_ability_selected = "Slash";
				}
				// SLOW
				if(allies[ally_to_give_command].getAbilities()[ability_currently_selected] == "Slow" && allies[ally_to_give_command].getEnergy() >= 250) {
					need_to_select_enemy = true;
					allies[ally_to_give_command].set_maximum_cast(3.0);
					tmp_ability_selected = "Slow";
				}
				battle_play_sound_effect("button");
			}

			// MENU
			else {

				// DEFEND
				if(actions[action_currently_selected] == "Defend") {

					allies[ally_to_give_command].set_need_to_cast();
					allies[ally_to_give_command].set_ability_being_cast("Defend");
					allies[ally_to_give_command].set_maximum_cast(3.5);
					allies[ally_to_give_command].set_defend(true);
					need_to_issue_command = false;
					need_to_select_enemy = false;
					need_to_select_potion = false;
					need_to_select_ability = false;
				
					action_currently_selected = 0;
					enemy_currently_selected = 0;
					battle_play_sound_effect("button");
				}

				// POTIONS
				else if(actions[action_currently_selected] == "Potions") {
					need_to_select_potion = true;
					battle_play_sound_effect("button");
				}

				// Attack
				else if(actions[action_currently_selected] == "Attack") {
					need_to_select_ability = true;
					battle_play_sound_effect("button");
				}
			}
		}

		//BACK
		else if(key == '2') {

			if(true == need_to_select_enemy) {
				need_to_select_enemy = false;
				battle_play_sound_effect("back");
			}
			else if(true == need_to_select_potion) {
				need_to_select_potion = false;
				battle_play_sound_effect("back");
			}
			else if(true == need_to_select_ability) {
				need_to_select_ability = false;
				battle_play_sound_effect("back");
			}
		}
	}
}

void Battle::cast_ability(int i) {

	if(allies[i].getAbilitySelected() == "Slash") {
		allies[i].set_attacking(true);
		combatant_animation_frame = 1;
		combatant_animation_cap = 7;
		battle_play_sound_effect("slash");
	}
	else if(allies[i].getAbilitySelected() == "Slow") {
		enemies[allies[i].getEnemySelected()].set_slowed(true);
		enemies[allies[i].getEnemySelected()].damage(150);
		allies[i].set_cast(0.0);
		battle_play_sound_effect("slow");
		need_to_issue_command = true;
	}
	else if(allies[i].getAbilitySelected() == "Healing Potion") {
		allies[i].heal(300);
		allies[i].set_cast(0.0);
		battle_play_sound_effect("potion");
		need_to_issue_command = true;
	}
	else {
		need_to_issue_command = true;
		allies[i].set_cast(0.0);
	}
}

void Battle::enemy_cast_ability(int i) {

	if(enemies[i].getAbilitySelected() == "Quick Attack" || enemies[i].getAbilitySelected() == "Slow Attack") {
		enemies[i].set_attacking(true);
		enemy_attack_animation_frame = 1;
		battle_play_sound_effect("wolf attack");
	}
	else {
		enemies[i].set_attacking(false);
		enemies[i].set_cast(0.0);
		assign_enemy_ability(i);
	}
}

void Battle::assign_enemy_ability(int i) {
	// assign the next ability that the enemy will use
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0,2); 

	float rand = dist6(rng);

	// quick attack
	if(rand == 0 && enemies[i].getEnergy() >= 150) {

		// get a target ------------------------------------------>
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(0,2); 

		float ally_targeted = dist6(rng);

		while(allies[ally_targeted].get_knocked_out() == true) {
			ally_targeted -= 1;
			if(ally_targeted < 0) {
				ally_targeted = allies.size() - 1;
			}
		}
		
		enemies[i].set_enemy_being_targeted(ally_targeted);
		enemies[i].set_ability_being_cast("Quick Attack");
		enemies[i].set_maximum_cast(1.8);
		enemies[i].expend_energy(120);
	}
	// slow attack
	else if(rand == 1 && enemies[i].getEnergy() >= 250) {

		// get a target ------------------------------------------>
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(0,2); 

		float ally_targeted = dist6(rng);

		while(allies[ally_targeted].get_knocked_out() == true) {
			ally_targeted -= 1;
			if(ally_targeted < 0) {
				ally_targeted = allies.size() - 1;
			}
		}

		enemies[i].set_enemy_being_targeted(ally_targeted);

		if(the_type == "boss" && (i != 1)) {
			enemies[i].set_ability_being_cast("Quick Attack");
			enemies[i].set_maximum_cast(1.8);
			enemies[i].expend_energy(120);
		}
		else {
			enemies[i].set_ability_being_cast("Slow Attack");
			enemies[i].set_maximum_cast(2.3);
			enemies[i].expend_energy(220);
		}
	}
	// defend
	else {
		enemies[i].set_defend(true);
		enemies[i].set_ability_being_cast("Defend");
		enemies[i].set_maximum_cast(3.2);
	}
}

void Battle::set_screen_parameters(int width, int height, sf::RenderWindow * the_window) {
	window_width = width;
	window_height = height;
}

void Battle::set_need_to_update_tile_size(double the_new_tile_size) {
	need_to_update_tile_size = true;
	new_tile_size = the_new_tile_size;
	tileSize = new_tile_size;
}

int Battle::get_morgans_health() {
	return allies[1].getHealth();
}