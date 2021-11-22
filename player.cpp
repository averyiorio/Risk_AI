#include <string>
#include <list>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include "territory.h"
#include "card.h"

typedef std::map<char, std::pair<char, char>> CONTINENT_DATA;

char Player::getContinentIncome(CONTINENT_DATA cd) const {
	char troop_bonus = 0;
	std::vector<char> continent_count{0,0,0,0,0,0};
	std::list<Territory*>::const_iterator itr = territories.begin();
	for (; itr != territories.end(); ++itr) {
		continent_count[(*itr)->getContinent()]++;
	}
	CONTINENT_DATA::iterator cd_itr;
	cd_itr = cd.begin();
	for (int j = 0; j < 6; j++) {
		if (continent_count[j] == cd_itr->second.first) {
			troop_bonus += cd_itr->second.second;
		}
		++cd_itr;
	}
	return troop_bonus;
}

short Player::getIncome(CONTINENT_DATA cd) const {
	return floor(num_territories) + getContinentIncome(cd);
}

bool Player::playCards(const Card& c1, const Card& c2, const Card& c3) {
	std::list<Card>::iterator c1_itr = find(cards.begin(), cards.end(), c1);
	std::list<Card>::iterator c2_itr = find(cards.begin(), cards.end(), c2);
	std::list<Card>::iterator c3_itr = find(cards.begin(), cards.end(), c3);
	if (c1_itr != cards.end() && c2_itr != cards.end() && c3_itr != cards.end()) {
		cards.erase(c1_itr);
		cards.erase(c2_itr);
		cards.erase(c3_itr);
		return true;
	}
	return false;
}

void Player::removeTerritory(Territory* t) {
	std::list<Territory*>::iterator itr = territories.begin();
	for (; itr != territories.end(); itr++) {
		if (*itr == t) {
			territories.erase(itr);
			num_territories--;
			return;
		}
	}
}
