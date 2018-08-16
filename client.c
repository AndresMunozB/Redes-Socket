/* 
    Autor: Andrés Muñoz
    Control 4 Redes de Computadores.
    Cliente.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int validate(int argc, char const *argv[]){
    if(argc != 4){
        printf("Error de parametros: <Protocolo> <Host> <Puerto>\n");
        return 0;
    }
    else if(strcmp(argv[1],"UDP") != 0 && strcmp(argv[1],"TCP") != 0 ){
        printf("El protocolo ingresado es invalido\n");
        return 0;
    }
    return 1;
}
int main(int argc, char const *argv[])
{
    if(!validate(argc,argv)){
        return -1;
    }
    char protocol[20];
    char host[50];
    char port[50];
    strcpy(protocol,argv[1]);
    strcpy(host,argv[2]);
    strcpy(port,argv[3]);



    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer_recv[1024];
    char buffer_send[1024];

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port));
    // Convertir direccion IPv4 and IPv6  a texto de forma binaria
    if(inet_pton(AF_INET, host, &serv_addr.sin_addr)<=0) 
    {
        printf("\n Address invalida/Address no soportada \n");
        return -1;
    }

    if(strcmp(protocol,"TCP") == 0){
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n socket ha fallado\n");
            return -1;
        }
    
        //Se crea una conexion al servidor
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\n connect ha fallado \n");
            return -1;
        }
        
        // Se envia un mensaje al servidor
        memset(buffer_send, '\0', 1024); 
        printf("Enviar mensaje  : ");
        scanf("%s",buffer_send);
        send(sock , buffer_send , strlen(buffer_send) , 0 );
        printf("Mensaje enviado : %s\n", buffer_send);
        

        printf("Esperando respuesta...\n");
        // Se recibe una respuesta del servidor
        memset(buffer_recv, '\0', 1024);
        recv( sock , buffer_recv, 1024, 0);
        printf("Mensaje recibido: %s\n",buffer_recv );
    }
    else if(strcmp(protocol,"UDP") == 0){
        if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket ha fallado");
            exit(EXIT_FAILURE);
        }
        int slen = sizeof(serv_addr);
        // Se envia un mensaje al servidor
        memset(buffer_send, '\0', 1024); 
        printf("Enviar mensaje  : ");
        scanf("%s",buffer_send);
        sendto(sock, buffer_send, strlen(buffer_send) , 0 , (struct sockaddr *) &serv_addr,slen );
        printf("Mensaje enviado : %s\n", buffer_send);

         
        // Se recibe una respuesta del servidor
        printf("Esperando respuesta...\n");
        memset(buffer_recv, '\0', 1024);
        recvfrom(sock, buffer_recv, 1024, 0, (struct sockaddr *) &serv_addr, &slen);
        printf("Mensaje recibido: %s\n",buffer_recv );


         
    }
    
   
    
    return 0;
}
