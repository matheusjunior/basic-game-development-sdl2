
//#define _WIN32_WINNT  0x501 
//#include <Ws2tcpip.h>

#include "telefone.hpp"

int Telefone::ligar(char *quem) {

	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)  goto saida;

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == Socket) goto saida;

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	if (getaddrinfo(quem, "8888", &hints, &saddress)) goto saida;

	struct in_addr addre;

	addre.S_un = ((struct sockaddr_in *)(saddress->ai_addr))->sin_addr.S_un;

	SockAddr.sin_port = htons(8888);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = addre.S_un.S_addr;

	if (0 != connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)))	goto saida;

	// Importante! diferente de zero = non-blocking.
	iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);

	return 1;

saida:
	WSACleanup();
	return 0;
}


int Telefone::esta_chamando(void) {

	SOCKADDR_IN serverInf;

	if (0 != WSAStartup(MAKEWORD(2, 2), &WsaDat))	goto saida;

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == Socket)	goto saida;


	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = INADDR_ANY;
	serverInf.sin_port = htons(8888);

	if (SOCKET_ERROR == bind(Socket, (SOCKADDR*)(&serverInf), sizeof (serverInf))) goto saida;

	listen(Socket, 1);

	TempSock = SOCKET_ERROR;

	// Olhe o telefone 5 vezes para saber se alguem esta ligando.
	int k;
	for (k = 0; (k < 5) && (SOCKET_ERROR == TempSock); ++k)
		TempSock = accept(Socket, NULL, NULL);

	if (SOCKET_ERROR == TempSock) goto saida;

	// Importante!
	// diferente de zero = non-blocking.
	iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);

	Socket = TempSock;

	iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);

	return 1;

saida:
	WSACleanup();
	return 0;
}


int Telefone::enviar(char c) {

	s[0] = c;
	s[1] = 0;

	send(Socket, s, 1, 0);

	return 0;
}

int Telefone::recebe() {

	int inDataLength = recv(Socket, s, 100, 0);
	if (inDataLength > 0) {
		return s[0];
	}
	else return 0;

};