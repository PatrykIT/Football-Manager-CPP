#include "Clubs.h"
#include "Transfers.hpp"
#include "History.h"
#include "Attributes.h"

#include <algorithm>
using namespace std;

int Club::_instance_number = 0;
mutex Club::transfer_list;

Club::Club(int budget) :_ID(Club::_instance_number++),
		_allowed_to_play (false), //Can not play yet. It has to have at least 10 outfield players, and players in each formation.
		_tactic_rating (0),
		_budget (budget),
		matches_won (0), matches_lost (0), matches_drawn (0),
		_attendance (stadium.Get_Capacity()),
		_ticket_prices (10),
		club_name (club_names[rand() % (sizeof(club_names) / sizeof(club_names[0]))]),
		city_name(cities[rand() % (sizeof(cities) / sizeof(cities[0]))])
{
	tactic[0] = -1; //Say that this position is not yet filled with players.
	tactic[1] = -1;
	tactic[2] = -1;

	defenders_in_first_squad.reserve(5);
	midfilders_in_first_squad.reserve(5);
	attackers_in_first_squad.reserve(5);

	players.reserve(23);

	history.reserve(50);
}

Club::~Club()
{
	for(unsigned int i = 0; i < players.size(); ++i)
		Transfers::get()->free_players.push_back(players[i]); //When deleting the club, this adds it's players to transfer list (makes them free agents).
}

void Club::Set_Tactic_Rating()
{
	double sum = 0;
	unsigned int i;
	for(i = 0; i < defenders_in_first_squad.size(); ++i)
		sum = sum + defenders_in_first_squad[i]->Get_Overall();
	for(i = 0; i < midfilders_in_first_squad.size(); ++i)
		sum = sum + midfilders_in_first_squad[i]->Get_Overall();
	for(i = 0; i < attackers_in_first_squad.size(); ++i)
		sum = sum + attackers_in_first_squad[i]->Get_Overall();

	_tactic_rating = sum / 10; // 10 - number of outfield players.
}

void Club::Allow_Playing()
{
	_allowed_to_play = true;
}

void Club::Print_Tactic_Rating() const
{
	cout << "\nRating of a squad: " << _tactic_rating << endl;
}

void Club::Print_History() const
{
	cout << "\n\t --- Club History ---" << endl;

	for(unsigned int i = 0; i < history.size(); ++i)
		cout << history[i] << endl;

	cout << endl << endl;
}

int Club::Get_ID() const
{
	return _ID;
}

bool Club::Check_if_Allowed_to_Play() const
{
	return _allowed_to_play;
}

double Club::Get_Tactic_Rating() const
{
	return _tactic_rating;
}


int Club::Add_Player_to_Club(Player &player)
{
	unique_lock<mutex> lock (players_vector);
	if( players.size() < 23 )
	{
		players.push_back(&player);
	}
	else
	{
		printf ("There is already 23 players, cannot add new player.\n");
		return -1;
	}
	lock.unlock();

	string information = "Bought ";
	information.append(player.name).append(" ").append(player.surname);

	lock_guard<mutex> lock_history (history_mutex);
	history.emplace_back(History::Save_History(information));

	return 0;
}


void Club::List_Players() const
{
	printf("\n---Listing players---\n\n");
	for (unsigned i = 0 ; i < players.size(); ++i)
	{
		printf("%s %s\nOverall: %.2f%\n", players[i]->name.c_str(), players[i]->surname.c_str(), players[i]->Get_Overall());
		switch (players[i]->Get_Position())
			{
			case 1:
				printf("Defender.\n");
				break;
			case 2:
				printf("Midfilder.\n");
				break;
			case 3:
				printf("Striker.\n");
				break;
			default:
				printf("POSITION NOT SET!\n");
				break;
			}
		printf("\n");
	}
}



