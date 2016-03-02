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
#include <signal.h>
#include <pthread.h>

#define backlog        5
#define BUFFSIZE       1024
#define SVR_MNC_SIZE   1024
#define MNC_NUM_SIZE    4
#define CLI_MNC_LEN     4
#define FAILED          0
#define SUCCESS         1


static int ok;
static int no;

typedef struct mnc {
    char mnc_num[MNC_NUM_SIZE];
} MNC;
MNC svr_mnc[SVR_MNC_SIZE];

typedef struct  mnc_node {
    char mnc_num[MNC_NUM_SIZE];
    int node_cn;
    struct  mnc_node *next;
} MNC_NODE;


int handle_mnc(MNC *out, char *in, char key);
MNC_NODE *create_list(void);
MNC_NODE *insert_list(MNC_NODE *head, MNC_NODE *node);
void *handle_msg(void *tdata);


int main(int argc, const char *argv[])
{
    /*1.创建套接字*/
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(9999);
    inet_aton(argv[1],&servaddr.sin_addr);

    /*2.绑定地址信息*/
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

#define MNC_LEN     4
#define MAX_IN_MNC_SIZE 7681

    int cmd = 0, ret, i = 0;
    char key = '&';
    char in_mnc[MAX_IN_MNC_SIZE];
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    pthread_t t_id;
    while(1)
    {

        do {
CFG:
            printf("\nPlease Config Server Before Start.\n");
            printf("   1 Add Mnc.\n");
            printf("   2 Del Mnc.\n");
            printf("   3 Mod Mnc.\n");
            printf("   4 List Mnc.\n");
            printf("   5 Checked History\n");
            printf("   6 Quit.\n");
                
            //printf("\n Note: And you can also  push 'ESC' button to come back this pad\n");

            printf(">> ");
            scanf("%d", &cmd);
        

            if (cmd == 1) { 
ADD:
                printf("<Add>: ");
                scanf("%s", in_mnc);
                //printf("in_mnc:%s\n", in_mnc);
                if (strncmp(in_mnc, "quit", 4) == 0) 
                    goto CFG;
                ret = handle_mnc(svr_mnc, in_mnc, key);
               
                goto ADD;
            } else if (cmd == 2) {
                memset (svr_mnc, 0, sizeof(svr_mnc));
            } else if (cmd == 3) {
                printf("<Mod>: xysb");
            } else if (cmd == 4) {
                for (i = 0; i < ret; i++) {        
                    printf("mnc_num: %s\n", svr_mnc[i].mnc_num);
                }
            } else if (cmd == 5) {
                printf("success: %d \n failed: %d\n", ok, no);
            } else if (cmd == 6) {
                //printf("xysb...\n");
                break ;
            }

        } while (1);


   
        /*3.监听（被动等待客户端链接）*/
        if(listen(sockfd, backlog) == -1)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        printf("\nServer Start.\nwaiting...\n");


        /*4.接收客户端链接请求*/
        int connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
        if(connfd == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("connect from %s:%d\n", inet_ntoa(clientaddr.sin_addr) , ntohs(clientaddr.sin_port));
       
        //printf("#############\n");
        pthread_create(&t_id, NULL, handle_msg, &connfd);       
    }

    /*6.关闭不用的套接字*/

    close(sockfd);
    //close(connfd);
    return 0;
}


int handle_mnc(MNC *out, char *in, char key)
{
    char *tmp;
    int i, n, of, cn;
    int len = sizeof(MNC_NODE);
    MNC_NODE *head, *p, *q;

    tmp = in;
    head = create_list();
    //printf("in: %s %c\n", in, key);

    for (of = 0, n = 0; in[of] != '\0'; of++, n++) {
        if (in[of] == key) { 
            if (n == 2) {
                tmp[2] = tmp[1];
                tmp[1] = tmp[0];
                tmp[0] = '0';
            }
            tmp[3] = '\0';

            p = (MNC_NODE *)malloc(len);
            memset (p, 0, len);
            strncpy(p->mnc_num, tmp, MNC_NUM_SIZE);
            //printf("p->mnc_num: %s\n tmp: %s\n", p->mnc_num, tmp);
            insert_list(head, p);
            
            n= 0;
            tmp = in + of + 1;
        }
    }
    cn = head->node_cn;


    p = head;
    for (i = 0; p->next != NULL; i++) {
        strncpy(out[i].mnc_num, p->next->mnc_num, MNC_NUM_SIZE);
        q = p;
        p = p->next;

        free(q);
    }   
    free(p);
    

    return cn;
}


MNC_NODE *create_list(void)
{
    int len;
    MNC_NODE *head;


    len = sizeof(MNC_NODE);

    head = (MNC_NODE *)malloc(len);
    memset(head, 0, len);
    head->next = NULL;

    return head;
}

//插入尾部
MNC_NODE *insert_list(MNC_NODE *head, MNC_NODE *node)
{
    MNC_NODE *p;


    p = head;
    while (p->next != NULL)
        p=p->next;

    //printf("%p\n", p);
    p->next = node;
    node->next = NULL;

    head->node_cn++;

    return head;
}

void *handle_msg(void *tdata) 
{
    int ret = 0, i = 0, res;
    int connfd = *((int *)tdata);
    char cli_mnc[CLI_MNC_LEN];
    int chk_status = SUCCESS;


    //printf("thread create success: tdata: %d connfd: %d \n", *((int *)tdata), connfd);

    do {

        //signal(SIGPIPE, SIG_IGN);  /*  handle pipe broken error. */

        if(recv(connfd, cli_mnc, sizeof(cli_mnc), 0) <= 0) {
            perror("recv");
            break;
        }
        printf("\nrecv:%s\n", cli_mnc);
        if (strncmp(cli_mnc, "quit", 4) == 0) break;

    
        for (i=0; i < SVR_MNC_SIZE; i++) {
            //printf("\nser_mnc:%s cli_mnc:%s", svr_mnc[i].mnc_num, cli_mnc);
            ret = strcmp(svr_mnc[i].mnc_num, cli_mnc);
            if (ret == 0) {
                printf("check success.ret: %d\n", ret);
                chk_status = SUCCESS;  
                if(res = send(connfd, &chk_status, sizeof(chk_status), 0) < 0) {
                    perror("send");
                    break;   
                }  
                printf("send res:%d\n", res);
                ok++; 
                break;
            }
        }

        if (ret != 0) {
            printf("check failed. ret: %d\n", ret);
            chk_status = FAILED;
            if(send(connfd, &chk_status, sizeof(chk_status), 0) <0) {
                perror("send");
                break;   
            }   
            no++;   
        }      
    } while (1);

    
    close (connfd);
    printf("thread exit\n");
    return ;
}
