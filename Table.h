#ifndef TABLE_H_
#define TABLE_H_

#include "Clubs.h"
#define number_of_clubs_in_ligue 4



struct Pair_Clubs //pairing for matches scheduling.
{
	int match_played; //0 - match not played yet. 1 - match played.
	Club ***clubs_paired; //Why is triple pointer needed?!
};

struct Kolejka
{
	Pair_Clubs **match; //this is a match.
	int matches_played; //if matches_played == number of clubs in league / 2, means whole round was played.
};

class Table
{
public:
	Club *clubs[number_of_clubs_in_ligue];
    Pair_Clubs *pair_of_clubs;
    Kolejka *kolejka;

	int current_round;
    int number_of_clubs;

    Table();
    void Add_Club_to_Table(Club **club);
};

void Sort_Table(struct Table **table); //Change to https://en.wikipedia.org/wiki/Quicksort#Algorithm || HeapSort || MergeSort
void Schedule_Season(struct Table **table);
void Schedule_Rounds(struct Table **table);
void Print_Table(const struct Table *table);
void Print_Rounds(const struct Table *table);
void Play_Round(struct Table **table);
void Give_Walkover(struct Table **table, int i);
void Print_by_Clubs_paired(struct Table *table);


int Check_if_All_Rounds_Played(struct Table **table);
int Check_if_Round_Played(struct Table **table, int *index_of_match_not_played);
int Assert_Table_Full(struct Table **table);
int Count_Combinations(int n, int k); //Counts combinations WITHOUT repetitions! So only uniqe combinations.
int Find_Index_of_Pair_In_Kolejka(struct Club **club_1, struct Club **club_2 ,struct Table **table); //For Play_Match(), so it could set 'match_played = 1' in struct Pair_Clubs.



#endif /* TABLE_H_ */
