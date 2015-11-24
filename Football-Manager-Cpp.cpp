
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
using namespace std;

#include "Clubs.h"
#include "Match.h"
#include "Table.h"
#include "Calendar.h"

extern Calendar calendar;

#define number_of_players 15

void Start()
{
	//printf("\nHello! You've been given %d players, and assigned a club.\n", number_of_players);

	Club *club = new Club;
	Player *players_1[number_of_players];

	int i;
	for (i = 0; i < number_of_players; ++i)
	{
		players_1[i] = new Player;
		club->Add_Player_to_Club(&players_1[i]);
	}

//--------------------------CREATING 7 RIVAL CLUBS---------------------------

	Club *club_2 = new Club;
	struct Player *players_2[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_2[i] = new Player;
		club_2->Add_Player_to_Club(&players_2[i]);
	}

	Club *club_3 = new Club;
	struct Player *players_3[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_3[i] = new Player;
		club_3->Add_Player_to_Club(&players_3[i]);
	}

	Club *club_4 = new Club;
	struct Player *players_4[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_4[i] = new Player;
		club_4->Add_Player_to_Club(&players_4[i]);
	}

	/*struct Club *club_5 = Create_Club();
	struct Player *players_5[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_5[i] = Create_Player();
		Add_Player_to_Club(&players_5[i], &club_5);
	}

	struct Club *club_6 = Create_Club();
	struct Player *players_6[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_6[i] = Create_Player();
		Add_Player_to_Club(&players_6[i], &club_6);
	}

	struct Club *club_7 = Create_Club();
	struct Player *players_7[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_7[i] = Create_Player();
		Add_Player_to_Club(&players_7[i], &club_7);
	}

	struct Club *club_8 = Create_Club();
	struct Player *players_8[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_8[i] = Create_Player();
		Add_Player_to_Club(&players_8[i], &club_8);
	}*/

//-----------------------------------------------------------------------------

	club->Print_History();

	Table *table = new Table;
	table->Add_Club_to_Table(&club);
	table->Add_Club_to_Table(&club_2);
	table->Add_Club_to_Table(&club_3);
	table->Add_Club_to_Table(&club_4);

	/*Add_Club_to_Table(&club_5, &table);
	Add_Club_to_Table(&club_6, &table);
	Add_Club_to_Table(&club_7, &table);
	Add_Club_to_Table(&club_8, &table);*/


	Set_Tactics(&club);
	Set_Tactics(&club_2);
	Set_Tactics(&club_3);
	Set_Tactics(&club_4);

	/*Set_Tactics(&club_5);
	Set_Tactics(&club_6);
	Set_Tactics(&club_7);
	Set_Tactics(&club_8);*/

	//Print_First_Squad(club);
	//Print_First_Squad(club_2);
	//Print_First_Squad(club_3);
	//Print_First_Squad(club_4);

	Schedule_Season(&table);

	Schedule_Rounds(&table);

	Print_Rounds(table);
	Print_by_Clubs_paired(table);

	Play_Round(&table);

	Print_Table(table);
}



int main()
{
	srand(time(NULL));
	calendar.Set_Current_Date();
	calendar.Print_Date();

	Start();

	cout << "Bye bye." << endl;
	return 0;
}
