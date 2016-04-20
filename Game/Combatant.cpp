#include "stdafx.h"
#include "Combatant.h"
#include <iostream>

Combatant::Combatant(string the_name, int the_max_health, double starting_health_percent, int the_max_energy, double the_current_cast, bool is_ally, bool knocked_out) {

	name = the_name;
	max_health = the_max_health;
	max_energy = the_max_energy;
	health = (the_max_health * starting_health_percent);
	energy = the_max_energy;
	energy_double = the_max_energy;
	maximum_cast = 3.0;
	current_cast = the_current_cast;
	ally = is_ally;

	if(knocked_out) {
		is_knocked_out = true;
	}
	else {
		is_knocked_out = false;
	}

	need_to_cast = false;
	is_defending = false;
	attacking = false;
	being_crit = false;
	being_crit_count = 0;
	slowed_count = 0;

	enemy_being_targeted = 0;
	ability_being_cast = "";

	abilities.push_back("Slash");
	abilities.push_back("Slow");
}

void Combatant::update() {
	
	if(current_cast != maximum_cast) {
		// update the cast time
		if(is_slowed == true) {
			current_cast += 0.008;
		}
		else {
			current_cast += 0.016;
		}
	}

	// if we have finished casting
	if(current_cast > maximum_cast) {
		need_to_cast = true;
		current_cast = maximum_cast;
	}

	if((energy_double + ((double)max_energy / 100 / 30)) > max_energy ) {
		energy_double = max_energy;
	}
	else {
		energy_double = energy_double + ((double)max_energy / 100 / 30);
		energy = energy_double;
	}
}

int Combatant::getHealth() {
	return health;
}

int Combatant::getMaxHealth() {
	return max_health;
}

int Combatant::getEnergy() {
	return energy;
}

int Combatant::getMaxEnergy() {
	return max_energy;
}

double Combatant::getCast() {
	return current_cast;
}

double Combatant::getMaxCast() {
	return maximum_cast;
}

bool Combatant::get_need_to_cast() {
	return need_to_cast;
}

vector <string> Combatant::getAbilities() {
	return abilities;
}

void Combatant::set_need_to_cast() {
	need_to_cast = false;
}

void Combatant::set_ability_being_cast(string ability) {
	ability_being_cast = ability;
}

void Combatant::set_enemy_being_targeted(int enemy) {
	enemy_being_targeted = enemy;
}

void Combatant::damage(int damage) {

	// take less damage if defending
	if(is_defending) {
		health -= (damage * 0.6);
	}
	else {
		health -= damage;
	}

	if(health <= 0) {
		need_to_cast = false;
		is_knocked_out = true;
		health = 0;
	}
}

void Combatant::heal(int heal) {

	health += heal;

	if(health > max_health) {
		health = max_health;
	}
}

string Combatant::getAbilitySelected() {
	return ability_being_cast;
}

int Combatant::getEnemySelected() {
	return enemy_being_targeted;
}

bool Combatant::get_knocked_out() {
	return is_knocked_out;
}

void Combatant::expend_energy(int the_energy) {
	energy = energy - the_energy;
	energy_double = energy_double - the_energy;
}

void Combatant::set_defend(bool bol) {
	is_defending = bol;
}