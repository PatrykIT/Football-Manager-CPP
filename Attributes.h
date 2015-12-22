#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_
#include <iostream>
#include <vector>
class Player;

extern std::vector<Player*> free_players;

static std::string names[] = { "Cristiano", "Adu", "Patrick", "Sergio", "Raphael", "Karim", "Gareth", "Luis", "Andres", "Willian", "Oscar" };
static std::string surnames[] = { "Ronaldo", "Costacurta", "Cyrklaff", "Ramos", "Varane", "Benzema", "Bale", "Suarez", "Iniesta", "Romero", "Gaucho" };

class Player
{
private:
	void _Set_Attributes();
	void _Set_Position();
	void _Set_Value();

	struct Attributes
	{
		struct Attacking_Attributes
		{
			int ball_control, dribbling, crossing, passing, first_touch, shooting, finishing, weak_foot;
			double overall;
		} attacking_attributes;

		struct Defending_Attributes
		{
			int stand_tackle, slide_tackle, marking, interceptions;
			double overall;
		} defending_attributes;

		struct Psyhical_Attributes
		{
			int accelaration, agility, strength, stamina;
			double overall;
		} psyhical_attributes;

		struct Mental_Attributes
		{
			int aggression, concetration, pressure_handling, work_rate, leadership, flair, game_reading;
			double overall;
		} mental_attributes;
	};

	Attributes attributes;

	int _age;
	int _position; //1 - Defender. 2 - Midfilder. 3 - Attacker
	double _overall; //counted on all overalls.
	double _value;

public:
	Player();
	Player& operator =(const Player &other);

	std::string name, surname;


	void Print_Attributes() const;
	void Print_Value() const;

	int Get_Age() const;
	int Get_Position() const;

	double Get_Overall() const;
	double Get_Value() const;
};

#endif /* ATTRIBUTES_H_ */
