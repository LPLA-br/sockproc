#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> //unix standards

#define PORTA 8080

/* conectando dois nós através de uma rede
 * para que possam trocar dados definidos
 * por estrutras.
 * CLIENTE */

typedef struct
{
	int8_t x;
	int8_t y;
}
Posicao;

void erro( char* msg )
{
	perror( msg );
	exit( EXIT_FAILURE );
}

int main( int argc, char const* argv[] )
{
	int status;
	int valread;
	int cliente_fd;

	struct sockaddr_in servidor_addr;

	char* ola = "OI MEU CHAPA SERVIDOR\n";

	char buffer[1024] = { 0 };

	if ( (cliente_fd = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		erro( "socket()\n" );
	}

	servidor_addr.sin_family = AF_INET;
	servidor_addr.sin_port = htons( PORTA );

	// converter endereços IPV4 e IPV6 de texto para binário
	if ( inet_pton( AF_INET, "127.0.0.1", &servidor_addr.sin_addr ) <= 0 )
	{
		printf( "ENDEREÇO INVÁLIDO. OTARIO!" );
		return -1;
	}

	if ( (status = connect( cliente_fd, (struct sockaddr*)&servidor_addr, sizeof(servidor_addr)) < 0 ) )
	{
		printf( "CONEXÃO COM O SERVIDOR FALHOU. OTARIO" );
		return -1;
	}
	send( cliente_fd, ola, strlen( ola ), 0 );
	printf( "mensagem enviada\n" );
	valread = read( cliente_fd, buffer, 1024 );
	printf( "RECEBIDO DO SERVIDOR: %s", buffer );

	close( cliente_fd );
	exit(0);
}
