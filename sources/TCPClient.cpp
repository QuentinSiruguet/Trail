#include "TCPClient.h"


void TCPClient::sendFrameNewThread(std::string ipAddr, int port, std::string frame)
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
	{
		printf("socket creation failed...\n");
	}
	else
	{
		printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
		servaddr.sin_port = htons(port);

		if (connect(sockfd, (struct sockaddr*) & servaddr, sizeof(servaddr)) != 0) 
		{
			printf("connection with the server failed...\n");
		}
		else
		{
			printf("connected to the server..\n");
			write(sockfd, frame.c_str(), sizeof(frame));
			close(sockfd);
		}
	}
}

void TCPClient::sendFrame(std::string ipAddr,int port, std::string frame)
{
	std::thread sendingThread(&TCPClient::sendFrameNewThread, this, ipAddr, port, frame);
	sendingThread.detach();
}
