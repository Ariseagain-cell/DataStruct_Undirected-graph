# DataStruct 数据结构实验-无向图
1. 设计任务、要求及所用软件环境或工具
设计任务：采用字符类型为元素类型和无头结点单链表为存储结构，实现抽象数据类型Graph。
编程环境：gcc version 8.1.0
开发工具：Visual Studio Code

2.抽象数据类型定义以及各基本操作的简要描述
ADT Graph{
           数据对象：V是具有相同特性数据元素的集合，称为顶点集
           数据关系：R={VR} 
VR={（v，w）| v , w∈V, (v , w)表示v和w之间存在路径} 
基本操作：
             InitGraph (G, n)
               操作结果：初始化含n个顶点且无边的kind类的图G
             BuildGraph (n, e, data[], edge[])
               操作结果：根据传入数据构造图G
             LocateVex(&G, v)
               初始条件：图G已存在
               操作结果：返回顶点v在图中的位序
             GetVex(G, k, w)
               初始条件：图G已存在，0≤k<G->Nv
               操作结果：用w返回图G中位序为k的元素的值
             OytVex(G, k, w)
               初始条件：图G已存在，0≤k<G->Nv
               操作结果：对图G的位序为k的顶点赋值w
             FirstAdjVex(G, k)
               初始条件：图G已存在，0≤k<G->Nv
               操作结果：返回图G中位序为k的顶点的第一个邻接顶点的位序
             NextAdjVex(G, k, m)
               初始条件：图G已存在，0≤k<G->Nv, 0≤m<G->Nv
               操作结果：返回m顶点为k顶点的邻接顶点，求图G中k顶点相对于m顶点的下一个邻接顶点的位序
             AddArc(G, v1, v2, weight)
               初始条件：图G已存在，0≤v1<G->Nv, 0≤v2<G->Nv
               操作结果：在图G中增加v1顶点到v2顶点的边或弧, 权值为weight
             RemoveArc(G, k, m)
               初始条件：图G已存在，0≤k<G->Nv, 0≤m<G->Nv
               操作结果：在图G中删除k顶点到m顶点的边或弧
             DFSTraverse(G, visit(), s)
               初始条件：图G已存在，0≤s<G->Nv
               操作结果：从位序为s的节点开始深度优先遍历图G
             BFSTraverse(G, visit(), s)
               初始条件：图G已存在，0≤s<G->Nv
               操作结果：从位序为s的节点开始广度优先遍历图G
             IsEdge(G, v, w)
               初始条件：图G已存在，0≤v<G->Nv, 0≤w<G->Nv
               操作结果：返回判断<V, W>是否图Graph中的一条边的判断结果
       } ADT Graph

3.存储结构定义

公用头文件UnG.h:

#ifndef UNG_H_INCLUDED
#define UNG_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 100  /* 最大顶点数 */
#define INFINITY 65535  /* ∞设为双字节无符号整数的最大值65535 */

#define OK 1
#define ERROR 0
#define UNVISITED 0
#define VISITED   1
#define TRUE 1
#define FALSE 0
#define OVERFLOW 0

int Visited[MaxVertexNum];//标识顶点是否被访问过

/* 自定义数据类型 */
typedef int Status; /* 表状态 */
typedef int Vertex; /* 用顶点下标表示顶点，为整型 */
typedef int WeightType; /* 边的权值 */
typedef char DataType; /* 顶点存储的数据类型设为字符型 */

/* 边的定义 */
typedef struct ENode {
    Vertex V1, V2; /* 有向边<V1, V2> */
    WeightType Weight; /* 权重 */
}ENode, * PtrToENode, * Edge;



	
（1）邻接矩阵数组存储结构

/*              邻接矩阵实现             */
typedef struct GNode {
    int Nv; /* 顶点数 */
    int Ne; /* 边数 */
    WeightType G[MaxVertexNum][MaxVertexNum]; /* 邻接矩阵 */
    DataType Data[MaxVertexNum]; /* 存顶点的数据 */
}GNode, * PtrToGNode, * MGraph;

（2）邻接表的链表存储结构

