#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <iostream>
#include <string>
#include <random>

#include "Clubs.h"

using namespace std;

int Club::_instance_number = 0;

Club::Club() :_ID(Club::_instance_number++), club_name (club_names[rand() % (sizeof(club_names) / sizeof(club_names[0]))]), city_name(cities[rand() % (sizeof(cities) / sizeof(cities[0]))])

{
	number_of_players = 0;

	tactic[0] = -1; //Say that this position is not yet filled with players.
	tactic[1] = -1;
	tactic[2] = -1;

	number_of_attackers_in_first_squad = 0;
	number_of_midfilders_in_first_squad = 0;
	number_of_defenders_in_first_squad = 0;

	_budget = 10000.000 + (rand() / (RAND_MAX / (99999.999 - 10000.000))); //values 1000.0000 - 99999.999 $

	_tactic_rating = 0;

	points = 0;
	goals_scored = 0, goals_conceded = 0;
	matches_played = 0;
	matches_won = 0, matches_lost = 0, matches_drawn = 0;

	number_of_stories = 50;
	_history = new History[number_of_stories];
	_history_messages_counter = 0;

	_allowed_to_play = 0; //Can not play yet. It has to have at least 10 outfield players, and players in each formation.
}

Club::~Club()
{
	delete []_history;
}

void Club::Increment_History_Messages_Counter()
{
	++_history_messages_counter;
}

int Club::Get_Message_Counter()
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

void Club::Set_Tactic_Rating(double sum)
{
	_tactic_rating = sum / 10; // 10 - number of outfield players.
}

void Club::Allow_Playing()
{
	_allowed_to_play = 1;
}

void Club::Print_Tactic_Rating()
{
	cout << "\nRating of a squad: " << _tactic_rating << endl;
}

void Club::Print_History()
{
	cout << endl << "\t --- Club History ---" << endl;
	for(int i = 0; i < Get_Message_Counter(); ++i)
	{
		cout << _history[i].message << endl;
	}
	cout << endl << endl;
}

int Club::Get_ID()
{
	return _ID;
}

char Club::Check_if_Allowed_to_Play()
{
	return _allowed_to_play;
}

double Club::Get_Tactic_Rating()
{
	return _tactic_rating;
}



