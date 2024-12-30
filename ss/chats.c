#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include "chat.h"

#define DEBUG

#define MAX_CLIENT 5
#define MAX_ID 32
#define MAX_BUF 256

// 클라이언트 정보를 저장하는 구조체
typedef struct {
    int sockfd;
    int inUse;
    char uid[MAX_ID];
} ClientType;

int Sockfd;
ClientType Client[MAX_CLIENT];
fd_set readfds, allfds;

// 사용 가능한 클라이언트 ID를 찾는 함수
int GetID() {
    int i;
    for (i = 0; i < MAX_CLIENT; i++) {
        if (!Client[i].inUse) {
            Client[i].inUse = 1;
            return i;
        }
    }
    return -1; // 모든 슬롯이 사용 중일 경우
}

// 다른 모든 클라이언트에게 메시지를 보내는 함수
void SendToOtherClients(int id, char *buf) {
    int i;
    char msg[MAX_BUF+MAX_ID];

    sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef DEBUG
    printf("%s", msg);
    fflush(stdout);
#endif

    for (i = 0; i < MAX_CLIENT; i++) {
        if (Client[i].inUse && (i != id)) {
            if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0) {
                perror("send");
                exit(1);
            }
        }
    }
}

// 클라이언트 요청을 처리하는 함수
void ProcessClient(int id) {
    char buf[MAX_BUF];
    int n;

    // 클라이언트 ID 수신
    if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0) {
        perror("recv");
        exit(1);
    }
    printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);

    // 클라이언트로부터 메시지 수신 및 처리
    if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0) {
        perror("recv");
        exit(1);
    }
    if (n == 0) {
        printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);
        close(Client[id].sockfd);
        Client[id].inUse = 0;
        FD_CLR(Client[id].sockfd, &allfds);
        strcpy(buf, "log-out.....\n");
        SendToOtherClients(id, buf);
    } else {
        SendToOtherClients(id, buf);
    }
}

// 서버를 종료하는 함수
void CloseServer(int signo) {
    int i;

    close(Sockfd);

    for (i = 0; i < MAX_CLIENT; i++) {
        if (Client[i].inUse) {
            close(Client[i].sockfd);
        }
    }

    printf("\nChat server terminated.....\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    int newSockfd, cliAddrLen, id, maxfd, i;
    struct sockaddr_in cliAddr, servAddr;
    int one = 1;

    signal(SIGINT, CloseServer);

    // 소켓 생성
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 소켓 옵션 설정 (주소 재사용)
    if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    // 서버 주소 구조체 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 소켓에 주소 바인딩
    if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 연결 대기열 생성
    listen(Sockfd, 5);

    printf("Chat server started.....\n");

    // fd_set 초기화
    FD_ZERO(&readfds);
    FD_ZERO(&allfds);
    FD_SET(Sockfd, &allfds);
    maxfd = Sockfd;

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        readfds = allfds;
        // select 함수를 사용하여 I/O 멀티플렉싱
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        // 새로운 연결 요청 처리
        if (FD_ISSET(Sockfd, &readfds)) {
            newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
            if (newSockfd < 0) {
                perror("accept");
                exit(1);
            }

            id = GetID();
            if (id < 0) {
                printf("Client capacity full\n");
                close(newSockfd);
            } else {
                Client[id].sockfd = newSockfd;
                FD_SET(newSockfd, &allfds);
                if (newSockfd > maxfd)
                    maxfd = newSockfd;
            }
        }

        // 기존 클라이언트의 요청 처리
        for (i = 0; i < MAX_CLIENT; i++) {
            if (Client[i].inUse && FD_ISSET(Client[i].sockfd, &readfds)) {
                ProcessClient(i);
            }
        }
    }
}
