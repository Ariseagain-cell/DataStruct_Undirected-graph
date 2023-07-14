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

/* *************** 邻接矩阵实现 ********************** */

typedef struct GNode {
    int Nv; /* 顶点数 */
    int Ne; /* 边数 */
    WeightType G[MaxVertexNum][MaxVertexNum]; /* 邻接矩阵 */
    DataType Data[MaxVertexNum]; /* 存顶点的数据(冗余) */
}GNode, * PtrToGNode, * MGraph;

// 初始化含n个顶点且无边的kind类的图G
MGraph InitGraph_M(MGraph Graph, int n);

// 创建图
MGraph BuildGraph_M(int Nv, int Ne, DataType* data, Edge* edge);

// 查找顶点v在图G中的位序
int LocateVex_M(MGraph G, DataType v);

// 取图G的k顶点的值到w
Status GetVex_M(MGraph G, int k, DataType* w);

// 对图G的k顶点赋值w
Status OytVex_M(MGraph G, int k, DataType w);

// 求图G中k顶点的第一个邻接顶点的位序
int FirstAdjVex_M(MGraph G, int k);

// m顶点为k顶点的邻接顶点，求图G中k顶点相对于m顶点的下一个邻接顶点的位序
int NextAdjVex_M(MGraph G, int k, int m);

// 在图G中增加v1顶点到v2顶点的边或弧, 权值为weight
void AddArc_M(MGraph G, Edge E);

// 在图G中删除k顶点到m顶点的边或弧
Status RemoveArc_M(MGraph G, int k, int m);

// 深度优先遍历图G
void DFSTraverse_M(MGraph G, Status ( *visit)(MGraph, int), Vertex S);

// 广度优先遍历图G
void BFSTraverse_M(MGraph G, Status ( *visit)(MGraph, int), Vertex S);

// 检查<V, W>是否图Graph中的一条边
Status IsEdge_M(MGraph G, Vertex V, Vertex W);

// 访问节点
Status visit_M(MGraph G, Vertex target);

// 测试函数，用来测试邻接矩阵实现接口的实现效果
void test_M();

/* *************** 邻接表实现 ********************** */

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
    /* 注意：很多情况下，顶点无数据，此时Data可以不用出现 */
}AdjList[MaxVertexNum];

/* 图节点的定义 */
typedef struct LGNode{
    int Nv;     /* 顶点数 */
    int Ne;     /* 边数   */
    AdjList G;  /* 邻接表 */
}LGNode, *PtrToLGNode, *LGraph;

// 初始化含n个顶点且无边的kind类的图G
LGraph InitGraph_L(LGraph G, DataType* vexs, int n, Edge* arcs, int e);

// 查找顶点v在图G中的位序
int LocateVex_L(LGraph G, DataType v);

// 取图G的k顶点的值到w
Status GetVex_L(LGraph G, int k, DataType* w);

// 对图G的k顶点赋值w
Status OytVex_L(LGraph G, int k, DataType w);

// 求图G中k顶点的第一个邻接顶点的位序
int FirstAdjVex_L(LGraph G, int k, PtrToAdjVNode *p);

// m顶点为k顶点的邻接顶点，求图G中k顶点相对于m顶点的下一个邻接顶点的位序
int NextAdjVex_L(LGraph G, int k, PtrToAdjVNode *p);

// 在图G中增加v1顶点到v2顶点的边或弧, 权值为weight
Status AddArc_L(LGraph G, Edge E);

// 访问顶点k的所有邻接点
Status VisitAllAdjVex_L(LGraph G, int k, Status (*visit)(LGraph, int));

// 深度优先遍历图G
Status DFSTraverse_L(LGraph G, Status ( *visit)(LGraph, int));

// 广度优先遍历图G
Status BFSTraverse_L(LGraph G, Status ( *visit)(LGraph, int));

// 打印DFS的简单路径
Status SimplePath_DFS(LGraph G, int s, int t, Status (*visit)(LGraph, int));

// BFS的最短路径
void ShortestPathLength_BFS(LGraph G, int s, int *D);

// 检查<V, W>是否图Graph中的一条边
Status IsEdge_L(LGraph G, Vertex V, Vertex W);

// 访问节点
Status visit_L(LGraph G, Vertex target);

// 测试函数，用来测试邻接矩阵实现接口的实现效果
void test_L();

// ************** 队列 *******************
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

#endif // UNG_H_INCLUDED
