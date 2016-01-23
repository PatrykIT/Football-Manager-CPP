#ifndef TABLE_H_
#define TABLE_H_

#include <array>
#include <map>

#define number_of_clubs_in_ligue 4

class Club;
class Player;

class Table
{
private:
	friend class Calendar; //for giving access to clubs
	friend class Transfers; //for helper function

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
	void Pick_Scorer(int goals_scored, Club &club_1);
	void Pick_Assister(Player *&player, Club &club_1);
	void Print_Assister(Player *&player);
    void Schedule_Rounds();
    void Give_Walkover(const int i);
    void Season_Finished();
    void Sort_Table();

    bool Table_Full() const;
    int Check_if_Round_Played(int *index_of_match_not_played) const;
    int Find_Index_of_Pair_In_Round(Club &club_1, Club &club_2) const; //For Play_Match(), so it could set 'match_played = 1' in struct Pair_Clubs.
    int Count_Combinations(int n, int k) const; //Counts combinations WITHOUT repetitions! So only unique combinations.
    int Check_which_Club_Needs_to_Buy(const int i) const; //Checks which club lacks players.
    int Calculate_Match_Winning_Odds(Club &club_1, Club &club_2) const; //0: should be draw. 1: club_1 should win. 2: club_2 should win.

    int current_round;
    int number_of_clubs;

    struct Player_Statistics
    {
    	int goals_scored, assists;
    };

    std::map<Player*, Player_Statistics> player_statistics; //Might as well used reference_wrapper from C++11 for Player*
    static void Add_Player_to_Observe(Player &player); //starts counting statistics to player_statistics when player is bought. Called only by Transfer class.

public:
    Table();
    ~Table();

    void Add_Club_to_Table(Club *&club);
    void Schedule_Season();
    void Print_Table() const;
    void Print_Rounds() const;
    void Play_Round();
    void Print_Players_Statistics() const;

};

#endif /* TABLE_H_ */
