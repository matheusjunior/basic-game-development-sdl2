
#define _WIN32_WINNT  0x501 
#include <Ws2tcpip.h>


class Telefone {
public:

	int         k;
	WSADATA     WsaDat;
	struct      hostent *host;
	struct      addrinfo *saddress;
	SOCKET      Socket, TempSock;
	SOCKADDR_IN SockAddr;
	u_long      iMode;

	char        s[1000];

	Telefone() { ; };
	~Telefone() { ; };

	int ligar(char *);
	int esta_chamando(void);
	int enviar(char);
	int recebe();
};