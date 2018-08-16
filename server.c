#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int validate(int argc, char const *argv[]){
    if(argc != 3){
        printf("Error de parametros: <Protocolo> <Puerto>\n");
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

    char protocol[50];
    char port[50];
    strcpy(protocol,argv[1]);
    strcpy(port,argv[2]);

    struct sockaddr_in client,server;
    int server_conn, client_conn;
    int opt = 1;
    char buffer_recv[1024];
    char buffer_send[1024];

    memset(&server, '0', sizeof(server)); 
    server.sin_family= AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = INADDR_ANY;
    
    if(strcmp(protocol,"TCP") == 0){
        if ((server_conn = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket ha fallado");
            exit(EXIT_FAILURE);
        }
        
        // Se cambia el modo del socket para que pueda volver a utilizarse
        if (setsockopt(server_conn, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            perror("setsockopt ha fallado");
            exit(EXIT_FAILURE);
        }
        
        //  Se dispone el socket en el puerto que se ha configurado
        if (bind(server_conn, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("bind ha fallado");
            exit(EXIT_FAILURE);
        }
        
        // Se pone a escuchar el servidor
        if (listen(server_conn, 3) < 0)
        {
            perror("listen ha fallado");
            exit(EXIT_FAILURE);
        }
        // Al aceptar una conexion se crea un nuevo socket 
        int long_client= sizeof(struct sockaddr_in);
        if ((client_conn = accept(server_conn, (struct sockaddr *)&server,&long_client ))<0)
        {
            perror("accept ha fallado");
            exit(EXIT_FAILURE);
        }
        
        // Se recibe un mensaje desde el cliente
        printf("Esperando mensaje...\n");
        memset(buffer_recv, '\0', 1024);
        recv( client_conn , buffer_recv, 1024, 0);
        printf("Mensaje recibido: %s\n",buffer_recv );
        
        // Se envia una respuesta al cliente
        memset(buffer_send, '\0', 1024);
        printf("Enviar respuesta: ");
        scanf("%s",buffer_send);
        send(client_conn , buffer_send , 1024 , 0 );
        printf("Mensaje enviado : %s\n",buffer_send);
    }
    else if(strcmp(protocol,"UDP") == 0){
        if ( (server_conn = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket ha fallado");
            exit(EXIT_FAILURE);
        }     
        
        // Se cambia el modo del socket para que pueda volver a utilizarse
        if (setsockopt(server_conn, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            perror("setsockopt ha fallado");
            exit(EXIT_FAILURE);
        }
        
        //  Se dispone el socket en el puerto que se ha configurado
        if (bind(server_conn, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("bind ha fallado");
            exit(EXIT_FAILURE);
        }

        printf("Esperando mensaje...\n");
        int long_client= sizeof(struct sockaddr_in);
        memset(buffer_recv, '\0', 1024);
        recvfrom(server_conn,buffer_recv,1024,0,(struct sockaddr *)&client,(socklen_t *)&long_client);
        printf("Mensaje recibido: %s\n",buffer_recv );

        memset(buffer_send, '\0', 1024); 
        printf("Enviar respuesta: ");
        scanf("%s",buffer_send);
        sendto(server_conn, buffer_send,strlen(buffer_send) , 0, (struct sockaddr*)&client, (socklen_t) long_client);
        printf("Mensaje enviado : %s\n",buffer_send);


    }

   
    return 0;
}
