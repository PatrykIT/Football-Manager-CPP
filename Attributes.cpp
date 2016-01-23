#include "Attributes.h"

Player::Player() :_age (rand() % (26) + 16), //(max_number + 1 - minimum_number)) + minimum_number
		name (names [ rand() % (sizeof(names) / sizeof(names[0])) ]),
		surname (surnames [ rand() % (sizeof(surnames) / sizeof(surnames[0])) ])

{
	_Set_Attributes();
	_Set_Position();
	_Set_Value();

	psyche.morale = 50; //Middle.
	psyche.form = 50;
}

void Player::_Set_Attributes()
{
	void *pointer_to_struct; /*It is a shortcut by which I initialize all struct members avoiding writing each attribute separately. It is not portable,
								and will be rewritten in the final version.	*/
	int attribute_value;
	int sum_of_attributes = 0; //our variable to count average rating from.
	int counter;

	//------------------------------------------ATTACKING----------------------------------------------------

	pointer_to_struct = static_cast<void*> (&(attributes.attacking_attributes));
	int number_of_attributes = 8; //number_of_atrributes doesn't involve 'overall' rating.

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40; //40 - 99 values.
		sum_of_attributes += attribute_value;

		*(static_cast<int*> (pointer_to_struct)) = attribute_value;		//setting an attribute to a given value.
		pointer_to_struct += sizeof(int);
	}

	attributes.attacking_attributes.Set_Overall();

	//------------------------------------------DEFENDING----------------------------------------------------

	pointer_to_struct = static_cast<void*> (&(attributes.defending_attributes));
	sum_of_attributes = 0;
	number_of_attributes = 4;

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40;
		sum_of_attributes += attribute_value;

		*(static_cast<int*> (pointer_to_struct)) = attribute_value;
		pointer_to_struct += sizeof(int);
	}

	attributes.defending_attributes.Set_Overall();

	//------------------------------------------PHYSICAL----------------------------------------------------

	pointer_to_struct = static_cast<void*> (&(attributes.psyhical_attributes));
	sum_of_attributes = 0;
	number_of_attributes = 4;

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40;
		sum_of_attributes += attribute_value;

		*(static_cast<int*> (pointer_to_struct)) = attribute_value;
		pointer_to_struct += sizeof(int);
	}

	attributes.psyhical_attributes.Set_Overall();

	//------------------------------------------MENTAL----------------------------------------------------

	pointer_to_struct = static_cast<void*> (&(attributes.mental_attributes));
	sum_of_attributes = 0;
	number_of_attributes = 7;

	for(counter = 0; counter < number_of_attributes; ++counter)
	{
		attribute_value = rand() %60 + 40;
		sum_of_attributes += attribute_value;

		*(static_cast<int*> (pointer_to_struct)) = attribute_value;
		pointer_to_struct += sizeof(int);
	}

	attributes.mental_attributes.Set_Overall();
	//----------------------------------------------------------------------------------------------------

	_Set_Overall();
}

void Player::_Set_Position()
{
	if( attributes.attacking_attributes.Get_Overall() - 10 > attributes.defending_attributes.Get_Overall() ) //Has big advantage of attacking skills.
		_position = 3;
	else if (attributes.attacking_attributes.Get_Overall() > attributes.defending_attributes.Get_Overall()) //Similar skills.
		_position = 2;
	else 																						//Better as a defender.
		_position = 1;
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

	printf("Overall: %.2f%\n", _overall);

	switch (_position)
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
	_value = _overall * 100;
}

void Player::Print_Value() const
{
	printf("Value: %.2f $\n", _value);
}

int Player::Get_Age() const
{
	return _age;
}

int Player::Get_Position() const
{
	return _position;
}

double Player::Get_Overall() const
{
	return _overall;
}

double Player::Get_Value() const
{
	return _value;
}

int Player::Get_Morale() const
{
	return psyche.morale;
}

void Player::_Set_Overall()
{
	attributes.attacking_attributes.Set_Overall();
	attributes.defending_attributes.Set_Overall();
	attributes.mental_attributes.Set_Overall();
	attributes.psyhical_attributes.Set_Overall();

	_overall = (attributes.attacking_attributes.Get_Overall() + attributes.defending_attributes.Get_Overall() + attributes.mental_attributes.Get_Overall() +
			attributes.psyhical_attributes.Get_Overall()) / 4.0;
	_Set_Value();
}

void Player::Attributes::Defending_Attributes::Set_Overall()
{
	double sum = interceptions + marking + slide_tackle + stand_tackle;
	overall = sum / 4;
}

void Player::Attributes::Attacking_Attributes::Set_Overall()
{
	double sum = 0;

	sum += ball_control;
	sum += crossing;
	sum += dribbling;
	sum += finishing;
	sum += first_touch;
	sum += passing;
	sum += shooting;
	sum += weak_foot;

	overall = sum / 8;
}

void Player::Attributes::Mental_Attributes::Set_Overall()
{
	double sum = 0;

	sum += aggression;
	sum += concetration;
	sum += flair;
	sum += game_reading;
	sum += leadership;
	sum += pressure_handling;
	sum += work_rate;

	overall = sum / 7;
}

void Player::Attributes::Psyhical_Attributes::Set_Overall()
{
	double sum = accelaration + agility + stamina + strength;
	overall = sum / 4;
}

double Player::Attributes::Attacking_Attributes::Get_Overall() const
{
	return overall;
}

double Player::Attributes::Defending_Attributes::Get_Overall() const
{
	return overall;
}

double Player::Attributes::Mental_Attributes::Get_Overall() const
{
	return overall;
}

double Player::Attributes::Psyhical_Attributes::Get_Overall() const
{
	return overall;
}







