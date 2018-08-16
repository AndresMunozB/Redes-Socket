#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 3000
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *message = "Hola Mundo! (Servidor)";
      
    // creando el socket 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket ha fallado");
        exit(EXIT_FAILURE);
    }
      
    // Se cambia el modo del socket para que pueda volver a utilizarse
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt ha fallado");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    //  Se dispone el socket en el puerto que se ha configurado
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind ha fallado");
        exit(EXIT_FAILURE);
    }
    
    // Se pone a escuchar el servidor
    if (listen(server_fd, 3) < 0)
    {
        perror("listen ha fallado");
        exit(EXIT_FAILURE);
    }
    // Al aceptar una conexion se crea un nuevo socket 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("accept ha fallado");
        exit(EXIT_FAILURE);
    }
    
    // Se recibe un mensaje desde el cliente
    valread = recv( new_socket , buffer, 1024, 0);
    printf("Mensaje recibido: %s\n",buffer );
    
    // Se envia una respuesta al cliente
    send(new_socket , message , strlen(message) , 0 );
    printf("Mensaje enviado : %s\n",message);
    close(new_socket);
    close(server_fd);
    return 0;
}