/*               邻接表实现               */
/* 邻接点的定义 */
typedef struct AdjVNode{
    Vertex AdjV;        /* 邻接点下标 */
    WeightType Weight;  /* 边权重     */
    struct AdjVNode* Next;     /* 指向下一邻接点的指针 */
}AdjVNode,*PtrToAdjVNode;

/* 顶点表头节点的定义 */
typedef struct VNode{
    PtrToAdjVNode FirstEdge; /* 边表头指针   */
    DataType Data;           /* 存顶点的数据 */
}AdjList[MaxVertexNum];

/* 图节点的定义 */
typedef struct LGNode{
    int Nv;     /* 顶点数 */
    int Ne;     /* 边数   */
    AdjList G;  /* 邻接表 */
}LGNode, *PtrToLGNode, *LGraph;

（3）队列

/*                  队列                  */
typedef int ElemType;//队列类型
typedef struct LQNode {
    ElemType data;
    struct LQNode* next;
}LQNode, * QueuePtr;//结点及其指针类型

typedef struct {
    QueuePtr front;//队头指针
    QueuePtr rear;//队尾指针
}LQueue;

// 初始化队列
Status InitQueue_LQ(LQueue* Q);
// 判断队列是否为空
Status QueueEmpty_LQ(LQueue Q);
// 入队
Status EnQueue_LQ(LQueue* Q, ElemType e);
// 出队
Status DeQueue_LQ(LQueue* Q, ElemType* e);

4.在所选择的存储结构上基本操作的实现

(1)邻接矩阵数组存储结构

1)初始化含n个顶点且无边的kind类的图G
使用malloc申请一个图节点GNode大小的空间，然后赋值图节点个数，并通过遍历初始化邻接矩阵，将其中元素都初始化为INFINITY

MGraph InitGraph_M(MGraph Graph, int n){
    Vertex V, W;

    Graph = (MGraph)malloc(sizeof(struct GNode));
    if(Graph == NULL){
        return NULL;
    }
    Graph->Nv = n;
    Graph->Ne = 0;
    // 初始化邻接矩阵
    for (V = 0; V < Graph->Nv; V++){
        for (W = 0; W < Graph->Nv; W++){
            Graph->G[V][W] = INFINITY;
        }
    }
    return Graph;
}

2)创建图
根据传入的数据构建图，先调用InitGraph_M()函数初始化图，再遍历调用AddArc_M()函数将边一一插入，最后遍历插入顶点数据，返回已经初始化好的图G

MGraph BuildGraph_M(int Nv, int Ne, DataType* data, Edge* edge){
    MGraph Graph = NULL;
    int i;
    Graph = InitGraph_M(Graph, Nv);
    if(Graph == NULL){
        return NULL;
    }
    printf("insert the data of edge:\n");
    if (Ne > 0){
        for (i = 0; i < Ne; i++){
            AddArc_M(Graph, edge[i]);
            printf("no: %d v1_pos:%d v2_pos:%d weight:%d\n", i, edge[i]->V1, edge[i]->V2, edge[i]->Weight);
        }
    }

    // 输入顶点数据
    printf("insert the data of v:\n");
    for (int V = 0; V < Graph->Nv; V++){
        Graph->Data[V] = data[V];
        printf("v_pos:%d data:%c \n", V, data[V]);
    }
    return Graph;
}

3)查找顶点v在图G中的位序
遍历图G中的顶点，直到找到顶点v，返回其位序，若找不到则跳出循环，返回-1

int LocateVex_M(MGraph G, DataType v){
    for (int i = 0; i < G->Nv;i++){
        if(G->Data[i] == v){
            return i;
        }
    }
    return -1;
}

4)取图G的k顶点的值到w
直接在顶点矩阵中取位序为k的顶点的值传给w，返回OK，若k不在合理范围内，返回ERROR

Status GetVex_M(MGraph G, int k, DataType* w){
    if(k < 0 || k > G->Nv){
        return ERROR;
    }
    *w = G->Data[k];
    return OK;
}

