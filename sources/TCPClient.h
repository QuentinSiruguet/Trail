#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#include "iostream"
#include <thread>

class TCPClient
{
private:
	void sendFrameNewThread(std::string ipAddr, int port, std::string frame);

public:
	void sendFrame(std::string ipAddr,int port, std::string frame);
};

