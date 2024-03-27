//
// Created by sazonov99 on 3/25/24.
//

#include <iostream>
#include <csignal>
#include <cstring>
#include <utility>
#include <arpa/inet.h>
#include "TcpClient.h"
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 2


void TcpClient::handleServer(){
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    char buffer[BUFFER_SIZE];

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(clientSocket, &readfds);


        // Ожидание событий в сокете с таймаутом
        int ready = select(clientSocket + 1, &readfds, nullptr, nullptr, &timeout);
        if (ready == -1) {
            std::cerr << "\n Error in select()" << std::endl;
            break;
        }
        else if (ready == 0) {
            if (!messages.empty()){
                msgMutex.lock();
                auto msg = messages.front();
                messages.pop_front();
                msgMutex.unlock();
                send(clientSocket, msg.c_str(), msg.length(), 0);
                sleep(1); //иначе сообщения "склеиваются", если успели накопиться
            }
        }

        // Если есть данные для чтения из сокета
        if (FD_ISSET(clientSocket, &readfds)) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (bytesReceived <= 0) {
                std::cerr << "\n Connection lost, reconnecting..." << std::endl;
                close(clientSocket);
                break;
            }
            std::cout << "\n Server response: " << buffer << std::endl;
        }
    }
}

void TcpClient::createSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        throw std::runtime_error("Error creating socket");
    }
}

void TcpClient::setServer() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    serverAddr.sin_port = htons(port);
}

bool TcpClient::Connect() {
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "\n Error connecting to server, retrying in 5 seconds..." << std::endl;
        sleep(5);
        return  false;
    }
    return true;
}

void TcpClient::start() {
    while (true) {
        createSocket();
        setServer();
        if (!Connect()){
            stop();
            continue;
        }
        std::cout << "\n Connected to server" << std::endl;
        handleServer();
    }
}

void TcpClient::stop() const {
    close(clientSocket);
}

TcpClient::TcpClient(int port, std::string addr) {
    this->addr = std::move(addr);
    this->port = port;
}

TcpClient::~TcpClient() {
    stop();
}

void TcpClient::sendMessage(const std::string& message) {
    msgMutex.lock();
    messages.push_back(message);
    msgMutex.unlock();
}
