#include "DatabaseAccess.h"

/*
this function opens the database. if database isn't existed, the function creates it.
input: none.
output: true if opened successfully and false otherwise.
*/
bool DatabaseAccess::open()
{
	std::string dbFileName = "Gallery.sqlite";
	std::ifstream checkExistence;

	checkExistence.open(dbFileName);
	int file_exist = 1;

	if (checkExistence)
	{
		file_exist = 0;
	}

	checkExistence.close();

	int res = sqlite3_open(dbFileName.c_str(), &this->_db);

	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}

	char** errMessage = nullptr;
	const char* sqlStatement = "";
	if (file_exist != 0)
	{
		const char* sqlStatement = "CREATE TABLE IF NOT EXISTS Users(ID int PRIMARY KEY, NAME TEXT);"
			"CREATE TABLE IF NOT EXISTS Albums(ID int PRIMARY KEY, NAME TEXT, CREATION_DATE TEXT, USER_ID int, FOREIGN KEY(USER_ID) REFERENCES USERS(ID));"
			"CREATE TABLE IF NOT EXISTS Pictures(ID int PRIMARY KEY, NAME TEXT, CREATION_DATE TEXT, LOCATION TEXT, ALBUM_ID int, FOREIGN KEY(ALBUM_ID) REFERENCES ALBUM(ID));"
			"CREATE TABLE IF NOT EXISTS TAGS(ID int PRIMARY KEY, PICTURE_ID int, USER_ID int, FOREIGN KEY(PICTURE_ID) REFERENCES PICTURES(ID), FOREIGN KEY(USER_ID) REFERENCES USERS(ID));";

		res = sqlite3_exec(this->_db, sqlStatement, nullptr, nullptr, errMessage);

		if (res != SQLITE_OK)
		{
			std::cout << errMessage;
			return false;
		}
	}

	return true;
}

/*
this funcion closes the access to the database. 
input: none.
output: none.
*/
void DatabaseAccess::close()
{
	sqlite3_close(this->_db);
	this->_db = nullptr;
}
