#include "Table.h"
using namespace std;

Table::Table()
{
	pair_of_clubs = NULL;
	round = NULL; //NULL'ing out because of a catch block - if allocation of pair_of_clubs fails, we cannot delete round (as it was unallocated yet). Deleting NULL has no effect.
	int i = -1; //in case round[i].match won't be allocated - then in a catch block we prevent it from being deallocated in loop.

	try //Instead of try - catch, we could make temporary unique_ptr's, that would automatically deallocate memory in case of failure.
	{
		pair_of_clubs = new Pair_Clubs[Count_Combinations(number_of_clubs_in_ligue, 2)];
		round = new Round[number_of_clubs_in_ligue - 1];
		for(i = 0; i < number_of_clubs_in_ligue - 1; ++i)
		{
			round[i].match = new Pair_Clubs*[sizeof(pair_of_clubs) * (number_of_clubs_in_ligue / 2)];
		}
	}

	catch (const bad_alloc&)
	{
		delete []pair_of_clubs;
		delete []round;
		for(int x = i; x >= 0; --x)
			delete round[x].match;
	}

	const int all_unique_combinations_of_matches = Count_Combinations(number_of_clubs_in_ligue, 2);
	for(i = 0; i < all_unique_combinations_of_matches; ++i)
	{
		pair_of_clubs[i] = (struct Pair_Clubs) {0}; // Match not played.  //C99. style! :)
	}

	Calendar::get()->Add_Table(*this);
	number_of_clubs = 0;
	current_round = 0;
}

void Table::Add_Club_to_Table(Club **club)
{
	if(Assert_Table_Full() == 0)
	{
		printf("Table full. Cannot add another club.\n");
		return;
	}

	clubs[number_of_clubs] = *club;

    ++number_of_clubs;
}

Table::~Table()
{
	delete []pair_of_clubs;

	for(int i = 0; i < number_of_clubs_in_ligue - 1; ++i)
		delete []round[i].match;
	delete []round;
}

void Table::Print_Table() const
{
    printf("\n\t---Table---\n");
    printf("\t\t\t|PTS|W|L|D|GS|GA|\n");
    for(int i = 0; i < number_of_clubs_in_ligue; ++i)
    {
    	printf("%s %s [%d]\t|%d  |%d|%d|%d|%d |%d |\n", clubs[i]->club_name.c_str(), clubs[i]->city_name.c_str(), clubs[i]->_ID, clubs[i]->points,
    			clubs[i]->matches_won, clubs[i]->matches_lost, clubs[i]->matches_drawn, clubs[i]->goals_scored, clubs[i]->goals_conceded);

    } cout << endl << endl << endl;
}

void Table::Schedule_Season() //Pairs every club ID with every club ID.
{
	cout << endl << "Scheduling season: " << endl;
	if(Assert_Table_Full() != 0)
	{
		printf("Cannot schedule season. Table not full.\n");
		return;
	}

	int a = 0; 				//first ID.
	int x = 1; 				//second ID. Always starts with ID one bigger than first ID.
	int c = 0; 				//pair_counter
	int last_value_of_x;
	int home_match = 0;

	int total_cycles_counter;

	for(total_cycles_counter = 0; total_cycles_counter < number_of_clubs_in_ligue; ++total_cycles_counter)
	{
		last_value_of_x = x;

		while(x != number_of_clubs)
		{

			if (home_match % 2 ==0)
			{
				pair_of_clubs[c].clubs_paired[0] = clubs[a];
				pair_of_clubs[c].clubs_paired[1] = clubs[x];
			}
			else
			{
				pair_of_clubs[c].clubs_paired[0] = clubs[x]; //Interchange clubs by the place, so every club hosts games at home, and away.
				pair_of_clubs[c].clubs_paired[1] = clubs[a];
			}

			printf("Scheduling: [%d] vs [%d] Pair: %d\n", (pair_of_clubs[c].clubs_paired[0])->Get_ID(), (pair_of_clubs[c].clubs_paired[1])->Get_ID(), c);

			++x;
			++c;
			++home_match;
		}

		++a;
		x = last_value_of_x + 1;
	}

	/* Example of algorithm usage: we have 4 clubs. It gives us {1-2, 1-3, 1-4, 2-3, 2-4, 3-4} We break it down to {1-2, 1-3, 1-4} {2-3, 2-4} {3-4}
	 * So: a=1, x=2. Merge {1-2}. ++x. Merge {1-3}. ++x. Merge {1-4}. Next ->	++a, x = last_val_of_x + 1. So: a=2, x=3. Merge {2-3}. ++x. Merge {2-4}.
	 * Next -> ++a, x = last_val + 1. So: a=3, x=4. Merge {3-4}.		DONE! ;)
	 */

	Schedule_Rounds();
}

