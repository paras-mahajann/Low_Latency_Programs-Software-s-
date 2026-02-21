#include<iostream>
#include<thread>
#include<arpa/inet.h>
#include<chrono>
#include<cstring>
#include<unistd.h>

void receiveMessages(int sock){
    char buffer[1024];

    while(true){
        int bytes = recv(sock,buffer,sizeof(buffer)-1,0);

        if(bytes<=0){
            std::cout<<"Disconnected from server.\n";
            break;
        }

        buffer[bytes] = '\0';
        std::cout<<buffer<<std::endl;
    }
}

int main(){
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&serverAddr.sin_addr);

    if(connect(clientSocket,(sockaddr*)&serverAddr,sizeof(serverAddr))<0){
        perror("Connection failed");
        return 1;
    }

    std::cout<<"Enter username: ";
    std::string username;
    std::getline(std::cin,username);

    send(clientSocket,username.c_str(),username.size(),0);

    std::thread t(receiveMessages,clientSocket);

    char message[1024];

    while(true){
        auto start = std::chrono::high_resolution_clock::now();

        std::cin.getline(message,1024);
        send(clientSocket,message,strlen(message),0);

        auto end = std::chrono::high_resolution_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout<<"[Latency: "<<latency<<" ms]"<<std::endl;

    }

    t.join();
    close(clientSocket);       

    return 0;
}