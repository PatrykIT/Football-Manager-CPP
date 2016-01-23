#ifndef HISTORY_H_
#define HISTORY_H_

#include <string>

class Club;

class History
{
public:
	History();
	History(std::string);
	History(History &&other);
	History(History &other);

	std::string message; //Description of an event, example: "Player moved club from %s to %s".
	std::string Append_Date() const;
	void Save_History(Club &club, std::string message) const;
};


#endif /* HISTORY_H_ */
