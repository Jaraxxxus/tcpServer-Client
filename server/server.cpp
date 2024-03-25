//
// Created by sazonov99 on 3/24/24.
//

#include "server.h"
#include "utils/utils.h"
#include <string>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <algorithm>
#include <thread>

std::string const BROADCAST = "--broadcast";
std::string const CLIENTSNUMBER = "--clientsCount";

void Server::broadcastMessage(int senderSocket, const std::string& message) {
    clientsMutex.lock(); // Захватываем мьютекс перед доступом к списку клиентов
    for (int socket : clients) {
        if (socket != senderSocket) {
            send(socket, message.c_str(), message.length(), 0);
        }
    }
    clientsMutex.unlock(); // Освобождаем мьютекс
}

Server::Server(int port) {
    this->port = port;
    // Создание сокета
    createSocket();
    // Настройка адреса сервера
    addrPrepare();
    // Связывание сокета с адресом
    bindSocketToAddr();
}

void Server::createSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
    }
}

void Server::addrPrepare() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
}

void Server::bindSocketToAddr() {
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error setting socket option" << std::endl;
    }

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
    }
}

void Server::start() {
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Error listening" << std::endl;
    }

    std::cout << "Server started on port "<< port << std::endl;

    while (true) {
        // Принятие входящего соединения
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrLen);
        if (clientSocket < 0) {
            std::cerr << "Error accepting client" << std::endl;
            continue;
        }

        // Добавление клиента в список
        clientsMutex.lock();
        clients.push_back(clientSocket);
        clientsMutex.unlock();
        std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        // Запускаем новый поток для обработки клиента
        std::thread clientThread(&Server::handleClient, this, clientSocket);
        clientThread.detach(); // Отсоединяем поток от основного потока выполнения
    }
}

void Server::stop() const{
    close(serverSocket);
}

Server::~Server() {
    stop();
}

void Server::handleClient(int clientSocket) {
    int socket = clientSocket;
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            shutdown(socket, SHUT_RDWR);

            // Клиент отключился
            close(socket);
            clientsMutex.lock();
            clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
            clientsMutex.unlock();
            std::cout << "Client disconnected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            break;
            // Закрываем соединение с клиентом
        }
        std::string message(buffer, bytesReceived);

        // Обработка команды от клиента
        handleMessage(message, socket);
    }
}

void Server::handleMessage(const std::string &message, int socket) {
    std::cout << "Received message: " << message << std::endl;
    if (message.substr(0, BROADCAST.length() ) == BROADCAST) {
        // Отправка сообщения другим клиентам
        broadcastMessage(socket, message.substr(BROADCAST.length() + 1));
    } else if (message == CLIENTSNUMBER) {
        std::string response = "Number of connected clients: " + std::to_string(clients.size()) + "\n";
        send(socket, response.c_str(), response.length(), 0);
    } else {
        std::string response = createCharCountTable(message);
        send(socket, response.c_str(), response.length(), 0);
    }
}
