#pragma once

#include "sqlite3.h"

class DataAccessTest
{
public:
	void buildTables();
	void addData();
	void changeData();
	void deleteData();
private:
	sqlite3* _db;

};