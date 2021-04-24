#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    
    //set up the port and buffer to send and receive messages
    int port = atoi(argv[1]);
    char buffer[256];
    
    //set up our socket
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    
    //server error checking
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    
    //binding the socket to its local address
    if (bind(sockfd, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
        cerr << "Error on binding" << endl;
        exit(0);
    }
    
    cout << "Connection established" << endl;
    
    listen(sockfd, 5);
    
    sockaddr_in cliAddr;
    socklen_t clilen = sizeof(cliAddr);
    
    int newsockfd = accept(sockfd, (sockaddr *) &cliAddr, &clilen);
    if (newsockfd < 0) {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    
    while (1) {
        cout << "Awaiting client response" << endl;
        memset(&buffer, 0, sizeof(buffer));
        recv(newsockfd, (char*)&buffer, sizeof(buffer), 0);
        
        if (!strcmp(buffer, "exit")) {
            cout << "Client has quit the session" << endl;
            break;
        }
        
        cout << "Client: " << buffer << endl;
        cout << ">";
        
        string input;
        getline(cin, input);
        
        memset(&buffer, 0, sizeof(buffer));
        strcpy(buffer, input.c_str());
        
        if (input == "exit") {
            send(newsockfd, (char*)&buffer, strlen(buffer), 0);
            break;
        }
        send(newsockfd, (char*)&buffer, strlen(buffer), 0);
    }
    
    close(newsockfd);
    close(sockfd);
    
    return 0;
}