int Club::Set_Tactics()
{
	if(players.size() < 11)
	{
		cout << "Not enough players to set tactic!" << endl;
		return -1;
	}

	sort(players.begin(), players.end(), [] (const Player* a, const Player* b) { return a->Get_Overall() > b->Get_Overall(); });

//---------------------------------------------------------------------SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------
	attackers_in_first_squad.clear(); midfilders_in_first_squad.clear(); defenders_in_first_squad.clear();

	unsigned int max_attackers = 3, min_attackers = 1, max_midfilders = 5, min_midfilders = 3, max_defenders = 5, min_defenders = 3;
	unsigned int i = 0;

	do //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1
	{
		if(players[i]->Get_Position() == 1 && (defenders_in_first_squad.size() < max_defenders ))
			if((defenders_in_first_squad.size() + 1 == max_defenders) && (midfilders_in_first_squad.size() == max_midfilders || attackers_in_first_squad.size() == max_attackers))
				{} //can not have two max of positions, because the third one would not get a place.
			else
			{
				//defenders_in_first_squad[number_of_defenders_in_first_squad] = players[i]; //For example, a third best player is the first best as a defender. We are binding his index in whole team "players[3]", to the index of a defenders "defender[1]".
				defenders_in_first_squad.push_back(players[i]);  //For example, a third best player is the first best as a defender. We are binding his index in whole team "players[3]", to the index of a defenders "defender[1]".
			}


		if(players[i]->Get_Position() == 2 && midfilders_in_first_squad.size() < max_midfilders)
			if((midfilders_in_first_squad.size() + 1 == max_midfilders)  && (attackers_in_first_squad.size() == max_attackers  ||  defenders_in_first_squad.size() == max_defenders))
				{}
			else
			{
				midfilders_in_first_squad.push_back(players[i]);
			}


		if(players[i]->Get_Position() == 3 && attackers_in_first_squad.size() < max_attackers)
			if((attackers_in_first_squad.size() + 1 == max_attackers) && (midfilders_in_first_squad.size() == max_midfilders || defenders_in_first_squad.size() == max_defenders))
				{}
			else
			{
				attackers_in_first_squad.push_back(players[i]);
			}

		++i;
	} while  ((attackers_in_first_squad.size() + midfilders_in_first_squad.size() + defenders_in_first_squad.size() < 10) && (i != players.size()));

	if( (attackers_in_first_squad.size() < min_attackers || midfilders_in_first_squad.size() < min_midfilders || defenders_in_first_squad.size() < min_defenders)
			|| (attackers_in_first_squad.size() + midfilders_in_first_squad.size() + defenders_in_first_squad.size() != 10 ))
	{
		printf("\nCouldn't set proper tactics! Not enough players for a position. [%d]\n", _ID);
		printf("Attackers: %lu\nMidfilders: %lu\nDefenders: %lu\n\n", attackers_in_first_squad.size(), midfilders_in_first_squad.size(), defenders_in_first_squad.size());
		return -1;
	}

//---------------------------------------------------------------------END OF SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------


	tactic[0] = defenders_in_first_squad.size(); //every value in index means number of players in this position.
	tactic[1] = midfilders_in_first_squad.size();
	tactic[2] = attackers_in_first_squad.size();

	Set_Tactic_Rating();

	Allow_Playing(); //Coming to this line, it means that we successfully created formation, and added players.

	return 0;
}


void Club::Print_Formation() const
{
	printf("\n---Current formation---\n");
	cout << club_name << " " << city_name << " plays: ";

	for(int i = 0; i < 3; ++i)
		printf("%d ", tactic[i]);
	printf("\n");
}

void Club::Print_First_Squad() const
{
	printf("\n---First Squad [%d]---\n", _ID);
	unsigned int i;
	for(i = 0; i < defenders_in_first_squad.size(); ++i)
	{
		cout << "Player: " << defenders_in_first_squad[i]->name << " " << defenders_in_first_squad[i]->surname << " | Overall: " << defenders_in_first_squad[i]->Get_Overall()
				<< " (DEF)" << endl;
	}

	for(i = 0; i < midfilders_in_first_squad.size(); ++i)
	{
		cout << "Player: " << midfilders_in_first_squad[i]->name << " " << midfilders_in_first_squad[i]->surname << " | Overall: " << midfilders_in_first_squad[i]->Get_Overall()
				<< " (MID)" << endl;
	}

	for(i = 0; i < attackers_in_first_squad.size(); ++i)
	{
		cout << "Player: " << attackers_in_first_squad[i]->name << " " << attackers_in_first_squad[i]->surname << " | Overall: " << attackers_in_first_squad[i]->Get_Overall()
				<< " (ATT)" << endl;
	}

	Print_Tactic_Rating();
	printf("\n");
}

