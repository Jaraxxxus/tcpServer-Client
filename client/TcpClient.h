//
// Created by sazonov99 on 3/25/24.
//

#ifndef CLIENT_TCPCLIENT_H
#define CLIENT_TCPCLIENT_H


#include <list>
#include <string>
#include <mutex>
#include <netinet/in.h>

class TcpClient {
private:
    std::list<std::string> messages;
    std::mutex msgMutex;
    int clientSocket{};
    struct sockaddr_in serverAddr{};
    std::string addr;
    int port;
public:
    void start();
    void stop() const;
    TcpClient(int port, std::string addr);
    ~TcpClient();
    void sendMessage(const std::string& message);

private:

    void createSocket();
    void setServer();
    bool Connect();

    void handleServer();
};


#endif //CLIENT_TCPCLIENT_H
