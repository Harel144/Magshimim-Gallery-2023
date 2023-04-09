#pragma once
#include <string>
#include <vector>
#include <map>

enum CommandType
{
	HELP = 0,
	// Album operations
	CREATE_ALBUM = 1,
	OPEN_ALBUM,
	CLOSE_ALBUM,
	DELETE_ALBUM,
	LIST_ALBUMS,
	LIST_ALBUMS_OF_USER,

	// Picture operations
	ADD_PICTURE,
	REMOVE_PICTURE,
	SHOW_PICTURE,
	LIST_PICTURES,
	TAG_USER,
	UNTAG_USER,
	LIST_TAGS,

	// User operations
	ADD_USER,
	REMOVE_USER,
	LIST_OF_USER,
	USER_STATISTICS,
	

	TOP_TAGGED_USER,
	TOP_TAGGED_PICTURE,
	PICTURES_TAGGED_USER,

	EXIT = 99
};

struct CommandPrompt {
	CommandType type;
	const std::string prompt;
};

struct CommandGroup {
	const std::string title;
	const std::vector<struct CommandPrompt> commands;
};