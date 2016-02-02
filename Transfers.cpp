#include "Transfers.hpp"
#include "Table.h"
#include "Attributes.h"
#include <atomic>

Transfers* Transfers::transfers = nullptr;
std::mutex Transfers::transfer_mutex;

Transfers* Transfers::get()
{
	/**
	 * Uses Double Checked Locking Pattern. 'transfers' should be of type std::atomic<Transfers*>, but it created a Seg Fault - don't know why, maybe compiler issues?
	 * http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/ (Used the version with memory barriers for educational purposes)
	 * http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
	 *
	 * The first barrier must prevent downwards migration of Singleton's construction (by another thread);
	 * the second barrier must prevent upwards migration of 'transfers' initialization. These are called "acquire" and "release" operations.
	 */

	Transfers *tmp = transfers;
	std::atomic_thread_fence(std::memory_order_acquire);

	if(tmp == nullptr)
	{
		std::lock_guard<std::mutex> my_lock (transfer_mutex);
		tmp = transfers;

		if(tmp == nullptr)
		{
			tmp = new Transfers;
			std::atomic_thread_fence(std::memory_order_release);
			transfers = tmp;
		}
	}
	return tmp;
}

/**
 * OR:
 * std::once_flag initFlag;
 *
 * void init (){ instance = new Transfers; }
 *
 * Transfers & getInstance (){
 * 	std::call_once (initFlag, init);
 * 	return *instance;
 * }
 */

Transfers::Transfers()
{
	free_players.reserve(30);

	for(unsigned j = 0; j < free_players.capacity(); ++j)
	{
		//free_players[j] = new Player;
		free_players.emplace_back(new Player);
	}
}

Transfers::~Transfers()
{
	for(unsigned int i = 0; i < free_players.size(); ++i)
	{
		delete free_players[i];
	}
}

void Transfers::Player_Bought(Player &player, Club &club) const
{
	Table::Add_Player_to_Observe(player, club);
}
