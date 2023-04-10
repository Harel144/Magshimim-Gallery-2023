#pragma once

#include "sqlite3.h"
#include "DatabaseAccess.h"

class DataAccessTest
{
public:
	void buildTables();
	void addData();
	void changeData();
	void deleteData();
private:
	DatabaseAccess _dbAccess;
};