#ifndef TRANSFERS_HPP_
#define TRANSFERS_HPP_
#include <vector>

#include "Attributes.h"
class Club;

class Transfers
{
private:
	friend class Club; //For giving access to clubs for buying players.

	Transfers();
	Transfers(const Transfers &other);
	Transfers& operator=(const Transfers& other);

	static Transfers *transfers;
	std::vector<Player*> free_players; //free agents - players without clubs, ready to be bought.

	void Player_Bought(Player &player);

public:
	static Transfers* get();
	~Transfers();
};



#endif /* TRANSFERS_HPP_ */
