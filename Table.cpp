#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include "Table.h"

Table::Table()
{
	pair_of_clubs = new Pair_Clubs[Count_Combinations(number_of_clubs_in_ligue, 2)];

	number_of_clubs = 0;

	int all_unique_combinations_of_matches = Count_Combinations(number_of_clubs_in_ligue, 2);
	int i;

	for(i = 0; i < all_unique_combinations_of_matches; ++i)
	{
		pair_of_clubs[i] = (struct Pair_Clubs) {0}; // Match not played.  //C99. style! :)
		pair_of_clubs[i].clubs_paired = (Club**) malloc (sizeof (Club**)); //Always 2 clubs as a pair.
	}

	kolejka = new Kolejka[number_of_clubs_in_ligue - 1];

	for(i = 0; i < number_of_clubs_in_ligue - 1; ++i)
		kolejka[i].match = (Pair_Clubs**) malloc (sizeof(pair_of_clubs) * (number_of_clubs_in_ligue / 2)); //(sizeof(struct Pair_Clubs) would be bad, because it wouldnt be array of pointer addresses.

	current_round = 0;
}

void Table::Add_Club_to_Table(Club **club)
{
	if( number_of_clubs == number_of_clubs_in_ligue )
	{
		printf("Table full. Cannot add another club.\n");
		return;
	}


	clubs[number_of_clubs] = *club;

    ++number_of_clubs;
}

void Table::Print_Table() const
{
    printf("\n\t---Table---\n");
    for(int i = 0; i < number_of_clubs; ++i)
    {
        cout << clubs[i]->club_name << " " << clubs[i]->city_name << " [" << clubs[i]->Get_ID() << "] " << "has " <<  clubs[i]->points << " points." << endl;

    }
}

void Table::Schedule_Season() //Pairs every club ID with every club ID.
{
	cout << endl << "Scheduling season: " << endl;
	if(Assert_Table_Full() != 1)
	{
		printf("Cannot schedule season. Table not full.\n");
		return;
	}

	int a = 0; 				//first ID.
	int x = 1; 				//second ID. Always starts with ID one bigger than first ID.
	int c = 0; 				//pair_counter
	int last_value_of_x;

	int total_cycles_counter;

	for(total_cycles_counter = 0; total_cycles_counter < number_of_clubs_in_ligue; ++total_cycles_counter)
	{
		last_value_of_x = x;

		while(x != number_of_clubs)
		{
			//pair_of_clubs[c].clubs_paired[0] = clubs + a;
			//pair_of_clubs[c].clubs_paired[1] = clubs + x;

			pair_of_clubs[c].clubs_paired[0] = clubs[a];
			pair_of_clubs[c].clubs_paired[1] = clubs[x];

			//printf("Scheduling: [%d] vs [%d] Pair: %d\n", (*pair_of_clubs[c].clubs_paired[0])->Get_ID(), (*pair_of_clubs[c].clubs_paired[1])->Get_ID(), c);
			printf("Scheduling: [%d] vs [%d] Pair: %d\n", (pair_of_clubs[c].clubs_paired[0])->Get_ID(), (pair_of_clubs[c].clubs_paired[1])->Get_ID(), c);

			++x;
			++c;
		}

		++a;
		x = last_value_of_x + 1;
	}

	/* Example of algorithm usage: we have 4 clubs. It gives us {1-2, 1-3, 1-4, 2-3, 2-4, 3-4} We break it down to {1-2, 1-3, 1-4} {2-3, 2-4} {3-4}
	 * So: a=1, x=2. Merge {1-2}. ++x. Merge {1-3}. ++x. Merge {1-4}. Next ->	++a, x = last_val_of_x + 1. So: a=2, x=3. Merge {2-3}. ++x. Merge {2-4}.
	 * Next -> ++a, x = last_val + 1. So: a=3, x=4. Merge {3-4}.		DONE! ;)
	 */
}

