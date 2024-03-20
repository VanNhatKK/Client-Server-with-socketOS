// Khai bao thu vien
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "libsocket.h"

// Ham con(in file .c)
//-------------------NHAN DU LIEU DA LUONG------------------//

void rev_data_from_server(int ClientsocketFD) 
{
    char buf[1024];
    while (1) 
    {
        // Nhan du lieu
        ssize_t check_size_rev = recv(ClientsocketFD, &buf, 1024, 0);
        if (check_size_rev > 0) 
        {
            printf("%s\n", buf);
        } 
        else if (check_size_rev == 0) 
        {
            break;
        }
    }
    close(ClientsocketFD);
}

void rec_data_p_from_server(int ClientsocketFD)
{
    pthread_t id;
    pthread_create(&id, NULL, rev_data_from_server,ClientsocketFD);
}


// Ham chinh
int main()
{
    // Gan socket
    int ClientsocketFD = Create_Socket();
    
    // Ket noi toi dia chi
    struct sockaddr_in* Clientaddress = Create_Address("192.168.220.142", 2000);
    int result = connect(ClientsocketFD,(struct sockaddr*)Clientaddress, sizeof(*Clientaddress));

    // Tra ve 0 neu thanh cong - (-1) neu that bai
    if(result==0)
    {
        printf("Client da ket noi thanh cong!\n");


    }
    else
    {
        printf("Client ket noi khong thanh cong!\n");
        printf("Ket thuc chuong trinh\n");
        exit(EXIT_FAILURE);
    }
    // Gui du lieu
    char *name = NULL;
    size_t nameSize= 0;
    printf("Dang ki ten: \n");
    ssize_t  name_check_size = getline(&name,&nameSize,stdin);
    name[name_check_size-1]=0;

    char *line = NULL;
    size_t lineSize= 0;
    printf("Type and we will send(type exit)...\n");

    char mess[1024];

    // Nhan du lieu
    rec_data_p_from_server(ClientsocketFD);


    while (1)
    {
        // Gui du lieu
        ssize_t  line_check_size = getline(&line,&lineSize,stdin);
        line[line_check_size-1]=0;
        sprintf(mess,"%s: %s",name,line);

        if(line_check_size>0)
        {
            if(strcmp(line,"exit")==0)
                break;
            else
                send(ClientsocketFD,mess,strlen(mess)+1,0);
        }
    }
    
    close(ClientsocketFD);
    return 0;
}