#pragma once
#include <unordered_set>
using namespace std;

class SimpleTcpServerReader
{
public:
	bool addEndPoint(int fd);
	bool removeEndPoint(int fd);
	void run();
private:
	//static unordered_set <int> connection_hash_set;
};