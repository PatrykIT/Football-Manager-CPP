#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_
#include <iostream>

struct Attacking_Attributes
{
	int ball_control, dribbling, crossing, passing, first_touch, shooting, finishing;
	int weak_foot; //from 1 to 5.
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
public:
	int age;
	int position; //1 - Defender. 2 - Midfilder. 3 - Attacker
	std::string name, surname;
	struct Attributes attributes;
	struct History *history[100];
	double overall; //counted on all overalls.

	Player();
	void Print_Attributes();
};

#endif /* ATTRIBUTES_H_ */
