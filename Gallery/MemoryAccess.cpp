#include <map>
#include <algorithm>

#include "ItemNotFoundException.h"
#include "MemoryAccess.h"



void MemoryAccess::printAlbums() 
{
	if(m_albums.empty()) {
		throw MyException("There are no existing albums.");
	}
	std::cout << "Album list:" << std::endl;
	std::cout << "-----------" << std::endl;
	for (const Album& album: m_albums) 	{
		std::cout << std::setw(5) << "* " << album;
	}
}

bool MemoryAccess::open()
{
	// create some dummy albums
	for (int i=0; i<5; ++i) {
		// create some dummy users
		std::stringstream name("User_"+std::to_string(i));

		User user(i, name.str());
		createUser(user);

		m_albums.push_back(createDummyAlbum(user));
	}

	return true;
}

void MemoryAccess::clear()
{
	m_users.clear();
	m_albums.clear();
}

auto MemoryAccess::getAlbumIfExists(const std::string & albumName)
{
	auto result = std::find_if(std::begin(m_albums), std::end(m_albums), [&](auto& album) { return album.getName() == albumName; });

	if (result == std::end(m_albums)) {
		throw ItemNotFoundException("Album not exists: ", albumName);
	}
	return result;

}

Album MemoryAccess::createDummyAlbum(const User& user)
{
	std::stringstream name("Album_" +std::to_string(user.getId()));

	Album album(user.getId(),name.str());

	for (int i=1; i<3; ++i)	{
		std::stringstream picName("Picture_" + std::to_string(i));

		Picture pic(i++, picName.str());
		pic.setPath("C:\\Pictures\\" + picName.str() + ".bmp");

		album.addPicture(pic);
	}

	return album;
}

const std::list<Album> MemoryAccess::getAlbums() 
{
	return m_albums;
}

const std::list<Album> MemoryAccess::getAlbumsOfUser(const User& user) 
{	
	std::list<Album> albumsOfUser;
	for (const auto& album: m_albums) {
		if (album.getOwnerId() == user.getId()) {
			albumsOfUser.push_back(album);
		}
	}
	return albumsOfUser;
}

void MemoryAccess::createAlbum(const Album& album)
{
	m_albums.push_back(album);
}

void MemoryAccess::deleteAlbum(const std::string& albumName, int userId)
{
	for (auto iter = m_albums.begin(); iter != m_albums.end(); iter++) {
		if ( iter->getName() == albumName && iter->getOwnerId() == userId ) {
			iter = m_albums.erase(iter);
			return;
		}
	}
}

bool MemoryAccess::doesAlbumExists(const std::string& albumName, int userId) 
{
	for (const auto& album: m_albums) {
		if ( (album.getName() == albumName) && (album.getOwnerId() == userId) ) {
			return true;
		}
	}

	return false;
}

Album MemoryAccess::openAlbum(const std::string& albumName) 
{
	for (auto& album: m_albums)	{
		if (albumName == album.getName()) {
			return album;
		}
	}
	throw MyException("No album with name " + albumName + " exists");
}

void MemoryAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture) 
{
	auto result = getAlbumIfExists(albumName);

	(*result).addPicture(picture);
}

void MemoryAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) 
{
	auto result = getAlbumIfExists(albumName);

	(*result).removePicture(pictureName);
}

void MemoryAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	auto result = getAlbumIfExists(albumName);

	(*result).tagUserInPicture(userId, pictureName);
}

void MemoryAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	auto result = getAlbumIfExists(albumName);

	(*result).untagUserInPicture(userId, pictureName);
}

void MemoryAccess::closeAlbum(Album& ) 
{
	// basically here we would like to delete the allocated memory we got from openAlbum
}

// ******************* User ******************* 
void MemoryAccess::printUsers()
{
	std::cout << "Users list:" << std::endl;
	std::cout << "-----------" << std::endl;
	for (const auto& user: m_users) {
		std::cout << user << std::endl;
	}
}

