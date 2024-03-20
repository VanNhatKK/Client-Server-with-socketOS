// Khai bao thu vien
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "libsocket.h"

// Ham con(in file .c)
//--------------------HAM ACCEPT CAC CLIENT---------------------------//
// Khoi tao bien struct tra ve 
struct Accept
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
};

struct Accept* Accept_Client(int ServersocketFD)
{
    struct sockaddr_in C_Clientaddress;
    // Tien hanh accept
    int size_sockaddr_in = sizeof(struct sockaddr_in);
    int C_ClientsocketFD = accept(ServersocketFD,(struct sockaddr*)&C_Clientaddress,&size_sockaddr_in);
    // Cac bien accept
    struct Accept *accept_socket_client = malloc(sizeof(struct Accept));
    accept_socket_client->address = C_Clientaddress;
    accept_socket_client->acceptedSocketFD = C_ClientsocketFD;
    // Kiem tra accept
    if(C_ClientsocketFD<0)
    {
        accept_socket_client->error = C_ClientsocketFD;
        printf("Loi ket noi: %d\n",C_ClientsocketFD);
    }
    else
        printf("Da co Client ket noi\n");


    return accept_socket_client;
}

//--------------------HAM NHAN VA IN DU LIEU---------------------------//  

struct Accept nsocket[10];
int nsocket_count =0;

void send_other_client(char* buf,int FD_from_Client)
{
    for(int i=0;i < nsocket_count;i++)
    {
        if(nsocket[i].acceptedSocketFD!=FD_from_Client)
        {
            ssize_t check_send = send(nsocket[i].acceptedSocketFD,buf,strlen(buf)+1,0);
            if(check_send<0)
            {
                printf("error\n");
            }
        }
    }

}

//-----------------------------------------------//

void rev_data_and_print(int FD_from_Client)
{
    char buf[1024];
    while (1)
    {
        // Nhan du lieu
        ssize_t check_size_rev = recv(FD_from_Client,&buf,1024,0);
        if(check_size_rev>0)
        {
            printf("%s\n",buf);

            // Gui du lieu nguoc lai cac other khac
            send_other_client(buf,FD_from_Client);
        }
        else if(check_size_rev==0)
            break;
            
    }
    close(FD_from_Client);
}

//-----------------------THAO TAC DA LUONG P_THREAD--------------------------------//
void rec_data_p(struct Accept* psocket)
{
    pthread_t id;
    pthread_create(&id,NULL,rev_data_and_print,psocket->acceptedSocketFD);
}

void accept_P_thread(int ServersocketFD)
{
    while(1)
    {
        struct Accept* client_accepted_FD = Accept_Client(ServersocketFD);
        nsocket[nsocket_count++] = *client_accepted_FD;
        rec_data_p(client_accepted_FD);
    }
}



// Ham chinh
int main()
{
    // Gan socket
    int ServersocketFD = Create_Socket();
    // Tao dia chi 
    struct sockaddr_in* Serveraddress = Create_Address("",2000);
    // May chu tu ket noi den dia chi nay
    int resultbind =  bind(ServersocketFD,(struct sockaddr*)Serveraddress,sizeof(*Serveraddress));
    if(resultbind == 0)
        printf("Da co may chu\n");

    // Lang nghe cac ket noi //10 max
    int listenClient = listen(ServersocketFD,10);
    if(listenClient==0)
         printf("Listening...\n");

    // Goi ham chap nhan ket noi da luong
    accept_P_thread(ServersocketFD);


    close(ServersocketFD);

    return 0;
}