void Club::Print_Whole_Squad() const
{
	printf("\n---Whole Squad [%d]---\n", _ID);
	for(unsigned int i = 0; i < players.size(); ++i)
	{
		if(players[i]->Get_Position() == 1)
			cout << i <<": " << players[i]->name << " " << players[i]->surname << " | Overall: " << players[i]->Get_Overall() << " (DEF)"
				<< " | Value: " << players[i]->Get_Value() << "$" << endl;
		else if(players[i]->Get_Position() == 2)
			cout << i <<": " << players[i]->name << " " << players[i]->surname << " | Overall: " << players[i]->Get_Overall() << " (MID)"
							<< " | Value: " << players[i]->Get_Value() << "$" << endl;
		else if(players[i]->Get_Position() == 3)
			cout << i <<": " << players[i]->name << " " << players[i]->surname << " | Overall: " << players[i]->Get_Overall() << " (ATT)"
							<< " | Value: " << players[i]->Get_Value() << "$" << endl;
	}

}

int Club::Buy_Player()
{
	/**
	 * Checks whether squad is full. If so, asks user if he want's to sell somebody, and calls Sell_Player() if yes.
	 * Asks user which position he is looking for.
	 * Gives mutual exclusion to tamper with transfer list.
	 * Searches whole transfer list, printing players that match criteria.
	 * Asks user to pick particular player.
	 * Checks whether budget allows to buy. If not, then asks user if he want's to sell somebody.
	 * Transfers player from transfer list vector, to club.players vector, erasing him from the former.
	 *
	 */

	Transfers *transfer =  Transfers::get();

	if (players.size() >= 23)
	{
		cout << "Squad is full. Cannot add another player." << endl;

		cout << "Would you like to sell? Y or N:\t";
		char confirm;
		cin >> confirm;
		if (confirm == 'y' || confirm == 'Y')
			Sell_Player();
		else
			return -1;
	}

	Print_Positions_Number();
	printf("\t--- Current budget: %f$ ---\n", _budget);

	while(1)
	{
		int position_to_buy;

		cout << endl << "What position would you like to buy? 1 - Def. 2 - Mid. 3 - Att." << endl <<"Choice: \t";

		do
		{
			cin >> position_to_buy;
		}
		while(position_to_buy < 1 && position_to_buy > 3);

		lock_guard<mutex> lock (transfer_list);
		bool position_found = false; // To indicate whether there is at least one player available.

		for(unsigned int i = 0; i < transfer->free_players.size(); ++i)
		{
			if (transfer->free_players.at(i)->Get_Position() == position_to_buy)
			{
				cout << endl << i << ": " << transfer->free_players[i]->name << " "<< transfer->free_players[i]->surname
						<< ". His overall: " << transfer->free_players[i]->Get_Overall() <<"%\t" << transfer->free_players[i]->Get_Age() << " years old.\n";
				transfer->free_players[i]->Print_Value();

				position_found = true;
			}
		}
		if (!position_found)
		{
			cout << "There is currently no one playing this position in transfer list. Would you like to look for another position, or exit? 0 for exit:\t" << endl;
			int choice; cin >> choice;

			if(choice == 0)
				return -1;
			else
				continue;
		}

		cout << "Pick number of a player you'd like to buy:\t";
		int number;
		bool good_position;

		do
		{
			good_position = true;

			cin >> number;
			if(number < 0 || number >= transfer->free_players.size())
			{
				cout << "Number out of range! Enter again:\t";
			}
			else if(transfer->free_players.at(number)->Get_Position() != position_to_buy)
			{
				cout << "Entered player plays in different position. Enter again:\t";
				good_position = false;
			}
		}
		while(number < 0 || number >= transfer->free_players.size() || !good_position);

		/* We now have a player picked, now we have to check whether budget allows us to buy. */

		if (_budget < transfer->free_players[number]->Get_Value())
		{
			printf("I am sorry. There is not enough money in the budget (%f$). Would you like to sell someone (Y), buy another player (N), or exit?\nY | N | E:\t", _budget);

			char choice;
			cin >> choice;
			if (choice == 'y' || choice == 'Y')
			{
				const int ret = Sell_Player();

				if(ret != 0 || _budget < transfer->free_players[number]->Get_Value())
				{
					cout << "Failed to meet 2 terms.\n";
					cout << "Do you want to keep searching, or exit? Y or N:\t";
					char choice; cin >> choice;
					if (choice == 'y' || choice == 'Y')
						continue; //Restart loop, in order to look for another player.
					else
						return -1;
				}
			}
			else if(choice == 'e' || choice == 'E') //If user wants to buy another player, then restart the loop.
				return -1;
			else
				continue;
		}

		/* Coming here, means everything is good, and player can be bought */

		const int ret = Add_Player_to_Club(*transfer->free_players[number]);

		if(ret == 0)
		{
			_budget = _budget - transfer->free_players[number]->Get_Value();

			transfer->Player_Bought(*transfer->free_players[number], *this); //Inform transfer list that we're buying from it.
			transfer->free_players.erase(transfer->free_players.begin() + number); //Remove player from transfer list.

			cout << "Player bought. Current budget: " << _budget << "$" << endl;

			cout << "Would you like to pick his place in the team, or you want the best available tactic?\n1 for non-automation:\t";

			int choice; cin >> choice;

			if (choice == 1)
				Set_Custom_Tactic();
			else
				Set_Tactics();

			return 0;
		}
		else
		{
			cout << "Cannot add new player." << endl;
			return -1;
		}
	}

	return 0;
}


