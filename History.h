#ifndef HISTORY_H_
#define HISTORY_H_

#include "Calendar.h"
#include "Clubs.h"

#include <iostream>

class Club;

class History
{
public:
	std::string message[100]; //Description of an event, example: "Player moved club from %s to %s".
	void Save_History(Club &club);

};


#endif /* HISTORY_H_ */
