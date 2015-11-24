#include "History.h"

extern Calendar calendar;

#include <iostream>
#include <string>

void History::Save_History(Club &club)
{
	std::string date = calendar.Add_Calendar_Date_To_History_Message();

	club.history->message[club.Get_Message_Counter()].append(date);

	club.Increment_History_Messages_Counter();
}
