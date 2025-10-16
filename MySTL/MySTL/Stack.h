#pragma once
#include "Vector.h" //以向量为基类，派生出栈模板类
template <typename T> class Stack : public Vector<T> { //将向量的首/末端作为栈底/顶
public: //size()、empty()以及其它开放接口，均可直接沿用
	void push(T const& e) { insert(size(), e); } //入栈：等效于将新元素作为向量的末元素插入
	T pop() { return remove(size() - 1); } //出栈：等效于删除向量的末元素
	T & top() { return (*this)[size() - 1]; } //取顶：直接返回向量的末元素
};

void convert(Stack<char>& S, __int64 n, int base) { //十进制数n到base进制的转换（迭代版）
	static char digit[] //0 < n, 1 < base <= 16，新进制下的数位符号，可规base取值范围适当扩充
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (n > 0) { //由低到高，逐一计算出新进制下的各数位
		int remainder = (int)(n % base); S.push(digit[remainder]); //余数（当前位）入栈
		n /= base; //n更新为其对base的除商
	}
} //新进制下由高到低的各数位，自顶而下保存于栈S中

void trim(const char exp[], int& lo, int& hi) { //删除表达式exp[lo, hi]不含括号的最长前缀、后缀
	while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')')) lo++; //查找第一个和
	while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')')) hi--; //最后一个括号
}
int divide(const char exp[], int lo, int hi) { //切分表达式exp[lo, hi]，使exp匹配仅当它们匹配
	int mi = lo; int crc = 1; //crc为[lo, mi]范围内左、右括号数目之差
	while ((0 < crc) && (++mi < hi)) //逐个检查各字符，直到左、右括号数目相等，或者越界
		{ if (exp[mi] == ')') crc--; if (exp[mi] == '(') crc++; } //左、右括号分别计数
	return mi; //若mi <= hi，则为合法切分点；否则，意味着局部不可能匹配
}
bool paren(const char exp[], int lo, int hi) { //检查表达式exp[lo, hi]是否括号匹配（递归版）
	trim(exp, lo, hi); if (lo > hi) return true; //清除不含括号的前缀、后缀
	if (exp[lo] != '(') return false; //首字符非左括号，则必不匹配
	if (exp[hi] != ')') return false; //末字符非右括号，则必不匹配
	int mi = divide(exp, lo, hi); //确定适当的切分点
	if (mi > hi) return false; //切分点不合法，意味着局部以至整体不匹配
	return paren(exp, lo + 1, mi - 1) && paren(exp, mi + 1, hi); //分别检查左、右子表达式
}

bool paren(const char exp[], int lo, int hi) { //表达式括号匹配检查，可兼顾三种括号
	Stack<char> S; //使用栈记录已发现但尚未匹配的左括号
	for (int i = 0; exp[i]; i++) /* 逐一检查当前字符 */
		switch (exp[i]) { //左括号直接进栈；右括号若与栈顶失配，则表达式必不匹配
		case '(': case '[': case '{': S.push(exp[i]); break;
		case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
		case ']': if ((S.empty()) || ('[' != S.pop())) return false; break;
		case '}': if ((S.empty()) || ('{' != S.pop())) return false; break;
		default: break; //非括号字符一律忽略
	}
	return S.empty(); //整个表达式扫描过后，栈中若仍残留（左）括号，则不匹配；否则（栈空）匹配
}
/*
float evaluate(char* S, char*& RPN) { //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
	Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈
	optr.push('\0'); //尾哨兵'\0'也作为头哨兵首先入栈
	while (!optr.empty()) { //在运算符栈非空之前，逐个处理表达式中各字符
		if (isdigit(*S)) { //若当前字符为操作数，则
			readNumber(S, opnd); append(RPN, opnd.top()); //读入操作数，并将其接至RPN末尾
		}
		else //若当前字符为运算符，则
			switch (orderBetween(optr.top(), *S)) { //规其不栈顶运算符之间优先级高低分别处理
			case '<': //栈顶运算符优先级更低时
				optr.push(*S); S++; //计算推迟，当前运算符进栈
				break;
			case '=': //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
				optr.pop(); S++; //脱括号并接收下一个字符
				break;
			case '>': { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
				char op = optr.pop(); append(RPN, op); //栈顶运算符出栈并续接至RPN末尾
				if ('!' == op) { //若属亍一元运算符
					float pOpnd = opnd.pop(); //叧需叏出一个操作数，并
					opnd.push(calcu(op, pOpnd)); //实斲一元计算，结果入栈
				}
				else { //对亍其它（二元）运算符
					float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //叏出后、前操作数
					opnd.push(calcu(pOpnd1, op, pOpnd2)); //实施二元计算，结果入栈
				}
				break;
			}
			default: exit(-1); //逢语法错误，不做处理直接退出
		}//switch
	}//while
	return opnd.pop(); //弹出并迒回最后的计算结果
}*/