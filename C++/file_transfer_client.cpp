#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>      //*For inet_pton
#include <unistd.h>     //For read
#include <string>
#include <fstream>
using namespace std;

int main()
{
    struct sockaddr_in address;
    struct sockaddr_in server_addr;
    char* connected = (char *)"Connected to client!";
    char buffer[1024];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    send(sock , connected , strlen(connected) ,0);
    
    int checkConnection = read(sock, buffer, 1024);     //read returns 0 if not connected
    ofstream fobject;
    fobject.open("file1.mp4", ios::binary);
    while(checkConnection != 0)
    {
        fobject.write(buffer, 1024);
        cout<<strlen(buffer)<<endl;
        bzero(buffer, 1024);
        checkConnection = read(sock, buffer, 1024);
    }
    fobject.close();
    close(sock);
    return 0;
}