#ifndef CALENDAR_H_
#define CALENDAR_H_

#include <time.h>
#include <stdio.h>
#include <string>
#include <memory>

class Calendar
{
private:
	friend class History;
	static Calendar *calendar;

	Calendar();
	Calendar(const Calendar &other);
	Calendar& operator=(const Calendar& other);

	int hour, minute, seconde, day_of_a_week, day_of_a_month, month, year;
	void Set_Hour();

public:
	static Calendar *get();
	~Calendar();

	void Print_Date() const;
	void Travel_Calendar(int days);

};




#endif /* CALENDAR_H_ */
