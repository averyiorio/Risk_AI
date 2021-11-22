#ifndef _CARD_H_
#define _CARD_H_
#include <string>
class Card {
public:
	//CONSTRUCTOR
	Card(const std::string & _name, char _type) : name(_name), type(_type) {}
	const std::string & getName() const {return name;}
	const char & getType() const {return type;}

	bool operator==(const Card& right) { return name == right.name && type == right.type; }

private:
	std::string name;
	//card type 1=infantry, 2=calvalry, 3=artillery, 4=wild
	char type;
};
#endif