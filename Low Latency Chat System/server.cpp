#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std;

void handleClient(int clientSocket)
{

    char buffer[1024];

    while (true)
    {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            cout << "Client disconnected\n";
            break;
        }

        buffer[bytesReceived] = '\0';

        cout<<"Client says: "<<buffer<<endl;

        send(clientSocket,buffer,sizeof(buffer),0);

    }

    close(clientSocket);
}


int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);
    cout << "server is running on port 8080" << endl;

    while (true)
    {
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        cout << "New Client connected..\n";

        thread t(handleClient, clientSocket);

        t.detach();
    }

    close(serverSocket);

    return 0;
}