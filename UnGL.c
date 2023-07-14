#include "header/UnG.h"

/**
 * @brief 查找顶点v在图G中的位序
 * 
 * @param G     图
 * @param v     顶点
 * @return 顶点v在图G中的位序, 失败则返回-1
 * */
int LocateVex_L(LGraph G, DataType v){
    for (int i = 0; i < G->Nv; i++){
        if (v == G->G[i].Data){
            return i;
        }
    }
    return -1;
}

/**
 * @brief 取图G的k顶点的值到w
 * 
 * @param G 图
 * @param k 顶点k
 * @param w 取数据的指针
 * @return Status 成功返回OK，否则返回ERROR 
 */
Status GetVex_L(LGraph G, int k, DataType* w){
    if(k < 0 || k >= G->Nv){
        return ERROR;
    }
    *w = G->G[k].Data;
    return OK;
}

/**
 * @brief 对图G的k顶点赋值w
 * 
 * @param G 图
 * @param k 顶点k
 * @param w 数据w
 * @return Status 
 */
Status OytVex_L(LGraph G, int k, DataType w){
    if(k < 0 || k >= G->Nv){
        return ERROR;
    }
    G->G[k].Data = w;
    return OK;
}

/**
 * @brief 创建含有n个顶点，e条边的无向图G
 * 
 * @param  G       邻接表
 * @param  vexs    顶点数组
 * @param  n       顶点数
 * @param  arcs    边数组
 * @param  e       边数
 * @return  成功返回OK，否则返回ERROR
 */
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

/**
 * @brief 在图G中增加顶点v1到顶点v2的边
 * @param G     图
 * @param Edge  需要插入的边，包含顶点和权值信息
 * */
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


/**
 * @brief 若图G中k顶点的邻接链表非空，则令指针p指向第一个结点，并返回其存储的邻接顶点位序，否则令指针p为NULL，并返回-1
 * @param G     图
 * @param k     顶点的位序
 * @param p     指向第一个顶点的指针
 * @return      k顶点邻接链表的第一个顶点的位序
 * */
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

/**
 * @brief 令p指向k顶点的邻接链表的下一个结点，若p不为空，则返回p指向的顶点的位序
 * @param G         图
 * @param k         顶点的位序
 * @param p         指向k顶点的邻接链表的某个结点的指针
 * @return          k顶点邻接链表的下一结点的位序
 * */
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


/**
 * @brief 访问节点
 * 
 * @param G 图
 * @param target 目标节点
 * @return Status 成功返回1，否则返回0
 */
Status visit_L(LGraph G, Vertex target){
    if(target < 0 || target >= G->Nv){
        return OVERFLOW;
    }
    printf("visit v %d : %c \n", target, G->G[target].Data);
    return OK;
}

/**
 * @brief 访问k顶点的所有邻接顶点
 * @param G     图
 * @param k     k顶点的位序
 * @param visit
 * */
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

/**
 * @brief 深度优先遍历实现函数
 * 
 * @param G 图
 * @param k 当前邻接头
 * @param visit 访问函数
 * @return Status 成功返回OK，否则返回ERROR
 */
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

/**
 * @brief 深度优先遍历
 * 
 * @param G 图
 * @param visit 访问函数
 * @return Status 成功返回OK，否则返回ERROR
 */
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

/**
 * @brief 广度优先遍历
 * 
 * @param G 图
 * @param visit 访问函数
 * @return Status 成功返回OK，否则返回ERROR
 */
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

/**
 * @brief 查看节点是否可达
 * 
 * @param G 图
 * @param s 节点s
 * @param t 节点t
 * @return Status 成功返回OK，否则返回ERROR
 */
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

/**
 * @brief 打印简单路径
 * 
 * @param G 图
 * @param s 顶点s
 * @param t 
 * @param visit 访问函数
 * @return Status 成功返回TRUE，否则返回ERROR
 */
