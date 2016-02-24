#ifndef TABLE_H_
#define TABLE_H_
#include "History.h"

#include <array>
#include <unordered_map>
#include <vector>
#include <mutex>

#define number_of_clubs_in_ligue 4

class Club;
class Player;

class Table
{
private:
	friend class Calendar; //for allowing Table, to call Calendar, which calls clubs to make them aware that New Year came.
	friend class Transfers; //for helper function Add_Player_to_Observe(Player &player, Club &club);
	friend class History; //So we can save history about things that happened in league throughout the game.

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

	std::mutex club_array;
	std::array<Club*, number_of_clubs_in_ligue> clubs;

	void Play_Match(Club &club_1, Club &club_2);
	void Pick_Scorer(int goals_scored, Club &club_1);
	void Pick_Assister(Player *&player, Club &club_1);
	void Print_Assister(Player *&player);
    void Schedule_Rounds();
    void Give_Walkover(const int i);
    void Sort_Table();
    void Interface_Message();
    void Play_Round();
    static void Add_Player_to_Observe(Player &player, Club &club); //starts counting statistics to player_statistics when player is bought. Called only by Transfer class.
    void Season_Finished();
    void Pick_Top_Scorer();


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
    	Club *club; //so table knows for which club player is currently playing.
    };

    struct Club_Statistics
    {
    	int points;
		unsigned int goals_scored, goals_conceded;
		unsigned int matches_played;
		unsigned int matches_won, matches_lost, matches_drawn;
    };

    static std::unordered_map<Player*, Player_Statistics> player_statistics; //Might as well used reference_wrapper from C++11 for Player*
    static std::unordered_map<Club*, Club_Statistics> club_statistics;
    std::vector<std::string> history;

public:
    Table();
    ~Table();

    void Add_Club_to_Table(Club *&club);
    void Schedule_Season();
    void Print_Table() const;
    void Print_Rounds() const;
    void Print_Players_Statistics() const;
    void Print_History() const;
    void Print_Date() const;
    void User_Interface();

};

#endif /* TABLE_H_ */
