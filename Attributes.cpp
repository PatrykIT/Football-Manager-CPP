#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>


#include "Attributes.h"

std::vector<Player*> free_players(30); //free agents - players without clubs, ready to be bought.

Player::Player() : name (names [ rand() % (sizeof(names) / sizeof(names[0])) ]), surname (surnames [ rand() % (sizeof(surnames) / sizeof(surnames[0])) ])
{
	age = rand() % (26) + 16; //(max_number + 1 - minimum_number)) + minimum_number
	_Set_Attributes();
	_Set_Position();
	_Set_Value();
}

Player& Player::operator =(const Player &other)
{
	age = other.age;
	position = other.position;
	name = other.name;
	surname = other.surname;
	attributes = other.attributes;
	overall = other.overall;
	value = other.value;

	return *this;
}


void Player::_Set_Attributes()
{
	void *pointer_to_struct;

	//------------------------------------------ATTACKING----------------------------------------------------

	pointer_to_struct = (void*) &(attributes.attacking_attributes); //double pointer.
	int number_of_attributes = 8; //number_of_atrributes doesn't involve 'overall' rating.
	int attribute_value;
	int sum_of_attributes = 0; //our variable to count average rating from.
	int counter;

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40; //40 - 99 values.
		sum_of_attributes += attribute_value;

		*(int*)pointer_to_struct = attribute_value; //setting an attribute to a given value.
		pointer_to_struct += sizeof(int);
	}

	attributes.attacking_attributes.overall = (double) sum_of_attributes / number_of_attributes;

	//------------------------------------------DEFENDING----------------------------------------------------

	pointer_to_struct = (void*) &(attributes.defending_attributes);
	number_of_attributes = 4;
	sum_of_attributes = 0;


	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40;
		sum_of_attributes += attribute_value;

		*(int*)pointer_to_struct = attribute_value;
		pointer_to_struct += sizeof(int);
	}

	attributes.defending_attributes.overall = (double) sum_of_attributes / number_of_attributes;

	//------------------------------------------PHYSICAL----------------------------------------------------

	pointer_to_struct = (void*) &(attributes.psyhical_attributes);
	number_of_attributes = 4;
	sum_of_attributes = 0;

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40;
		sum_of_attributes += attribute_value;

		*(int*)pointer_to_struct = attribute_value;
		pointer_to_struct += sizeof(int);
	}

	attributes.psyhical_attributes.overall = (double) sum_of_attributes / number_of_attributes;

	//------------------------------------------MENTAL----------------------------------------------------

	pointer_to_struct = (void*) &(attributes.mental_attributes);
	number_of_attributes = 7;
	sum_of_attributes = 0;

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40;
		sum_of_attributes += attribute_value;

		*(int*)pointer_to_struct = attribute_value;
		pointer_to_struct += sizeof(int);
	}

	attributes.mental_attributes.overall = (double) sum_of_attributes / number_of_attributes;




	overall = (double) (attributes.attacking_attributes.overall + attributes.defending_attributes.overall + attributes.psyhical_attributes.overall +
			attributes.mental_attributes.overall ) / 4;
}

void Player::_Set_Position()
{
	if( attributes.attacking_attributes.overall - 10 > attributes.defending_attributes.overall ) //Has big advantage of a attacking skills.
		position = 3;
	else if (attributes.attacking_attributes.overall > attributes.defending_attributes.overall) //Similar skills.
		position = 2;
	else 																						//Better as a defender.
		position = 1;
}

void Player::Print_Attributes() const
{
	printf("\n");
	//------------------------------------------ATTACKING----------------------------------------------------
	/*printf("Ball Control: %d\nDribbling: %d\nCrossing: %d\nPassing: %d\nFirst Touch: %d\nShooting: %d\nFinishing: %d\nWeak Foot: %d\n",
			attributes.attacking_attributes.ball_control, attributes.attacking_attributes.crossing, attributes.attacking_attributes.dribbling, attributes.attacking_attributes.finishing,
			attributes.attacking_attributes.first_touch, attributes.attacking_attributes.passing, attributes.attacking_attributes.shooting, attributes.attacking_attributes.weak_foot);
	printf("Attacking overall: %.2f%\n", attributes.attacking_attributes.overall); //%.2f    print as floating point, 2 characters after decimal point

	printf("Defending overall: %.2f%\n",attributes.defending_attributes.overall);

	printf("Mental overall: %.2f%\n", attributes.mental_attributes.overall);

	printf("Physical overall: %.2f\n", attributes.psyhical_attributes.overall); */

	printf("Overall: %.2f%\n", overall);

	switch (position)
	{
	case 1:
		printf("Defender.\n");
		break;
	case 2:
		printf("Midfilder.\n");
		break;
	case 3:
		printf("Striker.\n");
		break;
	default:
		printf("POSITION NOT SET!\n");
		break;
	}
	printf("\n");

}

void Player::_Set_Value()
{
	value = overall * 100;
}

int Player::Print_Value() const
{
	printf("Value: %.2f $\n", value);
	return 0;
}
