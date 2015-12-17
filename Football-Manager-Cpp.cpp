
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
using namespace std;

#include "Clubs.h"
#include "Table.h"
#include "Calendar.h"

Calendar *calendar = Calendar::get();

int number_of_players = 15;

void Start()
{
	//printf("\nHello! You've been given %d players, and assigned a club.\n", number_of_players);

	int i;

	for(i = 0; i < number_of_free_players; ++i)
		free_players[i] = new Player;

	Club *club = new Club;
	Player *players_1[number_of_players]; //redundant pointer?

	for (i = 0; i < number_of_players; ++i)
	{
		players_1[i] = new Player;
		club->Add_Player_to_Club(*players_1[i]);
	}

//--------------------------CREATING 7 RIVAL CLUBS--------------------------- (From old C version of this program, will edit this later)

	Club *club_2 = new Club;
	struct Player *players_2[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_2[i] = new Player;
		club_2->Add_Player_to_Club(*players_2[i]);
	}

	Club *club_3 = new Club;
	struct Player *players_3[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_3[i] = new Player;
		club_3->Add_Player_to_Club(*players_3[i]);
	}

	Club *club_4 = new Club;
	struct Player *players_4[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_4[i] = new Player;
		club_4->Add_Player_to_Club(*players_4[i]);
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

	Table *table = new Table;
	table->Add_Club_to_Table(&club);
	table->Add_Club_to_Table(&club_2);
	table->Add_Club_to_Table(&club_3);
	table->Add_Club_to_Table(&club_4);

	/*Add_Club_to_Table(&club_5, &table);
	Add_Club_to_Table(&club_6, &table);
	Add_Club_to_Table(&club_7, &table);
	Add_Club_to_Table(&club_8, &table);*/


	club->Set_Tactics();
	club_2->Set_Tactics();
	club_3->Set_Tactics();
	club_4->Set_Tactics();

	/*Set_Tactics(&club_5);
	Set_Tactics(&club_6);
	Set_Tactics(&club_7);
	Set_Tactics(&club_8);*/


	table->Schedule_Season();

	table->Print_Rounds();

	table->Play_Round();

	table->Print_Table();


	club->Print_History(); club_2->Print_History(); club_3->Print_History(); club_4->Print_History();


	for(i = 0; i < number_of_free_players; ++i)
		delete free_players[i];

	for(i = 0; i < number_of_players; ++i)
	{
		delete players_1[i]; delete players_2[i]; delete players_3[i]; delete players_4[i];
	}

	delete club; delete club_2; delete club_3; delete club_4;
	delete table;

}


/*
 * Dokończyć  Schedule_Rounds().
 * Dokończyć choice = 2 w Play_Round() - zrobić dogrywanie meczy w przyszłym czasie.
 * Add date traversal after playing round. Finish Travel_Calendar()
 * Transform to smart pointers.
 * Add option to sell players.
 * Fix Rating of a squad: 0  ?! Must be fixed!
 * Fix random values of a players - they're too similar, most clubs have overalls in the same % area.
 *
 * Ogarnij czy singleton dobrze działa.
 */

int main()
{
	srand(time(NULL));
	calendar->Set_Current_Date();
	calendar->Print_Date();
	Start();

	delete calendar;
	cout << "Bye bye." << endl;
	return 0;
}



/*
 * Make : history of players (dates of playing in which clubs, goals scored, assists, development, fee received).
 * Add stadium, so games at home could be easier for the home team. Add ticket prices, average attendance etc.
 * Count attendency based on the reputation of guest team (maybe by position in table?)
 *
 * Make it on-line - Two persons (client - server) can play in the same league.
 * Add save - load function.
 * Create morale of players, and dialogues of them with You. Every dialogue affects morale, which affects performance.
 * Players should improve / decline with time.
 */
