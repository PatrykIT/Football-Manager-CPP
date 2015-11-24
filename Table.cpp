#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include "Table.h"
#include "Match.h"

Table::Table()
{
	pair_of_clubs = new Pair_Clubs[Count_Combinations(number_of_clubs_in_ligue, 2)];

	number_of_clubs = 0;

	int all_unique_combinations_of_matches = Count_Combinations(number_of_clubs_in_ligue, 2);
	int i;

	for(i = 0; i < all_unique_combinations_of_matches; ++i)
	{
		pair_of_clubs[i] = (struct Pair_Clubs) {0}; // Match not played.  //C99. style! :)
		pair_of_clubs[i].clubs_paired = (Club***) malloc (sizeof(struct Club) * 2); //Always 2 clubs as a pair.
		//table->pair_of_clubs[i].clubs_paired = new Club[2];
	}

	kolejka = new Kolejka[number_of_clubs_in_ligue - 1];

	for(i = 0; i < number_of_clubs_in_ligue - 1; ++i)
		kolejka[i].match = (Pair_Clubs**) malloc (sizeof(pair_of_clubs) * (number_of_clubs_in_ligue / 2)); //(sizeof(struct Pair_Clubs) would be bad, because it wouldnt be array of pointer addresses.
		//table->kolejka[i].match = new Pair_Clubs[number_of_clubs_in_ligue / 2];

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


void Sort_Table(struct Table **table)
{
    int swapped, i;

    do
    {
    	swapped = 0;

        for(i = 0; i < (*table)->number_of_clubs - 1; ++i)
        {
            if(  (*(*table)->clubs[i]).points < (*(*table)->clubs[i+1]).points )
            {
                struct Club *tmp = (*table)->clubs[i];
                (*table)->clubs[i] = (*table)->clubs[i+1];
                (*table)->clubs[i+1] = tmp;
            }
            else
                ++swapped;
        }
    } while(swapped != (*table)->number_of_clubs - 1); //because swap operation takes one cycle less than number of players. //If there was no swap all the way, means everything is ordered.

}

void Print_Table(const struct Table *table)
{
    int i = 0; printf("\n\t---Table---\n");
    for(; i < table->number_of_clubs; ++i)
    {
        cout << table->clubs[i]->club_name << " " << table->clubs[i]->city_name << " [" << table->clubs[i]->Get_ID() << "] " << "has " <<  table->clubs[i]->points << " points." << endl;

    }
}

void Schedule_Season(struct Table **table) //Pairs every club ID with every club ID.
{
	if(Assert_Table_Full(table) != 1)
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

		while(x != (*table)->number_of_clubs)
		{
			(*table)->pair_of_clubs[c].clubs_paired[0] = (*table)->clubs + a;
			(*table)->pair_of_clubs[c].clubs_paired[1] = (*table)->clubs + x;

			printf("Scheduling: [%d] vs [%d] Pair: %d\n", (*(*table)->pair_of_clubs[c].clubs_paired[0])->Get_ID(), (*(*table)->pair_of_clubs[c].clubs_paired[1])->Get_ID(), c);

			++x;
			++c;
		}

		++a;
		x = last_value_of_x + 1;
	}

	/* Example of algorith usage: we have 4 clubs. It gives us {1-2, 1-3, 1-4, 2-3, 2-4, 3-4} We break it down to {1-2, 1-3, 1-4} {2-3, 2-4} {3-4}
	 * So: a=1, x=2. Merge {1-2}. ++x. Merge {1-3}. ++x. Merge {1-4}. Next ->	++a, x = last_val_of_x + 1. So: a=2, x=3. Merge {2-3}. ++x. Merge {2-4}.
	 * Next -> ++a, x = last_val + 1. So: a=3, x=4. Merge {3-4}.		DONE! ;)
	 */
}

void Schedule_Rounds(struct Table **table)
{

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
			(*table)->kolejka[current_round].match[i] = ((*table)->pair_of_clubs + start);
			(*table)->kolejka[current_round].match[i]->match_played = 0;

			++i;

			(*table)->kolejka[current_round].match[i] = ((*table)->pair_of_clubs + end);
			(*table)->kolejka[current_round].match[i]->match_played = 0;

			if( end - start == 1 ) //If we reached the middle.
				break;

			++current_round;
			++start;
			--end;
			i = 0;

		}
	}
}


int Find_Index_of_Pair_In_Kolejka(struct Club **club_1, struct Club **club_2 ,struct Table **table)
{
	//Returns index of paired clubs.

	int i;
	int current_round = (*table)->current_round;
	int number_of_matches_in_round = (number_of_clubs_in_ligue / 2);

	for(i = 0; i < number_of_matches_in_round; ++i)
	{
		if( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID() == (*club_1)->Get_ID() && (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID() == (*club_2)->Get_ID())

			return i;
	}

	return -1; //not found.
}


int Count_Combinations(int n, int k)
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


int Assert_Table_Full(struct Table **table)
{
	if( (*table)->number_of_clubs == number_of_clubs_in_ligue )
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
				printf("Match not played! [%d] vs [%d]\n", (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
						(*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());
				return -1;
			}
		}
	}

	return 1; //Coming here, means all matches were played in the round.
}


