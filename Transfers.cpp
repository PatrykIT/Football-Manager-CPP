#include "Transfers.hpp"
#include "Table.h"

Transfers* Transfers::transfers = nullptr;

Transfers* Transfers::get()
{
	if(transfers == nullptr)
	{
		transfers = new Transfers;

	}

	return transfers;
}

Transfers::Transfers()
{
	free_players.reserve(30);

	for(unsigned j = 0; j < free_players.capacity(); ++j)
	{
		//free_players[j] = new Player;
		free_players.push_back(new Player);
	}
}

Transfers::~Transfers()
{
	for(unsigned int i = 0; i < free_players.size(); ++i)
	{
		delete free_players[i];
	}
}

void Transfers::Player_Bought(Player &player)
{
	Table::Add_Player_to_Observe(player);
}