User MemoryAccess::getUser(int userId) {
	for (const auto& user : m_users) {
		if (user.getId() == userId) {
			return user;
		}
	}

	throw ItemNotFoundException("User", userId);
}

void MemoryAccess::createUser(User& user)
{
	m_users.push_back(user);
}

void MemoryAccess::deleteUser(const User& user)
{
	if (doesUserExists(user.getId())) {
	
		for (auto iter = m_users.begin(); iter != m_users.end(); ++iter) {
			if (*iter == user) {
				iter = m_users.erase(iter);
				return;
			}
		}
	}
}

bool MemoryAccess::doesUserExists(int userId) 
{
	auto iter = m_users.begin();
	for (const auto& user : m_users) {
		if (user.getId() == userId) {
			return true;
		}
	}
	
	return false;
}


// user statistics
int MemoryAccess::countAlbumsOwnedOfUser(const User& user) 
{
	int albumsCount = 0;

	for (const auto& album: m_albums) {
		if (album.getOwnerId() == user.getId()) {
			++albumsCount;
		}
	}

	return albumsCount;
}

int MemoryAccess::countAlbumsTaggedOfUser(const User& user) 
{
	int albumsCount = 0;

	for (const auto& album: m_albums) {
		const std::list<Picture>& pics = album.getPictures();
		
		for (const auto& picture: pics)	{
			if (picture.isUserTagged(user))	{
				albumsCount++;
				break;
			}
		}
	}

	return albumsCount;
}

int MemoryAccess::countTagsOfUser(const User& user) 
{
	int tagsCount = 0;

	for (const auto& album: m_albums) {
		const std::list<Picture>& pics = album.getPictures();
		
		for (const auto& picture: pics)	{
			if (picture.isUserTagged(user))	{
				tagsCount++;
			}
		}
	}

	return tagsCount;
}

float MemoryAccess::averageTagsPerAlbumOfUser(const User& user) 
{
	int albumsTaggedCount = countAlbumsTaggedOfUser(user);

	if ( 0 == albumsTaggedCount ) {
		return 0;
	}

	return static_cast<float>(countTagsOfUser(user)) / albumsTaggedCount;
}

User MemoryAccess::getTopTaggedUser()
{
	std::map<int, int> userTagsCountMap;

	auto albumsIter = m_albums.begin();
	for (const auto& album: m_albums) {
		for (const auto& picture: album.getPictures()) {
			
			const std::set<int>& userTags = picture.getUserTags();
			for (const auto& user: userTags ) {
				//As map creates default constructed values, 
				//users which we haven't yet encountered will start from 0
				userTagsCountMap[user]++;
			}
		}
	}

	if (userTagsCountMap.size() == 0) {
		throw MyException("There isn't any tagged user.");
	}

	int topTaggedUser = -1;
	int currentMax = -1;
	for (auto entry: userTagsCountMap) {
		if (entry.second < currentMax) {
			continue;
		}

		topTaggedUser = entry.first;
		currentMax = entry.second;
	}

	if ( -1 == topTaggedUser ) {
		throw MyException("Failed to find most tagged user");
	}

	return getUser(topTaggedUser);
}

Picture MemoryAccess::getTopTaggedPicture()
{
	int currentMax = -1;
	const Picture* mostTaggedPic = nullptr;
	for (const auto& album: m_albums) {
		for (const Picture& picture: album.getPictures()) {
			int tagsCount = picture.getTagsCount();
			if (tagsCount == 0) {
				continue;
			}

			if (tagsCount <= currentMax) {
				continue;
			}

			mostTaggedPic = &picture;
			currentMax = tagsCount;
		}
	}
	if ( nullptr == mostTaggedPic ) {
		throw MyException("There isn't any tagged picture.");
	}

	return *mostTaggedPic;	
}

std::list<Picture> MemoryAccess::getTaggedPicturesOfUser(const User& user)
{
	std::list<Picture> pictures;

	for (const auto& album: m_albums) {
		for (const auto& picture: album.getPictures()) {
			if (picture.isUserTagged(user)) {
				pictures.push_back(picture);
			}
		}
	}

	return pictures;
}
