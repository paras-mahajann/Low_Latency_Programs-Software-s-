#include<iostream>
#include<vector>
#include<arpa/inet.h>
#include<mutex>
#include<cstring>
#include<unistd.h>
#include<vector>
#include<thread>
#include<algorithm>


std::vector<int> clients;
std::mutex clientsMutex;

void broadcastMessage(const std::string &msg, int senderSocket){
    std::lock_guard<std::mutex> lock(clientsMutex);

    for(int client:clients){
        if(client != senderSocket){
            send(client,msg.c_str(),msg.size(),0);   
        }
    }
}

void handleClient(int clientSocket){
    char buffer[1024];
    std::string username;

    recv(clientSocket,buffer,sizeof(buffer),0);
    username = buffer;
    
    std::cout<<username<<" joined the chat.\n";

    broadcastMessage(username+" joined the chat.\n",clientSocket);
    
    while(true){

        int bytes = recv(clientSocket,buffer,sizeof(buffer),0);

        if(bytes<=0){
            std::cout<<username<<" disconnected\n";
            break;
        }

        buffer[bytes] = '\0';
        
        std::string message = username + ": " + buffer;
        std::cout<<message<<std::endl;

        broadcastMessage(message,clientSocket);
    }

    close(clientSocket);

    std::lock_guard<std::mutex> lock(clientsMutex);

    clients.erase(
        std::remove(clients.begin(),clients.end(),clientSocket),
        clients.end()
    );

    
}

int main(){
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    bind(serverSocket,(sockaddr*)&serverAddr,sizeof(sockaddr));

    listen(serverSocket,5);

    std::cout<<"Chat server is running on port 8080...\n";

    while(true){

        int clientSocket = accept(serverSocket,nullptr,nullptr);

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        std::thread t(handleClient,clientSocket);
        t.detach();
        
    }

    close(serverSocket);
    return 0;
}