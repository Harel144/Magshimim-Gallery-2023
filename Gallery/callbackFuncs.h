#pragma once
#ifndef CALLBACKFUNCS
#define CALLBACKFUNCS

int returnFirstArgument(void* data, int argc, char** argv, char** azColName);
int callbackGetAlbumData(void* data, int argc, char** argv, char** azColName);
int callbackGetAlbumList(void* data, int argc, char** argv, char** azColName);
int callbackPrintAlbumsData(void* data, int argc, char** argv, char** azColName);


#endif // ! CALLBACKFUNCS