void Table::Schedule_Rounds()
{

	cout << endl << "Scheduling rounds: " << endl;
	if(number_of_clubs_in_ligue % 2 == 0)
	{

	}

	if(number_of_clubs_in_ligue == 4) //Jeśli jest 4, to jest ciekawy sposób :D
	{
		int start = 0;
		int end = Count_Combinations(number_of_clubs_in_ligue, 2) - 1;
		int i = 0;
		int current_round = 0;

		while (1)
		{
			kolejka[current_round].match[i] = pair_of_clubs + start;
			kolejka[current_round].match[i]->match_played = 0;

			++i;

			kolejka[current_round].match[i] = (pair_of_clubs + end);
			kolejka[current_round].match[i]->match_played = 0;

			if( end - start == 1 ) //If we reached the middle.
				break;

			++current_round;
			++start;
			--end;
			i = 0;

		}
	}
}


int Table::Find_Index_of_Pair_In_Kolejka(struct Club **club_1, struct Club **club_2)
{
	//Returns index of paired clubs.

	int i;
	int number_of_matches_in_round = (number_of_clubs_in_ligue / 2);

	for(i = 0; i < number_of_matches_in_round; ++i)
	{
		if( (kolejka[current_round].match[i]->clubs_paired[0])->Get_ID() == (*club_1)->Get_ID() && (kolejka[current_round].match[i]->clubs_paired[1])->Get_ID() == (*club_2)->Get_ID())

			return i;
	}

	return -1; //not found.
}


int Table::Count_Combinations(int n, int k)
{

	/*
	 * Kombinację bez powtórzeń wykorzystujemy wtedy, gdy chcemy wiedzieć ile możemy stworzyć różnych układów k-elementowych, mając do dyspozycji n-elementów,
	 * przy czym kolejność elementów w układzie jest nieistotna, a elementy nie mogą się powtarzać.
	 *
	 *  Wzór na kombinację bez powtórzeń: n! / k! * (n-k)!
	 *
	 *  Silnia dla np. 4! : 4! =  1 * 2 * 3 * 4
	 *
	 */

	//---------------- We are counting silnias (factorials) and putting them to formula. -------------------------

	int n_minus_k = n - k;
	int i;

	int sum = 1;
	int current_silnia = n;

	for( i = 1; i < current_silnia + 1; ++i)
		sum = sum * i;

	n = sum;

	sum = 1;
	current_silnia = k;

	for(i = 1; i < current_silnia + 1; ++i)
		sum = sum * i;

	k = sum;

	sum = 1;
	current_silnia = n_minus_k;

	for(i = 1; i < current_silnia + 1; ++i)
		sum = sum * i;

	n_minus_k = sum;

	//-------------------Kombinacja bez powtórzeń. ---------------------------

	int result_permutation = n / (k * n_minus_k);
	return result_permutation;
}


int Table::Assert_Table_Full()
{
	if( number_of_clubs == number_of_clubs_in_ligue )
		return 1;
	else
		return -1;
}



