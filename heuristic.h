#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include "territory.h"
#include "card.h"
#include "game.h"


typedef std::list<std::pair<Territory *, double>> deploy_weights;
typedef std::list<std::pair<Territory *, std::pair<Territory *, double>>> attack_weights;
typedef std::list<std::pair<Territory *, std::pair<Territory *, double>>> move_weights;


const double CONTINENT_WEIGHT = 0.1;
const double ADJACENCY_WEIGHT = 0.;
const double TROOP_WEIGHT = 0.6;
const double RANDOM_WEIGHT = 0.1;

double generateTerritoryWeight(Territory * terr, Game& g) {
	std::map<char, Player> players = g.getPlayers();

	double weight = 0;

	std::vector<Territory> territories = g.getTerritories();
	double ratio = 0;
	char continent = terr->getContinent();
	for(std::map<char, Player>::iterator itr = players.begin();
		itr != players.end(); ++itr) {
		double new_ratio = 0;
		int tot_terr = 0;
		for(unsigned int i = 0; i < territories.size(); ++i) {
			Territory curr_terr = territories[i];
			if(curr_terr.getContinent() == continent ) {
				tot_terr++;
				if(itr->first == curr_terr.getPlayer()->getPlayer()) {
					new_ratio++;
				}
			}
		}
		new_ratio /= tot_terr;
		ratio = std::max(new_ratio, ratio);
	}

	weight += ratio * CONTINENT_WEIGHT;

	Player * curr = g.getPlayer();
	std::vector<std::pair<Territory *, bool>> adjacent = terr->getAdjacent();
	ratio = 0;
	for(unsigned int i = 0; i < adjacent.size(); i++) {
		Territory * curr_terr = adjacent[i].first;
		if(curr_terr->getPlayer() != curr) {
			ratio++;
		}
	}
	ratio /= adjacent.size();

	weight += ratio * ADJACENCY_WEIGHT;

	return weight;
}

double sigmoid(double x) {
	double exponent = std::exp(10*(x-.05));
	return 1 / (1 + exponent);
}

deploy_weights generateHeuristicDeploy(Game& g) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr = territories.begin();

	deploy_weights weights;

	for(; ter_itr != territories.end(); ++ter_itr) {
		double num = generateTerritoryWeight(*ter_itr, g);

		std::vector<std::pair<Territory *, bool>> adjacent = (*ter_itr)->getAdjacent();
		double ratio = 0;
		double total = 0;
		for(unsigned int i = 0; i < adjacent.size(); i++) {
			Territory * curr_terr = adjacent[i].first;
			if(curr_terr->getPlayer()->getPlayer() != p->getPlayer()) {
				ratio+=curr_terr->getArmy();
			}
			total+=curr_terr->getArmy();
		}
		num += (1-sigmoid(ratio/total)) * TROOP_WEIGHT;
		num += ((double) rand() / (RAND_MAX)) * RANDOM_WEIGHT;
		weights.push_front(std::make_pair(*ter_itr, num));
	}

	return weights;
}