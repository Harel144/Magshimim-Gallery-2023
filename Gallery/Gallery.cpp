#include <iostream>
#include <string>
#include "MemoryAccess.h"
#include "AlbumManager.h"
#include <ctime>

void printMenu();
int getCommandNumberFromUser()
{
	std::string message("\nPlease enter any command(use number): ");
	std::string numericStr("0123456789");
	
	std::cout << message << std::endl;
	std::string input;
	std::getline(std::cin, input);
	
	while (std::cin.fail() || std::cin.eof() || input.find_first_not_of(numericStr) != std::string::npos) {

		std::cout << "Please enter a number only!" << std::endl;

		if (input.find_first_not_of(numericStr) == std::string::npos) {
			std::cin.clear();
		}

		std::cout << std::endl << message << std::endl;
		std::getline(std::cin, input);
	}
	
	return std::atoi(input.c_str());
}

int main(void)
{
	// initialization data access
	MemoryAccess dataAccess;

	// initialize album manager
	AlbumManager albumManager(dataAccess);

	std::string albumName;

	printMenu();

	std::cout << "Type " << HELP << " to a list of all supported commands" << std::endl;
	
	do {
		int commandNumber = getCommandNumberFromUser();
		
		try	{
			albumManager.executeCommand(static_cast<CommandType>(commandNumber));
		} catch (std::exception& e) {	
			std::cout << e.what() << std::endl;
		}
	} 
	while (true);
}

/*
this function prints the menu.
input: none.
output: none.
*/
void printMenu()
{
	std::cout << "   Welcome to Gallery!" << std::endl;
	std::cout << "=========================" << std::endl;
	std::cout << "|Developer: Harel Mazar.|" << std::endl;

	std::time_t t = std::time(0);   // get time now

	std::tm* now = std::localtime(&t);
	std::cout << "|Current date: " << now->tm_mday << '/' << (now->tm_mon + 1) << '/' << (now->tm_year + 1900) << "|" << std::endl;
	std::cout << "|Current hour: " << now->tm_hour << ":" << now->tm_min << "    |" << std::endl;
	std::cout << "=========================" << std::endl;
}