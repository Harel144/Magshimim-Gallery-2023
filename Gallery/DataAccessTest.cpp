#pragma once
#include "DataAccessTest.h"
#include <iostream>

/*
this function opens a database and create tables in it, after creating
tables, the function closes the database.
input: none.
output: none.
*/
void DataAccessTest::buildTables()
{

	std::string dbFileName = "Gallery.sqlite";

	int res = sqlite3_open(dbFileName.c_str(), &this->_db);

	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	const char* sqlStatement = "CREATE TABLE IF NOT EXISTS Users(ID int PRIMARY KEY, NAME TEXT);"
		"CREATE TABLE IF NOT EXISTS Albums(ID int NOT NULL, NAME TEXT, CREATION_DATE TEXT, USER_ID int, FOREIGN KEY(USER_ID) REFERENCES USERS(ID));"
		"CREATE TABLE IF NOT EXISTS Pictures(ID int PRIMARY KEY, NAME TEXT, CREATION_DATE TEXT, LOCATION TEXT, ALBUM_ID int, FOREIGN KEY(ALBUM_ID) REFERENCES ALBUM(ID));"
		"CREATE TABLE IF NOT EXISTS TAGS(ID int PRIMARY KEY, PICTURE_ID int, USER_ID int, FOREIGN KEY(PICTURE_ID) REFERENCES PICTURES(ID), FOREIGN KEY(USER_ID) REFERENCES USERS(ID));";
	char** errMessage = nullptr;

	res = sqlite3_exec(this->_db, sqlStatement, nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cout << errMessage;
	}

	sqlite3_close(this->_db);
	this->_db = nullptr;
}

void DataAccessTest::addData()
{

}

void DataAccessTest::changeData()
{

}

void DataAccessTest::deleteData()
{

}