int Club::Sell_Player()
{
	/**
	 * Asks user which player he wants to sell from squad. Then, locks the mutex, copies player pointer to transfer_list vector,
	 * saves information to club history, and removes pointer from internal vector<players>.
	 */
	if (players.size() < 12)
	{
		cout << "Only " << players.size() << " in squad. Cannot sell." << endl;
		return -1;
	}

	Print_Whole_Squad();

	int player_to_sell;
	char confirm;

	do
	{
		do
		{
			cout << endl << "Which player would you like to sell?\nNr:\t ";
			cin >> player_to_sell;
		}
		while (player_to_sell < 0 || player_to_sell > players.size() - 1);

		cout << "You chose: " << players[player_to_sell]->name << " " << players[player_to_sell]->surname << endl << "Are you sure you want this player sold? Y or N" << endl;
		cin >> confirm;
	}
	while (confirm != 'Y' && confirm != 'y');

	unique_lock<mutex> lock (transfer_list); //Make sure no one accesses vector in transfer list.
	Transfers::get()->free_players.push_back(players[player_to_sell]);
	lock.unlock();
	
	_budget += players[player_to_sell]->Get_Value();

	string information = "Sold " + players[player_to_sell]->name + " " + players[player_to_sell]->surname + " for: " +
			(to_string(players[player_to_sell]->Get_Value())) + "$"; //ex: Sold Patrick Cyrklaff for: 7999$
	history.emplace_back(History::Save_History(information));

	swap(players[player_to_sell], players[players.size() -1]); //Swap the last player with sold player to put him in last position.
	players.pop_back(); //Delete sold player from vector.
	cout << "Player sold." << endl;

	if(_allowed_to_play) //If club was allowed to play, means this might have changed with selling a player. If the club was not allowed to play, selling another won't help in setting tactic :)
	{
		cout << "Would you like to pick tactic by youself or you want the best available tactic?\n1 for non-automation:\t";
		int choice;
		cin >> choice;
		if (choice == 1)
			Set_Custom_Tactic();
		else
			Set_Tactics();
	}

	return 0; //successfully sold a player.
}