void Table::Schedule_Rounds() //Not finished, right now only 4 clubs can be scheduled, will fix that later (it is easier to debug with only 4 teams than 20 ;) )
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
			round[current_round].match[i] = pair_of_clubs + start;
			round[current_round].match[i]->match_played = 0;

			++i;

			round[current_round].match[i] = pair_of_clubs + end;
			round[current_round].match[i]->match_played = 0;

			if( end - start == 1 ) //If we reached the middle.
				break;

			++current_round;
			++start;
			--end;
			i = 0;

		}
	}

	Print_Rounds();
}


int Table::Find_Index_of_Pair_In_Round(Club &club_1, Club &club_2) const //Returns index of of a match in current round.
{
	int i;
	int number_of_matches_in_round = (number_of_clubs_in_ligue / 2);

	for(i = 0; i < number_of_matches_in_round; ++i)
	{
		if( (round[current_round].match[i]->clubs_paired[0])->Get_ID() == club_1.Get_ID() && (round[current_round].match[i]->clubs_paired[1])->Get_ID() == club_2.Get_ID())
			return i;
	}

	return -1; //not found.
}


int Table::Count_Combinations(int n, int k) const
{
	/**
	 * Kombinację bez powtórzeń wykorzystujemy wtedy, gdy chcemy wiedzieć ile możemy stworzyć różnych układów k-elementowych, mając do dyspozycji n-elementów,
	 * przy czym kolejność elementów w układzie jest nieistotna, a elementy nie mogą się powtarzać.
	 *
	 *  Wzór na kombinację bez powtórzeń: n! / k! * (n-k)!
	 */

	//---------------- We are counting factorial's and putting them to formula. -------------------------

	int n_minus_k = n - k;
	int i;

	int sum = 1;
	int current_silnia = n;

	for(i = 1; i < current_silnia + 1; ++i)
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

	//------------------- Combinations without repetitions. ---------------------------

	int result_permutation = n / (k * n_minus_k);
	return result_permutation;
}


int Table::Assert_Table_Full() const
{
	if( number_of_clubs == number_of_clubs_in_ligue )
		return 0;
	else
		return -1;
}




int Table::Check_if_Round_Played(int *index_of_match_not_played) const
{
	for(int i = 0; i < number_of_clubs_in_ligue / 2 ; ++i)
	{
		if(round[current_round].match[i]->match_played == 0)
		{
			printf("\nMatch not played! [%d] vs [%d]\nNot checking any further.\n", (round[current_round].match[i]->clubs_paired[0])->Get_ID(),
					(round[current_round].match[i]->clubs_paired[1])->Get_ID());

			*index_of_match_not_played = i; //For Play_Round(), if we are giving walkover. We need to indicate, which match needs to have a walkover.

			return -1;
		}
	}
	return 0; //Coming here, means all matches were played in the round.
}


void Table::Print_Rounds() const
{
	printf("\n");
	int matches_in_round = number_of_clubs_in_ligue / 2;
	int current_round;

	for(current_round = 0; current_round < number_of_clubs_in_ligue - 1; ++current_round)
	{
		for(int i = 0; i < matches_in_round; ++i)
		{
			printf("Round nr: %d: [%d] vs [%d]\n", current_round, (round[current_round].match[i]->clubs_paired[0])->Get_ID(),
					(round[current_round].match[i]->clubs_paired[1])->Get_ID());
		}
	} cout << endl << endl;
}

