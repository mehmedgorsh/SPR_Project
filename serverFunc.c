#include "serverFunc.h"

//---------------------------------------------------------------------------------
void StartServer()
{
    int retval;
    int serversocket, newsocket;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addrsize;

    if( (serversocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("[-] Error in socket. \n");
        exit(1);
    }
    else
    {
        printf("[+] Server socket created. \n");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    retval = bind (serversocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if(retval < 0)
    {
        perror("[-] Binding error. \n");
        exit(1);
    }
    else
    {
        printf("[+] Binding successfull. \n");
    }
    
    
    retval = listen(serversocket, MAXCLIENT);
    if(retval == 0)
    {
        printf("[+] Listening... \n");
    }
    else
    {
        perror("[-] Error in listening. \n");
        exit(1);
    }

    addrsize = sizeof(serverAddr);
    while(1)
    {
        newsocket = accept(serversocket, (struct sockaddr *)&serverAddr, &addrsize);
        if(newsocket < 0)
        {
            perror("[-] Server accept failure. \n");
        }
        else
        {
            printf("[+] Accepted. \n");
                    
            menu(newsocket);
        }

    }
    close(serversocket);
    printf("[+] Server Closed. \n");
}

//---------------------------------------------------------------------------------
void menu(int connectionSocket)
{
    char message[MSGLEN] = "";
    char choice;
    int recvBytes;
    int sendBytes;
    boolean isLoginDataCorrect = FALSE;
    int fd;
    tLoginData tempUser = { { 0 } };
    char fname[MSGLEN] = "";

    do
    {
        recvBytes = recv(connectionSocket, message, MSGLEN, 0);
        if(recvBytes <= 0)
        {
            perror("[-] Recv Error. \n");
        }
        else
        {
            choice = message[0];

            if(isLoginDataCorrect == FALSE)
            {
                switch (choice)
                {
                case '0':
                    printf("[+] Close Client connection. \n");
                    break;
              
                case '1':
                    recvBytes = recv(connectionSocket, &tempUser, sizeof(tLoginData), 0);
                    if(recvBytes <= 0)
                    {
                        perror("[-] Recv Error. \n");
                    }
                    else
                    {   
                        isLoginDataCorrect = ChechUsrLogin(tempUser);
                    }

                    if(isLoginDataCorrect == FALSE)
                    {
                        message[0] = '0';
                    }
                    else
                    {
                        message[0] = '1';
                    }

                    sendBytes = send(connectionSocket, message, sizeof(message), 0);
                    memset(message, 0, sizeof(message));
                    if(sendBytes <= 0)
                    {
                        perror("[-] Error sending to LogData server. \n");
                    }

                    break;
                 case '2':
                    recvBytes = recv(connectionSocket, &tempUser, sizeof(tLoginData), 0);
                    if(recvBytes <= 0)
                    {
                        perror("[-] Recv Error. \n");
                    }
                    else
                    {   
                        isLoginDataCorrect = RegisterUsr(tempUser);
                    }

                    if(isLoginDataCorrect == FALSE)
                    {
                        message[0] = '0';
                    }
                    else
                    {
                        message[0] = '1';
                    }

                    sendBytes = send(connectionSocket, message, sizeof(message), 0);
                    memset(message, 0, sizeof(message));
                    if(sendBytes <= 0)
                    {
                        perror("[-] Error sending to LogData server. \n");
                    }
                    break;
                }
            }
            else
            {               
                switch (choice)
                {
                case '0':
                    printf("[+] Close Client connection. \n");
                    break;
                case '1':
                    ShowFilesList(connectionSocket);
                    break;
                case '2':
                    recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                    if(recvBytes <= 0)
                    {
                        perror("[-] Error recv file name. \n");
                    }
                    else
                    {
                        strncpy(fname, message, sizeof(message));
                        ReadFile(fname, connectionSocket);
                    }
                    break;
                case '3':
                    recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                    if(recvBytes <= 0)
                    {
                        perror("[-] Error recv file name. \n");
                    }
                    else
                    {
                        strncpy(fname, message, sizeof(message));
                        ReadFile(fname, connectionSocket);
                    }

                    break;
                case '4':
                    recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                    if(recvBytes <= 0)
                    {
                        perror("[-] Error recv file name. \n");
                    }
                    else
                    {
                        strncpy(fname, message, sizeof(message));
                        UploadFile(fname, connectionSocket);
                    }
                    break;
                default:
                    break;
                }
            }
        
        }
        memset(message, 0, sizeof(message));
       
    } while (choice != '0');
    
}

//---------------------------------------------------------------------------------
boolean ChechUsrLogin(tLoginData userP)
{
    printf("\tCHECKUSERLOGIN-function\n");
    tLoginData tempUser  = { { 0 } };
    boolean isUserReg   = FALSE;
    int fd          = 0;
    int readBytes   = 0;

    fd = open(UserDB, O_RDONLY, 0666);
    if(fd <= 0)
    {
        perror("[-] Error opening UserDB. \n");
    }
    else
    {
        while(1)
        {
            readBytes = read(fd, &tempUser, sizeof(tLoginData));
            if(readBytes <= 0)
            {
                break;
            }
            else
            {
                if( (strcmp(userP.username, tempUser.username) == 0) &&
                    (strcmp(userP.password, tempUser.password) == 0) )
                {
                    isUserReg = TRUE;
                    break;
                }
            }
          
        }
    }
    close(fd);
    
    return isUserReg;
}

//---------------------------------------------------------------------------------
boolean RegisterUsr(tLoginData userP)
{
    printf("\tREGISTERUSER-function\n");
    boolean isRegSuccessfull = TRUE;
    int writtenBytes = 0;
    int fd = 0;

    fd = open(UserDB, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd <= 0)
    {
        perror("[-] Error opening UserDB. \n");
        isRegSuccessfull = FALSE;
    }
    else
    {
        writtenBytes = write(fd, &userP, sizeof(tLoginData));
        if(writtenBytes <= 0)
        {
            perror("[-] Error writing in UserDB. \n");
            isRegSuccessfull = FALSE;
        }
    }
    close(fd);
    return isRegSuccessfull;
}

//---------------------------------------------------------------------------------
void ShowFilesList(int connectionSocket)
{
    int fd;
    char message[MSGLEN] = "";
    int readBytes;

    fd = open(lsFile, O_RDONLY, 0666);
    if(fd <= 0)
    {
        perror("[-] Error opening lsFile. \n");
    }
    else
    {
        while(1)
        {
            readBytes = read(fd, message, sizeof(message));
        
            if(send(connectionSocket, message, sizeof(message), 0) == -1)
            {
                perror("[-] Error showing list file. \n");
                break;
            }

            if(readBytes <= 0)
            {
                break;
            }

            memset(message, 0, sizeof(message));
        }
    }
    close(fd);
}

void ReadFile(const char * const filename, int connectionSocket)
{
    int fd;
    char message[MSGLEN] = "";
    int readBytes;
    fd = open(filename, O_RDONLY, 0666);

    if(fd <= 0)
    {
        perror("[-] Error opening selected file. \n");
    }
    else
    {
        while(1)
        {
            readBytes = read(fd, message, sizeof(message));
        
            if(send(connectionSocket, message, sizeof(message), 0) == -1)
            {
                perror("[-] Error showing list file. \n");
                break;
            }

            if(readBytes <= 0)
            {
                break;
            }

            memset(message, 0, sizeof(message));
        }
    }
    close(fd);
}

void UploadFile(const char * const filename, int connectionSocket)
{
    int fd;
    char message[MSGLEN] = "";
    int readBytes;
    fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);

    while(1)
    {
        readBytes = recv(connectionSocket, message, sizeof(message), 0);
        if(readBytes <= 0)
        {
            break;
        }

        write(fd, message, sizeof(message));
        memset(message, 0, sizeof(message));
    }
    close(fd);

    fd = open(lsFile, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd <= 0)
    {
        perror("[-] lsFile error. \n");
    }
    else
    {
        if(write(fd, filename, sizeof(filename)) <= 0)
        {
            perror("[-] Adding files in ls file. \n");
        }
    }
    close(fd);
}













