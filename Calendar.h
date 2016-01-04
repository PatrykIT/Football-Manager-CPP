#ifndef CALENDAR_H_
#define CALENDAR_H_
#include <vector>

class Table;

class Calendar
{
private:
	friend class History; //For access to time. We could make public getters, but I don't think it's bad design to friend history with time :)
	friend class Table; //For traveling time - making clubs in league aware of it's passing.

	std::vector<Table*> tables;
	Calendar();
	Calendar(const Calendar &other);
	Calendar& operator=(const Calendar& other);

	static Calendar *calendar;

	int hour, minute, seconde, day_of_a_week, day_of_a_month, month, year;

	void Set_Hour();
	void Travel_Calendar(int days);
	void Add_Table(Table &table);

public:
	~Calendar();
	static Calendar *get();

	void Print_Date() const;
};




#endif /* CALENDAR_H_ */
