#include "Stadium.h"


Stadium::Stadium() : stadium_name (stadium_names[rand() % (sizeof(stadium_names) / sizeof(stadium_names[0]))])
{
	_attendance = 0;
	_ticket_prices = 0;
}
