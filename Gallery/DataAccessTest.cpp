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
this function adds 3 users, 1 album to each user from the 3, 2 pictures to each album from the 3
and 2 tags on each picture from the 6 to the database.
input: none.
output: none.
*/
void DataAccessTest::addData()
{

	this->_dbAccess.open();
	
	User* firstUser = new User(1000,"Harel");
	User* secondUser = new User(1001, "Dvir");
	User* thirdUser = new User(1002, "Guy");
	Album* firstAl = new Album(900, "My Time");
	Album* secAl = new Album(901, "My Friend");
	Album* thirdAl = new Album(902, "Me");
	Picture* firstPic = new Picture(801, "My first picture");
	Picture* secPic = new Picture(802, "My second picture");
	Picture* thirdPic = new Picture(803, "My third picture");
	Picture* fourthPic = new Picture(804, "My fourth picture");
	Picture* fifthPic = new Picture(805, "My fifth picture");
	Picture* sixthPic = new Picture(806, "My sixth picture");

	try
	{
		this->_dbAccess.createUser(*firstUser);
		this->_dbAccess.createUser(*secondUser);
		this->_dbAccess.createUser(*thirdUser);
		
		this->_dbAccess.createAlbum(*firstAl);
		this->_dbAccess.createAlbum(*secAl);
		this->_dbAccess.createAlbum(*thirdAl);

		this->_dbAccess.addPictureToAlbumByName(firstAl->getName(), *firstPic);
		this->_dbAccess.addPictureToAlbumByName(firstAl->getName(), *secPic);

		this->_dbAccess.addPictureToAlbumByName(secAl->getName(), *thirdPic);
		this->_dbAccess.addPictureToAlbumByName(secAl->getName(), *fourthPic);

		this->_dbAccess.addPictureToAlbumByName(thirdAl->getName(), *fifthPic);
		this->_dbAccess.addPictureToAlbumByName(thirdAl->getName(), *sixthPic);

		this->_dbAccess.tagUserInPicture(firstAl->getName(), firstPic->getName(), secondUser->getId());
		this->_dbAccess.tagUserInPicture(firstAl->getName(), firstPic->getName(), thirdUser->getId());
		this->_dbAccess.tagUserInPicture(firstAl->getName(), secPic->getName(), secondUser->getId());
		this->_dbAccess.tagUserInPicture(firstAl->getName(), secPic->getName(), thirdUser->getId());

		this->_dbAccess.tagUserInPicture(secAl->getName(), thirdPic->getName(), firstUser->getId());
		this->_dbAccess.tagUserInPicture(secAl->getName(), thirdPic->getName(), thirdUser->getId());
		this->_dbAccess.tagUserInPicture(secAl->getName(), fourthPic->getName(), firstUser->getId());
		this->_dbAccess.tagUserInPicture(secAl->getName(), fourthPic->getName(), thirdUser->getId());

		this->_dbAccess.tagUserInPicture(thirdAl->getName(), fifthPic->getName(), firstUser->getId());
		this->_dbAccess.tagUserInPicture(thirdAl->getName(), fifthPic->getName(), secondUser->getId());
		this->_dbAccess.tagUserInPicture(thirdAl->getName(), sixthPic->getName(), firstUser->getId());
		this->_dbAccess.tagUserInPicture(thirdAl->getName(), sixthPic->getName(), secondUser->getId());
	}
	catch (const MyException& e)
	{
		std::cout << e.what() << std::endl;
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