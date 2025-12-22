#pragma once
#include <climits>
#include <stack>
#include <queue>

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; //边状态

template < typename Tv, typename Te> //顶点类型、边类型
class Graph { //图Graph模板类
private:
    void reset() { //所有顶点、边的辅助信息复位
        for (int i = 0; i < n; i++) { //所有顶点的
            status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; //状态，时间标签
            parent(i) = -1; priority(i) = INT_MAX; //（在遍历树中的）父节点，优先级数
            for (int j = 0; j < n; j++) //所有边的
                if (exists(i, j)) status(i, j) = UNDETERMINED; //状态
        }
    }
    void BFS(int, int&); //（连通域）广度优先搜索算法
    void DFS(int, int&); //（连通域）深度优先搜索算法
    void BCC(int, int&, std::stack<int>&); //（连通域）基于DFS的双连通分量分解算法
    bool TSort(int, int&, std::stack<Tv>*); //（连通域）基于DFS的拓扑排序算法
    template <typename PU> void PFS(int, PU); //（连通域）优先级搜索框架
public:
    // 顶点
    int n; //顶点总数
    virtual int insert(Tv const&) = 0; //插入顶点，返回编号
    virtual Tv remove(int) = 0; //删除顶点及其关联边，返回该顶点信息
    virtual Tv& vertex(int) = 0; //顶点v的数据（该顶点的确存在）
    virtual int inDegree(int) = 0; //顶点v的入度（该顶点的确存在）
    virtual int outDegree(int) = 0; //顶点v的出度（该顶点的确存在）
    virtual int firstNbr(int) = 0; //顶点v的首个邻接顶点
    virtual int nextNbr(int, int) = 0; //顶点v的（相对于顶点j的）下一邻接顶点
    virtual VStatus& status(int) = 0; //顶点v的状态
    virtual int& dTime(int) = 0; //顶点v的时间标签dTime
    virtual int& fTime(int) = 0; //顶点v的时间标签fTime
    virtual int& parent(int) = 0; //顶点v在遍历树中的父亲
    virtual int& priority(int) = 0; //顶点v在遍历树中的优先级数
    // 边：这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
    int e; //边总数
    virtual bool exists(int, int) = 0; //边(v, u)是否存在
    virtual void insert(Te const&, int, int, int) = 0; //在顶点v和u之间插入权重为w的边e
    virtual Te remove(int, int) = 0; //删除顶点v和u之间的边e，返回该边信息
    virtual EStatus& status(int, int) = 0; //边(v, u)的状态
    virtual Te& edge(int, int) = 0; //边(v, u)的数据（该边的确存在）
    virtual int& weight(int, int) = 0; //边(v, u)的权重
    // 算法
    void bfs(int); //广度优先搜索算法
    void dfs(int); //深度优先搜索算法
    void bcc(int); //基于DFS的双连通分量分解算法
    std::stack<Tv>* tSort(int); //基于DFS的拓扑排序算法
    void prim(int); //最小支撑树Prim算法
    void dijkstra(int); //最短路径Dijkstra算法
    template <typename PU> void pfs(int, PU); //优先级搜索框架
};

// 广度优先搜索BFS算法（全图）
template < typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s) {
    reset(); int clock = 0; int v = s; //初始化
    do //逐一检查所有顶点
        if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
            BFS(v, clock); //即从该顶点出发启动一次BFS
    while (s != (v = (++v % n))); //按序号检查，故不漏不重
}

// 广度优先搜索BFS算法（单个连通域）
template < typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int& clock) {
    std::queue<int> Q; //引入辅助队列
    status(v) = DISCOVERED; Q.push(v); //初始化起点
    while (!Q.empty()) { //在Q变空之前，不断
        int v = Q.front(); Q.pop(); dTime(v) = ++clock; //取出队首顶点v
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
            if (UNDISCOVERED == status(u)) { //若u尚未被发现，则
                status(u) = DISCOVERED; Q.push(u); //发现该顶点
                status(v, u) = TREE; parent(u) = v; //引入树边拓展支撑树
            }
            else { //若u已被发现，或者甚至已访问完毕，则
                status(v, u) = CROSS; //将(v, u)归类于跨边
            }
        status(v) = VISITED; //至此，当前顶点访问完毕
    }
}

