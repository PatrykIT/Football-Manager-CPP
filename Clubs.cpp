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

	players.reserve(23);

	history.reserve(50);
}

Club::~Club()
{
	for(unsigned int i = 0; i < players.size(); ++i)
		Transfers::get()->free_players.push_back(players[i]); //When deleting the club, this adds it's players to transfer list (makes them free agents).
		//free_players.push_back(players[i]);
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
		cout << "Player: " << defenders_in_first_squad[i]->name << " " << defenders_in_first_squad[i]->surname << " | Overall: " << defenders_in_first_squad[i]->Get_Overall() << " (DEF)" << endl;
	}

	for(i = 0; i < midfilders_in_first_squad.size(); ++i)
	{
		cout << "Player: " << midfilders_in_first_squad[i]->name << " " << midfilders_in_first_squad[i]->surname << " | Overall: " << midfilders_in_first_squad[i]->Get_Overall() << " (MID)" << endl;
	}

	for(i = 0; i < attackers_in_first_squad.size(); ++i)
	{
		cout << "Player: " << attackers_in_first_squad[i]->name << " " << attackers_in_first_squad[i]->surname << " | Overall: " << attackers_in_first_squad[i]->Get_Overall() << " (ATT)" << endl;
	}

	Print_Tactic_Rating();
	printf("\n");
}

void Club::Print_Whole_Squad() const
{
	printf("\n---Whole Squad [%d]---\n", _ID);
	for(unsigned int i = 0; i < players.size(); ++i)
	{
		cout << i <<": " << players[i]->name << " " << players[i]->surname << " | Overall: " << players[i]->Get_Overall() << " | Position: " << players[i]->Get_Position()
				<< " | Value: " << players[i]->Get_Value() << "$" << endl;
	}

}

int Club::Buy_Player()
{
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

	int position_to_buy;
	cout << endl << "What position would you like to buy? 1 - Def. 2 - Mid. 3 - Att." << endl <<"Choice: \t";

	do
	{
		cin >> position_to_buy;
	}
	while(position_to_buy < 1 && position_to_buy > 3);

	printf("\t--- Current budget: %f$ ---\n", _budget);


	for(unsigned int i = 0; i < transfer->free_players.size(); ++i)
	{
		if (transfer->free_players.at(i)->Get_Position() == position_to_buy)
		{
			cout << endl << "Found: " << transfer->free_players[i]->name << " "<< transfer->free_players[i]->surname
					<< ". His overall: " << transfer->free_players[i]->Get_Overall() <<"%" << endl; transfer->free_players[i]->Print_Value();
			cout << "Would you like to buy him, or look for other option?" << endl << "Y or N: \t";
			char buy; cin >> buy;

			if (buy == 'Y' || buy == 'y')
			{
				if (_budget < transfer->free_players[i]->Get_Value())
				{
					printf("I am sorry. There is not enough money in the budget (%f$). Would you like to sell someone (Y) or keep looking (N)?\nY or N:\t", _budget);

					char sell;
					cin >> sell;
					if (sell == 'y' || sell == 'Y')
					{
						const int ret = Sell_Player();

						if (ret == 0 && _budget >= transfer->free_players[i]->Get_Value())
							{} //Player sold, we can afford another player. Continue with the flow of the program, in order to buy player. (Didn't want to repeat lines, so I just put empty statement here, it will land in Add_Player_to_Club() anyways.)
						else
							continue;
					}
					else //means keep looking
						continue;
				}

				const int ret = Add_Player_to_Club(*transfer->free_players[i]);

				if(ret == 0)
				{

					_budget = _budget - transfer->free_players[i]->Get_Value();
					transfer->Player_Bought(*transfer->free_players[i], *this); //Inform transfer list that we're buying from it.
					transfer->free_players.erase(transfer->free_players.begin() + i); //Remove player from transfer list.

					cout << "Player bought. Current budget: " << _budget << "$" << endl;
					Set_Tactics();
					return 0; //Exit infinite loop
				}
				else
				{
					cout << "Cannot add new player." << endl;
					return -1;
				}
			}
		}
		else
		{
			cout << "Searching.." << endl;
			cout << transfer->free_players[i]->name << " "<< transfer->free_players[i]->surname << "  | Position: "
					<< transfer->free_players[i]->Get_Position() << " | Value: " <<  transfer->free_players[i]->Get_Value() << "$" << endl;
		}
	}

	cout << "Searched whole transfer list. You haven't decided. Exiting transfer list." << endl;
	return -1;
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
		Set_Tactics();

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
















