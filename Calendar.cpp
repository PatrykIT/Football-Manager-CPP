#include "Calendar.h"
#include <iostream>

Calendar* Calendar::calendar = NULL;


void Calendar::Set_Hour() //Used by History class to save messagess - it update's the time, without updating the date, because date is constantly modified by Travel_Calendar().
{
	//To do: Add checking weather updating the time would reset it - for example if old time is 23:00, and current time is 01:00, then we need to increment a day also.

	time_t raw_time;
	time(&raw_time); //returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.

	struct tm *my_date = localtime(&raw_time); //struct tm - this is a structure used to hold the time and date.

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
	if(day_of_a_month + days > 31) //because there can't be 32th day of a month (or more)
	{
		if(month == 12)
			month = 1;
		else
			++month;

		day_of_a_month += days;
		day_of_a_month = day_of_a_month - 31; //For example: 31 December and we want to travel 7 days. It will give us 7 January.
	}
	else
		day_of_a_month += days;
}


Calendar::Calendar()
{
	time_t raw_time;
	time(&raw_time); //returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.

	struct tm *my_date = localtime(&raw_time); //struct tm - this is a structure used to hold the time and date.


	year = my_date->tm_year + 1900; //The number of years since 1900
	month = my_date->tm_mon + 1; //Month, range 0 to 11. Adding +1 for better readability.
	day_of_a_month = my_date->tm_mday;
	day_of_a_week = my_date->tm_wday;
	hour = my_date->tm_hour;
	minute = my_date->tm_min;
	seconde = my_date->tm_sec;
}

Calendar* Calendar::get()
{
	if(calendar == NULL)
		calendar = new Calendar;

	return calendar;
}


Calendar::~Calendar()
{

}
