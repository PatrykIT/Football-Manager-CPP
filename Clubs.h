#ifndef CLUBS_H_
#define CLUBS_H_

#include <string>
#include "Attributes.h"
#include "History.h"

static std::string club_names[] = {"Real", "Manchester", "Arsenal", "Leeds", "Norwitch", "Lech", "Legia", "Atletico" }; //8
static std::string cities[] = {"Madrid", "United", "London", "York", "Bristol", "Poznan",  "Warszawa", "Cordoba"}; //8


class Club
{
private:
	int _budget;
	static int _instance_number;
	const int _ID;
	char _allowed_to_play;
	double _tactic_rating; //Tacting rating is an average from best 11 players on the pitch. It is to calculate match winning odds.
	int _history_messages_counter;
public:
	const std::string club_name;
	const std::string city_name;

	int number_of_players; //max - 23. It's a guard to avoid overflowing struct Player players[23].
	struct Player *players[23]; //23 players

	int tactic[3]; //Indicates how players are layed out in the pitch. For example: '433' means 4 - 3 - 3. //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1

	Player *attackers_in_first_squad[3];
	Player *midfilders_in_first_squad[5];
	Player *defenders_in_first_squad[5];

	int points;
	int goals_scored, goals_conceded;
	int matches_played;
	int matches_won, matches_lost, matches_drawn;

	int number_of_attackers_in_first_squad;
	int number_of_midfilders_in_first_squad;
	int number_of_defenders_in_first_squad;

	History *history; /* We can have 100 unique stories. Pointer because compiler said "array type has incomplete type". If the type is incomplete,
							it means that the it has been forward-declared, but has not yet been declared fully. */



	void Set_Tactic_Rating(double sum);
	void Allow_Playing();

	void Print_Tactic_Rating();
	void Increment_History_Messages_Counter();
	void Print_History();

	char Check_if_Allowed_to_Play();
	double Get_Tactic_Rating();

	int Add_Player_to_Club(Player **player);
	int Get_Message_Counter();

	int Get_ID();
	int Set_Tactics();
	void Print_Formation();
	void Print_First_Squad();
	void List_Players();

	Club();
};



#endif /* CLUBS_H_ */
