// SimpleTcpClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <winsock2.h> // For socket functions
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <string>

using namespace std;
int main()
{
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}

	// Create a socket (IPv4, TCP)
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ConnectSocket == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(9999);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == -1) {
		std::cout << "Failed to connect socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	bool done = false;
	do
	{
		cout << "Enter String to send. {Null to terminate}:";
		string str;
		std::getline(std::cin, str);
		send(ConnectSocket, str.c_str(), str.length(), 0);
		if (str.length() == 0)
			done = true;
	} while (!done);

	iResult = closesocket(ConnectSocket);
	WSACleanup();
}

