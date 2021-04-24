#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: hostname port" << endl;
        exit(0);
    }
    
    char *host = argv[1];
    int port = atoi(argv[2]);
    
    char buffer[256];
    struct hostent* server = gethostbyname(host);
    
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*server->h_addr_list));
    servAddr.sin_port = htons(port);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (connect(sockfd, (sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
        cerr << "Error connecting to the socket" << endl;
        exit(1);
    }
    
    cout << "Connected to the server!" << endl;
    
    while (1) {
        cout << ">";
        string input;
        getline(cin, input);
        
        memset(&buffer, 0, sizeof(buffer));
        strcpy(buffer, input.c_str());

        if (input == "exit") {
            send(sockfd, (char*)&buffer, strlen(buffer), 0);
            break;
        }
        
        send(sockfd, (char*)&buffer, strlen(buffer), 0);
        memset(&buffer, 0, sizeof(buffer));
        recv(sockfd, (char*)&buffer, sizeof(buffer), 0);
        
        if(!strcmp(buffer, "exit")) {
            cout << "Server has quit the session" << endl;
            break;
        }
        
        cout << "Server: " << buffer << endl;
    }
    
    close(sockfd);
    
    return 0;
}
