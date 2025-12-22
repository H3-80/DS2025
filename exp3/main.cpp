#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include "D:\work and study\person\C++\MySTL\MySTL\Graph.h"
#include "D:\work and study\person\C++\MySTL\MySTL\ggraph.h"
#include "D:\work and study\person\C++\MySTL\MySTL\Stack.h"
#include "D:\work and study\person\C++\MySTL\MySTL\Queue.h"
#include "D:\work and study\person\C++\MySTL\MySTL\Vector.h"

using namespace std;

// 图1的数据 - 根据您提供的连接信息修改
void buildGraph1(GraphMatrix<char, int>& graph) {
    // 插入顶点 A-H
    vector<int> vertices;
    for (char c = 'A'; c <= 'H'; c++) {
        vertices.push_back(graph.insert(c));
    }

    // 插入边（无向图，每个边插入两次）
    // 根据您提供的信息：
    // A-B:4, A-D:6, A-G:7, D-E:13, D-G:2, G-E:11, G-H:14, 
    // B-C:12, E-C:1, E-F:5, E-H:8, C-F:2, C-H:10, F-H:3

    // A的边
    graph.insert(0, 4, vertices[0], vertices[1]); // A-B:4
    graph.insert(0, 4, vertices[1], vertices[0]);
    graph.insert(0, 6, vertices[0], vertices[3]); // A-D:6
    graph.insert(0, 6, vertices[3], vertices[0]);
    graph.insert(0, 7, vertices[0], vertices[6]); // A-G:7
    graph.insert(0, 7, vertices[6], vertices[0]);

    // B的边
    graph.insert(0, 12, vertices[1], vertices[2]); // B-C:12
    graph.insert(0, 12, vertices[2], vertices[1]);

    // C的边
    graph.insert(0, 1, vertices[2], vertices[4]); // C-E:1
    graph.insert(0, 1, vertices[4], vertices[2]);
    graph.insert(0, 2, vertices[2], vertices[5]); // C-F:2
    graph.insert(0, 2, vertices[5], vertices[2]);
    graph.insert(0, 10, vertices[2], vertices[7]); // C-H:10
    graph.insert(0, 10, vertices[7], vertices[2]);

    // D的边
    graph.insert(0, 13, vertices[3], vertices[4]); // D-E:13
    graph.insert(0, 13, vertices[4], vertices[3]);
    graph.insert(0, 2, vertices[3], vertices[6]); // D-G:2
    graph.insert(0, 2, vertices[6], vertices[3]);

    // E的边
    graph.insert(0, 5, vertices[4], vertices[5]); // E-F:5
    graph.insert(0, 5, vertices[5], vertices[4]);
    graph.insert(0, 8, vertices[4], vertices[7]); // E-H:8
    graph.insert(0, 8, vertices[7], vertices[4]);

    // F的边
    graph.insert(0, 3, vertices[5], vertices[7]); // F-H:3
    graph.insert(0, 3, vertices[7], vertices[5]);

    // G的边
    graph.insert(0, 11, vertices[6], vertices[4]); // G-E:11
    graph.insert(0, 11, vertices[4], vertices[6]);
    graph.insert(0, 14, vertices[6], vertices[7]); // G-H:14
    graph.insert(0, 14, vertices[7], vertices[6]);
}

