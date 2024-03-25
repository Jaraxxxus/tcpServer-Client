#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>
#include <list>
#include <thread>

#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 2 // Таймаут в секундах

 std::list<std::string> messanges;

void thread1( ) {
    //std::cout << "Thread ID " << std::this_thread::get_id() << std::endl;
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        messanges.push_back(input);
    }
}

int main(int argc, char* argv[]) {

     
    std::thread th(thread1);
    /*
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }
    */

    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    std::string serverIP = "127.0.0.1";
    int serverPort = 7070;

    //std::string serverIP(argv[1]);
    //int serverPort = std::stoi(argv[2]);

    while (true) {
        // Создание сокета
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return 1;
        }

        // Настройка адреса сервера
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
        serverAddr.sin_port = htons(serverPort);

        // Подключение к серверу
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error connecting to server, retrying in 5 seconds..." << std::endl;
            close(clientSocket);
            sleep(5);
            continue;
        }

        std::cout << "Connected to server" << std::endl;
        std::string lastMessage;

        fd_set readfds;
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        while (true) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(clientSocket, &readfds);


            // Ожидание событий в сокете с таймаутом
            int ready = select(clientSocket + 1, &readfds, nullptr, nullptr, &timeout);
            if (ready == -1) {
                std::cerr << "Error in select()" << std::endl;
                break;
            }
            else if (ready == 0) {
                if (!messanges.empty()){
                    auto msg = messanges.front();
                    send(clientSocket, msg.c_str(), msg.length(), 0);
                    messanges.pop_front();
                }
            }

            // Если есть данные для чтения из сокета
            if (FD_ISSET(clientSocket, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
                if (bytesReceived <= 0) {
                    std::cerr << "Connection lost, reconnecting..." << std::endl;
                    close(clientSocket);
                    break;
                }
                std::cout << "Server: " << buffer << std::endl;
            }



            //std::cout << "Server response: "<<"\n" << buffer << std::endl;
        }
    }
    th.join();
    return 0;
}

