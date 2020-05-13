// Socket_Example.cpp : This project implements simple TCP Server and client 
//

#include <winsock2.h> // For socket functions
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <string>
#include <vector>
#include <unordered_set>
#include <thread>
#include "SimpleTcpServerReader.h"
using namespace std;
int main(int argc, char* argv[])
{
	// Create Two Threads
	// Main thread handles connection
	// Second thread reads data
	// Third thread sends data

	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}

	// Create a socket (IPv4, TCP)
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen to port 9999 on any address
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
									 // network byte order
	if (bind(sockfd, (struct sockaddr*) & sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Start listening. Hold at most 10 connections in the queue
	if (listen(sockfd, 10) < 0) {
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	SimpleTcpServerReader reader;
	// Grab a connection from the queue
	int addrlen = sizeof(sockaddr);
	std::thread::id read_thread_id;
	bool thread_created = false;
	std::thread t1;
	do
	{
		int connection = accept(sockfd, (struct sockaddr*) & sockaddr, &addrlen);
		if (connection < 0) {
			std::cout << "Failed to grab connection. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		reader.addEndPoint(connection);
		if (!thread_created)
		{
			t1 = std::thread(&SimpleTcpServerReader::run, reader);
			read_thread_id = t1.get_id();
			thread_created = true;
		}

	} while (1);
	
	// Close the connections
	//closesocket(connection);
	closesocket(sockfd);
}

