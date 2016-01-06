#include "History.h"

extern Calendar *calendar;

void History::Save_History(Club &club, std::string message) const
{
	/**
	 * Appends whole string (description of what happened + date) to a history object in club.
	 */

	std::string date = Append_Date();
	club.history.back().message.append(message);
	club.history.back().message.append(date);

	club.history.emplace_back(History{""}); //Make place for the next message.
	//club.history.emplace_back("");
}

std::string History::Append_Date() const
{
	/**
	 * This function adds a date in string format looking like: " on a day 07.12.2015 22:30:15 ".
	 */

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

	return date;
}

History::History() : message ("")
{}


History::History(std::string message) : message(message)
{}
