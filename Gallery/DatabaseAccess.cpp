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
			close();
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
	for (auto iter = this->_openAlbums.begin(); iter != this->_openAlbums.end(); iter++) {
		if (iter->getName() == pAlbum.getName() && iter->getOwnerId() == pAlbum.getOwnerId()) {
			iter = this->_openAlbums.erase(iter);
			return;
		}
	}
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

/*
this function checks if album is exist in ALBUMS table at the database.
input: album name, owner id.
output: true if the album exist and false otherwise.
*/
bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
	std::string sqlStatement = "SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "' AND USER_ID = " + std::to_string(userId) + ";";
	char** errMessage = nullptr;
	std::string albumID = "";

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &albumID, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at opening " << albumName << "!, " << std::endl;
		return false;
	}

	return albumID == "";
}

/*
this function returns the data of all the albums of a given user on std::list format.
input: user to get the albums of.
output: list of albums of given user.
*/
const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	int userID = user.getId();
	std::string sqlStatement = "SELECT * FROM ALBUMS WHERE USER_ID = " + std::to_string(userID) + ";";
	char** errMessage = nullptr;
	std::list<Album> albums;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackGetAlbumList, &albums, errMessage);
	
	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get albums of " << user.getName() << "!" << std::endl;
	}

	return albums;
}

/*
this function prints data about all of the albums from the database.
input: none.
output: none.
*/
void DatabaseAccess::printAlbums()
{
	std::string sqlStatement = "SELECT ALBUMS.NAME, USERS.NAME FROM ALBUMS INNER JOIN USERS ON ALBUMS.USER_ID = USERS.ID;";
	char** errMessage = nullptr;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackPrintAlbumsData, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't print albums!" << std::endl;
	}
}

/*
this function takes data from the database and inserts it to an album object in the field _openAlbums.
input: album to open.
output: album object with the data of the requested album.
*/
Album DatabaseAccess::openAlbum(const std::string& albumName)
{
	std::string sqlStatement = "SELECT * FROM ALBUMS WHERE NAME = '" + albumName + "' LIMIT 1; ";
	char** errMessage = nullptr;

	Album* newAl = new Album();
	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackGetAlbumData, newAl, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't open album!" << std::endl;
	}

	if (newAl->getName() == "")
	{
		std::cerr << "Error! album isn't exist!" << std::endl;
	}
	else
	{
		this->_openAlbums.push_back(*newAl);
	}

	return *newAl;
}

/*
this function returns all of the albums's data in the database.
input: none.
output: all the albums's data in the database at the format of std::list<Album>.
*/
const std::list<Album> DatabaseAccess::getAlbums()
{
	std::string sqlStatement = "SELECT * FROM ALBUMS;";
	char** errMessage = nullptr;
	std::list<Album> albums;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackGetAlbumList, &albums, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get albums!" << std::endl;
	}

	return albums;
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
		std::cerr << "Error! couldn't get id of " << albumName << "!" << std::endl;
		return;
	}

	sqlStatement = "SELECT ID FROM PICTURES WHERE NAME = '" + pictureName + "' AND ALBUM_ID = " +  albumID + ";";
	std::string pictureID = "";

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &pictureID, errMessage);

	if (pictureID == "")
	{
		std::cerr << "Error! picture doesn't exist!" << std::endl;
		return;
	}

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

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
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

	sqlStatement = "SELECT ID FROM PICTURES WHERE NAME = '" + pictureName + "' AND ALBUM_ID = " + albumID + ";";
	std::string pictureID = "";

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &pictureID, errMessage);

	if (pictureID == "")
	{
		std::cerr << "Error! picture doesn't exist!" << std::endl;
		return;
	}

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get id of " << pictureName << "!" << std::endl;
		return;
	}

	sqlStatement = "DELETE FROM TAGS WHERE PICTURE_ID = " + pictureID + " AND USER_ID = " + std::to_string(userId) + ";";

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't delete tag at " << pictureName << "!" << std::endl;
	}
	else
	{
		std::cout << "Untagged user at picture succsesfully!" << std::endl;
	}
}