int Check_if_All_Rounds_Played(struct Table **table)
{
	int i;
	int current_round = (*table)->current_round;

	for(current_round = 0; current_round < number_of_clubs_in_ligue - 1; ++current_round)
	{
		for(i = 0; i < number_of_clubs_in_ligue / 2 ; ++i)
		{
			if((*table)->kolejka[current_round].match[i]->match_played == 0)
			{
				printf("Match not played! [%d] vs [%d]\n", ((*table)->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
						((*table)->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());
				return -1;
			}
		}
	}

	return 1; //Coming here, means all matches were played in the round.
}


int Table::Check_if_Round_Played(int *index_of_match_not_played)
{
	printf("\n\tChecking if round was played fully:\n");
	int i;

	for(i = 0; i < number_of_clubs_in_ligue / 2 ; ++i)
	{
		if(kolejka[current_round].match[i]->match_played == 0)
		{
			printf("Match not played! [%d] vs [%d]\nNot checking any further.\n", (kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
					(kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());

			*index_of_match_not_played = i; //For Play_Round(), if we are giving walkover. We need to indicate, which match needs to have a walkover.

			return -1;
		}
	}

	printf("\n\tRound played fully.\n");
	return 1; //Coming here, means all matches were played in the round.
}


void Table::Print_Rounds() const
{
	printf("\n");
	int matches_in_round = number_of_clubs_in_ligue / 2;
	int current_round, i;

	for(current_round = 0; current_round < number_of_clubs_in_ligue - 1; ++current_round)
	{
		for(i = 0; i < matches_in_round; ++i)
		{
			printf("Round nr: %d: [%d] vs [%d]\n", current_round, (kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
					(kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());
		}
	}
}

void Table::Give_Walkover(int i) //zrobić printa by clubs_paierd.
{
	printf("\n");

	if (  kolejka[current_round].match[i]->clubs_paired[0]->Check_if_Allowed_to_Play() == 0 && kolejka[current_round].match[i]->clubs_paired[1]->Check_if_Allowed_to_Play() == 0 )
	{
		printf("Both clubs should give walkover, as they both cannot play.\nFor keeping it simple, walkover goes for the first team.\n");

		kolejka[current_round].match[i]->clubs_paired[0]->matches_won++;
		kolejka[current_round].match[i]->clubs_paired[0]->goals_scored += 3;
		kolejka[current_round].match[i]->clubs_paired[0]->points += 3;


		kolejka[current_round].match[i]->clubs_paired[1]->matches_lost++;
		kolejka[current_round].match[i]->clubs_paired[1]->goals_conceded += 3;

	}
	else if(  kolejka[current_round].match[i]->clubs_paired[0]->Check_if_Allowed_to_Play() == 0 ) //If home club cannot play.
	{
		printf("Home Club [%d] is giving a walkover to [%d].\n", kolejka[current_round].match[i]->clubs_paired[0]->Get_ID(),
				kolejka[current_round].match[i]->clubs_paired[1]->Get_ID());

		kolejka[current_round].match[i]->clubs_paired[1]->matches_won++;
		kolejka[current_round].match[i]->clubs_paired[1]->goals_scored += 3;
		kolejka[current_round].match[i]->clubs_paired[1]->points += 3;


		kolejka[current_round].match[i]->clubs_paired[0]->matches_lost++;
		kolejka[current_round].match[i]->clubs_paired[0]->goals_conceded += 3;

	}
	else //If away team cannot play.
	{
		printf("Away Club [%d] is giving a walkover to [%d].\n", kolejka[current_round].match[i]->clubs_paired[1]->Get_ID(),
				kolejka[current_round].match[i]->clubs_paired[0]->Get_ID());

		kolejka[current_round].match[i]->clubs_paired[0]->matches_won++;
		kolejka[current_round].match[i]->clubs_paired[0]->goals_scored += 3;
		kolejka[current_round].match[i]->clubs_paired[0]->points += 3;


		kolejka[current_round].match[i]->clubs_paired[1]->matches_lost++;
		kolejka[current_round].match[i]->clubs_paired[1]->goals_conceded += 3;
	}

	kolejka[current_round].match[i]->match_played = 1;
}

void Table::Check_which_Club_Needs_to_Buy(int i)
{
	printf("\n");

	if ( kolejka[current_round].match[i]->clubs_paired[0]->Check_if_Allowed_to_Play() == 0 && kolejka[current_round].match[i]->clubs_paired[1]->Check_if_Allowed_to_Play() == 0 )
	{
		printf("Both clubs should buy.\n");
		kolejka[current_round].match[i]->clubs_paired[0]->Buy_Player();
		kolejka[current_round].match[i]->clubs_paired[1]->Buy_Player();
	}
	else if(  (kolejka[current_round].match[i]->clubs_paired[0])->Check_if_Allowed_to_Play() == 0 ) //If home club cannot play.
	{
		printf("Home Club [%d] has to buy.\n", kolejka[current_round].match[i]->clubs_paired[0]->Get_ID());
		kolejka[current_round].match[i]->clubs_paired[0]->Buy_Player();
	}
	else //If away team cannot play.
	{
		printf("Away Club [%d] has to buy.\n", kolejka[current_round].match[i]->clubs_paired[1]->Get_ID());
		kolejka[current_round].match[i]->clubs_paired[1]->Buy_Player();
	}

}

void Table::Play_Round()
{
	/* Mam obecną kolejkę. Dla każdego meczu (pary) wywołuję funkcję Play_Match(). Następnie wywołuję Check_if_Round_Played(). Jeśli zwróci 1, to robię ++(*table)->current_round.
	 * W przeciwnym wypadku, Play_Round() musi zwrócić -1, i miejsce, z którego wywoływałem Play_Round():
	 * a) nie może grać dalej, nakazać klubowi naprawić błąd (kupić gracza do brakującej taktyki, czy coś)
	 * b) grać dalej, pamiętając że mecz się nie odbył, i odrobić go w następnych terminach.
	 * c) funkcja Play_Match oddaje walkowera za klub który nie mógł grać.
	 * */

	if(current_round > number_of_clubs - 1 - 1) //number_of_clubs - 1 is a number of rounds to play.  -1 because current_round starts from 0.
	{
		cout << "Final round already reached, cannot play further." << endl;
		return;
	}
	int i;
	for(i = 0; i < number_of_clubs_in_ligue / 2; ++i)
	{
		Play_Match(kolejka[current_round].match[i]->clubs_paired + 0, kolejka[current_round].match[i]->clubs_paired + 1);
	}

	int index_of_match_not_played = -1;

	if( Check_if_Round_Played(&index_of_match_not_played) == -1)
	{
		printf("\nRound not played fully!\n");
		printf("Enter choice:\n1) Buy player to set tactic or something like that:D\n2) Play rest of the matches in future.\n3) Give walkover.\nYour choice: \t");

		int choice;
		cin >> choice;

		if (choice == 1) //tu zrobić pętlę.
		{
			do
			{
				Check_which_Club_Needs_to_Buy(index_of_match_not_played);
				Play_Match( &(kolejka[current_round].match[index_of_match_not_played]->clubs_paired[0]), &(kolejka[current_round].match[index_of_match_not_played]->clubs_paired[1]) );
			}
			while( (Check_if_Round_Played(&index_of_match_not_played) != 1) );

		}
		else if (choice == 2)
		{
			printf("Okay. Match will need to be replayed in future. To do: make some flag to mark that this particular match needs to be played.\n");
			current_round++;
		}
		else if (choice == 3)
		{
			do
			{
				Give_Walkover(index_of_match_not_played);
			}
			while( (Check_if_Round_Played(&index_of_match_not_played) != 1) );

		}
	}

	++current_round;
}


void Table::Play_Match(Club **club_1, Club **club_2)
{
	printf("\nNow playing: [%d] vs [%d]\n", (*club_1)->Get_ID(), (*club_2)->Get_ID() );

	if ( Assert_Table_Full() != 1)
	{
		printf ("Cannot play match. League is not full yet.\n");
		return;
	}

	if ((*club_1)->Check_if_Allowed_to_Play() == 0 && (*club_2)->Check_if_Allowed_to_Play()== 0)
	{
		printf ("Clubs are not allowed to play.\n");
		return;
	}
	else if ((*club_1)->Check_if_Allowed_to_Play() == 0 || (*club_2)->Check_if_Allowed_to_Play() == 0)
	{
		printf ("One club is not allowed to play.\n");
		return;
	}

	if((*club_1)->Get_Tactic_Rating() > (*club_2)->Get_Tactic_Rating())
	{
		cout << (*club_1)->club_name << " " << (*club_1)->city_name << " won!" << endl ;

		(*club_1)->points += 3;


		++(*club_1)->matches_played;
		++(*club_2)->matches_played;

		++(*club_1)->matches_won;
		++(*club_2)->matches_lost;

	}
	else if((*club_2)->Get_Tactic_Rating() > (*club_1)->Get_Tactic_Rating())
	{
		cout << (*club_2)->club_name << " " << (*club_2)->city_name << " won!" << endl ;

		(*club_2)->points += 3;

		++(*club_1)->matches_played;
		++(*club_2)->matches_played;

		++(*club_2)->matches_won;
		++(*club_1)->matches_lost;
	}
	else
	{
		printf("There was a draw!\n");

		(*club_1)->points += 1;
		(*club_2)->points += 1;

		++(*club_1)->matches_played;
		++(*club_2)->matches_played;

		++(*club_1)->matches_drawn;
		++(*club_2)->matches_drawn;
	}


	int match_index = Find_Index_of_Pair_In_Kolejka(club_1, club_2);

	if(match_index == -1)
	{
		printf("Couldn't find an index of a match.\n");
		return;
	}

	kolejka[current_round].match[match_index]->match_played = 1;
	//Sort_Table(table);
}