5)对图G的k顶点赋值w
若k在合理范围内，则直接将w赋值给位序为k的元素，返回OK，否则返回ERROR

Status OytVex_M(MGraph G, int k, DataType w){
    if(k < 0 || k > G->Nv){
        return ERROR;
    }
    G->Data[k] = w;
    return OK;
}

6)求图G中k顶点的第一个邻接顶点的位序
遍历邻接矩阵的k行，找到第一个值小于INFINITY的元素，返回其位序，若找不到，则跳出循环，返回-1

int FirstAdjVex_M(MGraph G, int k){
    if(k < 0 || k >= G->Nv){
        return -1;
    }
    for (int i = 0; i < G->Nv;i++){
        if(G->G[k][i] < INFINITY){
            return i;
        }
    }
    return -1;
}

7)m顶点为k顶点的邻接顶点，求图G中k顶点相对于m顶点的下一个邻接顶点的位序
遍历邻接矩阵的第k行，从m+1列开始遍历，找到第一个值小于INFINITY的元素，返回其位序，若找不到，则跳出循环，返回-1

int NextAdjVex_M(MGraph G, int k, int m){
    if(k < 0 || m < 0 || k >= G->Nv){
        return -1;
    }
    for (int i = m + 1; i < G->Nv;i++){
        if(G->G[k][i] < INFINITY){
            return i;
        }
    }
    return -1;
}

8)在图G中增加v1顶点到v2顶点的边或弧, 权值为weight
向邻接矩阵中的第v1行v2列插入权值weight，这里因为是无向图，所以对于v2行v1列要进行相同的操作，最后将图的边数自增

void AddArc_M(MGraph G, Edge E){
    //插入边<V1,V2>
    G->G[E->V1][E->V2] = E->Weight;
    //无向图还需要插入<V2, V1>
    G->G[E->V2][E->V1] = E->Weight;
    G->Ne++;
}

9)在图G中删除k顶点到m顶点的边或弧
先判断k顶点到m顶点的边是否存在，即邻接矩阵第k行m列的值是否小于INFINITY, 若存在，则将其值赋值为INFINITY，同时将第m行k列的值赋值为INFINITY，并使图边数自减，返回OK，否则返回ERROR

Status RemoveArc_M(MGraph G, int k, int m){
    if(G->G[k][m] < INFINITY){
        G->G[k][m] = INFINITY;
        G->G[m][k] = INFINITY;
        G->Ne--;
        return OK;
    }
    return ERROR;
}

10)深度优先遍历图G
通过递归实现DFS，每访问一个顶点，就在Visited[]数组内标记为VISITED，先序遍历访问下一个顶点，若该顶点与当前访问顶点存在一条边，且该顶点没被访问过，则递归访问该顶点

void DFSTraverse_M(MGraph G, Status ( *visit)(MGraph, int), Vertex S){
    Vertex W;
    //访问S
    visit(G, S);
    Visited[S] = VISITED;
    //先序遍历
    for (W = 0; W < G->Nv; W++){
        if (!Visited[W] && IsEdge_M(G, S, W)){
            DFSTraverse_M(G, visit, W);
        }
    }
}

11)广度优先遍历图G
通过使用队列的数据结构来实现BFS。每访问一个顶点后，将其未被访问的邻接顶点入队。层序遍历访问下一顶点，知道队列为空跳出循环

void BFSTraverse_M(MGraph G, Status ( *visit)(MGraph, int), Vertex S){
    LQueue* Q = (LQueue *)malloc(sizeof(LQueue));
    InitQueue_LQ(Q);
    Vertex V, W;

    // 访问顶点S Visit(S)
    visit(G, S);
    Visited[S] = VISITED;
    EnQueue_LQ(Q, S);
    // 层序遍历
    while (!QueueEmpty_LQ(*Q)){
        DeQueue_LQ(Q, &V);
        for (W = 0; W < G->Nv; W++){
            if (!Visited[W] && IsEdge_M(G, V, W)){
                // 访问顶点W
                visit(G, W);
                Visited[W] = VISITED;
                EnQueue_LQ(Q, W);
            }
        }
    }
}

