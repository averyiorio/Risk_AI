#include <list>
#include <vector>
#include <algorithm>
#include "territory.h"
#include "card.h"
#include "game.h"


typedef std::list<std::pair<Territory *, double>> deploy_weights;
typedef std::list<std::pair<Territory *, std::pair<Territory *, double>>> attack_weights;
typedef std::list<std::pair<Territory *, std::pair<Territory *, double>>> move_weights;



double generateTerritoryWeight(Territory * terr, const Game& g) {
	Player * curr = g.getPlayer();
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
			Terr curr_terr = territories[i];
			if(curr_terr.getContinent() == continent ) {
				tot_terr++;
				if(itr->second == curr_terr.getPlayer()) {
					new_ratio++;
				}
			}
		}
		new_ratio /= tot_terr;
		ratio = std::max(new_ratio, ratio);
	}

	std::vector<std::pair<Territory *, bool>> adjacent = terr->getAdjacent();

	for(unsigned int i = 0; i < adjacent.size(); i++) {
		Territory * curr_terr = adjacent[i].first;
		//
	}
}