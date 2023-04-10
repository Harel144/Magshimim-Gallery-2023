#pragma once
#include "DataAccessTest.h"
#include "DatabaseAccess.h"

/*
this function opens a database and create tables in it, after creating
tables, the function closes the database.
input: none.
output: none.
*/
void DataAccessTest::buildTables()
{
	this->_dbAccess.open();
	this->_dbAccess.close();
}

/*
this function adds 3 users to the database.
input: none.
output: none.
*/
void DataAccessTest::addData()
{

	this->_dbAccess.open();
	
	User* firstUser = new User(1000,"Harel");
	User* secondUser = new User(1001, "Dvir");
	User* thirdUser = new User(1002, "Guy");

	try
	{
		this->_dbAccess.createUser(*firstUser);
		this->_dbAccess.createUser(*secondUser);
		this->_dbAccess.createUser(*thirdUser);
	}
	catch (const MyException& e)
	{
		e.what();
	}
	
	this->_dbAccess.close();
}

void DataAccessTest::changeData()
{

	this->_dbAccess.open();

	this->_dbAccess.close();
}

void DataAccessTest::deleteData()
{

	this->_dbAccess.open();

	this->_dbAccess.close();
}