12)检查<V, W>是否图Graph中的一条边
检查邻接矩阵中第v行第w列的值是否小于INFINITY，若是则<V,W>是一条边，返回OK，否则返回ERROR

Status IsEdge_M(MGraph G, Vertex V, Vertex W){
    return G->G[V][W] < INFINITY ? OK : ERROR;
}

13)访问函数，输出目标
如果target（元素位序）在合理范围内，打印访问该顶点的值

Status visit_M(MGraph G, Vertex target){
    if(target < 0 || target >= G->Nv){
        return OVERFLOW;
    }
    printf("visit v %d : %c \n", target, G->Data[target]);
    return OK;
}

(2)邻接表的链表存储结构

1)查找顶点v在图G中的位序
顺序遍历顶点直到发现顶点v，返回其所在位序，没找到则跳出循环，返回-1

int LocateVex_L(LGraph G, DataType v){
    for (int i = 0; i < G->Nv; i++){
        if (v == G->G[i].Data){
            return i;
        }
    }
    return -1;
}

2)对图G的k顶点赋值w
先判断k是否在合理范围内，然后直接向邻接表中的顶点k赋值

Status OytVex_L(LGraph G, int k, DataType w){
    if(k < 0 || k >= G->Nv){
        return ERROR;
    }
    G->G[k].Data = w;
    return OK;
}

3)取图G的k顶点的值到w
先判断k是否在合理范围内，然后直接向w指针赋值顶点k的数据

Status GetVex_L(LGraph G, int k, DataType* w){
    if(k < 0 || k >= G->Nv){
        return ERROR;
    }
    *w = G->G[k].Data;
    return OK;
}

4)创建含有n个顶点，e条边的无向图G
先初始化创建一个图，将顶点数和边数存入图G中，然后建立顶点数组，插入顶点元素，并将每个顶点对邻接顶点的第一条边指针设为NULL。然后插入边数据，在两个顶点的边表头指针都要插入该条边，初始化完成后返回图G

LGraph InitGraph_L(LGraph G, DataType* vexs, int n, Edge* arcs, int e){
    int i, j, k;              // 控制循环
    PtrToAdjVNode p, q;       // 邻接顶点，用于插入
    G = (LGraph)malloc(sizeof(GNode));
    G->Nv = n;
    G->Ne = e;
    // 建立顶点数组
    printf("insert the data of v:\n");
    for (i = 0; i < G->Nv; i++) {
        printf("v_pos:%d data:%c \n", i, vexs[i]);
        G->G[i].Data = vexs[i];
        G->G[i].FirstEdge = NULL;
    }
    printf("insert the data of edge\n");
    for (k = 0; k < G->Ne; k++) {
        i = arcs[k]->V1;
        j = arcs[k]->V2;
printf("no: %d v1_pos:%d v2_pos:%d weight:%d\n", k, i, j, arcs[k]->Weight);
        if (i < 0 || j < 0){
            return NULL;
        }
        p = (PtrToAdjVNode)malloc(sizeof(AdjVNode));
        if (NULL == p){
            return NULL;
        }
        p->AdjV = j;
        p->Next = G->G[i].FirstEdge;
        p->Weight = arcs[k]->Weight;
        G->G[i].FirstEdge = p;

        q = (PtrToAdjVNode)malloc(sizeof(AdjVNode));
        if (NULL == q){
            return NULL;
        }
        q->AdjV = i;
        q->Next = G->G[j].FirstEdge;
        q->Weight = arcs[k]->Weight;
        G->G[j].FirstEdge = q;
    }
    return G;
}

5)在图G中增加顶点v1到顶点v2的边
增加顶点v1到顶点v2的边时，因为该数据结构是无向图，所以在增加v1到v2边的同时增加v2到v1的边

