#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include "Match.h"
#include "Table.h"

void Play_Match(struct Club **club_1, struct Club **club_2, Table &table)
{
	printf("\nNow playing: [%d] vs [%d]\n", (*club_1)->Get_ID(), (*club_2)->Get_ID() );

	if ( table.Assert_Table_Full() != 1)
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


	int match_index = table.Find_Index_of_Pair_In_Kolejka(club_1, club_2);

	if(match_index == -1)
	{
		printf("Couldn't find an index of a match.\n");
		return;
	}

	table.kolejka[table.current_round].match[match_index]->match_played = 1;
	//Sort_Table(table);
}