void Club::Print_Positions_Number() const
{
	printf("Current positions in first squad [%d]:\n", _ID);
	cout << "Attackers: " << attackers_in_first_squad.size() << endl << "Midfielders: " << midfilders_in_first_squad.size() << endl <<
			"Defenders: " << defenders_in_first_squad.size() << endl;
}

void Club::Set_Ticket_Prices()
{
	if(matches_won > matches_lost)
		_ticket_prices = (matches_won - matches_lost) * 10; //for ex. 3 more wins, than set price at 30$.
	else
		_ticket_prices = 9;
}

void Club::Set_Attendancy()
{
	//In future: implement better algorithm affecting attendance.
	if(matches_won > matches_lost)
		_attendance = stadium.Get_Capacity();
	else
		_attendance = stadium.Get_Capacity() / 2;
}

void Club::Improve_Skills_After_Match(bool won)
{

	unsigned int i;
	for(i = 0; i < defenders_in_first_squad.size(); ++i)
	{
		++defenders_in_first_squad[i]->attributes.defending_attributes.interceptions;
		++defenders_in_first_squad[i]->attributes.defending_attributes.marking;
		if (won)
		{
			++defenders_in_first_squad[i]->attributes.defending_attributes.slide_tackle;
			++defenders_in_first_squad[i]->attributes.defending_attributes.stand_tackle;
		}
		defenders_in_first_squad[i]->_Set_Overall();
	}

	for(i = 0; i < midfilders_in_first_squad.size(); ++i)
	{
		++midfilders_in_first_squad[i]->attributes.mental_attributes.pressure_handling;
		if (won) //extra improving for winners.
		{
			++midfilders_in_first_squad[i]->attributes.mental_attributes.leadership;
			++midfilders_in_first_squad[i]->attributes.mental_attributes.flair;
			++midfilders_in_first_squad[i]->attributes.mental_attributes.concetration;
			++midfilders_in_first_squad[i]->attributes.mental_attributes.game_reading;
			++midfilders_in_first_squad[i]->attributes.mental_attributes.work_rate;
		}
		midfilders_in_first_squad[i]->_Set_Overall();
	}

	for(i = 0; i < attackers_in_first_squad.size(); ++i)
	{
		++attackers_in_first_squad[i]->attributes.attacking_attributes.ball_control;
		++attackers_in_first_squad[i]->attributes.attacking_attributes.first_touch;
		++attackers_in_first_squad[i]->attributes.attacking_attributes.passing;
		++attackers_in_first_squad[i]->attributes.attacking_attributes.weak_foot;
		if (won)
		{
			++attackers_in_first_squad[i]->attributes.attacking_attributes.dribbling;
			++attackers_in_first_squad[i]->attributes.attacking_attributes.finishing;
			++attackers_in_first_squad[i]->attributes.attacking_attributes.shooting;
		}
		attackers_in_first_squad[i]->_Set_Overall();
	}

	Set_Tactic_Rating();
}

void Club::Update_Players_Morale(bool result)
{
	unsigned int i;
	if (result == true)
	{
		for(i = 0; i < players.size(); ++i)
		{
			players[i]->psyche.morale += 10;

		if (players[i]->psyche.morale > 100 )
			players[i]->psyche.morale = 100; //In case we went above 100, we're setting the maximum.
		}
	}
	else
	{
		for(i = 0; i < players.size(); ++i)
		{
			players[i]->psyche.morale -= 10;

		if (players[i]->psyche.morale < 0 )
			players[i]->psyche.morale = 0;
		}
	}

}

void Club::Year_Passed()
{
	for(unsigned int i = 0; i < players.size(); ++i)
	{
		if(players[i]->_age < 24)
			Improve_Skills_New_Year(players[i], players[i]->_position);
		else if(players[i]->_age > 29)
			Decline_Skills_New_Year(players[i], players[i]->_position);
	}

	Set_Tactic_Rating();
}

