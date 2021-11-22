#include <vector>
#include <string>
#include <iostream>
#include "territory.h"
#include "player.h"


//This function adds the adjacent territory
//Used recursion to auto add the current territory to the other
//one as an adjecent as well!
//Note other is a default argument as true. In theory you could make 
//One way borders if you set other to false.
void Territory::addAdjacent(Territory * terr, bool direct, bool other) {
    for(unsigned int i = 0; i < adjacent.size(); ++i) {
        Territory * t = adjacent[i].first;
        if(t->getName() == terr->getName()) {
            return;
        }
    }
    
    adjacent.push_back(std::make_pair(terr, direct));
    if(other) { 
    	terr->addAdjacent(this, direct, false);
    }
}

void Territory::print() const {
    std::cout << name << ":"<<std::endl;
    std::cout << "\tOwned by: ";
    if (player == NULL) {
        std::cout << "Nobody" << std::endl;
    }
    else {
        std::cout << "Player " << (int)(player->getPlayer()) << std::endl;
    }
    std::cout << "\tCurrently holds " << army << " units" << std::endl;
}