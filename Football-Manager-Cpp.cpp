#include "Table.h"
#include "Transfers.hpp"
#include "Clubs.h"
#include "Attributes.h"
#include "History.h"

#include <chrono>
using namespace std;

void Print_Key_Bindings(int number_of_clubs, Club **clubs)
{
	int c;
	for(c = 0; c < number_of_clubs; ++c)
	{
		printf("%d: ||| %s %s |||\n",  c, clubs[c]->club_name.c_str(), clubs[c]->city_name.c_str());
	}

	printf("%d: ||| TABLE |||\n", c);
	printf("-1:||| QUIT |||\n\n");
}


void Start()
{
	int number_of_players_in_team = 15;
	int number_of_clubs = 4;

	/*---------------------------------------CREATING FOOTBALL WORLD--------------------------------------------- */

	Table *table = new Table;
	Player *players[number_of_clubs * number_of_players_in_team]; //Players to be distributed to teams.
	Club *clubs[number_of_clubs];

	int counter_players = 0; //Counter outside the loop, because he needs to keep track of last value.

	for(int i = 0; i < number_of_clubs; ++i)
	{
		clubs[i] = new Club;

		for(; counter_players < number_of_players_in_team; ++counter_players) //Inserts "number_of_players_in_team" to each team.
		{
			players[counter_players] = new Player;
			clubs[i]->Add_Player_to_Club(*players[counter_players]);
		}
		number_of_players_in_team += 15; //As the "counter_players" grew, "number_of_players_in_team" grows by the same margin, so the loop doesn't stop after first iteration.

		clubs[i]->Set_Tactics();

		table->Add_Club_to_Table(clubs[i]);
	}

	table->Schedule_Season();

	/*---------------------------------------MAIN LOOP OF THE GAME--------------------------------------------- */

	while(1)
	{

		Print_Key_Bindings(number_of_clubs, clubs);

		cout << "Enter a number which object you want to access:\t";
		int choice;
		cin >> choice; cout << endl;

		if(choice < number_of_clubs && choice != -1) //For example, having 20 clubs, means the first 20 objects will be clubs.
			clubs[choice]->User_Interface();
		else if (choice == number_of_clubs) //TABLE
			table->User_Interface();
		else if (choice == 100) //For INFO reminder of key bindings
			Print_Key_Bindings(number_of_clubs, clubs);
		else if(choice == -1)
			break;
		else
			cout << "Wrong number!" << endl << endl;
	}


	for(int i = 0; i < number_of_clubs; ++i)
		delete clubs[i];
	delete table;
}


/*
 * Dokończyć  Schedule_Rounds().
 * Dokończyć choice = 2 w Play_Round() - zrobić dogrywanie meczy w przyszłym czasie.
 * Make it user - friendly (dialogues etc).
 * Finish Season_Finished() - create top goalscorers etc.
 * Add ability to pick player to buy from transfer list. So print each player, and let the user pick number.
 */


int main()
{
	auto start_time = chrono::high_resolution_clock::now();
	srand(time(NULL));
	Start();

	delete Transfers::get();


	auto end_time = chrono::high_resolution_clock::now();
	cout << "\n\nBye bye." << endl;
	cout << "Running time: " <<  chrono::duration_cast<chrono::seconds>(end_time - start_time).count() << " sec." << endl;
	return 0;
}



/*
 * Make it on-line - Two persons (client - server) can play in the same league.
 * Create dialogues of players with You. Every dialogue affects morale, which affects performance.
 * Transform to smart pointers.
 * Add save - load function.
 */