void Club::Improve_Skills_New_Year(Player *&player, int position)
{
	if (position == 3)
	{
		player->attributes.attacking_attributes.ball_control += 2;
		player->attributes.attacking_attributes.crossing += 2;
		player->attributes.attacking_attributes.dribbling += 2;
		player->attributes.attacking_attributes.finishing += 2;
		player->attributes.attacking_attributes.first_touch += 2;
		player->attributes.attacking_attributes.passing += 2;
		player->attributes.attacking_attributes.passing += 2;
		player->attributes.attacking_attributes.shooting += 2;
		player->attributes.attacking_attributes.weak_foot += 2;
	}
	if (position == 2)
	{
		player->attributes.mental_attributes.aggression += 2;
		player->attributes.mental_attributes.concetration += 2;
		player->attributes.mental_attributes.flair += 2;
		player->attributes.mental_attributes.game_reading += 2;
		player->attributes.mental_attributes.leadership += 2;
		player->attributes.mental_attributes.pressure_handling += 2;
		player->attributes.mental_attributes.work_rate += 2;
	}
	if (position == 1)
	{
		player->attributes.defending_attributes.interceptions += 2;
		player->attributes.defending_attributes.marking += 2;
		player->attributes.defending_attributes.slide_tackle += 2;
		player->attributes.defending_attributes.stand_tackle += 2;
	}

	player->attributes.psyhical_attributes.accelaration += 2; //Physical attributes get better no matter the position.
	player->attributes.psyhical_attributes.agility += 2;
	player->attributes.psyhical_attributes.stamina += 2;
	player->attributes.psyhical_attributes.strength += 2;

	player->_Set_Overall();
}



void Club::Decline_Skills_New_Year(Player *&player, int position)
{
	if (position == 3)
	{
		player->attributes.attacking_attributes.ball_control -= 2;
		player->attributes.attacking_attributes.crossing -= 2;
		player->attributes.attacking_attributes.dribbling -= 2;
		player->attributes.attacking_attributes.finishing -= 2;
		player->attributes.attacking_attributes.first_touch -= 2;
		player->attributes.attacking_attributes.passing -= 2;
		player->attributes.attacking_attributes.passing -= 2;
		player->attributes.attacking_attributes.shooting -= 2;
		player->attributes.attacking_attributes.weak_foot -= 2;
	}
	if (position == 2)
	{
		player->attributes.mental_attributes.concetration += 3; //With age, mental skills get even better.
		player->attributes.mental_attributes.game_reading += 3;
		player->attributes.mental_attributes.leadership += 4;
		player->attributes.mental_attributes.pressure_handling += 4;
		player->attributes.mental_attributes.aggression -= 2;
		player->attributes.mental_attributes.flair -= 2;
		player->attributes.mental_attributes.work_rate -= 2;
	}
	if (position == 1)
	{
		player->attributes.defending_attributes.interceptions -= 2;
		player->attributes.defending_attributes.marking -= 2;
		player->attributes.defending_attributes.slide_tackle -= 2;
		player->attributes.defending_attributes.stand_tackle -= 2;
	}

	player->attributes.psyhical_attributes.accelaration -= 2;
	player->attributes.psyhical_attributes.agility -= 2;
	player->attributes.psyhical_attributes.stamina -= 2;
	player->attributes.psyhical_attributes.strength -= 2;

	player->_Set_Overall();
}


