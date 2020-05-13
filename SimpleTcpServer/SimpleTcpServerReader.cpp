#include "SimpleTcpServerReader.h"
#include <winsock2.h>
#include <iostream>

using namespace std;
unordered_set <int> connection_hash_set;
auto SimpleTcpServerReader::addEndPoint(int fd)->bool
{
	bool status = true;
	if (connection_hash_set.count(fd) == 0)
	{
		connection_hash_set.insert(fd);
	}
	else
	{
		status = false;
	}
	return status;
}
auto SimpleTcpServerReader::removeEndPoint(int fd)->bool
{
	bool status = true;
	if (connection_hash_set.count(fd) == 1)
	{
		connection_hash_set.erase (fd);
	}
	else
	{
		status = false;
	}
	return status;
}

auto SimpleTcpServerReader::run() -> void
{
	// initial check till first socket is available
	do 
	{
		fd_set read_fd_set;
		FD_ZERO(&read_fd_set);
		for (auto w : connection_hash_set)
		{
			FD_SET(w,&read_fd_set);
		}
		TIMEVAL timeout;
		timeout.tv_sec = 60;
		auto numset = select(0, &read_fd_set, nullptr, nullptr, &timeout);
		if (numset <= 0)
			continue;
		for (auto w : connection_hash_set)
		{
			if (FD_ISSET(w, &read_fd_set))
			{
				// Read from the connection
				string incoming;
				vector <char> data(4024);
				auto bytesRead = recv(w, &data[0], sizeof(data), 0);
				if (bytesRead != -1)
				{
					incoming.append(data.begin(), data.end());
					cout << w << " : "  << incoming.c_str() << endl;
				}
			}
		}
	} while (1);

}