// 图2的数据 - 根据您提供的连接信息修改
void buildGraph2(GraphMatrix<char, int>& graph) {
    // 插入顶点 A-L
    vector<int> vertices;
    for (char c = 'A'; c <= 'L'; c++) {
        vertices.push_back(graph.insert(c));
    }

    // 根据您提供的信息插入边（无向图，每条边插入两次）：
    // A-B, A-E, B-F, C-F, C-H, C-D, D-H, E-F, E-I, F-G, F-J, F-K, F-I, G-K, J-K, K-L

    // A的边
    graph.insert(0, 1, vertices[0], vertices[1]); // A-B
    graph.insert(0, 1, vertices[1], vertices[0]);
    graph.insert(0, 1, vertices[0], vertices[4]); // A-E
    graph.insert(0, 1, vertices[4], vertices[0]);

    // B的边
    graph.insert(0, 1, vertices[1], vertices[5]); // B-F
    graph.insert(0, 1, vertices[5], vertices[1]);

    // C的边
    graph.insert(0, 1, vertices[2], vertices[5]); // C-F
    graph.insert(0, 1, vertices[5], vertices[2]);
    graph.insert(0, 1, vertices[2], vertices[7]); // C-H
    graph.insert(0, 1, vertices[7], vertices[2]);
    graph.insert(0, 1, vertices[2], vertices[3]); // C-D
    graph.insert(0, 1, vertices[3], vertices[2]);

    // D的边
    graph.insert(0, 1, vertices[3], vertices[7]); // D-H
    graph.insert(0, 1, vertices[7], vertices[3]);

    // E的边
    graph.insert(0, 1, vertices[4], vertices[5]); // E-F
    graph.insert(0, 1, vertices[5], vertices[4]);
    graph.insert(0, 1, vertices[4], vertices[8]); // E-I
    graph.insert(0, 1, vertices[8], vertices[4]);

    // F的边
    graph.insert(0, 1, vertices[5], vertices[6]); // F-G
    graph.insert(0, 1, vertices[6], vertices[5]);
    graph.insert(0, 1, vertices[5], vertices[9]); // F-J
    graph.insert(0, 1, vertices[9], vertices[5]);
    graph.insert(0, 1, vertices[5], vertices[10]); // F-K
    graph.insert(0, 1, vertices[10], vertices[5]);
    graph.insert(0, 1, vertices[5], vertices[8]); // F-I
    graph.insert(0, 1, vertices[8], vertices[5]);

    // G的边
    graph.insert(0, 1, vertices[6], vertices[10]); // G-K
    graph.insert(0, 1, vertices[10], vertices[6]);

    // J的边
    graph.insert(0, 1, vertices[9], vertices[10]); // J-K
    graph.insert(0, 1, vertices[10], vertices[9]);

    // K的边
    graph.insert(0, 1, vertices[10], vertices[11]); // K-L
    graph.insert(0, 1, vertices[11], vertices[10]);
}

