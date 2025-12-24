# DS2025
MySTL 数据结构与算法实验 📁
目录结构
DS2025/

├── MySTL/               # STL容器和算法实现

│   ├── BinNode.h        # 二叉树节点模板

│   ├── BinTree.h        # 二叉树模板

│   ├── Bitmap.h         # 位图实现
│   ├── ggraph.h         # 图邻接矩阵实现

│   ├── Graph.h          # 图算法框架

│   ├── huffman.h        # Huffman编码实现

│   ├── List.h           # 双向链表模板

│   ├── listNode.h       # 链表节点结构

│   ├── Queue.h          # 队列模板（基于链表）

│   ├── sort.h           # 排序算法实现

│   ├── Stack.h          # 栈模板（基于向量）

│   └── Vector.h         # 向量（动态数组）模板

├── exp1/                # 实验一：线性数据结构应用

│   └── main.cpp         # Huffman编码实现

├── exp2/                # 实验二：二叉树与表达式计算

│   └── main.cpp         # 复数运算、排序、表达式计算器

├── exp3/                # 实验三：图算法分析

│   └── main.cpp         # BFS、DFS、最短路径、最小生成树

├── exp4/                # 实验四：排序算法应用

│   └── main.cpp         # 非极大值抑制(NMS)算法性能分析

└── README.md            # 项目说明文档
🚀 快速开始
1. 编译运行
方法一：逐个编译
# 实验一：Huffman编码
g++ -std=c++11 -I./MySTL exp1/main.cpp -o exp1 && ./exp1

# 实验二：表达式计算器
g++ -std=c++11 -I./MySTL exp2/main.cpp -o exp2 && ./exp2

# 实验三：图算法
g++ -std=c++11 -I./MySTL exp3/main.cpp -o exp3 && ./exp3

# 实验四：排序与NMS
g++ -std=c++11 -I./MySTL exp4/main.cpp -o exp4 && ./exp4
方法二：CMake（推荐）
mkdir build && cd build
cmake .. && cmake --build .
# 运行可执行文件（Windows/Linux下路径不同）
📚 实验简介

实验一：线性数据结构
Huffman编码与解码
文本压缩与解压
使用Vector、List、Stack、Queue等

实验二：二叉树与表达式计算
复数运算与排序
增强版表达式计算器
支持三角函数、对数等运算

实验三：图算法
邻接矩阵表示法
BFS/DFS遍历
Dijkstra最短路径
Prim最小生成树

实验四：排序算法应用
四种排序算法对比
非极大值抑制(NMS)算法
性能分析与测试

⚠️ 注意事项
环境要求：C++11标准，g++ 5.0+ 或 MSVC 2015+
头文件路径：所有头文件在MySTL/目录下
数据文件：实验一需要104.txt文本文件（即演讲文稿）
编译命令：确保包含路径正确：-I./MySTL

🔧 技术栈
C++11
自定义STL容器
经典算法实现
跨平台（Windows/Linux/macOS）
