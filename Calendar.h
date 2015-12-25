#ifndef CALENDAR_H_
#define CALENDAR_H_

class Calendar
{
private:
	Calendar();
	Calendar(const Calendar &other);
	Calendar& operator=(const Calendar& other);

	friend class History;
	static Calendar *calendar;

	int hour, minute, seconde, day_of_a_week, day_of_a_month, month, year;
	void Set_Hour();

public:
	~Calendar();
	static Calendar *get();

	void Print_Date() const;
	void Travel_Calendar(int days);
};




#endif /* CALENDAR_H_ */