void Table::Give_Walkover(const int i)
{
	/**
	 * One team gives 3 points to the opposite team.
	 */

	printf("\n");

	if (round[current_round].match[i]->clubs_paired[0]->Check_if_Allowed_to_Play() == 0 && round[current_round].match[i]->clubs_paired[1]->Check_if_Allowed_to_Play() == 0)
	{
		printf("Both clubs should give walkover, as they both cannot play.\nFor keeping it simple, walkover goes for the first team.\n");

		round[current_round].match[i]->clubs_paired[0]->matches_won++;
		round[current_round].match[i]->clubs_paired[0]->goals_scored += 3;
		round[current_round].match[i]->clubs_paired[0]->points += 3;


		round[current_round].match[i]->clubs_paired[1]->matches_lost++;
		round[current_round].match[i]->clubs_paired[1]->goals_conceded += 3;

	}
	else if(round[current_round].match[i]->clubs_paired[0]->Check_if_Allowed_to_Play() == 0) //If home club cannot play.
	{
		printf("Home Club [%d] is giving a walkover to [%d].\n", round[current_round].match[i]->clubs_paired[0]->Get_ID(),
				round[current_round].match[i]->clubs_paired[1]->Get_ID());

		round[current_round].match[i]->clubs_paired[1]->matches_won++;
		round[current_round].match[i]->clubs_paired[1]->goals_scored += 3;
		round[current_round].match[i]->clubs_paired[1]->points += 3;


		round[current_round].match[i]->clubs_paired[0]->matches_lost++;
		round[current_round].match[i]->clubs_paired[0]->goals_conceded += 3;

		int money = round[current_round].match[i]->clubs_paired[0]->_budget / 2; //For every walkover, team has to give away half of a budget.
		round[current_round].match[i]->clubs_paired[0]->_budget -= money;

		round[current_round].match[i]->clubs_paired[1]->_budget += money;
	}
	else //If away team cannot play.
	{
		printf("Away Club [%d] is giving a walkover to [%d].\n", round[current_round].match[i]->clubs_paired[1]->Get_ID(),
				round[current_round].match[i]->clubs_paired[0]->Get_ID());

		round[current_round].match[i]->clubs_paired[0]->matches_won++;
		round[current_round].match[i]->clubs_paired[0]->goals_scored += 3;
		round[current_round].match[i]->clubs_paired[0]->points += 3;


		round[current_round].match[i]->clubs_paired[1]->matches_lost++;
		round[current_round].match[i]->clubs_paired[1]->goals_conceded += 3;

		int money = round[current_round].match[i]->clubs_paired[1]->_budget / 2;
		round[current_round].match[i]->clubs_paired[1]->_budget -= money;

		round[current_round].match[i]->clubs_paired[0]->_budget += money;

	}
	++round[current_round].match[i]->clubs_paired[0]->matches_played;
	++round[current_round].match[i]->clubs_paired[1]->matches_played;

	round[current_round].match[i]->match_played = 1;
}

