#pragma once
#include "DataAccessTest.h"
#include <iostream>
#include <fstream>

/*
this function opens a database and create tables in it, after creating
tables, the function closes the database.
input: none.
output: none.
*/
void DataAccessTest::buildTables()
{
	if (!openDb())
	{
		return;
	}

	const char* sqlStatement = "CREATE TABLE IF NOT EXISTS Users(ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT);"
		"CREATE TABLE IF NOT EXISTS Albums(ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT, CREATION_DATE TEXT, USER_ID int, FOREIGN KEY(USER_ID) REFERENCES USERS(ID));"
		"CREATE TABLE IF NOT EXISTS Pictures(ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT, CREATION_DATE TEXT, LOCATION TEXT, ALBUM_ID int, FOREIGN KEY(ALBUM_ID) REFERENCES ALBUM(ID));"
		"CREATE TABLE IF NOT EXISTS TAGS(ID INTEGER PRIMARY KEY AUTOINCREMENT, PICTURE_ID int, USER_ID int, FOREIGN KEY(PICTURE_ID) REFERENCES PICTURES(ID), FOREIGN KEY(USER_ID) REFERENCES USERS(ID));";
	char** errMessage = nullptr;

	int res = sqlite3_exec(this->_db, sqlStatement, nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cout << errMessage;
	}

	sqlite3_close(this->_db);
	this->_db = nullptr;
}

/*
this function adds 3 users to the database.
input: none.
output: none.
*/
void DataAccessTest::addData()
{
	if (!openDb())
	{
		return;
	}

	const char* sqlStatement = "INSERT INTO USERS(NAME,ID) VALUES ('Harel');"
							   "INSERT INTO USERS(NAME,ID) VALUES ('Guy');"
							   "INSERT INTO USERS(NAME,ID) VALUES ('Dvir');";
	char** errMessage = nullptr;

	int res = sqlite3_exec(this->_db, sqlStatement, nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cout << errMessage;
	}

	sqlite3_close(this->_db);
	this->_db = nullptr;
}

void DataAccessTest::changeData()
{
	if (!openDb())
	{
		return;
	}

	sqlite3_close(this->_db);
	this->_db = nullptr;
}

void DataAccessTest::deleteData()
{
	if (!openDb())
	{
		return;
	}

	sqlite3_close(this->_db);
	this->_db = nullptr;
}

/*
this function opens the database at _db field and creates it if necessary.
input: none.
output: true if the database opened successfully and otherwise false.
*/
bool DataAccessTest::openDb()
{
	std::string dbFileName = "Gallery.sqlite";
	std::ifstream checkExistence;

	checkExistence.open(dbFileName);
	checkExistence.close();

	int res = sqlite3_open(dbFileName.c_str(), &this->_db);

	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}
	return true;
}
