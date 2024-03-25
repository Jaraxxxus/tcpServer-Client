//
// Created by sazonov99 on 3/24/24.
//

#ifndef UNTITLED2_SERVER_H
#define UNTITLED2_SERVER_H

#include <vector>
#include <csignal>
#include <netinet/in.h>
#include <string>
#include <mutex>
#include <unordered_map>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

class Server {
private:
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    std::vector<int> clients;
    int port;
    std::mutex clientsMutex; // Мьютекс для безопасного доступа к списку клиентов
    std::unordered_map<int, struct sockaddr_in> clientAddrs; // Сохраняем адреса клиентов
public:
    Server(int port);
    ~Server();

    void start();
    void stop() const;
private:
    void handleMessage(const std::string& message, int clientSocket);

    void createSocket();

    void addrPrepare();

    void bindSocketToAddr();

    void broadcastMessage(int senderSocket, const std::string& message);

    void handleClient(int clientSocket);
};


#endif //UNTITLED2_SERVER_H
