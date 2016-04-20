#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include "stdafx.h"

using namespace std;

class Combatant {

	public:

		Combatant(string name, int max_health, double starting_health_percent, int max_energy, double current_cast, bool ally, bool knocked_out);
		void update();

		void set_need_to_cast();

		int getHealth();
		int getMaxHealth();

		int getEnergy();
		int getMaxEnergy();

		double getCast();
		double getMaxCast();

		double get_crit_count() {
			return being_crit_count;
		}

		double get_slowed_count() {
			return slowed_count;
		}

		string getAbilitySelected();
		int getEnemySelected();

		bool get_need_to_cast();
		bool get_knocked_out();

		bool get_is_defending() {
			return is_defending;
		}

		bool get_attacking() {
			return attacking;
		}

		bool get_being_crit() {
			return being_crit;
		}

		bool get_slowed() {
			return is_slowed;
		}

		void set_attacking(bool att) { 
			attacking = att; 
		}

		string getName() {
			return name;
		}

		void set_cast(double new_cast) {
			current_cast = new_cast;
		}

		vector <string> getAbilities();

		void set_ability_being_cast(string ability);
		void set_enemy_being_targeted(int enemy);
		void damage(int damage);
		void heal(int heal);
		void expend_energy(int energy);

		void set_being_crit(bool crit) {
			being_crit = crit;
		}

		void set_slowed(bool slow) {
			is_slowed = slow;
		}

		void set_maximum_cast(double max) {
			maximum_cast = max;
		}

		void set_being_crit_count(double count) {
			being_crit_count = count;
		}

		void set_slowed_count(double count) {
			slowed_count = count;
		}

		void set_defend(bool bol);

	private:

		int health;
		int max_health;
		int energy;
		int max_energy;

		double energy_double;

		double current_cast;
		double maximum_cast;

		bool is_knocked_out;
		bool ally;
		bool need_to_cast;
		bool attacking;
		bool is_defending;

		bool being_crit;
		double being_crit_count;

		bool is_slowed;
		double slowed_count;

		string name;
		vector <string> abilities;
		string ability_being_cast;

		int enemy_being_targeted;
};
