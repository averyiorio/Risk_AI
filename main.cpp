#include <list>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "territory.h"
#include "maps.h"
#include "card.h"
#include "game.h"

typedef std::map<std::string, std::pair<char, char>> CONTINENT_DATA;


//trims the weights on a deploy to make all invalid deploys not happen
void trimDeploy(std::list<std::pair<Territory *, double>>& weights, const Game& g) {
	Player * p = g.getPlayer();

	for(std::map<int, double>::iterator w_itr = weights.begin();
	 w_itr != weights.end(); ++w_itr) {
		for(std::list<Territory*>::const_iterator t_itr = p->getTerritories().begin(); 
			t_itr != p->getTerritories().end(); ++t_itr) {
			if(w_itr->first != t_itr) {
				w_itr->second = 0;
			}
		}
	}
}


int main() {
	int players;
	std::cout<<"How many players?"<<std::endl;
	std::cin>>players;
	std::cout<<"What map would you like to play?"<<std::endl
	<<"Options:"<<std::endl
	<<"\t[0]Classic"<<std::endl;
	int map;
	std::cin>>map;
	std::string file = "";
	if(map == 0) {
		file = "classic";
	}

	//Create a vector of current territories
	std::vector<Territory> territories;
	//Fill those territories with the "classic map"
	const CONTINENT_DATA cd;

	read_map(file, territories, cd);
	std::list<Card> cards;

	for(unsigned int i = 0; i < territories.size(); ++i) {
		cards.push_front(Card(territories[i].getName(), i % 3));
	}

	Game game(territories, cards);
	for(int i = 0; i < players; ++i) {
		game.addPlayer(i);
	}

	std::vector<short> armies;
	for(int i = 0; i < players; ++i) {
		armies.push_back(-5*players + 50);
	} 

	game.initialize(armies);
	game.print_data();

	for(char p = 0; p < players; ++p) {
		for(int i = 1; i < armies[p]; ++i) {
			int terr_index = 0;
			game.deploy(terr_index);
			game.nextPlayer();
		}
	}
	
	game.print_data();

	while(game.getPlayers().size() > 1) {

		for(int i = 0; i < game.getPlayer()->getIncome(cd); ++i) {
			int terr_index = 0;
			game.deploy(terr_index);
		}


		bool wants_to_attack = true;
		Territory * attacker;
		Territory * defender;
		while(player wants to attack) {
			game.attack(attacker, defender);
		}

		// game.move();
	}
}