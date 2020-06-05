#ifndef _CLIENTFUNC_H__
#define _CLIENTFUNC_H__

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
// Types
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
// Constants
//---------------------------------------------------------------------------------
#define PORT 5577
#define MSGLEN 1024
static const char * const ip = "127.0.0.1";

//---------------------------------------------------------------------------------
// Client functions
//---------------------------------------------------------------------------------
void StartClient();
void menu(int connectionSocket);

#endif // _CLIENTFUNC_H__