#ifndef HISTORY_H_
#define HISTORY_H_

#include <string>
#include <mutex>

class History
{
public:
	History();
	History(std::string);
	History(History &&other);
	History(History &other);

	static std::mutex date_mutex;
	std::string message; //Description of an event, example: "Player moved club from %s to %s".
	static std::string Append_Date();
	static std::string Save_History(std::string message);
};


#endif /* HISTORY_H_ */