int Table::Check_which_Club_Needs_to_Buy(const int i) const
{
	printf("\n");
	int bought;

	if ( round[current_round].match[i]->clubs_paired[0]->Check_if_Allowed_to_Play() == 0 && round[current_round].match[i]->clubs_paired[1]->Check_if_Allowed_to_Play() == 0 )
	{
		printf("Both clubs should buy.\n");
		bought = round[current_round].match[i]->clubs_paired[0]->Buy_Player();
		bought = round[current_round].match[i]->clubs_paired[1]->Buy_Player();
	}
	else if(  (round[current_round].match[i]->clubs_paired[0])->Check_if_Allowed_to_Play() == 0 ) //If home club cannot play.
	{
		printf("Home Club [%d] has to buy.\n", round[current_round].match[i]->clubs_paired[0]->Get_ID());
		bought = round[current_round].match[i]->clubs_paired[0]->Buy_Player();
	}
	else //If away team cannot play.
	{
		printf("Away Club [%d] has to buy.\n", round[current_round].match[i]->clubs_paired[1]->Get_ID());
		bought = round[current_round].match[i]->clubs_paired[1]->Buy_Player();
	}

	return bought; //successfully bought a player.

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

	Calendar::get()->Print_Date();
	cout << "Round: " << current_round + 1 << endl;

	int i;
	for(i = 0; i < number_of_clubs_in_ligue / 2; ++i)
	{
		Play_Match(*round[current_round].match[i]->clubs_paired[0], *round[current_round].match[i]->clubs_paired[1]);
	}

	int index_of_match_not_played = -1;

	if(Check_if_Round_Played(&index_of_match_not_played) == -1)
	{
		printf("\n\tRound not played fully!\n");
		printf("Enter choice:\n1) Buy player to set tactic or something like that:D\n2) Play rest of the matches in future.\n3) Give walkover.\nYour choice: \t");

		int choice;
		cin >> choice;

		if (choice == 1)
		{
			do
			{
				if ( Check_which_Club_Needs_to_Buy(index_of_match_not_played) == 0) //successfully bought a player
					Play_Match( *round[current_round].match[index_of_match_not_played]->clubs_paired[0], *round[current_round].match[index_of_match_not_played]->clubs_paired[1] );
				else
					Give_Walkover(index_of_match_not_played);

			}
			while( (Check_if_Round_Played(&index_of_match_not_played) != 0) );

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
			while( (Check_if_Round_Played(&index_of_match_not_played) != 0) );

		}
	}

	Print_Table();
	++current_round;
	Calendar::get()->Travel_Calendar(7);

	if(current_round == number_of_clubs - 1)
		Season_Finished();
}


void Table::Play_Match(Club &club_1, Club &club_2)
{
	printf("\nNow playing: [%d] vs [%d]\n", club_1.Get_ID(), club_2.Get_ID());
	cout << "Attendance: " << club_1._attendance << " at " << club_1.stadium.stadium_name << "." << endl;
//-------------------------------------- Check if match can be played --------------------------------------------------
	if (Assert_Table_Full() != 0)
	{
		printf ("Cannot play match. League is not full yet.\n");
		return;
	}

	if (club_1.Check_if_Allowed_to_Play() == 0 && club_2.Check_if_Allowed_to_Play() == 0)
	{
		printf ("Clubs are not allowed to play.\n");
		return;
	}
	else if (club_1.Check_if_Allowed_to_Play() == 0 || club_2.Check_if_Allowed_to_Play() == 0)
	{
		printf ("One club is not allowed to play.\n");
		return;
	}

//----------------------------------------------------------------------------------------------------------------------

	const int better_club = Calculate_Match_Winning_Odds(club_1, club_2);

	if (better_club == 1)
	{
		//cout << "\t" << club_1.club_name << " " << club_1.city_name << " won!" << endl;
		printf("\t%s %s won![%d]\n", club_1.club_name.c_str(), club_1.city_name.c_str(), club_1._ID);

		club_1.points += 3;
		++club_1.matches_won;
		++club_2.matches_lost;

		club_1.Update_Players_Morale(1);
		club_2.Update_Players_Morale(0);

		club_1.Improve_Skills_After_Match(true);
		club_2.Improve_Skills_After_Match(false);
	}

	else if (better_club == 2)
	{
		//cout << "\t" << club_2.club_name << " " << club_2.city_name << " won!" << endl;
		printf("\t%s %s won![%d]\n", club_2.club_name.c_str(), club_2.city_name.c_str(), club_2._ID);

		club_2.points += 3;
		++club_2.matches_won;
		++club_1.matches_lost;

		club_1.Update_Players_Morale(0);
		club_2.Update_Players_Morale(1);

		club_1.Improve_Skills_After_Match(false);
		club_2.Improve_Skills_After_Match(true);

	}
	else
	{
		cout << "\t" << "There was a draw!" << endl;

		club_1.points += 1;
		club_2.points += 1;

		++club_1.matches_drawn;
		++club_2.matches_drawn;
	}

	++club_1.matches_played;
	++club_2.matches_played;

	club_1.Set_Attendancy();
	club_2.Set_Attendancy();

	club_1.Set_Ticket_Prices();
	club_2.Set_Ticket_Prices();



	club_1._budget += club_1._ticket_prices * club_1._attendance;

	int match_index = Find_Index_of_Pair_In_Round(club_1, club_2);

	if(match_index == -1)
	{
		cout << "Couldn't find an index of a match." << endl;
		return;
	}

	round[current_round].match[match_index]->match_played = 1;
}

