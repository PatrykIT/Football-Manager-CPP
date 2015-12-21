#include "Calendar.h"
#include <iostream>

Calendar* Calendar::calendar = NULL;


void Calendar::Set_Current_Date()
{
	time_t raw_time;
	time(&raw_time); //returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.

	struct tm *my_date = localtime(&raw_time); //struct tm - this is a structure used to hold the time and date.


	year = my_date->tm_year + 1900; //The number of years since 1900
	month = my_date->tm_mon + 1; //Month, range 0 to 11
	day_of_a_month = my_date->tm_mday;
	day_of_a_week = my_date->tm_wday;
	hour = my_date->tm_hour;
	minute = my_date->tm_min;
	seconde = my_date->tm_sec;
}


void Calendar::Print_Date() const
{
	printf ("Todays date: %d.%d.%d\n\t     %d:%d:%d\n", day_of_a_month, month, year,
			hour, minute, seconde);
}

void Calendar::Travel_Calendar(int days)
{
	if(day_of_a_month + days > 31)
	{
		day_of_a_month += days;

		int rest_of_days = day_of_a_month - 31;
		month++;
		day_of_a_month = 1 + rest_of_days;
	}
}


Calendar::Calendar()
{
	Set_Current_Date();
}

Calendar* Calendar::get()
{
	if(calendar == NULL)
	{
		calendar = new Calendar;
		return calendar;
	}

	return calendar;
}


Calendar::~Calendar()
{

}
