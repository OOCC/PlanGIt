#define MAX_SQUEUE_SIZE 100 //队列的最大长度 
#include <stdio.h>

typedef  char datatype;

typedef struct{
    datatype data[MAX_SQUEUE_SIZE];  //队列的存储空间
    int front,rear;          //队头，队尾指针
}squeue,*squlink;

//置空队
ClearQueue(squlink Q){ 
    Q->front=Q->rear=0;
}

//判断队列是否为空 
int EmptyQueue(squlink Q){
    if(Q->front==Q->rear){
        return(1);
    }else return(0);
}

//进队
int EnterQueue(squlink Q,datatype e){ //元素e进队
    if((Q->rear+1)%MAX_SQUEUE_SIZE==Q->front){//队满
        printf("队列已满\n"); 
        return(0);
    }else{ 
        Q->rear=(Q->rear+1)%MAX_SQUEUE_SIZE;
        Q->data[Q->rear]=e;
        return(1);
    } 
}

//出队
datatype DeQueue(squlink Q){    
    if(EmptyQueue(Q)){//队空
        printf("队列为空\n"); 
        return(' ');
    }else {
        Q->front=(Q->front+1)%MAX_SQUEUE_SIZE;
        return(Q->data[Q->front]);
    }
}

int Lenqueue(squlink Q){ //求队Q中当前元素个数
    int i=(Q->rear-Q->front+MAX_SQUEUE_SIZE)%MAX_SQUEUE_SIZE;
    return(i);
}

int main(){
    char input;
    squeue sq;
    ClearQueue(&sq);
    while(1){
        input=getchar();
        if(input=='0'){
            DeQueue(&sq);
        }else if(input=='@'){
            while(!EmptyQueue(&sq)){
                printf("%c",DeQueue(&sq));
            }
            system("pause");
            return 0;
        }else{
            EnterQueue(&sq,input);
        }
        fflush(stdin);
    }
    
}


//链式队列

#include <stdio.h>

typedef char DataType;

typedef struct SqueneNode{
    DataType data;
    struct SqueneNode *next;  
}LSqueneNode,*pSqueneNode;//链表队列的结点 

typedef struct{
    LSqueneNode *front,*rear;
}LQueue;//表示链表队列的结构体

//初始化队列
void intiLQueue(LQueue * q ){
    if(q){
        q->front=(LSqueneNode*)malloc(sizeof(LSqueneNode));
        if(q->front){
            q->front->next=NULL;
            q->rear=q->front;
        }
    }
}

 
//清空队列 
void clearLQueue(LQueue * q){
    pSqueneNode  p,ptemp;
    if(q!=NULL){
        if(q->front!=NULL){
            p=q->front->next;
            while(p!=NULL){//释放队列空间 
                ptemp=p->next;
                free(p);
                p=ptemp;
            }
            q->front->next=NULL;
            q->rear=q->front;
        }
    }
} 

//判断队列是否为空
int isLQueueEmpty(LQueue * q){
    if(q!=NULL){
        if(q->rear==q->front)return 1;
        else return 0;
    }
    printf("null point Error\n"); 
    return -1;
}

//进队
void EnterLQueue(LQueue * q,DataType e){
    LSqueneNode* p;
    if(q){
        p=(LSqueneNode*)malloc(sizeof(LSqueneNode));
        if(p){
            p->data=e;
            p->next=NULL;
            q->rear->next=p;
            q->rear=p;
        }
    }
}