// 打印邻接矩阵
void printAdjacencyMatrix(GraphMatrix<char, int>& graph) {
    cout << "邻接矩阵:" << endl;
    cout << "  ";
    for (int i = 0; i < graph.n; i++) {
        cout << graph.vertex(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < graph.n; i++) {
        cout << graph.vertex(i) << " ";
        for (int j = 0; j < graph.n; j++) {
            if (graph.exists(i, j)) {
                cout << graph.weight(i, j) << " ";
            }
            else {
                cout << "0 ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// 打印BFS遍历结果
void printBFSResult(GraphMatrix<char, int>& graph, int start) {
    cout << "从顶点 " << graph.vertex(start) << " 开始的BFS遍历顺序: ";
    graph.bfs(start);

    // 按照发现时间排序输出顶点
    Vector<pair<int, char>> vertices;
    for (int i = 0; i < graph.n; i++) {
        vertices.insert(vertices.size(), make_pair(graph.dTime(i), graph.vertex(i)));
    }

    // 按发现时间排序
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = i + 1; j < vertices.size(); j++) {
            if (vertices[i].first > vertices[j].first) {
                swap(vertices[i], vertices[j]);
            }
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        cout << vertices[i].second << " ";
    }
    cout << endl;
}

// 打印DFS遍历结果
void printDFSResult(GraphMatrix<char, int>& graph, int start) {
    cout << "从顶点 " << graph.vertex(start) << " 开始的DFS遍历顺序: ";
    graph.dfs(start);

    // 按照发现时间排序输出顶点
    Vector<pair<int, char>> vertices;
    for (int i = 0; i < graph.n; i++) {
        vertices.insert(vertices.size(), make_pair(graph.dTime(i), graph.vertex(i)));
    }

    // 按发现时间排序
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = i + 1; j < vertices.size(); j++) {
            if (vertices[i].first > vertices[j].first) {
                swap(vertices[i], vertices[j]);
            }
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        cout << vertices[i].second << " ";
    }
    cout << endl;
}

// Dijkstra优先级更新器
template <typename Tv, typename Te>
struct DijkstraPU {
    void operator()(Graph<Tv, Te>* g, int s, int w) {
        if (g->status(w) != VISITED) {
            if (g->priority(w) > g->priority(s) + g->weight(s, w)) {
                g->priority(w) = g->priority(s) + g->weight(s, w);
                g->parent(w) = s;
            }
        }
    }
};

// Prim优先级更新器
template <typename Tv, typename Te>
struct PrimPU {
    void operator()(Graph<Tv, Te>* g, int s, int w) {
        if (g->status(w) != VISITED) {
            if (g->priority(w) > g->weight(s, w)) {
                g->priority(w) = g->weight(s, w);
                g->parent(w) = s;
            }
        }
    }
};

// 打印最短路径
void printShortestPaths(GraphMatrix<char, int>& graph, int start) {
    cout << "从顶点 " << graph.vertex(start) << " 到各顶点的最短路径:" << endl;

    // 使用Dijkstra算法
    graph.pfs(start, DijkstraPU<char, int>());

    for (int i = 0; i < graph.n; i++) {
        if (i != start) {
            cout << "到顶点 " << graph.vertex(i) << ": 距离=" << graph.priority(i);

            // 重建路径
            Vector<char> path;
            for (int v = i; v != -1; v = graph.parent(v)) {
                path.insert(0, graph.vertex(v));
            }

            cout << ", 路径: ";
            for (int j = 0; j < path.size(); j++) {
                cout << path[j] << " ";
            }
            cout << endl;
        }
    }
    cout << endl;
}

// 打印最小生成树
void printMST(GraphMatrix<char, int>& graph, int start) {
    cout << "从顶点 " << graph.vertex(start) << " 开始的最小生成树:" << endl;

    // 使用Prim算法
    graph.pfs(start, PrimPU<char, int>());

    int totalWeight = 0;
    for (int i = 0; i < graph.n; i++) {
        if (graph.parent(i) != -1) {
            cout << "边: " << graph.vertex(graph.parent(i)) << "-"
                << graph.vertex(i) << ", 权重: " << graph.weight(graph.parent(i), i) << endl;
            totalWeight += graph.weight(graph.parent(i), i);
        }
    }
    cout << "最小生成树总权重: " << totalWeight << endl << endl;
}

// 简化的双连通分量分析
void analyzeBiconnectedComponents(GraphMatrix<char, int>& graph, int start) {
    cout << "从顶点 " << graph.vertex(start) << " 开始的双连通分量分析:" << endl;

    // 执行DFS遍历
    graph.dfs(start);

    // 基于图结构的关节点分析
    Vector<char> articulationPoints;

    // 检查每个顶点是否为关节点
    for (int i = 0; i < graph.n; i++) {
        char currentVertex = graph.vertex(i);

        // 简化的判断：如果顶点有多个子树，则可能是关节点
        int componentCount = 0;
        for (int j = 0; j < graph.n; j++) {
            if (graph.parent(j) == i) {
                componentCount++;
            }
        }

        // 对于根节点，需要至少2个子树才是关节点
        // 对于非根节点，有子树就可能是关节点
        if ((i == start && componentCount >= 2) || (i != start && componentCount >= 1)) {
            // 进一步检查：如果移除该顶点会增加连通分量数量，则是关节点
            bool isArticulation = false;

            // 简化的判断：检查该顶点的度数
            int degree = graph.outDegree(i);
            if (degree > 1) {
                // 检查是否有回边绕过该顶点
                bool hasAlternativePath = false;
                for (int j = 0; j < graph.n && !hasAlternativePath; j++) {
                    if (graph.exists(i, j)) {
                        for (int k = 0; k < graph.n; k++) {
                            if (k != i && graph.exists(j, k) && graph.dTime(k) < graph.dTime(i)) {
                                hasAlternativePath = true;
                                break;
                            }
                        }
                    }
                }

                if (!hasAlternativePath) {
                    isArticulation = true;
                }
            }

            if (isArticulation) {
                articulationPoints.insert(articulationPoints.size(), currentVertex);
            }
        }
    }

    // 输出结果
    if (articulationPoints.size() > 0) {
        cout << "关节点: ";
        for (int i = 0; i < articulationPoints.size(); i++) {
            cout << articulationPoints[i] << " ";
        }
        cout << endl;
    }
    else {
        cout << "关节点: 无" << endl;
    }

    cout << endl;
}

int main() {
    cout << "===== 图1分析 =====" << endl;

    // 创建图1
    GraphMatrix<char, int> graph1;
    buildGraph1(graph1);

    // (1) 输出邻接矩阵
    cout << "图1的邻接矩阵:" << endl;
    printAdjacencyMatrix(graph1);

    // (2) BFS和DFS
    cout << "从A点出发的遍历:" << endl;
    printBFSResult(graph1, 0); // 从A点出发
    printDFSResult(graph1, 0);
    cout << endl;

    // (3) 最短路径和最小支撑树
    cout << "最短路径和最小支撑树:" << endl;
    printShortestPaths(graph1, 0);
    printMST(graph1, 0);

    cout << "===== 图2分析 =====" << endl;

    // 创建图2
    GraphMatrix<char, int> graph2;
    buildGraph2(graph2);

    // 输出图2的邻接矩阵以便验证
    cout << "图2的邻接矩阵:" << endl;
    printAdjacencyMatrix(graph2);

    // (4) 双连通分量分析（从不同起点）
    cout << "双连通分量分析:" << endl;

    // 从不同起点进行分析
    analyzeBiconnectedComponents(graph2, 0); // 从A开始
    analyzeBiconnectedComponents(graph2, 2); // 从C开始  
    analyzeBiconnectedComponents(graph2, 5); // 从F开始

    cout << "实验完成！" << endl;

    return 0;
}