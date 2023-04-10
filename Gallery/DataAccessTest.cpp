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