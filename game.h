#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <map>
#include <list>
#include "player.h"
#include "card.h"
#include "territory.h"

class Game {
public:
	//Pass in the map and card when the game is created
	Game(const std::vector<Territory> territories_, const std::list<Card> cards_)
	 : territories(territories_), cards(cards_) {}

	void addPlayer(char index);

	//Starts the next players turn in deploy stage
	void deploy(char terr_index);
	//Starts the next players turn in attack stage
	void attack_turn();
	void attack(Territory* attacker, Territory* defender);
	//Starts the next players turn in move stage
	void move();

	//Moves to the next player
	void nextPlayer() { ++player;}

	//Gets current player
	Player * getPlayer() const { return &(player->second);}

	//Gets all players
	std::map<char, Player>& getPlayers() { return players; }

	//Checks if any players are dead any distributes their cards
	void check_players_dead();

	void print_data() const;

	Territory& getTerritory(char i)  { return territories[i]; };

	void initialize(std::vector<short>& armies);

private:
	//Holds an iterator to the current player
	std::map<char, Player>::iterator player;

	//Holds the current territories for the game
	std::vector<Territory> territories;

	//Holds all of the players
	std::map<char, Player> players;

	//Holds a list of the current cards that are available for draw
	std::list<Card> cards;

};

#endif