#include "DatabaseAccess.h"

/*
this function returns the first argument a sql table is giving.
input: pointer to data from outside, amount of columns, data of columns and name of columns
output: 0.
*/
int returnFirstArgument(void* data, int argc, char** argv, char** azColName)
{
	std::string* strData = static_cast<std::string*>(data);
	*strData = argv[0];
	return 0;
}

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


/*============================\
							  |
	ALBUM RELATED FUNCTIONS   |
							  |
\============================*/


/*
this function creates an album at ALBUMS table at the database.
input: album object.
output: none.
*/
void DatabaseAccess::createAlbum(const Album & album)
{
	std::string name = album.getName();
	std::string creationDate = album.getCreationDate();
	int ownerId = album.getOwnerId();

	if (doesAlbumExists(name, ownerId))
	{
		std::cerr << "Album is already exist!" << std::endl;
		return;
	}

	const std::string sqlStatement = "INSERT INTO ALBUMS(NAME, CREATION_DATE, USER_ID) VALUES('" + name + "','" + creationDate + "','" + std::to_string(ownerId) + "');";
	char** errMessage = nullptr;
	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't create album." << name << "!" << std::endl;
	}
}

/*
this function close an open album.
input: album to close.
output: none.
*/
void DatabaseAccess::closeAlbum(Album& pAlbum)
{
	delete &pAlbum;
}

/*
this function deletes an album from the database.
input: album's name, owner id.
output: none.
*/
void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	if (!doesAlbumExists(albumName, userId))
	{
		std::cerr << "Album isn't exist!" << std::endl;
		return;
	}

	const std::string sqlStatement = "DELETE FROM ALBUMS WHERE USER_ID = " + std::to_string(userId) + " AND NAME = " + albumName + ";";
	
	char** errMessage = nullptr;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't deleted album " << albumName << "!" << std::endl;
	}
}


/*==============================\
							     |
	PICTURES RELATED FUNCTIONS   |
							     |
\==============================*/

/*
this function adds values to the TAGS table (aka tagging a user on a picture).
input: name of album of the picture, name of the picture, user id.
output: none.
*/
void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	if (!doesAlbumExists(albumName, userId))
	{
		std::cerr << "Album doesn't exist!" << std::endl;
		return;
	}

	std::string sqlStatement = "SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "' AND USER_ID = " + std::to_string(userId) + ";";
	char** errMessage = nullptr;
	std::string albumID;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &albumID, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get id of " << albumName << "!, " << std::endl;
		return;
	}

	sqlStatement = "SELECT ID FROM PICTURES WHERE NAME = '" + pictureName + "' AND ALBUM_ID = " +  albumID + ";";
	std::string pictureID;

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &pictureID, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get id of " << pictureName << "!" << std::endl;
		return;
	}

	sqlStatement = "INSERT INTO TAGS(PICTURE_ID, USER_ID) VALUES( " +pictureID + ", " + std::to_string(userId) + ");";

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't tag user at " << pictureName << "!" << std::endl;
	}
	else
	{
		std::cout << "Tagged user at picture succsesfully!" << std::endl;
	}
}