int Table::Calculate_Match_Winning_Odds(Club &club_1, Club &club_2) const
{
	cout << "Ratings of clubs: " << club_1.Get_Tactic_Rating() << " || " << club_2.Get_Tactic_Rating() << endl;

	int sum_of_morale_club1 = 0, sum_of_morale_club2 = 0;
	int club1_chances = 5, club2_chances = 0; //club_1 has advantage from the start, because it plays at home.

//-------------------------------------------------------------------Counting morale of both teams-------------------------------------------

	unsigned int i;
	for(i = 0; i < club_1.number_of_defenders_in_first_squad; ++i)
		sum_of_morale_club1 += club_1.defenders_in_first_squad[i]->Get_Morale();

	for(i = 0; i < club_1.number_of_midfilders_in_first_squad; ++i)
		sum_of_morale_club1 += club_1.midfilders_in_first_squad[i]->Get_Morale();

	for(i = 0; i < club_1.number_of_attackers_in_first_squad; ++i)
		sum_of_morale_club1 += club_1.attackers_in_first_squad[i]->Get_Morale();


	for(i = 0; i < club_2.number_of_defenders_in_first_squad; ++i)
		sum_of_morale_club2 += club_2.defenders_in_first_squad[i]->Get_Morale();

	for(i = 0; i < club_2.number_of_midfilders_in_first_squad; ++i)
		sum_of_morale_club2 += club_2.midfilders_in_first_squad[i]->Get_Morale();

	for(i = 0; i < club_2.number_of_attackers_in_first_squad; ++i)
		sum_of_morale_club2 += club_2.attackers_in_first_squad[i]->Get_Morale();

//--------------------------------------------------------------------------------------------------------------------------------------------

	if (sum_of_morale_club1 > sum_of_morale_club2 )
	{
		club1_chances = (sum_of_morale_club1 - sum_of_morale_club2) / 5; //  Divide by 5 because morale have lesser influence than rating of a squad.
	}
	else if (sum_of_morale_club2 > sum_of_morale_club1)
	{
		club2_chances = (sum_of_morale_club2 - sum_of_morale_club1) / 5;
	}

	if(club_1.Get_Tactic_Rating() > club_2.Get_Tactic_Rating() + 1) // +1 so it has a margin for a draw - if clubs are too close in rating, it should be a draw.
	{
		club1_chances += ((club_1.Get_Tactic_Rating() - club_2.Get_Tactic_Rating()) * 2) ;
	}
	else if (club_2.Get_Tactic_Rating() > club_1.Get_Tactic_Rating() + 1)
	{
		club2_chances += (club_2.Get_Tactic_Rating() - club_1.Get_Tactic_Rating());
	}

	int better_chance; //0 - even chances. 1: club_1 better chances. 2: club_2 better chances

	if (club1_chances > club2_chances + 1)
		better_chance = 1;
	else if (club2_chances > club1_chances + 1)
		better_chance = 2;
	else
		better_chance = 0;

	return better_chance;
}


void Table::Season_Finished()
{
	cout << "Season is finished!" << endl;
	Sort_Table();
	Print_Table();


}


void Table::Sort_Table()
{
	for(int x = 0; x < number_of_clubs; ++x)
	{
		int index_of_max = x;

		for(int y = x; y < number_of_clubs; ++y)
		{
			if(clubs[index_of_max]->points < clubs[y]->points)
			{
				index_of_max = y;
			}
		}

		Club *temp = clubs[x];
		clubs[x] = clubs[index_of_max];
		clubs[index_of_max] = temp;
	}
}
