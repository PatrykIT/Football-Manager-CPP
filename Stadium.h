#ifndef STADIUM_H_
#define STADIUM_H_

#include <vector>
#include <string>
class Club;

static std::vector<std::string> stadium_names = {"Camp Nou", "Santiago Bernabeu", "Stamford Bridge", "Anfield Park", "Etihad Stadium", "San Siro", "Goodison Park", "Old Trafford"};

class Stadium
{
private:
	const unsigned int _capacity = 10000 + rand() % 50000; //values from 10.000 to 60.000;

public:
	const std::string stadium_name;
	unsigned int Get_Capacity();
	Stadium();
};




#endif /* STADIUM_H_ */
