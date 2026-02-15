#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<thread>
#include<cstring>

using namespace std;

void receiveMessages(int clientSocket){
    char buffer[1024];

    while (true)
    {
        int bytes = recv(clientSocket,buffer,sizeof(buffer),0);
        if(bytes<=0){
            cout<<"Disconnected from server\n";
            break;
        }

        buffer[bytes] = '\0';

        cout<<"\nserver: "<<buffer<<endl;
    }    

}

int main(){

    int clientSocket = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&serverAddr.sin_addr);

    connect(clientSocket,(sockaddr*)&serverAddr,sizeof(serverAddr));

    thread t(receiveMessages,clientSocket);

    char message[1024];
    while (true)
    {
        cin.getline(message,1024);
        send(clientSocket,message,strlen(message),0);
    }

    t.join();
    close(clientSocket);
    


    return 0;
}