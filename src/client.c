#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define CLT_PORT    2962            // Порт клиентов для приема
#define CLT_ADDR    "127.0.0.1"
#define GRP_ADDR    "239.0.0.34"    // Адрес multicast группы
#define MSG_MAXLEN  64              // Максимальная длина передаваемого или
                                    // получаемого сообщения

int main(int argc, void *argv[])
{
    int                 sock;           // Сокет клиента    
    struct sockaddr_in  addr_clt;       // Данные клиента
    struct sockaddr_in  addr_srv;       // Данные сервера
    struct ip_mreqn     mreq;           // Данные для регистрации в группе
    char                msg[MSG_MAXLEN];// Получаемое сообщение
    socklen_t           addr_size;      // Размер  struct sockaddr_in

    // Создаем сокет и именуем его
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("socket");
        exit(-1);
    }
    addr_size = sizeof(addr_clt);
    addr_clt.sin_family = AF_INET;
    addr_clt.sin_port = htons(CLT_PORT);
    addr_clt.sin_addr.s_addr = inet_addr(CLT_ADDR);
    if (bind(sock, (struct sockaddr *)&addr_clt, addr_size) == -1){
        close(sock);
        perror("bind");
        exit(-1);
    }

    // Регистрируемся в multicast группе
    //htonl(INADDR_ANY);
    //inet_addr(CLT_ADDR);
    mreq.imr_multiaddr.s_addr = inet_addr(GRP_ADDR);
    mreq.imr_address.s_addr = inet_addr(CLT_ADDR);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
	    perror("setsockopt");
	    exit(1);
    }

    // Ждем multicast сообщения
    while (1){
        if (recvfrom(sock, msg, MSG_MAXLEN, 0, (struct sockaddr *)&addr_srv, &addr_size) == -1){
            perror("recvfrom");            
            break;
        }
        if (!strcmp(msg, "disconnect"))
            break;
        printf("Received from server: %s\n", msg);
    }

    close(sock);
    exit(0);
}
