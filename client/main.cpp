#include <iostream>
#include <thread>
#include "TcpClient.h"
#include "utils/utils.h"




int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        //return 1;
    }
    int port = std::atoi(argv[2]);
    std::string ipAddress = argv[1];

    //TcpClient client( 7070, "127.0.0.1" );
    TcpClient client( port, ipAddress );

    std::thread th(inputHandlerThread, std::ref(client));

    client.start();

    th.join();
    return 0;
}

