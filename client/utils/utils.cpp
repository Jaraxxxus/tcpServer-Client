//
// Created by sazonov99 on 3/25/24.
//

#include "utils.h"
#include <iostream>

void inputHandlerThread(TcpClient &client) {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        std::cout << "\n";
        client.sendMessage(input);
    }
}