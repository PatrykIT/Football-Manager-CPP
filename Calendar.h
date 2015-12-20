#ifndef CALENDAR_H_
#define CALENDAR_H_

#include <time.h>
#include <stdio.h>
#include <string>

class Calendar
{
private:
	friend class History;

	static Calendar *calendar;
	Calendar();
	int hour, minute, seconde, day_of_a_week, day_of_a_month, month, year;

public:

	void Set_Current_Date();
	void Print_Date() const;
	void Travel_Calendar(int days);
	int Measure_Time();

	static Calendar *get();
	~Calendar();
};




#endif /* CALENDAR_H_ */
