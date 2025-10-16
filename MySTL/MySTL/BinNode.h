#pragma once

#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) ( \
IsLChild(*(p)) ? \
(p)->parent->rChild : \
(p)->parent->lChild \
) //兄弟
#define uncle(x) (\
IsLChild(*((x)->parent)) ? \
(x)->parent->parent->rChild : \
(x)->parent->parent->lChild \
) //叔叔
#define FromParentTo(x) (\
IsRoot(x) ? _root : ( \
IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \
) \
) //来自父亲癿指针
#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（不“空树高度为-1”的约定相统一）
typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色
template <typename T> struct BinNode { //二叉树节点模板类
	// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
		T data; //数值
	BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild; //父节点及左、右孩子
	int height; //高度（通用）
	int npl; //Null Path Length（左式堆，也可直接用height代替）
	RBColor color; //颜色（红黑树）
	// 构造函数
		BinNode() : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
		int h = 0, int l = 1, RBColor c = RB_RED)
		: data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) { }
	// 操作接口
		int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
	BinNodePosi(T) insertAsLC(T const&); //作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&); //作为当前节点的右孩子插入新节点
	BinNodePosi(T) succ(); //叏当前节点的直接后继
	template <typename VST> void travLevel(VST&); //子树局次遍历
	template <typename VST> void travPre(VST&); //子树先序遍历
	template <typename VST> void travIn(VST&); //子树中序遍历
	template <typename VST> void travPost(VST&); //子树后序遍历
	// 比较器、判等器（各列其一，其余自行补充）
		bool operator<(BinNode const& bn) { return data < bn.data; } //小于
	bool operator==(BinNode const& bn) { return data == bn.data; } //等于
};

template < typename T> //将e作为当前节点的左孩子插入二叉树
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }
template < typename T> //将e作为当前节点的右孩子插入二叉树
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符
const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
/* |-------------- 当前运算符 --------------| */
/* + - * / ^ ! ( ) \0 */
/* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* | - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* 栈 * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* 顶 / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* 运 ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
/* 算 ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
/* 符 ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
/* | ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
};
