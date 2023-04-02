#pragma once
#ifndef CALLBACKFUNCS
#define CALLBACKFUNCS

int returnFirstArgument(void* data, int argc, char** argv, char** azColName);
int callbackGetAlbumData(void* data, int argc, char** argv, char** azColName);
int callbackGetAlbumList(void* data, int argc, char** argv, char** azColName);
int callbackPrintAlbumsData(void* data, int argc, char** argv, char** azColName);
int callbackPrintUserData(void* data, int argc, char** argv, char** azColName);
int callbackGetUserData(void* data, int argc, char** argv, char** azColName);
int callbackGetPictureData(void* data, int argc, char** argv, char** azColName);
int callbackGetPictureList(void* data, int argc, char** argv, char** azColName);

#endif // ! CALLBACKFUNCS
