#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include "utils/utils.h"
#include "server.h"


#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

std::vector<int> clients;


/*

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Создание сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(6565);

    // Связывание сокета с адресом
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Error listening" << std::endl;
        return 1;
    }

    std::cout << "Server started on port 6565" << std::endl;

    while (true) {
        // Принятие входящего соединения
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            std::cerr << "Error accepting client" << std::endl;
            continue;
        }

        // Добавление клиента в список
        clients.push_back(clientSocket);
        std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        // Обработка команд от клиентов
        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (bytesReceived <= 0) {
                // Клиент отключился
                close(clientSocket);
                clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
                std::cout << "Client disconnected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
                break;
            }

            std::string message(buffer, bytesReceived);

            // Обработка команды от клиента
            if (message == "count") {
                std::string response = createCharCountTable(message);
                send(clientSocket, response.c_str(), response.length(), 0);
            } else if (message == "clients") {
                std::string response = "Number of connected clients: " + std::to_string(clients.size()) + "\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            } else {
                // Отправка сообщения другим клиентам
                broadcastMessage(clientSocket, message);
            }
        }
    }

    // Закрытие сокета
    close(serverSocket);

    return 0;
}*/

int main(){
    Server server(7070);
    server.start();
}