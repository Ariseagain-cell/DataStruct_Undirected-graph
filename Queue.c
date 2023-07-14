#include "header/UnG.h"

Status InitQueue_LQ(LQueue* Q)
{ //构造一个空队列Q 
	(*Q).front = (*Q).rear = (QueuePtr)malloc(sizeof(LQNode));//申请内存空间
	if (!(*Q).front)
		exit(OVERFLOW);//申请内存空间失败
	(*Q).front->next = NULL;//队尾指向空
	return OK;
}
Status QueueEmpty_LQ(LQueue Q)
{ //若Q为空队列,则返回TRUE,否则返回FALSE 
	if (Q.front == Q.rear)//队列为空
		return TRUE;
	else
		return FALSE;
}

Status EnQueue_LQ(LQueue* Q, ElemType e)
{ // 插入元素e为Q的新的队尾元素 
	LQNode* p;
	p = (LQNode*)malloc(sizeof(LQNode));//申请内存空间
	if (NULL == p)return OVERFLOW;
	p->data = e; p->next = NULL;//e插入空队列
	if (NULL == Q->front)Q->front = p;//e插入非空队列
	else Q->rear->next = p;
	Q->rear = p;//队尾指针指向新的队尾
	return OK;

}

Status DeQueue_LQ(LQueue* Q, ElemType* e)
{ //若队列不空,删除Q的队头元素,用e返回其值,并返回OK,否则返回ERROR 

	QueuePtr p;
	if ((*Q).front == (*Q).rear)
		return FALSE;
	p = (*Q).front->next;//指向对头结点
	*e = p->data;//保存队头结点的数据到e
	(*Q).front->next = p->next;
	if ((*Q).rear == p)(*Q).rear = (*Q).front;//队列只有一个结点，删除后队列为空
	free(p);//释放被删的对头结点
	return OK;
}