#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h> //unix standards

#define PORTA 8080

/* conectando dois nós através de uma rede
 * para que possam trocar dados definidos
 * por estrutras.
 * SERVIDOR */

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
	int servidor_fd; //descritor do socket <- socket()
	int novo_socket;
	int valread;

	struct sockaddr_in endereco;

	int opt = 1;
	int addrlen = sizeof( endereco );

	char buffer[ 1024 ] = { 0 };

	char* ola = "OI MEU CHAPA\n";

	/* CRIANDO DESCRITOR DO SOCKET socket( dominio, tipo, protocolo )*/
	if ( ( servidor_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
	{
		erro( "SOCKET FALHOU => socket()\n" );
	}

	// acoplando socket à porta 8080		OPÇÕES
	if ( setsockopt( servidor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) )
	{
		erro( "SOCKET FALHOU => setsockopt()\n" );
	}

	endereco.sin_family = AF_INET;
	endereco.sin_addr.s_addr = INADDR_ANY;
	endereco.sin_port = htons( PORTA );

	if ( bind( servidor_fd, (struct sockaddr*)&endereco, sizeof(endereco) ) < 0 )
	{
		erro( "SOCKET FALHOU => bind()\n" );
	}
	if ( listen( servidor_fd, 3 ) < 0 )
	{
		erro( "SOCKET FALHOU => listen()\n" );
	}
	if ( ( novo_socket = accept( servidor_fd, (struct sockaddr*)&endereco, (socklen_t*)&addrlen )) < 0 )
	{
		erro( "SOCKET FALHOU => accept()\n" );
	}

	valread = read( novo_socket, buffer, 1024 );
	printf( "%s\n", buffer );

	send( novo_socket, ola, strlen(ola), 0 );
	printf( "SERVER LOG: Mensagem de olá foi enviada." );

	close( novo_socket );
	shutdown( servidor_fd, SHUT_RDWR );

	exit(0);
}
