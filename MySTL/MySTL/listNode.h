#pragma once
typedef int Rank;
#define ListNodePosi(T) ListNode<T>*

template <typename T> struct ListNode {
	T data; ListNode(T) pred; ListNode(T) succ;
	//���캯��
	ListNode() {}	//���header��trailer�Ĺ���
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		:data (e), pred (p), succ (s) { }

	ListNodePosi(T) insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);
};
