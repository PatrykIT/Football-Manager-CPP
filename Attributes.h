#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_
#include <iostream>
#include <atomic>

/* NOTE: This file should be named "Players.h" */
class Club;
class Player;

static std::string names[] = { "Cristiano", "Adu", "Patrick", "Sergio", "Raphael", "Karim", "Gareth", "Luis", "Andres", "Willian", "Oscar" };
static std::string surnames[] = { "Ronaldo", "Costacurta", "Cyrklaff", "Ramos", "Varane", "Benzema", "Bale", "Suarez", "Iniesta", "Romero", "Gaucho" };

class Player
{
private:
	friend class Club; //for improving players after game, and updating their morales.

	void _Set_Attributes();
	void _Set_Position();
	void _Set_Value();
	void _Set_Overall();

	struct Attributes
	{
		struct Attacking_Attributes
		{
			std::atomic<int> ball_control, dribbling, crossing, passing, first_touch, shooting, finishing, weak_foot;
			void Set_Overall();
			double Get_Overall() const;
		private:
			std::atomic<double> overall;
		} attacking_attributes;

		struct Defending_Attributes
		{
			std::atomic<int> stand_tackle, slide_tackle, marking, interceptions;
			void Set_Overall();
			double Get_Overall() const;
		private:
			std::atomic<double> overall;
		} defending_attributes;

		struct Psyhical_Attributes
		{
			std::atomic<int> accelaration, agility, strength, stamina;
			void Set_Overall();
			double Get_Overall() const;
		private:
			std::atomic<double> overall;
		} psyhical_attributes;

		struct Mental_Attributes
		{
			std::atomic<int> aggression, concetration, pressure_handling, work_rate, leadership, flair, game_reading;
			void Set_Overall();
			double Get_Overall() const;
		private:
			std::atomic<double> overall;
		} mental_attributes;
	} attributes;

	struct Psyche
	{
		std::atomic<int> morale; //Updated after playing match. In future: injures will also affect.
		std::atomic<int> form;

	} psyche;

	int _age;
	int _position; //1 - Defender. 2 - Midfielder. 3 - Attacker
	double _overall; //counted on all overalls.
	double _value;


public:
	Player();

	std::string name, surname;

	void Print_Attributes() const;
	void Print_Value() const;

	int Get_Age() const;
	int Get_Position() const;
	int Get_Morale() const;

	double Get_Overall() const;
	double Get_Value() const;
};

#endif /* ATTRIBUTES_H_ */
