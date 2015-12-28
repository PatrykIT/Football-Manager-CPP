#include <algorithm>
#include "Clubs.h"

using namespace std;

int Club::_instance_number = 0;

Club::Club() :_ID(Club::_instance_number++), club_name (club_names[rand() % (sizeof(club_names) / sizeof(club_names[0]))]), city_name(cities[rand() % (sizeof(cities) / sizeof(cities[0]))])
{
	players.reserve(23);

	tactic[0] = -1; //Say that this position is not yet filled with players.
	tactic[1] = -1;
	tactic[2] = -1;
	_tactic_rating = 0;

	number_of_attackers_in_first_squad = 0;
	number_of_midfilders_in_first_squad = 0;
	number_of_defenders_in_first_squad = 0;

	_budget = 10000.000 + (rand() / (RAND_MAX / (99999.999 - 10000.000))); //values 1000.0000 - 99999.999 $

	points = 0;
	goals_scored = 0, goals_conceded = 0;
	matches_played = 0;
	matches_won = 0, matches_lost = 0, matches_drawn = 0;

	number_of_stories = 50;
	_history = new History[number_of_stories];
	_history_messages_counter = 0;

	_attendance = stadium.Get_Capacity();
	_ticket_prices = 10;

	_allowed_to_play = false; //Can not play yet. It has to have at least 10 outfield players, and players in each formation.
}

Club::~Club()
{
	for(unsigned int i = 0; i < players.size(); ++i)
		free_players.push_back(players[i]); //When deleting the club, this adds it's players to transfer list (makes them free agents).

	players.clear();
	delete []_history;
}

void Club::Increment_History_Messages_Counter()
{
	++_history_messages_counter;
}

int Club::Get_Message_Counter() const
{
	return _history_messages_counter;
}

void Club::Resize_History()
{
	number_of_stories = number_of_stories * 2;

	History *tmp = new History[number_of_stories];
	copy(_history, _history + _history_messages_counter, tmp);

	delete []_history;

	_history = tmp;

}

void Club::Set_Tactic_Rating()
{
	double sum = 0;
	unsigned int i;
	for(i = 0; i < number_of_defenders_in_first_squad; ++i)
		sum = sum + defenders_in_first_squad[i]->Get_Overall();
	for(i = 0; i < number_of_midfilders_in_first_squad; ++i)
		sum = sum + midfilders_in_first_squad[i]->Get_Overall();
	for(i = 0; i < number_of_attackers_in_first_squad; ++i)
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
	cout << endl << "\t --- Club History ---" << endl;
	for(int i = 0; i < Get_Message_Counter(); ++i)
	{
		cout << _history[i].message << endl;
	}
	cout << endl << endl;
}

int Club::Get_ID() const
{
	return _ID;
}

char Club::Check_if_Allowed_to_Play() const
{
	return _allowed_to_play;
}

double Club::Get_Tactic_Rating() const
{
	return _tactic_rating;
}



int Club::Add_Player_to_Club(Player &player)
{
	if( players.size() < 23 )
	{
		players.push_back(&player);
	}
	else
	{
		printf ("There is already 23 players, cannot add new player.\n");
		return -1;
	}


	string tmp = "Bought ";
	tmp.append((player).name).append(" ").append((player).surname);

	_history[_history_messages_counter].message = tmp;
	_history->Save_History(*this);

	return 0;
}




