// Tat ca cac thu vien can dung
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
// Ham tao ra socket IPv4
int Create_Socket()
{
    return(socket(AF_INET,SOCK_STREAM,0));
}
// Ham tao dia chi socket
//Dia chi la 1 ham struct
struct sockaddr_in* Create_Address(char* ip, int port)
{
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in)); // Khai bao bien dia chi struct
    // Cau hinh dia chi
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    // Chuyen doi ip
    if(strlen(ip)==0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET,ip,&address->sin_addr.s_addr);

    return address;
}