Status AddArc_L(LGraph G, Edge E){
    PtrToAdjVNode  p;
    if (E->V1 < 0 || E->V1 >= G->Nv || E->V2 < 0 || E->V2 > G->Nv){
        // 顶点不存在
        return ERROR;
    }
    p = G->G[E->V1].FirstEdge;
    // 判断要插入的边是否存在
    while (p != NULL) {        
        if (E->V2 == p->AdjV){
            // 要插入的边已存在
            return ERROR;   
        } 
        p = p->Next;
    }
    // 插入从v1到v2的边
    p = (PtrToAdjVNode)malloc(sizeof(AdjVNode));
    if (p == NULL){
        return OVERFLOW;
    }
    p->AdjV = E->V2;
    p->Weight = E->Weight;
    p->Next = G->G[E->V1].FirstEdge;
    G->G[E->V1].FirstEdge = p;
    // 插入从v1到v2的边
    p = (PtrToAdjVNode)malloc(sizeof(AdjVNode));
    if (p == NULL){
        return OVERFLOW;
    }
    p->AdjV = E->V1;
    p->Weight = E->Weight;
    p->Next = G->G[E->V2].FirstEdge;
    G->G[E->V2].FirstEdge = p;
    G->Ne++;
    return  OK;
}

6)若图G中k顶点的邻接链表非空，则令指针p指向第一个结点，并返回其存储的邻接顶点位序，否则令指针p为NULL，并返回-1
返回顶点k第一个邻接点的位序，先查看k是否在合理范围内，然后取p指向k的第一个邻接点的边，若存在第一个邻接点，返回其位序，否则返回-1

int FirstAdjVex_L(LGraph G, int k, PtrToAdjVNode *p){
    if (k < 0 || k >= G->Nv){
        // k顶点不存在
        return -1;
    }
    *p = G->G[k].FirstEdge;
    if (*p != NULL) {
        return (*p)->AdjV;
    } else {
        return -1;
    }
}

7)令p指向k顶点的邻接链表的下一个结点，若p不为空，则返回p指向的顶点的位序
先检查顶点k的位序是否在正常范围内，判断指向顶点k的边表的某个顶点的指针是否为空。将该指针向后移动一位，此时若不为空，则返回其位序，否则返回-1

int NextAdjVex_L(LGraph G, int k, PtrToAdjVNode *p){
    if (k < 0 || k >= G->Nv){
        // k顶点不存在
        return -1;
    }    
    if (*p == NULL){
        // p为空
        return -1;
    }        
    *p = (*p)->Next;
    if (*p != NULL){
        return (*p)->AdjV;
    } else {
        return -1;
    }
}

8)访问节点
检查target是否在合理范围内，若是则输出顶点数据，返回OK，否则返回OVERFLOW

Status visit_L(LGraph G, Vertex target){
    if(target < 0 || target >= G->Nv){
        return OVERFLOW;
    }
    printf("visit v %d : %c \n", target, G->G[target].Data);
    return OK;
}

9)访问k顶点的所有邻接顶点
先判断k是否在合理范围内，然后通过FirstAdjvex_L()函数找到k的第一个邻接点的位序, 访问它，通过NextAdjVex_L()将i转移至下一邻接点的位序，访问下一个邻接点。访问结束后跳出循环，返回OK

Status VisitAllAdjVex_L(LGraph G, int k, Status ( *visit)(LGraph, int)){
    int i;
    PtrToAdjVNode p = NULL;
    if (k < 0 || k >= G->Nv){
        return ERROR;
    }
    for (i = FirstAdjVex_L(G, k, &p); i >= 0; i = NextAdjVex_L(G, k, &p)) {
        if (ERROR == visit(G, i)){
            return ERROR;
        }
    }
    return OK;
}

10)深度优先遍历
从顶点数组的第一位开始遍历所有邻接顶点，通过DFS_AL()函数递归深度优先访问未访问过的顶点