void Club::Set_Custom_Tactic()
{
	Print_Whole_Squad();
	Print_Formation();
	Print_First_Squad();

	cout << "Do you want to swap players (from same position), or rearrange formation?\n1 or 2:\t";
	int choice;
	do
	{
		cin >> choice;
	} while (choice != 1 && choice != 2);

//-------------------------------------------------------------------CHOICE 1------------------------------------------------------------------------------------------------------------
	if (choice == 1 ) //SWAPPING
	{
		int player1, player2;
		do
		{
			cout << "Write numbers of players that you want to swap.\nRemember, they must play the same position.\n:\t" << endl;
			cin >> player1 >> player2;
			if(player1 > players.size() || player2 > players.size()) //If user gave number bigger than people in team.
				continue;
		} while(players.at(player1)->Get_Position() != players.at(player2)->Get_Position());

		cout << "Changing " << players[player1]->name << " " << players[player1]->surname << " with " <<
				players[player2]->name << " " << players[player2]->surname << "." << endl;

		swap(players[player1], players[player2]); //Swapping players in the 'whole squad' container.

		switch(players[player1]->Get_Position())
		{
			case 1:
			{
					auto player_iter = find(defenders_in_first_squad.begin(), defenders_in_first_squad.end(), players[player2]); /* Searching by player2, because it is a number,
					not really a player. So when we swapped players, the second number is the position where original player is now(the one that was swapped to 'weaker' player) */
					if(player_iter != defenders_in_first_squad.end()) //If player swapped, previously was in first team
					{
						defenders_in_first_squad.erase(player_iter); //Remove old player from first squad.
						defenders_in_first_squad.push_back(players[player1]); //Add new player to first squad.
					}
					break;
			}
			case 2:
			{
				auto player_iter = find(midfilders_in_first_squad.begin(), midfilders_in_first_squad.end(), players[player2]);
				if(player_iter != midfilders_in_first_squad.end())
				{
					midfilders_in_first_squad.erase(player_iter);
					midfilders_in_first_squad.push_back(players[player1]);
				}
				break;
			}
			case 3:
			{
				auto player_iter = find(attackers_in_first_squad.begin(), attackers_in_first_squad.end(), players[player2]);
				if(player_iter != attackers_in_first_squad.end())
				{
					attackers_in_first_squad.erase(player_iter);
					attackers_in_first_squad.push_back(players[player1]);
				}
				break;
			}
		}

	}
//-------------------------------------------------------------------CHOICE 2------------------------------------------------------------------------------------------------------------
	else if (choice == 2) //Changing formation
	{
		tactic[0] = 0; //Zero out formation, to create new.
		tactic[1] = 0;
		tactic[2] = 0;

		int nr_defs, nr_mids, nr_att;
		bool formation_changed = false;

		while(!formation_changed)
		{
			do
			{
				cout << "What formation would you like to play? Enter 3 numbers:\t";
				cin >> nr_defs >> nr_mids >> nr_att;
			} while ( (nr_defs < 3 || nr_defs > 5) || (nr_mids < 3 || nr_mids > 5) || (nr_att < 1 || nr_att > 3) ||
					nr_defs + nr_mids + nr_att != 10);

			//-------------------------------Checking if there is enough players in squad for this tactic.------------------------------
			int nr_of_defs = 0, nr_of_mids = 0, nr_of_atts = 0;

			for(auto it = players.begin(); it != players.end(); ++it)
			{
				switch ( (*it)->Get_Position() )
				{
					case 1:
						++nr_of_defs;
						break;
					case 2:
						++nr_of_mids;
						break;
					case 3:
						++nr_of_atts;
						break;
				}
			}
			//-------------------------------If there are enough players for each formation.-----------------------------------------------
			if (nr_of_defs - nr_defs >= 0 && nr_of_mids - nr_mids >= 0 && nr_of_atts - nr_att >= 0)
			{
				tactic[0] = nr_defs;
				tactic[1] = nr_mids;
				tactic[2] = nr_att;

				defenders_in_first_squad.clear();
				midfilders_in_first_squad.clear();
				attackers_in_first_squad.clear();
				//-------------------------------------------------PICKING DEFENDERS-----------------------------------------
				cout << "Pick: " << nr_defs << " defenders from 'Whole Squad' listing.\n";
				for(int i = 0; i < nr_defs; ++i)
				{
					int defender_nr;
					cin >> defender_nr;
					if(defender_nr > players.size() || players.at(defender_nr)->Get_Position() != 1)
					{
						--i; //If number was bigger than size of squad, or picked player plays in other position, then restart this iteration.
						continue;
					}
					defenders_in_first_squad.push_back(players[defender_nr]);
				}
				//-------------------------------------------------PICKING MIDFIELDERS----------------------------------------
				cout << "Pick: " << nr_mids << " midfielders from 'Whole Squad' listing.\n";
				for(int i = 0; i < nr_mids; ++i)
				{
					int midfielder_nr;
					cin >> midfielder_nr;
					if(midfielder_nr > players.size() || players.at(midfielder_nr)->Get_Position() != 2)
					{
						--i; //If number was bigger than size of squad, or picked player plays in other position, then restart this iteration.
						continue;
					}
					midfilders_in_first_squad.push_back(players[midfielder_nr]);
				}
				//-------------------------------------------------PICKING ATTACKERS------------------------------------------
				cout << "Pick: " << nr_att << " attackers from 'Whole Squad' listing.\n";
				for(int i = 0; i < nr_att; ++i)
				{
					int attacker_nr;
					cin >> attacker_nr;
					if(attacker_nr > players.size() || players.at(attacker_nr)->Get_Position() != 3)
					{
						--i; //If number was bigger than size of squad, or picked player plays in other position, then restart this iteration.
						continue;
					}
					attackers_in_first_squad.push_back(players[attacker_nr]);
				}
			}
			//-------------------------------If there was not enough of players for some formation.----------------------------------------
			else
			{
				cout << "Lacking players to set this tactic, precisely:\n";
				if (nr_of_defs - nr_defs < 0)
					cout << "Not enough defenders.\n";
				if (nr_of_mids - nr_mids < 0)
					cout << "Not enough midfielders.\n";
				if (nr_of_atts - nr_att < 0)
					cout << "Not enough attackers.\n"; cout << endl << endl;
				cout << "Would you like to set another formation, or exit?\n0 for exit, other for continue:\t" << endl;
				int exit;
				cin >> exit;
				if (exit == 0)
					return;

				continue;
			}
			formation_changed = true;
			Set_Tactic_Rating();
		}
	}

	Allow_Playing();

	Print_Whole_Squad();
	Print_Formation();
	Print_First_Squad();
}

