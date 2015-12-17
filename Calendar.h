#ifndef CALENDAR_H_
#define CALENDAR_H_

#include <time.h>
#include <stdio.h>
#include <string>

class Calendar
{
private:
	static Calendar *calendar;
	static bool calendar_present;
	Calendar();


public:
	int hour, minute, seconde,  day_of_a_week,  day_of_a_month, month, year;

	void Set_Current_Date();
	void Print_Date();
	void Travel_Calendar(int days);

	static Calendar *get();
	~Calendar();
};




#endif /* CALENDAR_H_ */
