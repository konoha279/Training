#include "Client.h"

void Client::Connect(char * ip, PCSTR PORT)
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		ExitProcess(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		ExitProcess(1);
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			ExitProcess(1);
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);
}

int Client::Send(char * sendbuf)
{
	return send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
}

int Client::Send(std::string sendbuf)
{
	return send(ConnectSocket, sendbuf.c_str(), (DWORD) sendbuf.length(), 0);
}

int Client::Shutdown()
{
	return shutdown(ConnectSocket, SD_SEND);
}

int Client::receive(char * text)
{
	return recv(ConnectSocket, text, DEFAULT_BUFLEN, 0);
}

int Client::receive(std::string & text)
{
	char *tmpC;
	if (!recv(ConnectSocket, tmpC, DEFAULT_BUFLEN, 0))
		return 0;
	text = tmpC;
	return 1;
}

bool Client::isConnected()
{
	if (ConnectSocket == INVALID_SOCKET) {
		return false;
	}
	return true;
}
