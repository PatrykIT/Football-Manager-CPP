#ifndef TABLE_H_
#define TABLE_H_

#include "Clubs.h"
#include <array>

#define number_of_clubs_in_ligue 4


class Table
{
private:
	friend class Calendar; //for giving access to clubs

	Table(const Table& other);
    Table& operator=(const Table& other);

    struct Pair_Clubs //pairing for scheduling matches.
	{
		int match_played; //0 - match not played yet. 1 - match played.
		Club *clubs_paired[2]; //this is two clubs that play a match.
	} *matches;

	struct Round
	{
		Pair_Clubs **match; //this is an array of matches
		int matches_played; //if matches_played == number of clubs in league / 2, means whole round was played.
	} *round;

	std::array<Club*, number_of_clubs_in_ligue> clubs;

	void Play_Match(Club &club_1, Club &club_2);
    void Schedule_Rounds();
    void Give_Walkover(const int i);
    void Season_Finished();
    void Sort_Table();

    bool Table_Full() const;
    int Check_if_Round_Played(int *index_of_match_not_played) const;
    int Find_Index_of_Pair_In_Round(Club &club_1, Club &club_2) const; //For Play_Match(), so it could set 'match_played = 1' in struct Pair_Clubs.
    int Count_Combinations(int n, int k) const; //Counts combinations WITHOUT repetitions! So only unique combinations.
    int Check_which_Club_Needs_to_Buy(const int i) const; //Checks which club lacks players.
    int Calculate_Match_Winning_Odds(Club &club_1, Club &club_2) const; //0 - should be draw. 1: club_1 should win. 2: club_2 should win.

public:
    Table();
    ~Table();

    int current_round;
    int number_of_clubs;

    void Add_Club_to_Table(Club **club);
    void Schedule_Season();
    void Print_Table() const;
    void Print_Rounds() const;
    void Play_Round();

};

#endif /* TABLE_H_ */
