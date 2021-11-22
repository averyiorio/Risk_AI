#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include <list>
#include <map>

//Forward declaration of Player
class Player;

#include "territory.h"
#include "card.h"

typedef std::map<std::string, std::pair<char, char>> CONTINENT_DATA;

class Player{
public:
	Player() : player_num(0) {}
	Player(char _player_num) : player_num(_player_num) {}

	//ACCESSORS
	char getPlayer() const {return player_num;}
	char getNumCards() const {return cards.size();}
	const std::list<Card>& getCards() const {return cards;}
	char getNumTerritories() const {return num_territories;}
	const std::list<Territory*>& getTerritories() const {return territories;}
	char getContinentIncome(CONTINENT_DATA cd) const;
	short getIncome(CONTINENT_DATA cd) const;

	//MODIFIERS
	void addCard(Card c) {cards.push_front(c);}
	void addTerritory(Territory* t) {territories.push_front(t); num_territories++; }

	bool playCards(const Card& c1, const Card& c2, const Card& c3);
	void removeTerritory(Territory* t);

private:
	char player_num;
	char num_territories = 0;
	std::list<Territory*> territories;
	std::list<Card> cards;
	std::map<char, char> continents;
};

#endif