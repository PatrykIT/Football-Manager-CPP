#ifndef TABLE_H_
#define TABLE_H_

#include "Clubs.h"
#define number_of_clubs_in_ligue 4



struct Pair_Clubs //pairing for matches scheduling.
{
	int match_played; //0 - match not played yet. 1 - match played.
	Club **clubs_paired;
};

struct Kolejka
{
	Pair_Clubs **match; //this is a match.
	int matches_played; //if matches_played == number of clubs in league / 2, means whole round was played.
};

class Table
{
private:
	void Play_Match(Club **club_1, Club **club_2);
	Club *clubs[number_of_clubs_in_ligue];
    Pair_Clubs *pair_of_clubs;
    Kolejka *kolejka;

    void Schedule_Rounds();
public:

    int current_round;
    int number_of_clubs;

    Table();
    ~Table();

    void Add_Club_to_Table(Club **club);
    void Schedule_Season();
    void Print_Table() const;
    void Print_Rounds() const;
    void Play_Round();
    void Give_Walkover(int i);
    int Check_which_Club_Needs_to_Buy(int i);

    int Check_if_Round_Played(int *index_of_match_not_played);
    int Find_Index_of_Pair_In_Kolejka(Club **club_1, Club **club_2); //For Play_Match(), so it could set 'match_played = 1' in struct Pair_Clubs.
    int Assert_Table_Full();
    int Count_Combinations(int n, int k); //Counts combinations WITHOUT repetitions! So only unique combinations.
};

#endif /* TABLE_H_ */
