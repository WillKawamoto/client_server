#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

int main()
{
	int fd;
	int port = 8080;
	//char ip[] = "127.0.0.1";
	struct sockaddr_in server;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
	{
		cout << "CLIENT SOCKET FAILED" << endl;
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons((short) port);
	server.sin_addr.s_addr = INADDR_ANY;
	cout << "Socket created!" << endl;

	//connects client to the address in server
	if( connect(fd, (struct sockaddr*) &server, sizeof(server)) == -1)
	{
		cout << "FAILED TO CONNECT" << endl;
		exit(1);
	}
	cout << "CONNECTED!" << endl;
	int buffSize = 4096;
	char buffer[buffSize];
	int ret = read(fd, buffer, buffSize);
	if(ret <= 0)
	{
		cout << "WOW CLIENT CAN'T EVEN READ" << endl;
	}
	cout << string(buffer, ret) << endl;
	strcpy(buffer, "HI SERVER!\n");
	send(fd, buffer, 11, 0);
}
	
