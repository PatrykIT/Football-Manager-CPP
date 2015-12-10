#include "Calendar.h"

class Calendar calendar;

#include <string>

void Calendar::Set_Current_Date()
{
	time_t raw_time;
	time(&raw_time); //returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.

	struct tm *my_date = localtime(&raw_time); //struct tm - this is a structure used to hold the time and date.


	calendar.year = my_date->tm_year + 1900; //The number of years since 1900
	calendar.month = my_date->tm_mon + 1; //Month, range 0 to 11
	calendar.day_of_a_month = my_date->tm_mday;
	calendar.day_of_a_week = my_date->tm_wday;
	calendar.hour = my_date->tm_hour;
	calendar.minute = my_date->tm_min;
	calendar.seconde = my_date->tm_sec;
}



void Calendar::Print_Date()
{
	printf ("Todays date: %d.%d.%d\n\t     %d:%d:%d\n", calendar.day_of_a_month, calendar.month, calendar.year,
			calendar.hour, calendar.minute, calendar.seconde);
}

void Calendar::Travel_Calendar(int days)
{
	if(calendar.day_of_a_month + days > 31)
	{
		calendar.day_of_a_month += days;

		int rest_of_days = calendar.day_of_a_month - 31;
		calendar.month++;
		calendar.day_of_a_month = 1 + rest_of_days;
	}
}

std::string Calendar::Add_Calendar_Date_To_History_Message()
{
	calendar.Set_Current_Date();

	std::string tmp = "";
	std::string calendar_string = "";

	tmp += " on a day: ";

	calendar_string.append(std::to_string(calendar.day_of_a_month));
	tmp.append(calendar_string + ".");
	calendar_string.clear();

	calendar_string.append(std::to_string(calendar.month));
	tmp.append(calendar_string + ".");
	calendar_string.clear();


	calendar_string.append(std::to_string(calendar.year)) ;
	tmp.append(calendar_string);
	calendar_string.clear();

	return tmp;
}
