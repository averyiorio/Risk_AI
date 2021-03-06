#include <string>
#include <list>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include "card.h"
#include "territory.h"
#include "game.h"
#include "json.hpp"

using json = nlohmann::json;
	
void Game::addPlayer(char index) {
	players[index] = Player(index);
}

void Game::initialize(std::vector<short>& armies) {
	std::vector<char> n_territories;
	for(unsigned int i = 0; i < territories.size(); i++) {
		n_territories.push_back(i);
	}
	std::random_shuffle(n_territories.begin(), n_territories.end());

	player = players.begin();
	int p = 0;
	for(unsigned int i = 0; i < territories.size(); i++) {

		territories[n_territories[i]].changePlayer(&(player->second));
		player->second.addTerritory(&(territories[n_territories[i]]));
		territories[n_territories[i]].addArmy(1);
		armies[p]--;

		++player;
		++p;
		if(player == players.end()) {
			player = players.begin();
			p = 0;
		}
	}
	player = players.begin();
}
 
void Game::deploy(Territory * terr) {
	terr->addArmy(1);
}

void Game::print_data() const {
	std::map<char, Player>::const_iterator p_itr = players.begin();
	while(p_itr != players.end()) {
		std::cout << "Player " << (int)(p_itr->first);
		std::cout<<" owns "<<(int)(p_itr->second.getNumTerritories()) << " territories"<<std::endl;
		++p_itr;
	}
	for(unsigned int i = 0; i < territories.size(); ++i) {
		std::cout<<"["<<i<<"]";
		territories[i].print();
	}
	std::cout<<std::endl;
}

void Game::print_data(json& output) const {
	for(unsigned int i = 0; i < territories.size(); ++i) {
		territories[i].getData(output, turn);
	}
}

char roll_dice() {
	return 1 + (rand() % 6);
}

void Game::attack(Territory* attacker, Territory* defender) {
	std::vector<int> attack_dice;
	std::vector<int> defend_dice;
	while (attacker->army > 1) {
		for (int rolls = 0; rolls < std::min(3, attacker->army-1); rolls++) {
			attack_dice.push_back(roll_dice());
		}
		for (int rolls = 0; rolls < std::min((short)2, defender->army); rolls++) {
			defend_dice.push_back(roll_dice());
		}
		std::sort(attack_dice.begin(), attack_dice.end());
		std::sort(defend_dice.begin(), defend_dice.end());

		for (unsigned int die = 0; die < std::min(attack_dice.size(), defend_dice.size()); die++) {
			if (attack_dice[die] > defend_dice[die]) {
				defender->army--;
			}
			else {
				attacker->army--;
			}
		}
		attack_dice.clear();
		defend_dice.clear();
		if (defender->army == 0) {
			defender->player->removeTerritory(defender);
			attacker->player->addTerritory(defender);
			defender->army = attacker->army - 1;
			attacker->army = 1;

			if(defender->player->getNumTerritories() == 0) {
				std::list<Card> cards = defender->player->getCards();
				std::list<Card>::iterator c_itr = cards.begin();
				for(; c_itr != cards.end(); ++c_itr) {
					attacker->player->addCard(*c_itr);
				}
				players.erase(defender->player->getPlayer());
			}

			defender->player = attacker->player;
			return;
		}
	}
}

void Game::move(Territory * first, Territory * second, short amount) {
	first->addArmy(-amount);
	second->addArmy(amount);
}

void Game::nextPlayer() {
	++player;
	if(player == players.end()) {
		player = players.begin();
		turn++;
		// std::cout<<"Turn"<<turn<<" ended"<<std::endl;
	}
}
