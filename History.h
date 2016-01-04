#ifndef HISTORY_H_
#define HISTORY_H_

#include "Calendar.h"
#include "Clubs.h"

class Club;

class History
{
public:
	std::string message; //Description of an event, example: "Player moved club from %s to %s".
	std::string Append_Date() const;
	void Save_History(Club &club) const;
};


#endif /* HISTORY_H_ */
