#include "Stadium.h"
#include <algorithm>

Stadium::Stadium() : stadium_name (stadium_names[rand() % stadium_names.size()])
{
	stadium_names.erase(find(stadium_names.begin(), stadium_names.end(), stadium_name)); //Delete name from available stadium names, so every stadium has unique name.

	if(stadium_names.empty())
		stadium_names.push_back("UNNAMED STADIUM"); //In case there is less names available than clubs, avoid crashing.
}

unsigned int Stadium::Get_Capacity() const
{
	return _capacity;
}