int Check_if_Round_Played(struct Table **table, int *index_of_match_not_played)
{
	printf("\n\tChecking if round was played fully:\n");
	int i;
	int current_round = (*table)->current_round;

	for(i = 0; i < number_of_clubs_in_ligue / 2 ; ++i)
	{
		if((*table)->kolejka[current_round].match[i]->match_played == 0)
		{
			printf("Match not played! [%d] vs [%d]\nNot checking any further.\n", (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
					(*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());

			*index_of_match_not_played = i; //For Play_Round(), if we are giving walkover. We need to indicate, which match needs to have a walkover.

			return -1;
		}
	}

	printf("\n\tRound played fully.\n");
	return 1; //Coming here, means all matches were played in the round.
}


void Print_Rounds(const struct Table *table)
{
	printf("\n");
	int matches_in_round = number_of_clubs_in_ligue / 2;
	int current_round, i;

	for(current_round = 0; current_round < number_of_clubs_in_ligue - 1; ++current_round)
	{
		for(i = 0; i < matches_in_round; ++i)
		{
			printf("Round nr: %d: [%d] vs [%d]\n", current_round, (*table->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
					(*table->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());
		}
	}
}

void Print_by_Clubs_paired(struct Table *table) //Why is this wrong?!?!?!
{
	printf("\n"); 	printf("Current round: %d\n", table->current_round);

	printf ("0. [%d] vs [%d]\n", (*table->kolejka[0].match[0]->clubs_paired[0])->Get_ID(), (*table->kolejka[table->current_round].match[0]->clubs_paired[1])->Get_ID());
	printf ("1. [%d] vs [%d]\n", (*table->kolejka[0].match[1]->clubs_paired[0])->Get_ID(), (*table->kolejka[table->current_round].match[1]->clubs_paired[1])->Get_ID());
	printf ("2. [%d] vs [%d]\n", (*table->kolejka[1].match[0]->clubs_paired[0])->Get_ID(), (*table->kolejka[table->current_round].match[0]->clubs_paired[1])->Get_ID());
	printf ("3. [%d] vs [%d]\n", (*table->kolejka[1].match[1]->clubs_paired[0])->Get_ID(), (*table->kolejka[table->current_round].match[1]->clubs_paired[1])->Get_ID());
	printf ("4. [%d] vs [%d]\n", (*table->kolejka[2].match[0]->clubs_paired[0])->Get_ID(), (*table->kolejka[table->current_round].match[0]->clubs_paired[1])->Get_ID());
	printf ("5. [%d] vs [%d]\n", (*table->kolejka[2].match[1]->clubs_paired[0])->Get_ID(), (*table->kolejka[table->current_round].match[1]->clubs_paired[1])->Get_ID());
}

void Give_Walkover(struct Table **table, int i) //zrobić printa by clubs_paierd.
{
	printf("\n");
	int current_round = (*table)->current_round;

	if (  (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Check_if_Allowed_to_Play() == 0 && (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->Check_if_Allowed_to_Play() == 0 )
	{
		printf("Both clubs should give walkover, as they both cannot play.\nFor keeping it simple, walkover goes for the first team.\n");

		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->matches_won)++;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->goals_scored) += 3;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->points) += 3;


		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->matches_lost)++;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->goals_conceded) += 3;

	}
	else if(  (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Check_if_Allowed_to_Play() == 0 ) //If home club cannot play.
	{
		printf("Home Club [%d] is giving a walkover to [%d].\n", (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID(),
				(*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID());

		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->matches_won)++;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->goals_scored) += 3;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->points) += 3;


		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->matches_lost)++;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->goals_conceded) += 3;

	}
	else //If away team cannot play.
	{
		printf("Away Club [%d] is giving a walkover to [%d].\n", (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->Get_ID(),
				(*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->Get_ID());

		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->matches_won)++;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->goals_scored) += 3;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[0])->points) += 3;


		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->matches_lost)++;
		( (*(*table)->kolejka[current_round].match[i]->clubs_paired[1])->goals_conceded) += 3;
	}

	(*table)->kolejka[current_round].match[i]->match_played = 1;
}

void Play_Round(struct Table **table)
{
	int current_round = (*table)->current_round;

	/* Mam obecną kolejkę. Dla każdego meczu (pary) wywołuję funkcję Play_Match(). Następnie wywołuję Check_if_Round_Played(). Jeśli zwróci 1, to robię ++(*table)->current_round.
	 * W przeciwnym wypadku, Play_Round() musi zwrócić -1, i miejsce, z którego wywoływałem Play_Round():
	 * a) nie może grać dalej, nakazać klubowi naprawić błąd (kupić gracza do brakującej taktyki, czy coś)
	 * b) grać dalej, pamiętając że mecz się nie odbył, i odrobić go w następnych terminach.
	 * c) funkcja Play_Match oddaje walkowera za klub który nie mógł grać.
	 * */

	int i;
	for(i = 0; i < number_of_clubs_in_ligue / 2; ++i)
	{
		Play_Match((*table)->kolejka[current_round].match[i]->clubs_paired[0], (*table)->kolejka[current_round].match[i]->clubs_paired[1], table);
	}

	int index_of_match_not_played = -1;

	if( Check_if_Round_Played(table, &index_of_match_not_played) == -1)
	{
		printf("\nRound not played fully!\n");
		printf("Enter choice:\n1) Buy player to set tactic or something like that:D\n2) Play rest of the matches in future.\n3) Give walkover.\nYour choice: \t");

		int choice;
		scanf("%d", &choice);

		if (choice == 1)
			return;
		else if (choice == 2)
		{
			printf("Okay. Match will need to be replayed in future. To do: make some flag to mark that this particular match needs to be played.\n");
			(*table)->current_round++;
		}
		else if (choice == 3)
		{
			do
			{
				Give_Walkover(table, index_of_match_not_played);
			}
			while( (Check_if_Round_Played(table, &index_of_match_not_played) != 1) );

		}
	}

	(*table)->current_round++;

}







