# TCP/IP Server and Client Application

## Description:
This is a simple TCP/IP server and client application designed to handle communication between clients and the server. The server listens for incoming connections on port 7070 and responds to various requests from the client. The primary functionality includes parsing messages sent by the client and returning a table containing the count of Latin letters only. Additionally, the client can request information such as --clientsCount or --broadcast from the server.

## Usage:
1. **Server**: The server is a basic TCP/IP server hardcoded to listen on port 7070. It waits for incoming connections from clients and handles their requests.

2. **Client**: The client interacts with the server by sending messages and receiving responses. It can send messages to be parsed by the server and request specific information such as the count of Latin letters in a message, the count of connected clients, or broadcast messages to all connected clients.

## Features:
- **TCP/IP Communication**: Utilizes TCP/IP protocol for communication between server and client.
- **Message Parsing**: The server parses messages received from clients and returns a table containing the count of Latin letters only.
- **Client Requests**: The client can request information such as the count of connected clients or broadcast messages to all clients.
- **Hardcoded Configuration**: The server is hardcoded to listen on port 7070 for simplicity.

## Getting Started:
To use this application, follow these steps:

1. **Clone Repository**: Clone this repository to your local machine.
2. **Compile**: Compile the server and client source files.
3. **Run Server**: Execute the server binary, which will start listening on port 7070.
4. **Run Client**: Run the client binary to establish a connection with the server and start sending requests.

## Example Usage:
1. --clientsCount
2. --broadcast Attention: Meeting in 30 minutes!