int Club::Add_Player_to_Club(Player &player)
{
	if( number_of_players < 23 )
	{
		players[number_of_players] = &player;
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

	++number_of_players;
	return 0;
}




void Club::List_Players()
{
	printf("\n---Listing players---\n\n");
	int i;
	for ( i = 0 ; i < number_of_players; ++i)
	{
		printf("%s %s\nOverall: %.2f%\n", players[i]->name, players[i]->surname, players[i]->overall);
		switch (players[i]->position)
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
	if(number_of_players < 11)
	{
		printf("Not enough players to set tactic!\n");
		return -1;
	}

//------------------------------------------------------------------------ORDERING PLAYERS FROM BEST TO WORSE-------------------------------------------------------------------------
	int i;
	int swapped;
	do
	{
		swapped = 0;

		for(i = 0; i < number_of_players - 1; ++i)
		{
			if( players[i]->overall < players[i+1]->overall)
			{
				struct Player *tmp = players[i];
				players[i] = players[i+1];
				players[i+1] = tmp;
			}
			else
				++swapped;
		}
	} while(swapped != number_of_players - 1); //because swap operation takes one cycle less than number of players. //If there was no swap all the way, means everything is ordered.

//------------------------------------------------------------------------------END OF ORDERING-------------------------------------------------------------------------


//---------------------------------------------------------------------SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------
	number_of_attackers_in_first_squad = 0, number_of_midfilders_in_first_squad = 0, number_of_defenders_in_first_squad = 0;
	int max_attackers = 3, min_attackers = 1, max_midfilders = 5, min_midfilders = 3, max_defenders = 5, min_defenders = 3;
	int current_attackers = 0, current_midfilders = 0, current_defenders = 0;
	int attacker_index = 0, midfilder_index = 0, defender_index = 0;
	i = 0;

	//NOTE: Maybe current_attackers, current mid, current def are unnecessary.

	do //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1
	{
		if(players[i]->position == 1 && (current_defenders < max_defenders ))
			if(  (current_defenders + 1 == max_defenders) && (current_midfilders == max_midfilders || current_attackers == max_attackers) )
			{} //can not have two max of positions, because the third one would not get a place.
		else
		{
			defenders_in_first_squad[defender_index] = players[i]; //For example, a third best player is the first best as a defender. We are binding his index in whole team "players[3]", to the index of a defenders "defender[1]".
			++current_defenders;
			++defender_index;
			++number_of_defenders_in_first_squad;
		}


		if(players[i]->position == 2 && current_midfilders < max_midfilders)
			if(  (current_midfilders + 1 == max_midfilders)  && (current_attackers == max_attackers  ||  current_defenders == max_defenders) )
				{}
		else
		{
			midfilders_in_first_squad[midfilder_index] = players[i];
			++current_midfilders;
			++midfilder_index;
			++number_of_midfilders_in_first_squad;
		}


		if(players[i]->position == 3 && current_attackers < max_attackers)
			if(  (current_attackers + 1 == max_attackers) && (current_midfilders == max_midfilders || current_defenders == max_defenders) )
				{}
			else
			{
				attackers_in_first_squad[attacker_index] = players[i];
				++current_attackers;
				++attacker_index;
				++number_of_attackers_in_first_squad;
			}

		++i;
	} while  ( (current_attackers + current_midfilders + current_defenders < 10) && (i != number_of_players));

	if( (current_attackers < min_attackers || current_midfilders < min_midfilders || current_defenders < min_defenders) || (current_attackers +
			current_midfilders + current_defenders != 10 ))
	{
		printf("Couldn't set proper tactics! Not enough players for a position.\n");
		printf("Attackers: %d\nMidfilders: %d\nDefenders: %d\n", current_attackers, current_midfilders, current_defenders);
		return -1;
	}

//---------------------------------------------------------------------END OF SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------


	tactic[0] = current_defenders; //every value in index means number of players in this position.
	tactic[1] = current_midfilders;
	tactic[2] = current_attackers;


	double sum = 0;

	for(i = 0; i < current_defenders; ++i)
		sum = sum + defenders_in_first_squad[i]->overall;
	for(i = 0; i < current_midfilders; ++i)
		sum = sum + midfilders_in_first_squad[i]->overall;
	for(i = 0; i <current_attackers; ++i)
		sum = sum + (*attackers_in_first_squad[i]).overall;

	Set_Tactic_Rating(sum);

	Allow_Playing(); //Coming to this line, it means that we successfully created formation, and added players.

	return 0;
}


void Club::Print_Formation()
{
	printf("\n---Current formation---\n");
	cout << club_name << " " << city_name << "plays: ";
	int i = 0;
	for(; i < 3; ++i)
		printf("%d ", tactic[i]);
	printf("\n");
}

void Club::Print_First_Squad()
{
	printf("\n---First Squad [%d]---\n", _ID);
	int i;
	for(i = 0; i < number_of_defenders_in_first_squad; ++i)
	{
		cout << "Player: " << defenders_in_first_squad[i]->name << " " << defenders_in_first_squad[i]->surname << " | Overall: " << defenders_in_first_squad[i]->overall << " (DEF)" << endl;
	}

	for(i = 0; i < number_of_midfilders_in_first_squad; ++i)
	{
		cout << "Player: " << midfilders_in_first_squad[i]->name << " " << midfilders_in_first_squad[i]->surname << " | Overall: " << midfilders_in_first_squad[i]->overall << " (MID)" << endl;
	}

	for(i = 0; i < number_of_attackers_in_first_squad; ++i)
	{
		cout << "Player: " << attackers_in_first_squad[i]->name << " " << attackers_in_first_squad[i]->surname << " | Overall: " << attackers_in_first_squad[i]->overall << " (ATT)" << endl;
	}

	Print_Tactic_Rating();
	printf("\n");
}

int Club::Buy_Player()
{
	if (number_of_players >= 23)
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

		for(int i = 0; i < number_of_free_players; ++i) //Sprawdzić przypadek, kiedy brakuje np. 2 zawodników. Może za drugim zakupem zabraknąć kasy, wtedy walkower leci.
		{
			if(free_players.at(i)->position == position_to_buy && _budget > free_players[i]->value) //Second time, there is no need to use 'at()', because if we came to second, means it is valid index. :)
			{
				cout << endl << "Found: " << free_players[i]->name << " "<< free_players[i]->surname << ". His overall: " << free_players[i]->overall <<"%" << endl;
				cout << "He costs: " << free_players[i]->Print_Value() << endl; //Print Value() cannot be void because "forming reference to void" O.o   Check this out!
				cout << "Would you like to buy him, or look for other option?" << endl << "Y or N: \t";
				char buy; cin >> buy;

				if (buy == 'Y' || buy == 'y')
				{
					//int ret = Add_Player_to_Club( &free_players[i] );
					int ret = Add_Player_to_Club( *free_players[i] );

					if(ret == 0)
					{
						printf("Budget before: %f\n", _budget);

						bought = true;
						_budget = _budget - free_players[i]->value;

						printf("Budget after: %f\n", _budget);
					}
					else
					{
						cout << "Cannot add new player." << endl;
						return -1;
					}

					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				printf ("Searching..\n");
				cout << free_players[i]->name << " "<< free_players[i]->surname << "  | Position: " << free_players[i]->position << " | Value: " << free_players[i]->value << "$" << endl;
			}
		}

		if(bought == true)
		{
			printf("Player bought.\n");
			Set_Tactics();
			this->Print_First_Squad();
			break; //Exit infinite loop
		}
		else //Searched all players, didn't buy anyone.
		{
			cout << "Searched whole transfer list. You haven't decided, or your budget is too little. Giving walkower." << endl; //Tutaj dodać zapytanie, czy chciałbym kogoś sprzedać.
			return -1;
		}
	}

	return 1; //successfully bought a player.
}


void Club::Print_Positions_Number()
{
	cout << "Current positions in first squad: " << endl;
	cout << "Attackers: " << number_of_attackers_in_first_squad << endl << "Midfielders: " << number_of_midfilders_in_first_squad << endl <<
			"Defenders: " << number_of_defenders_in_first_squad << endl;
}