// 深度优先搜索DFS算法（全图）
template < typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
    reset(); int clock = 0; int v = s; //初始化
    do //逐一检查所有顶点
        if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
            DFS(v, clock); //即从该顶点出发启动一次DFS
    while (s != (v = (++v % n))); //按序号检查，故不漏不重
}

// 深度优先搜索DFS算法（单个连通域）
template < typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
    dTime(v) = ++clock; status(v) = DISCOVERED; //发现当前顶点v
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
        switch (status(u)) { //并视其状态分别处理
        case UNDISCOVERED: //u尚未发现，意味着支撑树可在此拓展
            status(v, u) = TREE; parent(u) = v; DFS(u, clock); break;
        case DISCOVERED: //u已被发现但尚未访问完毕，应属被后代指向的祖先
            status(v, u) = BACKWARD; break;
        default: //u已访问完毕（VISITED，有向图），则视承袭关系分为前向边或跨边
            status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
        }
    status(v) = VISITED; fTime(v) = ++clock; //至此，当前顶点v方告访问完毕
}

// 基于DFS的拓扑排序算法
template < typename Tv, typename Te>
std::stack<Tv>* Graph<Tv, Te>::tSort(int s) {
    reset(); int clock = 0; int v = s;
    std::stack<Tv>* S = new std::stack<Tv>; //用栈记录排序顶点
    do {
        if (UNDISCOVERED == status(v))
            if (!TSort(v, clock, S)) { //clock并非必需
                while (!S->empty()) //任一连通域（亦即整图）非DAG
                    S->pop(); break; //则不必继续计算，故直接返回
            }
    } while (s != (v = (++v % n)));
    return S; //若输入为DAG，则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
}

// 基于DFS的拓扑排序算法（单趟）
template < typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int& clock, std::stack<Tv>* S) {
    dTime(v) = ++clock; status(v) = DISCOVERED; //发现顶点v
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
        switch (status(u)) { //并视u的状态分别处理
        case UNDISCOVERED:
            parent(u) = v; status(v, u) = TREE;
            if (!TSort(u, clock, S)) //从顶点u处出发深入搜索
                return false; //若u及其后代不能拓扑排序（则全图亦必如此），故返回并报告
            break;
        case DISCOVERED:
            status(v, u) = BACKWARD; //一旦发现后向边（非DAG），则
            return false; //不必深入，故返回并报告
        default: //VISITED (digraphs only)
            status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITED; S->push(vertex(v)); //顶点被标记为VISITED时，随即入栈
    return true; //v及其后代可以拓扑排序
}

// 优先级搜索（全图）
template <typename Tv, typename Te> template < typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {
    reset(); int v = s; //初始化
    do //逐一检查所有顶点
        if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
            PFS(v, prioUpdater); //即从该顶点出发启动一次PFS
    while (s != (v = (++v % n))); //按序号检查，故不漏不重
}

// 优先级搜索（单个连通域）
template <typename Tv, typename Te> template < typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater) {
    priority(s) = 0; status(s) = VISITED; parent(s) = -1; //初始化，起点s加至PFS树中
    while (1) { //将下一顶点和边加至PFS树中
        for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) //枚举s的所有邻居w
            prioUpdater(this, s, w); //更新顶点w的优先级及其父顶点
        for (int shortest = INT_MAX, w = 0; w < n; w++)
            if (status(w) == UNDISCOVERED) //从尚未加入遍历树的顶点中
                if (shortest > priority(w)) //选出下一个
                {
                    shortest = priority(w); s = w;
                } //优先级最高的顶点s
        if (VISITED == status(s)) break;
        status(s) = VISITED; status(parent(s), s) = TREE; //将s及其与父顶点的联边加入遍历树
    }
}

// 为未实现的算法提供空实现
template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int) {
    // 双连通分量分解算法 - 待实现
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::prim(int) {
    // 最小支撑树Prim算法 - 待实现
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(int) {
    // 最短路径Dijkstra算法 - 待实现
}