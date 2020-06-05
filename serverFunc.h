#ifndef _SERVERFUNC_H__
#define _SERVERFUNC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

//---------------------------------------------------------------------------------
// types
//---------------------------------------------------------------------------------
#define FALSE 0
#define TRUE (!FALSE)
typedef unsigned char boolean;

typedef struct loginData
{
    char username[32];
    char password[32];
}tLoginData;

//---------------------------------------------------------------------------------
// constants
//---------------------------------------------------------------------------------
#define PORT 5577
#define MSGLEN 1024
#define MAXCLIENT 10
static const char * const ip = "127.0.0.1";
static const char * const UserDB = "LogUserDB";
static const char * const lsFile = "lsfile.txt";

//---------------------------------------------------------------------------------
// Server functions
//---------------------------------------------------------------------------------
void StartServer();
void menu(int connectionSocket);
void ShowFilesList(int connectionSocket);
void ReadFile(const char * const filename, int connectionSocket);
void UploadFile(const char * const filename, int connectionSocket);
boolean ChechUsrLogin(tLoginData userP);
boolean RegisterUsr(tLoginData userP);


#endif // _SERVERFUNC_H__