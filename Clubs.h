#ifndef CLUBS_H_
#define CLUBS_H_

#include "Attributes.h"
#include "History.h"

class History;
static std::string club_names[] = {"Real", "Manchester", "Arsenal", "Leeds", "Norwitch", "Lech", "Legia", "Atletico"};
static std::string cities[] = {"Madrid", "United", "London", "York", "Bristol", "Poznan",  "Warszawa", "Cordoba"};


class Club
{
private:
	friend class Table;
	friend class History;

	static int _instance_number;
	const int _ID;
	bool _allowed_to_play;
	double _tactic_rating; //Tacting rating is an average from best 11 players on the pitch. It is to calculate match winning odds.
	float _budget;

	int points;
	int goals_scored, goals_conceded;
	int matches_played;
	int matches_won, matches_lost, matches_drawn;

	int number_of_players; //max - 23. It's a guard to avoid overflowing struct Player players[23].
	int number_of_attackers_in_first_squad;
	int number_of_midfilders_in_first_squad;
	int number_of_defenders_in_first_squad;

	Player *players[23]; //23 players CHANGE TO VECTOR
	int tactic[3]; //Indicates how players are layed out in the pitch. For example: '433' means 4 - 3 - 3. //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1

	Player *attackers_in_first_squad[3];
	Player *midfilders_in_first_squad[5];
	Player *defenders_in_first_squad[5];

	void Set_Tactic_Rating(double sum);
	void Allow_Playing();
	void Resize_History();
	void Increment_History_Messages_Counter();

	char Check_if_Allowed_to_Play() const;

	int Get_ID() const;
	int Buy_Player();
	int Sell_Player();

	History *_history;

	int _history_messages_counter;
	int	number_of_stories;

	Club(const Club& other);
	Club& operator=(const Club& other);

public:
	Club();
	~Club();
	const std::string club_name;
	const std::string city_name;

	void Print_Tactic_Rating() const;
	void Print_History() const;
	void Print_Formation() const;
	void Print_First_Squad() const;
	void Print_Whole_Squad() const;
	void List_Players() const;
	void Print_Positions_Number() const;

	int Add_Player_to_Club(Player &player);
	int Get_Message_Counter() const;
	int Set_Tactics();
	int Get_Number_of_Players() const;

	double Get_Tactic_Rating() const;
};



#endif /* CLUBS_H_ */
