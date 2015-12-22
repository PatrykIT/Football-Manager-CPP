#include "History.h"

extern Calendar *calendar;

#include <iostream>
#include <string>

void History::Save_History(Club &club)
{
	calendar->Set_Hour();

	std::string date = "";
	std::string calendar_string = "";

	date += " on a day: ";

	calendar_string.append(std::to_string(calendar->day_of_a_month));
	date.append(calendar_string + ".");
	calendar_string.clear();

	calendar_string.append(std::to_string(calendar->month));
	date.append(calendar_string + ".");
	calendar_string.clear();


	calendar_string.append(std::to_string(calendar->year));
	date.append(calendar_string);
	calendar_string.clear();

	calendar_string.append(" ").append(std::to_string(calendar->hour));
	date.append(calendar_string + ":");
	calendar_string.clear();

	calendar_string.append(std::to_string(calendar->minute));
	date.append(calendar_string + ":");
	calendar_string.clear();

	calendar_string.append(std::to_string(calendar->seconde));
	date.append(calendar_string);
	calendar_string.clear();

	club._history[club.Get_Message_Counter()].message.append(date);

	club.Increment_History_Messages_Counter();

	if (club.Get_Message_Counter() == club.number_of_stories) //if we reached max of allocated histories.
	{
		club.Resize_History();
	}
}
