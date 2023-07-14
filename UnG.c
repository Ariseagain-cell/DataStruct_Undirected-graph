#include "header/UnG.h"

/**
 * @brief 初始化含n个顶点且无边的kind类的图G
 * 
 * @param n 顶点个数
 */
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

/**
 * @brief 创建图
 * 
 * @param Nv 顶点数
 * @param Ne 边数
 * @param data 顶点数据数组指针
 * @param edge 边数据数组指针
 * @return MGraph 返回创建成功的图
 */
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

/**
 * @brief 查找顶点v在图G中的位序
 * 
 * @param G 图
 * @param v 点v的值
 * @return int 如果存在则返回位序，否则返回-1
 */
int LocateVex_M(MGraph G, DataType v){
    for (int i = 0; i < G->Nv;i++){
        if(G->Data[i] == v){
            return i;
        }
    }
    return -1;
}

/**
 * @brief 取图G的k顶点的值到w
 * 
 * @param G 图
 * @param k k顶点
 * @param w 获取值的指针
 * @return Status 如果获取成功返回OK，否则返回ERROR
 */
Status GetVex_M(MGraph G, int k, DataType* w){
    if(k < 0 || k > G->Nv){
        return ERROR;
    }
    *w = G->Data[k];
    return OK;
}

/**
 * @brief 对图G的k顶点赋值w
 * 
 * @param G 图
 * @param k 被赋值顶点k
 * @param w 准备赋值的值w
 * @return Status 如果获取成功返回OK，否则返回ERROR
 */
Status OytVex_M(MGraph G, int k, DataType w){
    if(k < 0 || k > G->Nv){
        return ERROR;
    }
    G->Data[k] = w;
    return OK;
}

/**
 * @brief 求图G中k顶点的第一个邻接顶点的位序
 * 
 * @param G 图
 * @param k 顶点k
 * @return int 成功获取返回位序，否则返回-1
 */
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

/**
 * @brief m顶点为k顶点的邻接顶点，求图G中k顶点相对于m顶点的下一个邻接顶点的位序
 * 
 * @param G 图
 * @param k v2
 * @param m v1
 * @return int v3的位序 v1-v2-v3, 如果找不到则返回-1
 */
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

/**
 * @brief 在图G中增加v1顶点到v2顶点的边或弧, 权值为weight
 * 
 * @param G 图
 * @param E 边
 */
void AddArc_M(MGraph G, Edge E){
    //插入边<V1,V2>
    G->G[E->V1][E->V2] = E->Weight;
    //无向图还需要插入<V2, V1>
    G->G[E->V2][E->V1] = E->Weight;
    G->Ne++;
}

/**
 * @brief 在图G中删除k顶点到m顶点的边或弧
 * 
 * @param G 图
 * @param k v1
 * @param m v2
 * @return Status 存在边并删除后为1, 否则返回0
 */
Status RemoveArc_M(MGraph G, int k, int m){
    if(G->G[k][m] < INFINITY){
        G->G[k][m] = INFINITY;
        G->G[m][k] = INFINITY;
        G->Ne--;
        return OK;
    }
    return ERROR;
}

/**
 * @brief 深度优先遍历图G
 * 
 * @param G 图
 * @param visit 遍历函数（一般为printf输出）
 * @param S 初始起点
 */
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

/**
 * @brief 广度优先遍历图G
 * 
 * @param G 图
 * @param visit 遍历函数（一般为printf输出）
 * @param S 初始起点
 */
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

/**
 * @brief 检查<V, W>是否图Graph中的一条边
 * 
 * @param G 图
 * @param V 顶点v1
 * @param W 顶点v2
 * @return Status 是的话返回OK，否则返回ERROR
 */
Status IsEdge_M(MGraph G, Vertex V, Vertex W){
    return G->G[V][W] < INFINITY ? OK : ERROR;
}

/**
 * @brief 访问函数，输出目标
 * 
 * @param G      图
 * @param target 目标
 */
Status visit_M(MGraph G, Vertex target){
    if(target < 0 || target >= G->Nv){
        return OVERFLOW;
    }
    printf("visit v %d : %c \n", target, G->Data[target]);
    return OK;
}

/**
 * @brief 测试函数，用来测试邻接矩阵实现接口的实现效果 
 */
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