Status DFS_AL(LGraph G, int k, Status ( *visit)(LGraph, int)){
    PtrToAdjVNode p = NULL;
    if(ERROR == visit(G, k)){
        return ERROR;
    }
    Visited[k] = VISITED;
    for (int i = FirstAdjVex_L(G, k , &p); i >= 0; i = NextAdjVex_L(G, k, &p)) {
        if (UNVISITED == Visited[i]){
            if(ERROR == DFS_AL(G, i, visit)){
                return ERROR;
            }
        }
    }
    return OK;
}
Status DFSTraverse_L(LGraph G, Status (*visit)(LGraph, int)){
    for (int i = 0; i < G->Nv; i++){
        Visited[i] = UNVISITED;
    }
    for (int i = 0; i < G->Nv; i++){
        if (UNVISITED == Visited[i]){
            if(ERROR == DFS_AL(G, i, visit)){
                return ERROR;
            }
        }
    }
    return OK;
}

11)广度优先遍历
与数组实现类似，从第一个顶点开始遍历所有邻接顶点，访问该顶点并将其入队，然后使其出队访问其邻接点并将其邻接点入队，重复以上操作直到所有顶点表头节点都访问完毕

Status BFSTraverse_L(LGraph G, Status ( *visit)(LGraph, int)){
    int i, j, k;
    PtrToAdjVNode p = NULL;
    for (int i = 0; i < G->Nv; i++){
        Visited[i] = UNVISITED;
    }
    LQueue Q;
    InitQueue_LQ(&Q);
    for (i = 0; i < G->Nv; i++) {
        if (UNVISITED == Visited[i]) {
            if (ERROR == visit(G, i)){
                return ERROR;
            }
            Visited[i] = VISITED;
            EnQueue_LQ(&Q, i);
            while(OK == DeQueue_LQ(&Q, &k)) {
                for (j = FirstAdjVex_L(G, k, &p); j >= 0; j = NextAdjVex_L(G, k, &p))
                    if (UNVISITED == Visited[j]) {
                        if (ERROR == visit(G, j)){
                            return ERROR;
                        }
                        Visited[j] = VISITED;
                        EnQueue_LQ(&Q, j);
                    }
            }
        }
    }
    return OK;
}

12)查看节点是否可达
遍历s的所有邻接点，递归的去深入查看是否有路径可达顶点t

Status isReachable(LGraph G, int s, int t){
    Status found = FALSE;
    PtrToAdjVNode p = NULL;
    Visited[s] = VISITED;
    if (s == t){
        return OK;
    }
    for (int i = FirstAdjVex_L(G, s, &p); i >= 0; i = NextAdjVex_L(G, s, &p)) {
        if (UNVISITED == Visited[i]){
            found = isReachable(G, i, t);
        }
    }
    return found;
}

13)打印简单路径
DFS递归遍历倒序打印简单路径，found用来记录是否已经找到目标节点s，for循环的条件中限制了只有在未找到目标节点时才进行循环递归访问

Status SimplePath_DFS(LGraph G, int s, int t, Status ( *visit)(LGraph, int)){
    Status found = FALSE;
    PtrToAdjVNode p = NULL;
    Visited[s] = VISITED;
    if (s == t)
        found = TRUE;
    else {
        for (int i = FirstAdjVex_L(G, s, &p); i >= 0 && found==FALSE ; i = NextAdjVex_L(G, s, &p)){
            if (UNVISITED == Visited[i]) {
                found = SimplePath_DFS(G, i, t, visit);
            }
        }
    }
    if (found == TRUE) {
        visit(G, s);
        return TRUE;
    }
    else{
        return ERROR;
    }
}

14)广度优先遍历查询最短路径
先初始化距离数组为无穷大，借助队列来广度优先遍历记录最短路径

void ShortestPathLength_BFS(LGraph G, int s, int* D){
    int i, j;
    PtrToAdjVNode p;
    LQueue Q;
    InitQueue_LQ(&Q);
    // 初始化距离为无穷大
    for (i = 0; i < G->Nv; i++){
        D[i] = INFINITY;
    }
    D[s] = 0;
    Visited[s] = VISITED;
    EnQueue_LQ(&Q, s);
    while (OK == DeQueue_LQ(&Q, &i)) {
        for (j = FirstAdjVex_L(G, i, &p); j >= 0; j = NextAdjVex_L(G, i, &p)) {
            if (UNVISITED == Visited[j]) {
                D[j] = D[i] + 1;
                Visited[j] = VISITED;
                EnQueue_LQ(&Q, j);
            }
        }
    }
}
5.程序清单，输入的数据及各基本操作的测试结果
(1)邻接矩阵的数组实现
Main.c文件的主函数