Status SimplePath_DFS(LGraph G, int s, int t, Status ( *visit)(LGraph, int)){
    Status found = FALSE;
    PtrToAdjVNode p = NULL;
    Visited[s] = VISITED;
    if (s == t){
        found = TRUE;
    } else {
        for (int i = FirstAdjVex_L(G, s, &p); i >= 0 && found==FALSE ; i = NextAdjVex_L(G, s, &p)){
            if (UNVISITED == Visited[i]) {
                found = SimplePath_DFS(G, i, t, visit);
            }
        }
    }
    if (found == TRUE) {
        visit(G, s);
        return TRUE;
    } else{
        return ERROR;
    }
}

/**
 * @brief 广度优先遍历查询最短路径
 * 
 * @param G 图
 * @param s 起始节点
 * @param D 存储最短路径的数组
 */
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

// ************** 以下为测试函数 *****************

/**
 * @brief 测试函数
 * 
 */
void test_L(){

    printf("********************************************\n");
    printf("******** Linked list implementation ********\n");
    printf("********************************************\n");
    int nv = 5;
    int ne = 6;
    PtrToAdjVNode p = NULL;
    LGraph G = NULL;
    printf("start  init===\n");
    DataType *vexs = (DataType *)malloc(sizeof(DataType) * nv);
    vexs[0] = 'A';
    vexs[1] = 'B';
    vexs[2] = 'C';
    vexs[3] = 'D';
    vexs[4] = 'E';
    Edge *arcs = (Edge *)malloc(sizeof(Edge) * ne);
    for (int j = 0; j < ne;j++){
        arcs[j] = (Edge)malloc(sizeof(ENode));
    }
    arcs[0]->V1 = 0; 
    arcs[0]->V2 = 1; 
    arcs[0]->Weight = 1;
    arcs[1]->V1 = 0; 
    arcs[1]->V2 = 2; 
    arcs[1]->Weight = 1;
    arcs[2]->V1 = 1; 
    arcs[2]->V2 = 3; 
    arcs[2]->Weight = 1;
    arcs[3]->V1 = 2; 
    arcs[3]->V2 = 3; 
    arcs[3]->Weight = 1;
    arcs[4]->V1 = 2; 
    arcs[4]->V2 = 4; 
    arcs[4]->Weight = 1;
    arcs[5]->V1 = 3; 
    arcs[5]->V2 = 4; 
    arcs[5]->Weight = 1;
    G = InitGraph_L(G, vexs, nv, arcs, ne);
    printf("finish init===\n");
    if(NULL == G){
        return;
    }
    Edge edge = (Edge)malloc(sizeof(ENode));
    edge->V1 = 4;
    edge->V2 = 3;
    edge->Weight = 1;
    AddArc_L(G, edge);                                 
    printf("find the first adjVex of 'A' : %c \n", G->G[FirstAdjVex_L(G, 0, &p)].Data); 
    printf("find the next adjVex of 'A' : %c  \n", G->G[NextAdjVex_L(G, 0, &p)].Data);   
    printf("find all adjvex of 'A' :\n");
    VisitAllAdjVex_L(G, 0, visit_L);         
    printf("DFS:\n");
    DFSTraverse_L(G, visit_L);
    printf("BFS:\n");
    BFSTraverse_L(G, visit_L);
    for (int i = 0; i < G->Nv; i++){
        Visited[i] = UNVISITED;
    }
    // check if reach
    if(TRUE == isReachable(G, 0, 3))        
        printf("Yes\n");
    for (int i = 0; i < G->Nv; i++){
        Visited[i] = UNVISITED;
    }
    printf("find simple road of 'E' to 'A': \n");
    SimplePath_DFS(G, 0, 4, visit_L);   
    // E点到A点的简单路径的倒序
    int *D = (int *)malloc(sizeof(int) * G->Nv);
    for (int i = 0; i < G->Nv; i++){
        Visited[i] = UNVISITED;
    }
    ShortestPathLength_BFS(G, 0, D);
    printf("\n");
    for (int i = 0; i < G->Nv; i++) {
        printf("the min distance of 'A' to '%c' : %d\n",G->G[i].Data, D[i]);
    }
}