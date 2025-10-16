#pragma once
#include "Vector.h" //������Ϊ���࣬������ջģ����
template <typename T> class Stack : public Vector<T> { //����������/ĩ����Ϊջ��/��
public: //size()��empty()�Լ��������Žӿڣ�����ֱ������
	void push(T const& e) { insert(size(), e); } //��ջ����Ч�ڽ���Ԫ����Ϊ������ĩԪ�ز���
	T pop() { return remove(size() - 1); } //��ջ����Ч��ɾ��������ĩԪ��
	T & top() { return (*this)[size() - 1]; } //ȡ����ֱ�ӷ���������ĩԪ��
};

void convert(Stack<char>& S, __int64 n, int base) { //ʮ������n��base���Ƶ�ת���������棩
	static char digit[] //0 < n, 1 < base <= 16���½����µ���λ���ţ��ɹ�baseȡֵ��Χ�ʵ�����
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (n > 0) { //�ɵ͵��ߣ���һ������½����µĸ���λ
		int remainder = (int)(n % base); S.push(digit[remainder]); //��������ǰλ����ջ
		n /= base; //n����Ϊ���base�ĳ���
	}
} //�½������ɸߵ��͵ĸ���λ���Զ����±�����ջS��

void trim(const char exp[], int& lo, int& hi) { //ɾ�����ʽexp[lo, hi]�������ŵ��ǰ׺����׺
	while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')')) lo++; //���ҵ�һ����
	while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')')) hi--; //���һ������
}
int divide(const char exp[], int lo, int hi) { //�зֱ��ʽexp[lo, hi]��ʹexpƥ���������ƥ��
	int mi = lo; int crc = 1; //crcΪ[lo, mi]��Χ������������Ŀ֮��
	while ((0 < crc) && (++mi < hi)) //��������ַ���ֱ������������Ŀ��ȣ�����Խ��
		{ if (exp[mi] == ')') crc--; if (exp[mi] == '(') crc++; } //�������ŷֱ����
	return mi; //��mi <= hi����Ϊ�Ϸ��зֵ㣻������ζ�žֲ�������ƥ��
}
bool paren(const char exp[], int lo, int hi) { //�����ʽexp[lo, hi]�Ƿ�����ƥ�䣨�ݹ�棩
	trim(exp, lo, hi); if (lo > hi) return true; //����������ŵ�ǰ׺����׺
	if (exp[lo] != '(') return false; //���ַ��������ţ���ز�ƥ��
	if (exp[hi] != ')') return false; //ĩ�ַ��������ţ���ز�ƥ��
	int mi = divide(exp, lo, hi); //ȷ���ʵ����зֵ�
	if (mi > hi) return false; //�зֵ㲻�Ϸ�����ζ�žֲ��������岻ƥ��
	return paren(exp, lo + 1, mi - 1) && paren(exp, mi + 1, hi); //�ֱ��������ӱ��ʽ
}

bool paren(const char exp[], int lo, int hi) { //���ʽ����ƥ���飬�ɼ����������
	Stack<char> S; //ʹ��ջ��¼�ѷ��ֵ���δƥ���������
	for (int i = 0; exp[i]; i++) /* ��һ��鵱ǰ�ַ� */
		switch (exp[i]) { //������ֱ�ӽ�ջ������������ջ��ʧ�䣬����ʽ�ز�ƥ��
		case '(': case '[': case '{': S.push(exp[i]); break;
		case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
		case ']': if ((S.empty()) || ('[' != S.pop())) return false; break;
		case '}': if ((S.empty()) || ('{' != S.pop())) return false; break;
		default: break; //�������ַ�һ�ɺ���
	}
	return S.empty(); //�������ʽɨ�����ջ�����Բ����������ţ���ƥ�䣻����ջ�գ�ƥ��
}
/*
float evaluate(char* S, char*& RPN) { //�ԣ����޳��׿ո�ģ����ʽS��ֵ����ת��Ϊ�沨��ʽRPN
	Stack<float> opnd; Stack<char> optr; //������ջ�������ջ
	optr.push('\0'); //β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
	while (!optr.empty()) { //�������ջ�ǿ�֮ǰ�����������ʽ�и��ַ�
		if (isdigit(*S)) { //����ǰ�ַ�Ϊ����������
			readNumber(S, opnd); append(RPN, opnd.top()); //��������������������RPNĩβ
		}
		else //����ǰ�ַ�Ϊ���������
			switch (orderBetween(optr.top(), *S)) { //���䲻ջ�������֮�����ȼ��ߵͷֱ���
			case '<': //ջ����������ȼ�����ʱ
				optr.push(*S); S++; //�����Ƴ٣���ǰ�������ջ
				break;
			case '=': //���ȼ���ȣ���ǰ�����Ϊ�����Ż���β���ڱ�'\0'��ʱ
				optr.pop(); S++; //�����Ų�������һ���ַ�
				break;
			case '>': { //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
				char op = optr.pop(); append(RPN, op); //ջ���������ջ��������RPNĩβ
				if ('!' == op) { //����ءһԪ�����
					float pOpnd = opnd.pop(); //���腨��һ������������
					opnd.push(calcu(op, pOpnd)); //ʵ��һԪ���㣬�����ջ
				}
				else { //��ء��������Ԫ�������
					float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //������ǰ������
					opnd.push(calcu(pOpnd1, op, pOpnd2)); //ʵʩ��Ԫ���㣬�����ջ
				}
				break;
			}
			default: exit(-1); //���﷨���󣬲�������ֱ���˳�
		}//switch
	}//while
	return opnd.pop(); //������ކ�����ļ�����
}*/