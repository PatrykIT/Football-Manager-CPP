#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_
#include <iostream>
#include <vector>
class Player;

extern int number_of_free_players;
extern std::vector<Player*> free_players;//(number_of_free_players); //10 players ready to be bought. std::<array> would be more appropriate if the size wouldn't change throughout the program.

struct Attacking_Attributes
{
	int ball_control, dribbling, crossing, passing, first_touch, shooting, finishing, weak_foot;
	double overall;
};

struct Defending_Attributes
{
	int stand_tackle, slide_tackle, marking, interceptions;
	double overall;
};

struct Psyhical_Attributes
{
	int accelaration, agility, strength, stamina;
	double overall;
};

struct Mental_Attributes
{
	int aggression, concetration, pressure_handling, work_rate, leadership, flair, game_reading;
	double overall;
};

struct Attributes
{
	struct Attacking_Attributes attacking_attributes;
	struct Defending_Attributes defending_attributes;
	struct Psyhical_Attributes psyhical_attributes;
	struct Mental_Attributes mental_attributes;
};




static std::string names[] = { "Cristiano", "Adu", "Patrick", "Sergio", "Raphael", "Karim", "Gareth", "Luis", "Andres", "Willian", "Oscar" };
static std::string surnames[] = { "Ronaldo", "Costacurta", "Cyrklaff", "Ramos", "Varane", "Benzema", "Bale", "Suarez", "Iniesta", "Romero", "Gaucho" };

class Player
{
private:
	void _Set_Attributes();
	void _Set_Position();
	void _Set_Value();
public:
	int age;
	int position; //1 - Defender. 2 - Midfilder. 3 - Attacker
	const std::string name, surname;
	struct Attributes attributes;
	struct History *history[100];
	double overall; //counted on all overalls.
	double value;

	Player();
	void Print_Attributes() const;
	int Print_Value() const;
};

#endif /* ATTRIBUTES_H_ */