/*
this function inserts picture to the database with the picture's album's name.
input: picture's album's name, picture object with picture data.
output: none.
*/
void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
	std::string sqlStatement = "SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "' LIMIT 1;";
	char** errMessage = nullptr;
	std::string albumID;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &albumID, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get album id!" << std::endl;
		return;
	}

	if (albumID == "")
	{
		std::cerr << "Error! Album isn't exist!" << std::endl;
		return;
	}

	sqlStatement = "INSERT INTO PICTURES(NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('" + picture.getName() + "', '" + picture.getPath() + "', '" + picture.getCreationDate() + "'," + albumID + ");";

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't add picture to album!" << std::endl;
	}

}

/*
this function removes a picture from the database with the name of the
album of the picture and the name of the picture.
input: picture's album's name, picture's name.
output: none.
*/
void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) 
{
	std::string sqlStatement = "SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "' LIMIT 1;";
	char** errMessage = nullptr;
	std::string albumID = "";

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &albumID, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't get album id!" << std::endl;
		return;
	}
	
	if (albumID == "")
	{
		std::cerr << "Error! Album isn't exist!" << std::endl;
		return;
	}

	sqlStatement = "DELETE FROM PICTURES WHERE ALBUM_ID = " + albumID + " AND NAME = '" + pictureName + "';";

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't remove picture from album!" << std::endl;
	}

}


/*=============================\
							   |
	  USER RELATED FUNCTIONS   |
							   |
\=============================*/


/*
this function prints data about all of the users from the database.
input: none.
output: none.
*/
void DatabaseAccess::printUsers()
{
	std::string sqlStatement = "SELECT NAME FROM USERS;";
	char** errMessage = nullptr;

	std::cout << "USER LIST:" << std::endl << "==========" << std::endl;
	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackPrintUserData, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't print albums!" << std::endl;
	}
}

/*
this function gets user data from the database and inserting it into an User object and returning it.
input: user id.
output: user object with data of the user with that id.
*/
User DatabaseAccess::getUser(int userId)
{
	std::string sqlStatement = "SELECT * FROM USERS WHERE ID = " + std::to_string(userId) + ";";
	char** errMessage = nullptr;

	User* newUser = new User(-1, "");

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackGetUserData, newUser, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error! couldn't print albums!" << std::endl;
	}

	if (newUser->getId() == -1)
	{
		std::cerr << "Error! user doesn't exist!" << std::endl;
	}
	else
	{
		this->_openUsers.push_back(*newUser);
	}

	return *newUser;
}

/*
this function creates a user at USERS table at the database.
input: user object.
output: none.
*/
void DatabaseAccess::createUser(User& user)
{
	std::string sqlStatement = "INSERT INTO USERS(NAME) VALUES('" + user.getName() + "');";
	char** errMessage = nullptr;

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at getting user data from sql database." << std::endl;
	}
}

/*
this function deletes user from the data base including every related data to him.
input: user object.
output: none.
*/
void DatabaseAccess::deleteUser(const User& user)
{
	std::string userId = std::to_string(user.getId());
	std::string sqlStatement = "BEGIN;";
	char** errMessage = nullptr;
	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	sqlStatement = "DELETE FROM TAGS WHERE USER_ID = " + userId + ";";

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at deleting user tags from sql database." << std::endl;
		sqlStatement = "ROLLBACK;";
		res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);
		return;
	}

	sqlStatement = "DELETE FROM PICTURES WHERE ALBUM_ID = (SELECT ID FROM ALBUMS WHERE USER_ID = " + userId +");";

	res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at deleting user pictures from sql database." << std::endl;
		sqlStatement = "ROLLBACK;";
		res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);
		return;
	}

	sqlStatement = "DELETE FROM ALBUMS WHERE USER_ID = " + userId + ";";
	res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at deleting user albums from sql database." << std::endl;
		sqlStatement = "ROLLBACK;";
		res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);
		return;
	}

	sqlStatement = "DELETE FROM USERS WHERE ID = " + userId + ";";
	res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at deleting user from sql database." << std::endl;
		sqlStatement = "ROLLBACK;";
		res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);
	}
	else
	{
		sqlStatement = "COMMIT;";
		res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);
	}
}

/*
this function checks if user is exist in USERS table at the database.
input: user id.
output: true if the user exist and false otherwise.
*/
bool DatabaseAccess::doesUserExists(int userId)
{
	std::string sqlStatement = "SELECT ID FROM USERS WHERE ID = " + std::to_string(userId) + ";";
	char** errMessage = nullptr;
	std::string userID = "";

	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), returnFirstArgument, &userID, errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "Error at getting user data from sql database." << std::endl;
		return false;
	}

	return userID == "";
}