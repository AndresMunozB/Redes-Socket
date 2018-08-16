#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 3000
  
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *message = "Hola Mundo! (Cliente)";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n socket ha fallado\n");
        return -1;
    }
  
	
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convertir direccion IPv4 and IPv6  a texto de forma binaria
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\n Address invalida/ Address no soportada \n");
        return -1;
    }
    
	//Se crea una concexion al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n connect ha fallado \n");
        return -1;
    }
    
    // Se envia un mensaje al servidor
    send(sock , message , strlen(message) , 0 );
    printf("Mensaje enviado : %s\n", message);
    
    // Se recibe una respuesta del servidor
    valread = recv( sock , buffer, 1024, 0);
    printf("Mensaje recibido: %s\n",buffer );
    
    return 0;
}