void Club::List_Players() const
{
	printf("\n---Listing players---\n\n");
	for (unsigned i = 0 ; i < players.size(); ++i)
	{
		printf("%s %s\nOverall: %.2f%\n", players[i]->name, players[i]->surname, players[i]->Get_Overall());
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


bool Compare_Overall (Player* a, Player* b) { return a->Get_Overall() > b->Get_Overall(); }

int Club::Set_Tactics()
{
	if(players.size() < 11)
	{
		cout << "Not enough players to set tactic!" << endl;
		return -1;
	}

	sort(players.begin(), players.end(), Compare_Overall); //Ordering players from best to worse.

//---------------------------------------------------------------------SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------
	number_of_attackers_in_first_squad = 0, number_of_midfilders_in_first_squad = 0, number_of_defenders_in_first_squad = 0;
	unsigned int max_attackers = 3, min_attackers = 1, max_midfilders = 5, min_midfilders = 3, max_defenders = 5, min_defenders = 3;
	unsigned int i = 0;

	do //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1
	{
		if(players[i]->Get_Position() == 1 && (number_of_defenders_in_first_squad < max_defenders ))
			if((number_of_defenders_in_first_squad + 1 == max_defenders) && (number_of_midfilders_in_first_squad == max_midfilders || number_of_attackers_in_first_squad == max_attackers))
			{} //can not have two max of positions, because the third one would not get a place.
		else
		{
			defenders_in_first_squad[number_of_defenders_in_first_squad] = players[i]; //For example, a third best player is the first best as a defender. We are binding his index in whole team "players[3]", to the index of a defenders "defender[1]".
			++number_of_defenders_in_first_squad;
		}


		if(players[i]->Get_Position() == 2 && number_of_midfilders_in_first_squad < max_midfilders)
			if((number_of_midfilders_in_first_squad + 1 == max_midfilders)  && (number_of_attackers_in_first_squad == max_attackers  ||  number_of_defenders_in_first_squad == max_defenders))
				{}
		else
		{
			midfilders_in_first_squad[number_of_midfilders_in_first_squad] = players[i];
			++number_of_midfilders_in_first_squad;
		}


		if(players[i]->Get_Position() == 3 && number_of_attackers_in_first_squad < max_attackers)
			if((number_of_attackers_in_first_squad + 1 == max_attackers) && (number_of_midfilders_in_first_squad == max_midfilders || number_of_defenders_in_first_squad == max_defenders))
				{}
			else
			{
				attackers_in_first_squad[number_of_attackers_in_first_squad] = players[i];
				++number_of_attackers_in_first_squad;
			}

		++i;
	} while  ((number_of_attackers_in_first_squad + number_of_midfilders_in_first_squad + number_of_defenders_in_first_squad < 10) && (i != players.size()));

	if( (number_of_attackers_in_first_squad < min_attackers || number_of_midfilders_in_first_squad < min_midfilders || number_of_defenders_in_first_squad < min_defenders) || (number_of_attackers_in_first_squad +
			number_of_midfilders_in_first_squad + number_of_defenders_in_first_squad != 10 ))
	{
		printf("\nCouldn't set proper tactics! Not enough players for a position. [%d]\n", _ID);
		printf("Attackers: %d\nMidfilders: %d\nDefenders: %d\n\n", number_of_attackers_in_first_squad, number_of_midfilders_in_first_squad, number_of_defenders_in_first_squad);
		return -1;
	}

//---------------------------------------------------------------------END OF SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------


	tactic[0] = number_of_defenders_in_first_squad; //every value in index means number of players in this position.
	tactic[1] = number_of_midfilders_in_first_squad;
	tactic[2] = number_of_attackers_in_first_squad;

	Set_Tactic_Rating();

	Allow_Playing(); //Coming to this line, it means that we successfully created formation, and added players.

	return 0;
}


void Club::Print_Formation() const
{
	printf("\n---Current formation---\n");
	cout << club_name << " " << city_name << "plays: ";

	for(int i = 0; i < 3; ++i)
		printf("%d ", tactic[i]);
	printf("\n");
}

void Club::Print_First_Squad() const
{
	printf("\n---First Squad [%d]---\n", _ID);
	unsigned int i;
	for(i = 0; i < number_of_defenders_in_first_squad; ++i)
	{
		cout << "Player: " << defenders_in_first_squad[i]->name << " " << defenders_in_first_squad[i]->surname << " | Overall: " << defenders_in_first_squad[i]->Get_Overall() << " (DEF)" << endl;
	}

	for(i = 0; i < number_of_midfilders_in_first_squad; ++i)
	{
		cout << "Player: " << midfilders_in_first_squad[i]->name << " " << midfilders_in_first_squad[i]->surname << " | Overall: " << midfilders_in_first_squad[i]->Get_Overall() << " (MID)" << endl;
	}

	for(i = 0; i < number_of_attackers_in_first_squad; ++i)
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
	if (players.size() >= 23)
	{
		cout << "Squad is full. Cannot add another player." << endl;
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

	while(1) //Infinite loop, in case a player never decides - he will have to. :)
	{
		bool bought = false;

		for(unsigned int i = 0; i < free_players.size(); ++i)
		{
			if (free_players.at(i)->Get_Position() == position_to_buy)
			{
				cout << endl << "Found: " << free_players[i]->name << " "<< free_players[i]->surname << ". His overall: " << free_players[i]->Get_Overall() <<"%" << endl; free_players[i]->Print_Value();
				cout << "Would you like to buy him, or look for other option?" << endl << "Y or N: \t";
				char buy; cin >> buy;

				if (buy == 'Y' || buy == 'y')
				{
					if (_budget < free_players[i]->Get_Value())
					{
						printf("I am sorry. There is not enough money in the budget (%f$). Would you like to sell someone (Y) or keep looking (N)?\nY or N:\t", _budget);

						char sell;
						cin >> sell;
						if (sell == 'y' || sell == 'Y')
						{
							const int ret = Sell_Player();

							if (ret == 0 && _budget >= free_players[i]->Get_Value())
								{} //Player sold, we can afford another player. Continue with the flow of the program, in order to buy player. (Didn't want to repeat lines, so I just put empty statement here, it will land in Add_Player_to_Club() anyways.)_
							else
								continue;
						}
						else //means keep looking
							continue;
					}

					const int ret = Add_Player_to_Club( *free_players[i] );

					if(ret == 0)
					{
						bought = true;
						cout << "Player bought." << endl;

						_budget = _budget - free_players[i]->Get_Value();
						free_players.erase(free_players.begin() + i); //Remove player from free players (transfer list).
					}
					else
					{
						cout << "Cannot add new player." << endl;
						return -1;
					}

					break;
				}
			}
			else
			{
				cout << "Searching.." << endl;
				cout << free_players[i]->name << " "<< free_players[i]->surname << "  | Position: " << free_players[i]->Get_Position()
						<< " | Value: " <<  free_players[i]->Get_Value() << "$" << endl;
			}
		}

		if (bought == true)
		{
			Set_Tactics();
			Print_First_Squad();

			break; //Exit infinite loop
		}
		else
		{
			cout << "Searched whole transfer list. You haven't decided. Exiting transfer list." << endl;
			return -1;
		}
	}

	return 0; //successfully bought a player.
}

int Club::Sell_Player()
{
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


	free_players.push_back(players[player_to_sell]);
	_budget += players[player_to_sell]->Get_Value();

	string tmp = "Sold ";
	tmp.append(players[player_to_sell]->name).append(" ").append(players[player_to_sell]->surname);

	_history[_history_messages_counter].message = tmp;
	_history->Save_History(*this);

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
	cout << "Attackers: " << number_of_attackers_in_first_squad << endl << "Midfielders: " << number_of_midfilders_in_first_squad << endl <<
			"Defenders: " << number_of_defenders_in_first_squad << endl;
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

void Club::Improve_Skills(bool won)
{
	unsigned int i;
	for(i = 0; i < number_of_defenders_in_first_squad; ++i)
	{
		++defenders_in_first_squad[i]->attributes.defending_attributes.interceptions;
		++defenders_in_first_squad[i]->attributes.defending_attributes.marking;
		++defenders_in_first_squad[i]->attributes.defending_attributes.slide_tackle;
		++defenders_in_first_squad[i]->attributes.defending_attributes.stand_tackle;
	}

	for(i = 0; i < number_of_midfilders_in_first_squad; ++i)
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
	}

	for(i = 0; i < number_of_attackers_in_first_squad; ++i)
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















