#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<thread>
using namespace std;

void receiveMessages(int sock){
    char buffer[1024];
    while(true){
        int bytes = recv(sock,buffer,sizeof(buffer),0);
        if(bytes<=0) break;
        buffer[bytes] = '\0';
        cout<<"\nServer: "<<buffer<<endl;

    }
}

int main(){
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in serverAddr;
    serverAddr.sin_family  = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&serverAddr.sin_addr);

    connect(sock,(sockaddr*)&serverAddr,sizeof(serverAddr));


    thread t(receiveMessages,sock);

    char msg[1024];
    while(true){
        cin.getline(msg,1024);
        send(sock,msg,strlen(msg),0);
    }

    t.join();
    close(sock);


    return 0;
}