void Club::Interface_Message()
{
	printf("1: \t\t +++++ %s %s [%d] +++++ \n", club_name.c_str(), city_name.c_str(), _ID);
	printf("1: \t ----- Print_Tactic_Rating ----- \n");
	printf("2: \t ----- Print_History -----\n");
	printf("3: \t ----- Print_Formation -----\n");
	printf("4: \t ----- Print_First_Squad ----- \n");
	printf("5: \t ----- Print_Whole_Squad -----\n");
	printf("6: \t ----- List_Players -----\n");
	printf("7: \t ----- Print number of players in each formation. -----\n");
	printf("8: \t ----- Set_Custom_Tactic -----\n");
	printf("9: \t ----- Buy_Player -----\n");
	printf("10: \t ----- Sell_Player -----\n");
	printf("100: \t ----- Print Key Bindings -----\n");
	printf("0: EXIT\n");
}


void Club::User_Interface()
{
	Interface_Message();


	int choice = -1;

	while(choice != 0)
	{
		cout << "Enter number:\t "; cin >> choice; cout << endl;

		switch(choice)
		{
		case 1:
			Print_Tactic_Rating();
			break;
		case 2:
			Print_History();
			break;
		case 3:
			Print_Formation();
			break;
		case 4:
			Print_First_Squad();
			break;
		case 5:
			Print_Whole_Squad();
			break;
		case 6:
			List_Players();
			break;
		case 7:
			Print_Positions_Number();
			break;
		case 8:
			Set_Custom_Tactic();
			break;
		case 9:
			Buy_Player();
			break;
		case 10:
			Sell_Player();
			break;
		case 100:
			Interface_Message();
			break;

		default:
			if(choice != 0)
				cout << "Wrong number! Please try again.\n";
			break;
		}
	}
}
























