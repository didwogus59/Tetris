#pragma once
#include<WinSock2.h>
#include<ws2tcpip.h>

#include <iostream>
#pragma comment(lib, "ws2_32")

#define PORT 4578
#define PACKET_SIZE 2048
class multi_host {

	WSADATA wsaDATA;
	SOCKET hListen;
	SOCKADDR_IN tListenAddr = {};
	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient;
public:
	multi_host() {
		WSAStartup(MAKEWORD(2, 2), &wsaDATA);

		hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		tListenAddr.sin_family = AF_INET;
		tListenAddr.sin_port = htons(PORT);
		tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	}

	void bind_socket() {
		bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	}

	void listen_client() {
		listen(hListen, SOMAXCONN);
		std::cout << "connected!\n";
	}
	
	void accept_request() {
		hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);
	}
	void communication(int** mybase, int** yourbase, int map_x, int map_y) {
		char cBuffer[PACKET_SIZE] = {};
		char cMsg[PACKET_SIZE] = {};
		int len = 0;
		recv(hClient, cBuffer, PACKET_SIZE, 0);
		for (int i = 0; i < map_x; i++) {
			for (int j = 0; j < map_y; j++) {
				yourbase[i][j] = cBuffer[len];
				cMsg[len] = mybase[i][j];
				len++;
			}
		}
		send(hClient, cMsg, len, 0);
	}

	void close_socket() {
		closesocket(hClient);
		closesocket(hListen);
		WSACleanup();
	}
};

class client {

	WSADATA wsaDATA;
	SOCKET hSocket;
	SOCKADDR_IN tAddr = {};
	PCSTR SERVER_IP;
public:
	client(char* ip_addr) {
		SERVER_IP = ip_addr;
		WSAStartup(MAKEWORD(2, 2), &wsaDATA);
		hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		tAddr.sin_family = AF_INET;
		tAddr.sin_port = htons(PORT);
		inet_pton(AF_INET, SERVER_IP, &(tAddr.sin_addr.s_addr));
	}

	void connect_server() {
		connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	}

	void communicate_server(int** mybase, int** yourbase, int map_x, int map_y) {
		char cMsg[PACKET_SIZE] = {};
		char cBuffer[PACKET_SIZE] = {};
		int len = 0;
		for (int i = 0; i < map_x; i++) {
			for (int j = 0; j < map_y; j++) {
				cMsg[len] = mybase[i][j];
				len++;
			}
		}
		send(hSocket, cMsg, len, 0);

		recv(hSocket, cBuffer, PACKET_SIZE, 0);

		len = 0;
		for (int i = 0; i < map_x; i++) {
			for (int j = 0; j < map_y; j++) {
				yourbase[i][j] = cBuffer[len];
				len++;
			}
		}
	}

	void close_socket() {
		closesocket(hSocket);
		WSACleanup();
	}
};