#include <list>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "territory.h"
#include "maps.h"
#include "card.h"
#include "game.h"


typedef std::map<char, std::pair<char, char>> CONTINENT_DATA;


typedef std::list<std::pair<Territory *, double>> deploy_weights;

//trims the weights on a deploy to make all invalid deploys not happen
void trimDeploy(std::list<std::pair<Territory *, double>>& weights, const Game& g) {
	Player * p = g.getPlayer();

	for(deploy_weights::iterator w_itr = weights.begin();
	 w_itr != weights.end(); ++w_itr) {
		if(w_itr->first->getPlayer() != p) {
			w_itr->second = 0;
		}
	}
}

typedef std::list<std::pair<Territory *, std::pair<Territory *, double>>> attack_weights;

//trims the weights on an attack to make all invalid attacks not happen
void trimAttack(attack_weights& weights, const Game& g) {
	Player * p = g.getPlayer();

	for(attack_weights::iterator w_itr = weights.begin();
	 w_itr != weights.end(); ++w_itr) {
	 	//Check if the defending territory is not owned by the player or
	 	//if the attacking terriroy is owned by the player or
	 	//if the attacker does not haev enough army
	 	if(w_itr->first->getPlayer() != p || w_itr->second.first->getPlayer() == p ||
	 		w_itr->first->getArmy() <= 1) {
	 		w_itr->second.second = 0;
	 	}
	}
}

typedef std::list<std::pair<Territory *, std::pair<Territory *, double>>> move_weights;

void trimMove(move_weights& weights, const Game& g) {
	Player * p = g.getPlayer();

	for(move_weights::iterator w_itr = weights.begin();
	 w_itr != weights.end(); ++w_itr) {
	 	//Check if the defending territory is not owned by the player or
	 	//if the attacking terriroy is not owned by the player
	 	if(w_itr->first->getPlayer() != p || w_itr->second.first->getPlayer() != p) {
	 		w_itr->second.second = 0;

	 	}
	 	else {
	 		//Check if they are directly adjacent or not
			std::vector<std::pair<Territory *, bool>> adj_territories;
			adj_territories = w_itr->first->getAdjacent();
			std::vector<std::pair<Territory *, bool>>::iterator adj_itr
			 = adj_territories.begin();
			bool is_adj = false;
			for(; adj_itr != adj_territories.end(); ++adj_itr) {
				if(adj_itr->first == w_itr->second.first) {
					is_adj = true;
				}
	 		}
	 		if(!is_adj) {
	 			w_itr->second.second = 0;
	 		}
	 	}
	}
}

deploy_weights generateDeployWeights(const Game& g) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr = territories.begin();

	deploy_weights weights;

	for(; ter_itr != territories.end(); ++ter_itr) {
		double num = ((double) rand() / (RAND_MAX));
		weights.push_front(std::make_pair(*ter_itr, num));
	}

	return weights;
}

attack_weights generateAttackWeights(const Game& g) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr = territories.begin();

	attack_weights weights;

	for(; ter_itr != territories.end(); ++ter_itr) {
		std::vector<std::pair<Territory *, bool>> adj_territories;
		adj_territories = (*ter_itr)->getAdjacent();
		std::vector<std::pair<Territory *, bool>>::iterator adj_itr = adj_territories.begin();
		for(; adj_itr != adj_territories.end(); ++adj_itr) {
			double num = ((double) rand() / (RAND_MAX));
			weights.push_front(std::make_pair(*ter_itr,
			 std::make_pair(adj_itr->first, num)));
		}
	}

	return weights;
}

attack_weights generateMoveWeights(const Game& g) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr1 = territories.begin();
	std::list<Territory*>::const_iterator ter_itr2;

	attack_weights weights;

	for(; ter_itr1 != territories.end(); ++ter_itr1) {
		for(ter_itr2 = territories.begin(); ter_itr2 != territories.end(); ++ter_itr2) {
			double num = ((double) rand() / (RAND_MAX));
			weights.push_front(std::make_pair(*ter_itr1,
			 std::make_pair(*ter_itr2, num)));
		}
	}

	return weights;
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
	
	srand((unsigned) time(0));

	//Create a vector of current territories
	std::vector<Territory> territories;
	//Fill those territories with the "classic map"
	CONTINENT_DATA cd;

	std::list<Card> cards;

	for(unsigned int i = 0; i < territories.size(); ++i) {
		cards.push_front(Card(territories[i].getName(), i % 3));
	}
	Game game(cards);
	read_map(file, territories, cd, game);

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
			deploy_weights dweights = generateDeployWeights(game);
			trimDeploy(dweights, game);
			deploy_weights::iterator best = dweights.begin();
			for(deploy_weights::iterator itr = dweights.begin(); itr != dweights.end(); ++itr) {	
				if(itr->second > best->second) {
					best = itr;
				}
			}
			game.deploy(best->first);
		}
		game.nextPlayer();
	}
	
	game.print_data();

	while(game.getPlayers().size() > 1) {

		for(int i = 0; i < game.getPlayer()->getIncome(cd); ++i) {
			deploy_weights dweights = generateDeployWeights(game);
			trimDeploy(dweights, game);
			deploy_weights::iterator best = dweights.begin();
			for(deploy_weights::iterator itr = dweights.begin(); itr != dweights.end(); ++itr) {
				if(itr->second > best->second) {
					best = itr;
				}
			}
			game.deploy(best->first);
		}


		while(true) {
			attack_weights aweights = generateAttackWeights(game);
			trimAttack(aweights, game);
			attack_weights::iterator best = aweights.begin();
			for(attack_weights::iterator itr = aweights.begin(); itr != aweights.end(); ++itr) {
				if(itr->second.second > best->second.second) {
					best = itr;
				}
			}
			if(best->second.second < 0.6 || aweights.size() == 0) {
				break;
			}

			game.attack(best->first, best->second.first);
		}

		move_weights mweights = generateMoveWeights(game);
		trimMove(mweights, game);
		move_weights::iterator best = mweights.begin();
		for(move_weights::iterator itr = mweights.begin(); itr != mweights.end(); ++itr) {
			if(itr->second.second > best->second.second) {
				best = itr;
			}
		}
		if(best->first != NULL && best->second.first != NULL) {
			short num = best->first->getArmy();
			num *= best->second.second;
			game.move(best->first, best->second.first, num);
		}
		game.nextPlayer();
		game.print_data();
	}
}