#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "territory.h"
#include "maps.h"
#include "card.h"
#include "game.h"
#include "heuristic.h"
#include "json.hpp"

using json = nlohmann::json;

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

move_weights generateMoveWeights(const Game& g) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr1 = territories.begin();
	std::list<Territory*>::const_iterator ter_itr2;

	move_weights weights;

	for(; ter_itr1 != territories.end(); ++ter_itr1) {
		for(ter_itr2 = territories.begin(); ter_itr2 != territories.end(); ++ter_itr2) {
			double num = ((double) rand() / (RAND_MAX));
			weights.push_front(std::make_pair(*ter_itr1,
			 std::make_pair(*ter_itr2, num)));
		}
	}

	return weights;
}

void runGame(Game& game, int players,  CONTINENT_DATA cd, int i, const std::vector<double>& weights) {
	for(int i = 0; i < players; ++i) {
		game.addPlayer(i);
	}

	std::vector<short> armies;
	for(int i = 0; i < players; ++i) {
		armies.push_back(-5*players + 50);
	} 

	game.initialize(armies);
	// game.print_data();

	for(char p = 0; p < players; ++p) {
		for(int i = 1; i < armies[p]; ++i) {
			deploy_weights dweights;
			if(game.getPlayer()->getPlayer() == 0) {
				dweights = generateHeuristicDeploy(game, weights);
			} else {
				dweights = generateDeployWeights(game);
			}
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
	
	// game.print_data();

	json output;
	while(game.getPlayers().size() > 1) {
		Player * player = game.getPlayer();
		short armies = player->getIncome(cd);
		std::list<Card> pcards = player->getCards();
		if(player->getNumCards() == 3) {
			
		}
		std::map<Territory*, int> deploy_map;
		for(int i = 0; i < armies; ++i) {
			deploy_weights dweights;
			if(game.getPlayer()->getPlayer() == 0) {
				dweights = generateHeuristicDeploy(game, weights);
			} else {
				dweights = generateDeployWeights(game);
			}
			trimDeploy(dweights, game);
			deploy_weights::iterator best = dweights.begin();
			for(deploy_weights::iterator itr = dweights.begin(); itr != dweights.end(); ++itr) {
				if(itr->second > best->second) {
					best = itr;
				}
			}
			deploy_map[best->first] += 1;
			game.deploy(best->first);
		}


		while(true) {
			attack_weights aweights;
			if(game.getPlayer()->getPlayer() == 0) {
				aweights = generateHeuristicAttack(game, weights);
			} else {
				aweights = generateAttackWeights(game);
			}
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

		move_weights mweights;
		if(game.getPlayer()->getPlayer() == 0) {
			mweights = generateHeuristicMove(game, weights);
		} else {
			mweights = generateMoveWeights(game);
		}
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
		game.print_data(output);
	}
	std::string file = "games/" + std::to_string(i) + ".json";
	std::ofstream o(file);
	o << std::setw(4) << output << std::endl;
}

int main() {
	int players = 4;
	std::string file = "classic";
	
	srand((unsigned) time(0));

	//Create a vector of current territories
	std::vector<Territory> territories;
	//Fill those territories with the "classic map"
	CONTINENT_DATA cd;

	std::list<Card> cards;

	for(unsigned int i = 0; i < territories.size(); ++i) {
		cards.push_front(Card(territories[i].getName(), i % 3));
	}

	//START GAME STUFF

	int games = 100;
	std::pair<double, std::vector<double>> best_weights;
	best_weights.first = 0;
	for(int j = 0; j < 50; j++) {
		std::vector<double> numVictories;
		for(int i = 0; i < players; i++) {
			numVictories.push_back(0);
		}
		std::vector<double> weights;
		for(int i = 0; i < 12; ++i) {
			weights.push_back(((double) rand() / (RAND_MAX)));
		}
		for(int i = 0; i < games; ++i) {

			Game game(cards);
			territories.clear();
			cd.clear();
			read_map(file, territories, cd, game);
			runGame(game, players, cd, i, weights);

			numVictories[game.getPlayer()->getPlayer()]++;

			std::cout << "\r" << (int)(i/(games/100)) << "% completed: ";
			std::cout << "[" << std::setw(20) << std::string(i/(games/20), '8') << "]";
			std::cout.flush();
		}
		std::cout<<"Completed simulation "<<j<<std::endl;
		if(numVictories[0] > best_weights.first) {
			best_weights = std::make_pair(numVictories[0], weights);
		}
	}
	std::cout<<"Best weights had a " << (best_weights.first/games)*100 <<
	"% winrate"<<std::endl;
	std::cout<<"Weights were:"<<std::endl;
	for(unsigned int i = 0; i < best_weights.second.size(); i++) {
		std::cout<<"\t"<<best_weights.second[i]<<std::endl;
	}
}