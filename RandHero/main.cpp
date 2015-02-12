#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

// number of total heroes, easy to change in future if defined this way
#define NUM_HEROES	108

void get_hero(std::string filename)
{
	// which hero?
	int hero = rand() % NUM_HEROES;
	std::string line;
	std::ifstream myfile (filename);
	if (myfile.is_open())
	{
		for (int i = 0; i < NUM_HEROES; ++i)
		{
			// if there are no more lines in the file (should never happen)
			if (!getline(myfile,line))
			{
				break;
			}
			// if we've found the hero we were looking for, tell user
			if (i == hero)
			{
				std::cout << "Congratulations! You've randomly selected : " << line << std::endl;
				myfile.close();
				return;
			}
		}
		myfile.close();
	}
	else
		std::cout << "Unable to open file. Check to make sure " << filename << "and this file are in the same directory.";
}

int main()
{
	// need to seed so we don't get the same hero every time
	srand (time (NULL));
	// file name that contains a list of all heroes
	std::string filename = "Heroes.txt";
	get_hero(filename);
	return 0;
}