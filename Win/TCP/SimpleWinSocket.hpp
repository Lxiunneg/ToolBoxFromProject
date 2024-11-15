// SimpleWinSocket.hpp
// Created on: 2024-11-15
// Author: Liujie
// Version: 1.0
//
// Description:
// This file contains the declaration of the SimpleWinSocket class,
// which provides a simple and easy-to-use interface for network communication
// using sockets in a Windows environment.
//
// Copyright (c) 2023 Your Company. All rights reserved.


#include <winsock2.h>    // Winsock API 
#include <ws2tcpip.h>    // 
#pragma comment(lib, "Ws2_32.lib") // link to Winsock lib

#include <string>
#include <thread>

/// <summary>
/// Callback interface format
/// </summary>
using Callback = void(*)(char* buffer, int len);

/// <summary>
/// The SimpleWinSocket class is a wrapper of sockets in the Windows environment
/// You can send and receive data on the port number of a specified IP
/// The data acceptance method provides an external callback function interface
/// </summary>
class SimpleWinSocket {
private:
	WSADATA wsaData; // create WSADATA strcuture,for storage Winsock data
	SOCKET ConnectSocket; // socket
	sockaddr_in clientService; // create sockaddr_in strcuture，for storage server data

	short protocol; // use which protocol? IPv4 or IPv6 

	std::string ip_addr; // server IP address 
	int port; // server process port
public:
	SimpleWinSocket(std::string ip_addr, int port, short protocol);
	~SimpleWinSocket();

	void InitSocket(std::string ip_addr, int port, short protocol);

	void ConnectToServer(int timeout);
	void SendToServer(char* sendBuffer, int len, int time);
	void RecvFromServer(char* recvBuffer, int len, Callback callback);
	void Disconnect();
};

/// <summary>
/// The default constructor of SimpleWinSocket class
/// </summary>
/// <param name="ip_addr">Server IP,default = 127.0.0.1 </param>
/// <param name="port">Server process port,default = 8080 </param>
/// <param name="protocol">Which protocol,default AF_INET,IPv4 </param>
SimpleWinSocket::SimpleWinSocket(std::string ip_addr = "127.0.0.1", int port = 8080, short protocol = AF_INET) {
	this->InitSocket(ip_addr, port, protocol);
}

/// <summary>
/// The destructor of SimpleWinSocket
/// </summary>
SimpleWinSocket::~SimpleWinSocket() {
	this->Disconnect();
}

/// <summary>
/// The init. funtion of SimpleWinSocket
/// </summary>
/// <param name="ip_addr">Server IP</param>
/// <param name="port">Server process port </param>
/// <param name="protocol">Which protocol,default AF_INET,IPv4 </param>
void SimpleWinSocket::InitSocket(std::string ip_addr, int port, short protocol = AF_INET) {
	this->ip_addr = ip_addr;
	this->port = port;

	this->protocol = protocol;

	this->clientService.sin_family = protocol;
	this->clientService.sin_port = htons(this->port);

	int result = WSAStartup(MAKEWORD(2, 2), &(this->wsaData)); // initializate Winsock lib
	if (result != 0) {              // check if initialization is successful 
		printf("Win Socket error: %d\n", result);
		exit(0);
	}

	this->ConnectSocket = socket(protocol, SOCK_STREAM, IPPROTO_TCP);
	if (this->ConnectSocket == INVALID_SOCKET) { // check if create socket is successful
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup(); // clean Winsock
		exit(0);
	}

	// use inet_pton() transfor IP string to binary code
	if (inet_pton(this->protocol, this->ip_addr.c_str(), &(clientService.sin_addr)) <= 0) {
		printf("inet_pton failed: %ld\n", WSAGetLastError());
		closesocket(this->ConnectSocket); // close socket
		WSACleanup(); // clean Winsock
		exit(0);
	}

	printf("WinSocket initializate successfully! (ip:%s:%d)\n", this->ip_addr.c_str(), this->port);
}

/// <summary>
/// The connect function of SimpleWinSocket class,can use timeout param to control connect time
/// </summary>
/// <param name="time">control connect time</param>
void SimpleWinSocket::ConnectToServer(int time = 0) {
	int result = connect(this->ConnectSocket, (SOCKADDR*)&(this->clientService), sizeof(this->clientService)); // try to connect 
	if (result == SOCKET_ERROR) {   // check if client is connect successful
		printf("connect error ,error code: %ld\n", WSAGetLastError());
		closesocket(this->ConnectSocket); // close socket
		WSACleanup(); // clean Winsock
		exit(0);
	}

	printf("Have connected -----> (%s:%d) .\n", this->ip_addr.c_str(), this->port);
}

/// <summary>
/// The send function of SimpleWinSocket Class,Send data to Server
/// Example:
/// 1. Send data is char example[Length] ---> SimpleWinSocket::SendToServer(example,Lenth);
/// 2. Send data is a structure,struct Example  ---> SimpleWinSocket::SendToServer((char*)&(Example),sizeof(Example));
/// 3. Regularly send data,2s --> SimpleWinSocket::SendToServer(example,Lenth,2);
/// Ps:Be careful the byte alignment problem,you can use `#pramg pack(1)` to avoid.
/// </summary>
/// <param name="sendBuffer">which data you want to send to server</param>
/// <param name="len">the length of data</param>
/// <param name="time">regularly send,default = 0</param>
void SimpleWinSocket::SendToServer(char* sendBuffer, int len, int time = 0) {
	std::this_thread::sleep_for(std::chrono::seconds(time));

	int bytesSent = send(this->ConnectSocket, sendBuffer, len, 0); // send data
	if (bytesSent == SOCKET_ERROR) { // check data if send successful
		printf("Send failure ,error code: %ld\n", WSAGetLastError());
		closesocket(ConnectSocket); // close socket
		WSACleanup(); // clean Winsock
		exit(0);
	}
	else {
		printf("The length of sended data: %ld\n", bytesSent);
	}
}

/// <summary>
/// The receiving data function of SimpleWinSocket class, receiving data from server
/// Example:
/// char recvBuf[1024];
/// void ExampleCallback(char* buf,int len);
/// SimpleWinSocket::RecvFromServer(recvBuf, 1024, ExampleCallback);
/// </summary>
/// <param name="recvBuffer">the buffer of data</param>
/// <param name="len">the lenght of buffer</param>
/// <param name="callback">the callback function,default = nullptr</param>
void SimpleWinSocket::RecvFromServer(char* recvBuffer, int len = 1024, Callback callback = nullptr) {

	printf("Waiting server send data ...\n");
	int bytesReceived = recv(ConnectSocket, recvBuffer, len, 0); // recving data
	if (bytesReceived > 0) {
		printf("Receive:%d\ndata: ", bytesReceived);
		if (callback != nullptr)
			callback(recvBuffer, bytesReceived);
		else {
			for (int i = 0; i < bytesReceived; i++)
				printf("0x%02X ", recvBuffer[i]);
		}
		printf("\n");
	}
	else if (bytesReceived == 0) {
		printf("Server connection is close.\n");
	}
	else {
		printf("Receive failure,error code: %ld\n", WSAGetLastError());
	}
}

/// <summary>
/// The disconnect function of SimpleWinSocket class
/// </summary>
void SimpleWinSocket::Disconnect() {
	// close
	closesocket(this->ConnectSocket);

	// clean Winsock
	WSACleanup();

	printf("Have disconnect ---/--> (%s:%d) .", this->ip_addr.c_str(), this->port);
}