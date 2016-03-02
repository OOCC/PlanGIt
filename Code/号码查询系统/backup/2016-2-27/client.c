/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2014年12月10日 星期三 14时31分54秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 1024
#define MNC_LEN 16    

int main(int argc, const char *argv[])
{

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = PF_INET;
    client_addr.sin_port = htons(8888);
    inet_aton(argv[1],&client_addr.sin_addr);


    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(9999);
    inet_aton(argv[1],&servaddr.sin_addr);

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
	perror("connect");
	exit(EXIT_FAILURE);
    }

    int cmd = 0;
    int result = 0;
    char *mnc;
    do {

CMD_LIST:
        printf("Cmd List:\n");
        printf("  1 Check MNC.\n");
        printf("  2 Quit.\n");


        printf(">> ");
        scanf("%d", &cmd);
   

        if (cmd == 1) { 
MNC_CMD: 
            printf("<mnc> ");
            scanf("%s", mnc);
            if (strncmp(mnc, "quit", 4) == 0) 
                goto CMD_LIST;

            if(send(sockfd, mnc, strlen(mnc)+1, 0) <= 0) {
                perror("send");
                exit(EXIT_FAILURE);
            }
            printf("send: %s\n", mnc);

            if(recv(sockfd, &result, sizeof(result), 0) <= 0) {
                perror("recv");
                return -1;
            }
            printf("recv: %d\n", result);

            if (result == 0) {
                printf("未找到指定MNC号\n");
            } else {
                printf("查找成功\n");
            }
            printf("\n");

            goto MNC_CMD;
        } 
		else if (cmd == 2) {
            break;
        } 
    
    } while(1);

    close(sockfd);

    return 0;
}
