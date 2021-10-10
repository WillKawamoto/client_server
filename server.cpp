#include <iostream>
#include <sys/socket.h> //for socket(), connect()
#include <netinet/in.h> //for scokaddr_in
#include <unistd.h> //for read()
#include <string.h> //strcpy()
#include <arpa/inet.h> //inet_ntop() (unnecessary function, just to see)

using namespace std;

string recv_client(int sockFd);

int main()
{
	int port = 8080;
	struct sockaddr_in server;
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
	//SOCK_STREAM and SOCK_DGRAM are options, SOCK_STREAM will be for TCP

	server.sin_family = AF_INET;
	server.sin_port = htons((short) port);
	memset(server.sin_zero, '\0', sizeof(server.sin_zero));
	//casting sockaddr_in* to sockaddr* is a normal thing and the docs are pretty
	//funny for it
	//https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
	server.sin_addr.s_addr = INADDR_ANY; //it will listen to everywhere pretty much
			//in terms of for IP at least (all available IPs for the machine)
	cout << "Address: " << server.sin_addr.s_addr << endl;
	char ip4[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &server.sin_addr, ip4, INET_ADDRSTRLEN);
	cout << "ip4: " << ip4 << endl;
	//.sin_addr.s_addr just gets the 32 bit IPv4 address
	//vs
	//inet_aton("127.0.0.1", &myaddr.sin_addr.s_addr);
		//this is just converting the ip address into the struct
		//inet_pton is the newer, better version
	//or inet_addr("127.0.0.1"); //obsolete, doesn't work with IPv6
	//which specifies the IP address
	if(bind(sockFd, (struct sockaddr *) &server, sizeof(server)) == -1)
	{
		cout << "COULDN'T BIND" << endl;
		exit(1);
	}
	int listeningQueue = 20;
	//listen() sets the socket to accept connections
	//also sets the max number of outstanding connections
	if(listen(sockFd, listeningQueue) == -1)
	{
		cout << "MUST BE DEAF" << endl;
		exit(1);
	}
	
	//gets the first connection from the listening queue, and
	//creates another socket which handles the connection, leaving
	//the original socket open to listen
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	cout << "sockFd: " << sockFd << endl;
	int connection;
	while(true)
	{
		connection = accept(sockFd, (struct sockaddr *) &client_addr, &client_addr_len);
		if(connection == -1)
		{
			cout << "Failed to connect" << endl;
			exit(1);
		}
		cout << "connection: " << connection << endl;
		char buffer[4096];
		strcpy(buffer, "Welcome to the server!\n");
		//the final param (0), is for flags, which we don't care about
		send(connection, buffer, 23, 0);// sends a message to the client
		cout << "CLIENT SAYS: " << endl;
		cout << recv_client(connection) << endl;
		//https://man7.org/linux/man-pages/man2/bind.2.html
		//This link is fantastic and provides code to get incoming connection
	}
}	 

string recv_client(int sockFd) {
	char buffer[4096];
	if(sockFd < 0)
	{
		cout << "NO SOCKET FOOL" << endl;
		exit(1);
	}
	//recv(connection, buffer, 4096, 0);
	//recv and read are the same except recv accepts flags as well
	int ret = recv(sockFd, buffer, sizeof(buffer), 0);
	//ret is the number of bytes read into the buffer
	if(ret < 0)
	{
		cout << "CONGRATS IT IS ILLITERATE" << endl;
	}
	return string(buffer, ret);
}
	
