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


const double DCONTINENT_WEIGHT = 0.15;
const double DADJACENCY_WEIGHT = 0;
const double DTROOP_WEIGHT = 0.65;
const double DRANDOM_WEIGHT = 0.2;

const double ACONTINENT_WEIGHT = 0.15;
const double AADJACENCY_WEIGHT = 0;
const double ATROOP_WEIGHT = 0.65;
const double ARANDOM_WEIGHT = 0.2;

const double MCONTINENT_WEIGHT = 0.15;
const double MADJACENCY_WEIGHT = 0;
const double MTROOP_WEIGHT = 0.65;
const double MRANDOM_WEIGHT = 0.2;

double generateContinentWeight(Territory * terr, Game& g, double weight) {
	std::map<char, Player> players = g.getPlayers();

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
	return ratio * weight;
}

double generateAdjacencyWeight(Territory * terr, Game& g, double weight) {
	Player * curr = g.getPlayer();
	std::vector<std::pair<Territory *, bool>> adjacent = terr->getAdjacent();
	double ratio = 0;
	for(unsigned int i = 0; i < adjacent.size(); i++) {
		Territory * curr_terr = adjacent[i].first;
		if(curr_terr->getPlayer() != curr) {
			ratio++;
		}
	}
	ratio /= adjacent.size();

	return ratio * weight;
}

double sigmoid(double x) {
	double exponent = std::exp(10*(x-.05));
	return 1 / (1 + exponent);
}

deploy_weights generateHeuristicDeploy(Game& g, const std::vector<double>& k_weights) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr = territories.begin();

	deploy_weights weights;

	for(; ter_itr != territories.end(); ++ter_itr) {
		double num = 0;
		num += generateContinentWeight(*ter_itr, g, k_weights[0]);
		num += generateAdjacencyWeight(*ter_itr, g, k_weights[1]);

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
		num += (1-sigmoid(ratio/total)) * k_weights[2];
		num += ((double) rand() / (RAND_MAX)) * k_weights[3];
		weights.push_front(std::make_pair(*ter_itr, num));
	}

	return weights;
}

attack_weights generateHeuristicAttack(Game& g, const std::vector<double>& k_weights) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr = territories.begin();

	attack_weights weights;

	for(; ter_itr != territories.end(); ++ter_itr) {
		std::vector<std::pair<Territory *, bool>> adj_territories;
		adj_territories = (*ter_itr)->getAdjacent();
		std::vector<std::pair<Territory *, bool>>::iterator adj_itr = adj_territories.begin();
		for(; adj_itr != adj_territories.end(); ++adj_itr) {
			double num = 0;
			num += generateContinentWeight(adj_itr->first, g, k_weights[4]);
			num += generateAdjacencyWeight(adj_itr->first, g, k_weights[5]);

			short troop_diff = (*ter_itr)->getArmy() - adj_itr->first->getArmy();
			short total_troops = (*ter_itr)->getArmy() + adj_itr->first->getArmy();

			num += (1-sigmoid(troop_diff/total_troops)) * k_weights[6];
			num += ((double) rand() / (RAND_MAX)) * k_weights[7];

			weights.push_front(std::make_pair(*ter_itr,
			 std::make_pair(adj_itr->first, num)));
		}
	}

	return weights;
}

move_weights generateHeuristicMove(Game& g, const std::vector<double>& k_weights) {
	Player * p = g.getPlayer();
	std::list<Territory*> territories = p->getTerritories();
	std::list<Territory*>::const_iterator ter_itr1 = territories.begin();
	std::list<Territory*>::const_iterator ter_itr2;

	move_weights weights;

	for(; ter_itr1 != territories.end(); ++ter_itr1) {
		for(ter_itr2 = territories.begin(); ter_itr2 != territories.end(); ++ter_itr2) {
			double num1 = 0;
			num1 += generateContinentWeight(*ter_itr1, g, k_weights[8]);
			num1 += generateAdjacencyWeight(*ter_itr1, g, k_weights[9]);

			std::vector<std::pair<Territory *, bool>> adjacent1 = (*ter_itr1)->getAdjacent();
			double ratio1 = 0;
			double total1 = 0;
			for(unsigned int i = 0; i < adjacent1.size(); i++) {
				Territory * curr_terr = adjacent1[i].first;
				if(curr_terr->getPlayer()->getPlayer() != p->getPlayer()) {
					ratio1+=curr_terr->getArmy();
				}
				total1+=curr_terr->getArmy();
			}
			num1 += (1-sigmoid(ratio1/total1)) * MTROOP_WEIGHT;

			double num2 = 0;
			num2 += generateContinentWeight(*ter_itr2, g, k_weights[8]);
			num2 += generateAdjacencyWeight(*ter_itr2, g, k_weights[9]);

			std::vector<std::pair<Territory *, bool>> adjacent2 = (*ter_itr2)->getAdjacent();
			double ratio2 = 0;
			double total2 = 0;
			for(unsigned int i = 0; i < adjacent2.size(); i++) {
				Territory * curr_terr = adjacent2[i].first;
				if(curr_terr->getPlayer()->getPlayer() != p->getPlayer()) {
					ratio2+=curr_terr->getArmy();
				}
				total2+=curr_terr->getArmy();
			}
			num2 += (1-sigmoid(ratio2/total2)) * k_weights[10];

			num1 = 1 - num1;
			double average = (num1 + num2)/2;
			average += ((double) rand() / (RAND_MAX)) * k_weights[11];
			average = sigmoid(average);
			weights.push_front(std::make_pair(*ter_itr1,
			 std::make_pair(*ter_itr2, average)));
		}
	}

	return weights;
}