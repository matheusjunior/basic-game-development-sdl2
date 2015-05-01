#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <iostream>

using namespace std;


class Server {
public:
	WSADATA WsaDat;
	SOCKET  Socket;
	SOCKET  TempSock;
	char    s[300];
	u_long  iMode;
	int     k;
	bool    exit;

	SOCKADDR_IN serverInf;

	void run();

	void gotoxy(int x, int y);

};

void Server::run() {
	cout << "Server is on!";

	if (0 != WSAStartup(MAKEWORD(2, 2), &WsaDat))	{
		cout << "lascou1";
		exit = true;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == Socket)	{
		cout << "lascou2";
		exit = true;
	}

	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = INADDR_ANY;
	serverInf.sin_port = htons(8888);

	if (SOCKET_ERROR == bind(Socket, (SOCKADDR*)(&serverInf), sizeof (serverInf)))	{
		cout << "lascou3";
		exit = true;
	}

	listen(Socket, 1);
	TempSock = SOCKET_ERROR;
	//system("cls");

	if (exit) return;

	cout << "running";

	while (SOCKET_ERROR == TempSock)	{
		cout << "zZz  (esperando alguem dar alo)";
		TempSock = accept(Socket, NULL, NULL);
	}


	// Importante!
	// diferente de zero = non-blocking.
	iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);

	Socket = TempSock;

	puts("Estamos conectados");

	strcpy_s(s, "Saudacoes, estranho");
	send(Socket, s, strlen(s), 0);

	k = 0;
	while (1) {

		gotoxy(10, 9);
		printf("Digite>");
		gets_s(s);

		// servidor manda mensagem (sem bloquear)
		send(Socket, s, strlen(s), 0);

		// e servidor testa acknowledge (tambem sem bloquear)
		// -> talvez o ack demore e so seja obtido no proximo loop.
		int inDataLength = recv(Socket, s, 100, 0);
		if (inDataLength > 0) {
			gotoxy(10, 10);
			printf("%d", ++k);
		}
	}


	shutdown(Socket, SD_SEND);
	closesocket(Socket);
}

void Server::gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