#include "header/UnG.h"

int main(){  
    // TEST func
    test_M();
    getchar();
    getchar();
    return 0;
}

UnG.c文件的测试函数，用来测试邻接矩阵实现接口的实现效果

void test_M(){

    printf("********************************************\n");
    printf("*********** Array implementation ***********\n");
    printf("********************************************\n");

    /** 初始化数据 **/
    printf("start  init===\n");
    int i;
    char temp = 'a';
    int nv = 9;
    int ne = 8;
    // 顶点数据
    DataType *initData = (DataType *)malloc(sizeof(DataType) * nv);
    initData[0] = '1';
    initData[1] = '2';
    initData[2] = '3';
    initData[3] = '5';
    initData[4] = '6';
    initData[5] = '4';
    initData[6] = '7';
    initData[7] = '8';
    initData[8] = '9';
    // 边数据
    Edge *initEdge = (Edge *)malloc(sizeof(Edge) * ne);
    for (int j = 0; j < ne;j++){
        initEdge[j] = (Edge)malloc(sizeof(ENode));
    }
    initEdge[0]->V1 = 0;
    initEdge[0]->V2 = 1;
    initEdge[0]->Weight = 1;

    initEdge[1]->V1 = 0;
    initEdge[1]->V2 = 2;
    initEdge[1]->Weight = 1;

    initEdge[2]->V1 = 0;
    initEdge[2]->V2 = 3;
    initEdge[2]->Weight = 1;

    initEdge[3]->V1 = 1;
    initEdge[3]->V2 = 4;
    initEdge[3]->Weight = 1;

    initEdge[4]->V1 = 1;
    initEdge[4]->V2 = 5;
    initEdge[4]->Weight = 1;

    initEdge[5]->V1 = 1;
    initEdge[5]->V2 = 6;
    initEdge[5]->Weight = 1;
    
    initEdge[6]->V1 = 4;
    initEdge[6]->V2 = 5;
    initEdge[6]->Weight = 1;

    initEdge[7]->V1 = 5;
    initEdge[7]->V2 = 8;
    initEdge[7]->Weight = 1;
    MGraph G = BuildGraph_M(nv, ne, initData, initEdge);
    /** 初始化数据完成 **/
    printf("finish init===\n");

    for(i = 0;i < MaxVertexNum;i++){
        Visited[i] = UNVISITED;
    }
    printf("BFS:\n");
    BFSTraverse_M(G, visit_M, 0);
    printf("DFS:\n");
    for(i = 0;i < MaxVertexNum;i++){
        Visited[i] = UNVISITED;
    }
    DFSTraverse_M(G, visit_M, 0);
    printf("insert Edge >> 2(3) - 7(8) DFS change ==== \n");
    Edge edge = (Edge)malloc(sizeof(ENode));
    edge->V1 = 2;
    edge->V2 = 7;
    edge->Weight = 1;
    AddArc_M(G, edge);
    for(i = 0;i < MaxVertexNum;i++){
        Visited[i] = UNVISITED;
    }
    DFSTraverse_M(G, visit_M, 0);
    RemoveArc_M(G, 2, 7);
    printf("remove Edge >> 2(3) - 7(8) DFS change ==== \n");
    for(i = 0;i < MaxVertexNum;i++){
        Visited[i] = UNVISITED;
    }
    DFSTraverse_M(G, visit_M, 0);

    printf("check the position of '4'(5): %d \n", LocateVex_M(G, '4'));
    printf("find the first adjVex 4(1): %d \n", FirstAdjVex_M(G, 4));
    printf("find the first adjVex 7(-1): %d \n", FirstAdjVex_M(G, 7));
    printf("find the next adjVex 1, 5(6): %d \n", NextAdjVex_M(G, 1, 5));
    GetVex_M(G, 3, &temp);
    printf("get value of 3(5): %c \n", temp);

}
