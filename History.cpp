#include "History.h"
#include "Clubs.h"
#include "Calendar.h"

#include <atomic>

std::mutex History::date_mutex;

History::History() : message ("")
{}

History::History(std::string message) : message(message)
{}

History::History(History&& other) : message (std::move(other.message))
{}

History::History(History &other) : message (other.message)
{}

std::string History::Save_History(std::string message)
{
	message.append(Append_Date()); //Check if move ctors working here!
	return message;
}
std::string History::Append_Date()
{
	/**
	 * This function adds a date in string format looking like: " on a day 07.12.2015 22:30:15 ".
	 */

	std::lock_guard<std::mutex> lock(date_mutex);

	Calendar::get().Set_Hour();

	std::string calendar_string = "";
	std::string date = " on a day: ";

	calendar_string.append(std::to_string(Calendar::get().day_of_a_month));
	date.append(calendar_string + ".");
	calendar_string.clear();

	calendar_string.append(std::to_string(Calendar::get().month));
	date.append(calendar_string + ".");
	calendar_string.clear();

	calendar_string.append(std::to_string(Calendar::get().year));
	date.append(calendar_string);
	calendar_string.clear();

	calendar_string.append(" ").append(std::to_string(Calendar::get().hour));
	date.append(calendar_string + ":");
	calendar_string.clear();

	calendar_string.append(std::to_string(Calendar::get().minute));
	date.append(calendar_string + ":");
	calendar_string.clear();

	calendar_string.append(std::to_string(Calendar::get().seconde));
	date.append(calendar_string);
	calendar_string.clear();

	return date;
}

