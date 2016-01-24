#include "Table.h"
#include "Transfers.hpp"
#include "Clubs.h"
#include "Attributes.h"
#include "History.h"
#include "Calendar.h"

using namespace std;


void Start()
{
	int number_of_players = 15;
	int i;

	Club *club = new Club;
	Player *players_1[number_of_players];

	for (i = 0; i < number_of_players; ++i)
	{
		players_1[i] = new Player;
		club->Add_Player_to_Club(*players_1[i]);
	}

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


	Table *table = new Table;
	table->Add_Club_to_Table(club);
	table->Add_Club_to_Table(club_2);
	table->Add_Club_to_Table(club_3);
	table->Add_Club_to_Table(club_4);


	club->Set_Tactics();
	club_2->Set_Tactics();
	club_3->Set_Tactics();
	club_4->Set_Tactics();

	table->Schedule_Season();

	table->Play_Round();
	table->Play_Round();
	table->Play_Round();

	club_2->Print_History();
	club->Print_History();
	club_3->Print_History();
	club_4->Print_History();
	delete club; delete club_2; delete club_3; delete club_4;
	delete table;
}


/*
 * Dokończyć  Schedule_Rounds().
 * Dokończyć choice = 2 w Play_Round() - zrobić dogrywanie meczy w przyszłym czasie.
 * Transform to smart pointers.
 * Try to change colours in console.
 * Make it user - friendly (dialogues etc).
 * Finish Season_Finished() - create top goalscorers etc.
 * Add threads.
 * Make class Transfer_Window. Only when it's open, we can buy players. After buying, we give signal to Table::player_statistics to observe new player.
 * Above is almost done, but static map player_statistics cannot be empty defined. Fix this bug.
 */


int main()
{
	srand(time(NULL));
	Start();

	delete Calendar::get();
	delete Transfers::get();

	cout << "Bye bye." << endl;
	return 0;
}



/*
 * Make it on-line - Two persons (client - server) can play in the same league.
 * Add save - load function.
 * Create dialogues of players with You. Every dialogue affects morale, which affects performance.
 */
