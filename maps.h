#ifndef _MAPS_H_
#define _MAPS_H_

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"
#include "territory.h"

using json = nlohmann::json;

//Function to read in json files with "map data"
//It works ?
void read_map(const std::string& file, std::vector<Territory>& territories,
 std::map<char, std::pair<char, char>>& cd, Game& g) {
	//Declare file stream
	std::ifstream in_str(file + ".json");
	json data;
	in_str >> data;
	typedef json::iterator itr;

	itr continent = data.begin();
	//Keep track of continent number
	char i = 0;
	while(continent != data.end()) {
		//.first is territory ampunt
		//.second is value
		std::pair<char, char> values(0,0);

		itr val_itr = continent->begin();
		for(itr ter_itr = val_itr->begin(); ter_itr != val_itr->end(); ++ter_itr) {
			territories.push_back(Territory(ter_itr.key(), i));
			values.first++;
		}

		++val_itr;
		//Trick it into being an int :)
		values.second = (int) *val_itr;
		cd[i] = values;
		++continent;
		++i;
	}

	g.addTerritories(territories);
		
	int t = 0;
	for(continent = data.begin(); continent != data.end(); ++continent) {
		for(itr ter_itr = continent->begin()->begin();
		 ter_itr != continent->begin()->end(); ++ter_itr) {
		 	for(itr adj_itr = ter_itr->begin(); adj_itr != ter_itr->end(); ++adj_itr) {
		 		std::string name = adj_itr.key();
				for(unsigned int i = 0; i < territories.size(); i++) {
					if(name == territories[i].getName()) {
						g.getTerritory(t).addAdjacent(&g.getTerritory(i), *adj_itr);
					}
				}
			}
			t++;
		}
	}
}

#endif