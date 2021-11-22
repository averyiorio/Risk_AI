#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include <map>
#include <vector>
#include <string>

//Forward Declaration of Territory for player
class Territory;

#include "player.h"
#include "game.h"

class Territory {
public:
	//CONSTRUCTOR
	Territory(const std::string& name_, char continent_)
	: name(name_), player(NULL), army(0), continent(continent_) {}

	friend class Game;

	//ACCESSORS
	const std::string& getName() const { return name;}
	Player* getPlayer() const {return player;}
	short getArmy() const { return army;}
	char getContinent() const { return continent;}
	const std::vector<std::pair<Territory *, bool>>& getAdjacent() const {return adjacent;}

	//MEMBER FUNCTIONS
	void addArmy(short amt) { army += amt;}
	void changePlayer(Player * player_) { player = player_; }
	void addAdjacent(Territory * terr, bool direct, bool other = true); 

	//Operator to make the territory function in a set
	bool operator<(const Territory& right) {return name < right.getName(); }

	void print() const;

private:
	//Holds the name of the territory
	std::string name;

	//Points to the current player with ownership
	Player * player;

	//Stores a number of the current army amount on the territory 
	short army;

	//Stores what continent this territory is part of
	char continent;

	//Stores
	std::vector<std::pair<Territory *, bool>> adjacent;
};



#endif