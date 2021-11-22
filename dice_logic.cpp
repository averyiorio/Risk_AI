#include <string>
#include <list>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>


char roll_dice() {
	srand((unsigned) time(0));
	return 1 + (rand() % 6);
}

void attack(Territory* attacker, Territory* defender) {
	std::vector<int> attack_dice;
	std::vector<int> defend_dice;
	while (attacker.army > 1) {
		for (int rolls = 0; rolls < std::min(3, attacker.army); rolls++) {
			attack_dice.push_back(roll_dice());
		}
		for (int rolls = 0; rolls < std::min(2, defender.army); rolls++) {
			attack_dice.push_back(roll_dice());
		}
		std::sort(attack_dice.begin(), attack_dice.end());
		std::sort(attack_dice.begin(), attack_dice.end());

		for (int die = 0; die < defend_dice.size(); die++) {
			if (attack_dice[die] > defend_dice[die]) {
				defender.army--;
			}
			else {
				attacker.army--;
			}
		}
		attack_dice.clear();
		defend_dice.clear();
		if (defender.army == 0) {
			defender.army == 1;
			attacker.army--;
			defender.player == attacker.player;
			return;
		}
	}
}