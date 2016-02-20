#ifndef CLUBS_H_
#define CLUBS_H_

#include "Stadium.h"
#include <vector>
#include <mutex>

class History;
class Player;
static const std::string club_names[] = {"Real", "Manchester", "Arsenal", "Leeds", "Norwitch", "Lech", "Legia", "Atletico"};
static const std::string cities[] = {"Madrid", "United", "London", "York", "Bristol", "Poznan",  "Warszawa", "Cordoba"};


class Club
{
private:
	friend class Calendar; //for Year_Passed()
	friend class Table; //So we can change matches played etc.
	friend class History; //So we can save history about things that happened in club throughout the game.

	Club(const Club& other);
	Club& operator=(const Club& other);

	static int _instance_number;
	const int _ID;

	bool _allowed_to_play;

	double _tactic_rating; //Tactic rating is an average from best 11 players on the pitch. It is to calculate match winning odds.
	int tactic[3]; //Indicates how players are laid out in the pitch. For example: '433' means 4 - 3 - 3. //Available tactics: 4 - 3 - 3  || 4 - 4 - 2 || 4 - 5 - 1 || 3 - 4 - 3 || 5 - 4 - 1

	float _budget;

	unsigned int matches_won, matches_lost, matches_drawn;

	std::vector<Player*> attackers_in_first_squad;
	std::vector<Player*> midfilders_in_first_squad;
	std::vector<Player*> defenders_in_first_squad;

	std::mutex players_vector;
	std::vector<Player*> players;

	std::mutex history_mutex;
	std::vector<std::string> history;

	static std::mutex transfer_list; //static because class Transfers is reachable by all clubs.

	Stadium stadium;
	unsigned int _attendance;
	unsigned int _ticket_prices;

	bool Check_if_Allowed_to_Play() const;

	void Set_Tactic_Rating();
	void Allow_Playing();
	void Set_Ticket_Prices();
	void Set_Attendancy();
	void Improve_Skills_After_Match(bool won);
	void Update_Players_Morale(bool result);
	void Year_Passed(); //When New Year comes, class Calendar should make club's aware with calling this function.
	void Interface_Message();
	int Check_User_Input(std::string) const;

	void Improve_Skills_New_Year(Player *&player, int position);
	void Decline_Skills_New_Year(Player *&player, int position);

	int Get_ID() const;

public:
	explicit Club(int budget = (10000.000 + (rand() / (RAND_MAX / (99999.999 - 10000.000))))); //values 1000.0000 - 99999.999 $
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
	void Set_Custom_Tactic();
	void User_Interface();

	int Add_Player_to_Club(Player &player);
	int Buy_Player();
	int Sell_Player();
	int Set_Tactics();

	double Get_Tactic_Rating() const; //can it be private?

};



#endif /* CLUBS_H_ */
