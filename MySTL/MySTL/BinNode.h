#pragma once

#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //����ӵ��һ������
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //ͬʱӵ����������
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) ( \
IsLChild(*(p)) ? \
(p)->parent->rChild : \
(p)->parent->lChild \
) //�ֵ�
#define uncle(x) (\
IsLChild(*((x)->parent)) ? \
(x)->parent->parent->rChild : \
(x)->parent->parent->lChild \
) //����
#define FromParentTo(x) (\
IsRoot(x) ? _root : ( \
IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \
) \
) //���Ը��װmָ��
#define BinNodePosi(T) BinNode<T>* //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ����������߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ
template <typename T> struct BinNode { //�������ڵ�ģ����
	// ��Ա��Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ��
		T data; //��ֵ
	BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild; //���ڵ㼰���Һ���
	int height; //�߶ȣ�ͨ�ã�
	int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
	RBColor color; //��ɫ���������
	// ���캯��
		BinNode() : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
		int h = 0, int l = 1, RBColor c = RB_RED)
		: data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) { }
	// �����ӿ�
		int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ(); //����ǰ�ڵ��ֱ�Ӻ��
	template <typename VST> void travLevel(VST&); //�����ִα���
	template <typename VST> void travPre(VST&); //�����������
	template <typename VST> void travIn(VST&); //�����������
	template <typename VST> void travPost(VST&); //�����������
	// �Ƚ������е�����������һ���������в��䣩
		bool operator<(BinNode const& bn) { return data < bn.data; } //С��
	bool operator==(BinNode const& bn) { return data == bn.data; } //����
};

template < typename T> //��e��Ϊ��ǰ�ڵ�����Ӳ��������
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }
template < typename T> //��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

#define N_OPTR 9 //���������
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //���������
//�ӡ������ˡ������˷����׳ˡ������š������š���ʼ������ֹ��
const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ� [ջ��] [��ǰ]
/* |-------------- ��ǰ����� --------------| */
/* + - * / ^ ! ( ) \0 */
/* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* | - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* ջ * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* �� / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* �� ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
/* �� ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
/* �� ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
/* | ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
};
