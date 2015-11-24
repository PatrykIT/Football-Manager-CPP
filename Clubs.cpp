#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <iostream>
#include <string>

#include "Clubs.h"
#include "History.h"

using namespace std;

int Club::_instance_number = 0;

Club::Club()
{
	club_name = club_names[rand() % (sizeof(club_names) / sizeof(club_names[0]))];
	city_name = cities[rand() % (sizeof(cities) / sizeof(cities[0]))];
	number_of_players = 0;

	tactic[0] = -1; //Say that this position is not yet filled with players.
	tactic[1] = -1;
	tactic[2] = -1;

	number_of_attackers_in_first_squad = 0;
	number_of_midfilders_in_first_squad = 0;
	number_of_defenders_in_first_squad = 0;
	_budget = rand() % (100000) + 10000; //values - 10.000 - 100.000 $
	_tactic_rating = 0;

	points = 0;
	goals_scored = 0, goals_conceded = 0;
	matches_played = 0;
	matches_won = 0, matches_lost = 0, matches_drawn = 0;

	history = new History;
	_history_messages_counter = 0;

	_allowed_to_play = 0; //Can not play yet. It has to have at least 10 outfield players, and players in each formation.
	_ID = _instance_number++;
}

void Club::Increment_History_Messages_Counter()
{
	++_history_messages_counter;
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
		cout << history->message[i] << endl;
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

int Club::Get_Message_Counter()
{
	return _history_messages_counter;
}

int Club::Add_Player_to_Club(Player **player)
{
	if( number_of_players < 23 )
	{
		players[number_of_players] = *player; //looking fantastic, haha! :D
	}
	else
	{
		printf ("There is already 23 players, cannot add new player.\n");
		return -1;
	}


	string tmp = "Bought ";
	tmp.append((*player)->name).append(" ").append((*player)->surname);


	history->message[_history_messages_counter] = tmp;
	history->Save_History(*this);

	++number_of_players;
	return 0;
}

void List_Players(const struct Club *club)
{
	printf("\n---Listing players---\n\n");
	int i;
	for ( i = 0 ; i < club->number_of_players; ++i)
	{
		printf("%s %s\nOverall: %.2f%\n", club->players[i]->name, (club->players[i])->surname, club->players[i]->overall);
		switch (club->players[i]->position)
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




int Set_Tactics(struct Club **club)
{
	if((*club)->number_of_players < 11)
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

		for(i = 0; i < (*club)->number_of_players - 1; ++i)
		{
			if( (*club)->players[i]->overall < (*club)->players[i+1]->overall)
			{
				struct Player *tmp = (*club)->players[i];
				(*club)->players[i] = (*club)->players[i+1];
				(*club)->players[i+1] = tmp;
			}
			else
				++swapped;
		}
	} while(swapped != (*club)->number_of_players - 1); //because swap operation takes one cycle less than number of players. //If there was no swap all the way, means everything is ordered.

//------------------------------------------------------------------------------END OF ORDERING-------------------------------------------------------------------------


//---------------------------------------------------------------------SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------
	int max_attackers = 3, min_attackers = 1, max_midfilders = 5, min_midfilders = 3, max_defenders = 5, min_defenders = 3;
	int current_attackers = 0, current_midfilders = 0, current_defenders = 0;
	int attacker_index = 0, midfilder_index = 0, defender_index = 0;
	i = 0;

	do //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1
	{
		if((*club)->players[i]->position == 1 && (current_defenders < max_defenders ))
			if(  (current_defenders + 1 == max_defenders) && (current_midfilders == max_midfilders || current_attackers == max_attackers) )
			{} //can not have two max of positions, because the third one would not get a place.
		else
		{
			(*club)->defenders_in_first_squad[defender_index] = (*club)->players[i]; //For example, a third best player is the first best as a defender. We are binding his index in whole team "players[3]", to the index of a defenders "defender[1]".
			++current_defenders;
			++defender_index;
			++(*club)->number_of_defenders_in_first_squad;
		}


		if((*club)->players[i]->position == 2 && current_midfilders < max_midfilders)
			if(  (current_midfilders + 1 == max_midfilders)  && (current_attackers == max_attackers  ||  current_defenders == max_defenders) )
				{}
		else
		{
			(*club)->midfilders_in_first_squad[midfilder_index] = (*club)->players[i];
			++current_midfilders;
			++midfilder_index;
			++(*club)->number_of_midfilders_in_first_squad;
		}


		if((*club)->players[i]->position == 3 && current_attackers < max_attackers)
			if(  (current_attackers + 1 == max_attackers) && (current_midfilders == max_midfilders || current_defenders == max_defenders) )
				{}
			else
			{
				(*club)->attackers_in_first_squad[attacker_index] = (*club)->players[i];
				++current_attackers;
				++attacker_index;
				++(*club)->number_of_attackers_in_first_squad;
			}

		++i;
	} while  ( (current_attackers + current_midfilders + current_defenders < 10) && (i != (*club)->number_of_players));

	if(current_attackers < min_attackers || current_midfilders < min_midfilders || current_defenders < min_midfilders)
	{
		printf("Couldnt set proper tatics! Not enough players for a position.\n");
		printf("Attackers: %d\nMidfilders: %d\nDefenders: %d\n", current_attackers, current_midfilders, current_defenders);
		return -1;
	}

//---------------------------------------------------------------------END OF SETTING BEST FORMATION && ASSIGNING BEST PLAYERS TO THER POSITION ON THE PITCH------------------------------------------------------


	(*club)->tactic[0] = current_defenders; //every value in index means number of players in this position.
	(*club)->tactic[1] = current_midfilders;
	(*club)->tactic[2] = current_attackers;


	double sum = 0;

	for(i = 0; i < current_defenders; ++i)
		sum = sum + (*club)->defenders_in_first_squad[i]->overall;
	for(i = 0; i < current_midfilders; ++i)
		sum = sum + (*club)->midfilders_in_first_squad[i]->overall;
	for(i = 0; i <current_attackers; ++i)
		sum = sum + (*(*club)->attackers_in_first_squad[i]).overall;

	(*club)->Set_Tactic_Rating(sum);

	(*club)->Allow_Playing(); //Coming to this line, it means that we succesfully created formation, and added players.
	return 0;
}


void Print_Formation(const struct Club *club)
{
	printf("\n---Current formation---\n");
	cout << club->club_name << " " << club->city_name << "plays: ";
	int i = 0;
	for(; i < 3; ++i)
		printf("%d ", club->tactic[i]);
	printf("\n");
}

void Print_First_Squad(struct Club *club)
{
	printf("\n---First Squad---\n");
	int i;
	for(i = 0; i < club->number_of_defenders_in_first_squad; ++i)
	{
		printf("Player: %s %s  Overall: %.2f%  (DEF)\n", club->defenders_in_first_squad[i]->name, club->defenders_in_first_squad[i]->surname, club->defenders_in_first_squad[i]->overall);
	}

	for(i = 0; i < club->number_of_midfilders_in_first_squad; ++i)
	{
		printf("Player: %s %s  Overall: %.2f%  (MID)\n", club->midfilders_in_first_squad[i]->name, club->midfilders_in_first_squad[i]->surname, club->midfilders_in_first_squad[i]->overall);
	}

	for(i = 0; i < club->number_of_attackers_in_first_squad; ++i)
	{
		printf("Player: %s %s  Overall: %.2f%  (ATT)\n", club->attackers_in_first_squad[i]->name, club->attackers_in_first_squad[i]->surname, club->attackers_in_first_squad[i]->overall);
	}

	club->Print_Tactic_Rating();
	printf("\n");
}








