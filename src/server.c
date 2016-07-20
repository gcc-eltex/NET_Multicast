#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define CLT_PORT    2962            // Порт клиентов для приема
#define GRP_ADDR    "239.0.0.34"    // Адрес multicast группы
#define MSG_MAXLEN  64              // Максимальная длина передаваемого или
                                    // получаемого сообщения

int main(int argc, void *argv[])
{
    int                 sock;           // Сокет сервера    
    struct sockaddr_in  addr_grp;       // Данные для отправки в multicast
    char                msg[MSG_MAXLEN];// Отправляемое / получаемое сообщение

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("socket");
        exit(-1);
    }
    /*
     * Поскольку идет multicast передача, то адрес и порт сервера не имеет 
     * значения. Необходимо только знать порт приема и адрес группы
     */
    addr_grp.sin_family = AF_INET;
    addr_grp.sin_port = htons(CLT_PORT);
    addr_grp.sin_addr.s_addr = inet_addr(GRP_ADDR);

    // Считываем сообщения и отправляем для multicast группы
    while (1){
        printf("Send to clients: ");
        scanf("%s", msg);
        sendto(sock, msg, strlen(msg) + 1, 0, (struct sockaddr *)&addr_grp, sizeof(addr_grp));
        if (!strcmp(msg, "disconnect"))
            break;
    }

    close(sock);
    exit(0);
}
