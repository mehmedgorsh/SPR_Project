#include "clientFunc.h"

//---------------------------------------------------------------------------------
void StartClient()
{
    int retval;
    int clientsocket;
    struct sockaddr_in serverAddr;

    if( (clientsocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("[-] Error creating client socket. \n");
        exit(1);
    }
    else
    {
        printf("[+] Client socket created. \n");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    retval = connect(clientsocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(retval == -1)
    {
        perror("[-] Error in connecting to the server. \n");
        exit(1);
    }
    else
    {
        printf("[+] Connection successfull. \n");
        menu(clientsocket);
    }

    
    close(clientsocket);
    printf("[+] Client closed. \n");
}

//---------------------------------------------------------------------------------
void menu(int connectionSocket)
{
    tLoginData logUser = { { 0 } };
    char message[MSGLEN] = "";
    char choice;
    boolean isLoginDataCorrect = FALSE;
    int sendBytes;
    int recvBytes;
    int fd;

    do
    {
        if(isLoginDataCorrect == FALSE)
        {
            printf("\n\n");
            printf("1) Login. \n");
            printf("2) Register. \n");
            printf("0) Exit. \n");

            printf("[+] Choice: ");
            fgets(message, sizeof(message) - 1, stdin);
            message[strlen(message) - 1] = '\0';
            choice = message[0];

            sendBytes = send(connectionSocket, message, sizeof(message), 0);
            memset(message, 0, sizeof(message));
            if(sendBytes <= 0)
            {
                perror("[-] Error sending to server. \n");
            }
            else
            {
                switch (choice)
                {
                case '0':
                    printf("[+] Exit. \n");
                    break;
                case '1':
                    printf("\n\t LOGIN\n");
                    printf("Username: ");
                    fgets(logUser.username, sizeof(logUser.username) - 1, stdin);
                    logUser.username[strlen(logUser.username) - 1] = '\0';

                    printf("Pasword: ");
                    fgets(logUser.password, sizeof(logUser.password) - 1, stdin);
                    logUser.password[strlen(logUser.password) - 1] = '\0';

                    sendBytes = send(connectionSocket, &logUser, sizeof(tLoginData), 0);
                    memset(message, 0, sizeof(message));
                    if(sendBytes <= 0)
                    {
                        perror("[-] Error sending to LogData server. \n");
                    }
                    else
                    {
                        recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                        if(message[0] == '0')
                        {
                            printf("[-] Error login. \n");
                        }
                        else
                        {
                            printf("[+] Welcome. \n");
                            isLoginDataCorrect = TRUE;
                        }
                        
                    }                   

                    break;
                case '2':
                    printf("\n\t REGISTER\n");
                    printf("Username: ");
                    fgets(logUser.username, sizeof(logUser.username) - 1, stdin);
                    logUser.username[strlen(logUser.username) - 1] = '\0';

                    printf("Pasword: ");
                    fgets(logUser.password, sizeof(logUser.password) - 1, stdin);
                    logUser.password[strlen(logUser.password) - 1] = '\0';

                    sendBytes = send(connectionSocket, &logUser, sizeof(logUser), 0);
                    memset(message, 0, sizeof(message));
                    if(sendBytes <= 0)
                    {
                        perror("[-] Error sending to RegData server. \n");
                    }
                    else
                    {
                        recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                        if(message[0] == '0')
                        {
                            printf("[-] Error login. \n");
                        }
                        else
                        {
                            printf("[+] Registration successfull. \n");
                            isLoginDataCorrect = TRUE;
                        }                     
                    }                   

                    break;  

                default:
                    printf("[-] Wrong choice... \n");
                    break;
                }
            }
        }
        else
        {
            printf("\n\n");
            printf("1) Show files on server. \n");
            printf("2) Read file from server. \n");
            printf("3) Download file from server. \n");
            printf("4) Upload file to server. \n");
            printf("0) Exit. \n");

            printf("Choice: ");

            fgets(message, sizeof(message) - 1, stdin);
            message[strlen(message) - 1] = '\0';
            choice = message[0];

            sendBytes = send(connectionSocket, message, sizeof(message), 0);
            memset(message, 0, sizeof(message));
            if(sendBytes <= 0)
            {
                perror("[-] Error sending to server. \n");
            }
            else
            {
                switch(choice)
                {
                    case '0':

                        printf("[+] Exit. \n");
                    break;

                    case '1':
                        printf("\n\n");
                        while(1)
                        {
                            recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                            printf("%s\n", message); 

                            if(recvBytes <= 0);
                            {
                                break;
                            }   
                            memset(message, 0, sizeof(message));                      
                        }
                        
                        break;

                    case '2':
                        printf("Enter filename: ");
                        fgets(message, sizeof(message) - 1, stdin);
                        message[strlen(message) - 1] = '\0';    
                        sendBytes = send(connectionSocket, message, sizeof(message), 0);
                        memset(message, 0, sizeof(message));

                        if(sendBytes <= 0)
                        {
                            perror("[-] Error sending to server. \n");
                        }
                        else
                        {
                            printf("\n\n");
                            while(1)
                            {
                                recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                                printf("%s\n", message);
                                if(recvBytes <= 0)
                                {
                                    break;
                                }

                                memset(message, 0, sizeof(message));
                                
                            }
                        }
                    
                        break;

                    case '3':
                        printf("Enter filename to download: ");
                        fgets(message, sizeof(message) - 1, stdin);
                        message[strlen(message) - 1] = '\0';    
                        sendBytes = send(connectionSocket, message, sizeof(message), 0);

                        fd = open(message, O_CREAT | O_WRONLY | O_APPEND, 0666);
                        memset(message, 0, sizeof(message));
                        if(fd <= 0)
                        {
                            perror("[-] Downloading error. \n");
                        }
                        else
                        {
                            while(1)
                            {
                                recvBytes = recv(connectionSocket, message, sizeof(message), 0);
                                if(recvBytes <= 0)
                                {
                                    break;
                                }
                                write(fd, message, sizeof(message));
                                memset(message, 0, sizeof(message));
                                
                            }
                            printf("[+] Dowloading successfull. \n");
                        }
                        
                        memset(message, 0, sizeof(message));
                        break;

                    case '4':
                        printf("Enter filename to upload: ");
                        fgets(message, sizeof(message) - 1, stdin);
                        message[strlen(message) - 1] = '\0';    
                        sendBytes = send(connectionSocket, message, sizeof(message), 0);

                        fd = open(message, O_RDONLY, 0666);
                        memset(message, 0, sizeof(message));
                        if(fd <= 0)
                        {
                            perror("[-] Uploading error. \n");
                        }
                        else
                        {
                            while(1)
                            {
                                recvBytes = read(fd, message, sizeof(message));
                                send(connectionSocket, message, sizeof(message), 0);
                                if(recvBytes <= 0)
                                {
                                    break;
                                }

                                memset(message, 0, sizeof(message));
                                
                            }
                            printf("[+] Uploading successfull. \n");
                        }
                        
                        memset(message, 0, sizeof(message));
                        break;
                    default:
                        printf("[-] Wrong choice. \n");
                        break;

                }
            }
            

        }
        memset(message, 0, sizeof(message));

    } while (choice != '0');
    
}

