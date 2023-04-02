#include "callbackFuncs.h"
#include "Album.h"
#include "User.h"
#include <list>
#include <string>

/*
this function returns the first argument a sql table is giving.
input: pointer to string from outside, amount of columns, data of columns and name of columns
output: 0.
*/
int returnFirstArgument(void* data, int argc, char** argv, char** azColName)
{
	std::string* strData = static_cast<std::string*>(data);
	*strData = std::string(argv[0]);
	return 0;
}

/*
this function sets data from the database into a given Album object.
input: pointer to Album object, amount of columns, data of columns and name of columns.
output: 0.
*/
int callbackGetAlbumData(void* data, int argc, char** argv, char** azColName)
{
	Album* myData = static_cast<Album*>(data);

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != nullptr)
		{
			if (strcmp(azColName[i], "USER_ID") == 0)
			{
				myData->setOwner(std::atoi(argv[i]));
			}
			else if (strcmp(azColName[i], "NAME") == 0)
			{
				myData->setName(std::string(argv[i]));
			}
			else if (strcmp(azColName[i], "CREATION_DATE") == 0)
			{
				myData->setCreationDate(std::string(argv[i]));
			}
		}
	}
	return 0;
}

/*
this function sets data from the database into a given Album list object.
input: pointer to Album list object, amount of columns, data of columns and name of columns.
output: 0.
*/
int callbackGetAlbumList(void* data, int argc, char** argv, char** azColName)
{
	std::list<Album>& myData = *(static_cast<std::list<Album>*>(data));

	Album currAlbum;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != nullptr)
		{
			if (strcmp(azColName[i], "USER_ID") == 0)
			{
				currAlbum.setOwner(std::atoi(argv[i]));
			}
			else if (strcmp(azColName[i], "NAME") == 0)
			{
				currAlbum.setName(std::string(argv[i]));
			}
			else if (strcmp(azColName[i], "CREATION_DATE") == 0)
			{
				currAlbum.setCreationDate(std::string(argv[i]));
			}
		}
	}

	myData.push_back(currAlbum);
	return 0;
}

int callbackPrintAlbumsData(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 2)
	{
		std::cout << "[" << argv[0] << "] -  by user " << argv[1];
	}
}

int callbackPrintUserData(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1)
	{
		std::cout << " - " << argv[1];
	}
}

/*
this function sets data from the database into a given Album object.
input: pointer to Album object, amount of columns, data of columns and name of columns.
output: 0.
*/
int callbackGetUserData(void* data, int argc, char** argv, char** azColName)
{
	User* myData = static_cast<User*>(data);

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != nullptr)
		{
			if (strcmp(azColName[i], "ID") == 0)
			{
				myData->setId(std::atoi(argv[i]));
			}
			else if (strcmp(azColName[i], "NAME") == 0)
			{
				myData->setName(std::string(argv[i]));
			}
		}
	}
	return 0;
}

/*
this function sets data from the database into a given User object.
input: pointer to user object, amount of columns, data of columns and name of columns.
output: 0.
*/
int callbackGetUserData(void* data, int argc, char** argv, char** azColName)
{
	User* myData = static_cast<User*>(data);

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != nullptr)
		{
			if (strcmp(azColName[i], "ID") == 0)
			{
				myData->setId(std::atoi(argv[i]));
			}
			else if (strcmp(azColName[i], "NAME") == 0)
			{
				myData->setName(std::string(argv[i]));
			}
		}
	}
	return 0;
}

/*
this function sets data from the database into a given picture object.
input: pointer to picture object, amount of columns, data of columns and name of columns.
output: 0.
*/
int callbackGetPictureData(void* data, int argc, char** argv, char** azColName)
{
	Picture* myData = static_cast<Picture*>(data);

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != nullptr)
		{
			if (strcmp(azColName[i], "ID") == 0)
			{
				myData->setId(std::atoi(argv[i]));
			}
			else if (strcmp(azColName[i], "NAME") == 0)
			{
				myData->setName(std::string(argv[i]));
			}
			else if (strcmp(azColName[i], "CREATION_DATE") == 0)
			{
				myData->setCreationDate(std::string(argv[i]));
			}
			else if (strcmp(azColName[i], "LOCATION") == 0)
			{
				myData->setPath(std::string(argv[i]));
			}
		}
	}

	return 0;
}

/*
this function sets the data from the data base into a Picture object
and checking if it exist in given list. if not, the function adding
the picture data into the list.
input: pointer to picture list object, amount of columns, data of columns and name of columns.
output: 0.
*/
int callbackGetPictureList(void* data, int argc, char** argv, char** azColName)
{
	std::list<Picture>& myData = *(static_cast<std::list<Picture>*>(data));
	Picture* currPic = new Picture(0,"");

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != nullptr)
		{
			if (strcmp(azColName[i], "ID") == 0)
			{
				currPic->setId(std::atoi(argv[i]));
			}
			else if (strcmp(azColName[i], "NAME") == 0)
			{
				currPic->setName(std::string(argv[i]));
			}
			else if (strcmp(azColName[i], "CREATION_DATE") == 0)
			{
				currPic->setCreationDate(std::string(argv[i]));
			}
			else if (strcmp(azColName[i], "LOCATION") == 0)
			{
				currPic->setPath(std::string(argv[i]));
			}
		}
	}

	for (const auto& picture : myData)
	{
		if (picture.getId() == currPic->getId())
		{
			return 0;
		}
	}

	myData.push_back(*currPic);
	return 0;
}