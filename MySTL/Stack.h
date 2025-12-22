#pragma once
#include "Vector.h"
#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

template <typename T> class Stack : public Vector<T> {
public:
    using Vector<T>::size;
    using Vector<T>::insert;
    using Vector<T>::remove;

    void push(T const& e) { this->insert(this->size(), e); }
    T pop() { return this->remove(this->size() - 1); }
    T& top() { return (*this)[this->size() - 1]; }
};

// 判断是否为运算符
bool isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 判断是否为运算符（字符串版本）
bool isOp(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "^";
}

// 判断是否为函数
bool isFunction(const string& s) {
    return s == "sin" || s == "cos" || s == "tan" || s == "log" || s == "ln" || s == "sqrt";
}

// 获取运算符优先级
int opPri(char op) {
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^': return 3;
    default: return 0;
    }
}

// 计算数学函数
double calculateFunction(const string& func, double arg) {
    if (func == "sin") return sin(arg);
    if (func == "cos") return cos(arg);
    if (func == "tan") return tan(arg);
    if (func == "log") {
        if (arg <= 0) throw runtime_error("Logarithm of non-positive number");
        return log10(arg);
    }
    if (func == "ln") {
        if (arg <= 0) throw runtime_error("Logarithm of non-positive number");
        return log(arg);
    }
    if (func == "sqrt") {
        if (arg < 0) throw runtime_error("Square root of negative number");
        return sqrt(arg);
    }
    throw runtime_error("Unknown function: " + func);
}

// 中缀转后缀（逆波兰）
string toRPN(const string& expr) {
    Stack<string> ops;
    string output;

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];

        if (isspace(c)) continue;

        // 处理函数名
        if (isalpha(c)) {
            string func;
            while (i < expr.length() && isalpha(expr[i])) {
                func += expr[i++];
            }
            ops.push(func);
            i--; // 回退一个字符，因为循环会i++
            continue;
        }

        if (isdigit(c) || c == '.') {
            // 处理数字
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                output += expr[i++];
            }
            output += ' ';
            i--;
        }
        else if (c == '(') {
            ops.push(string(1, c));
        }
        else if (c == ')') {
            while (!ops.empty() && ops.top() != "(") {
                output += ops.pop();
                output += ' ';
            }
            if (!ops.empty()) ops.pop(); // 弹出 '('
            // 如果栈顶是函数，也输出
            if (!ops.empty() && isFunction(ops.top())) {
                output += ops.pop();
                output += ' ';
            }
        }
        else if (isOp(c)) {
            // 处理负号
            if (c == '-' && (i == 0 || expr[i - 1] == '(')) {
                output += "0 "; // 为负号补0，变成 0 - x
            }

            while (!ops.empty() && ops.top() != "(" &&
                (isFunction(ops.top()) ||
                    (isOp(ops.top()[0]) && opPri(ops.top()[0]) >= opPri(c)))) {
                output += ops.pop();
                output += ' ';
            }
            ops.push(string(1, c));
        }
    }

    while (!ops.empty()) {
        output += ops.pop();
        output += ' ';
    }

    return output;
}

// 计算逆波兰表达式
double calcRPN(const string& rpn) {
    Stack<double> stk;
    stringstream ss(rpn);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            // 数字
            stk.push(stod(token));
        }
        else if (isOp(token)) {
            if (stk.size() < 2) throw runtime_error("Invalid RPN expression");

            double b = stk.pop();
            double a = stk.pop();
            double result = 0;

            switch (token[0]) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (fabs(b) < 1e-9) throw runtime_error("Division by zero");
                result = a / b;
                break;
            case '^': result = pow(a, b); break;
            }

            stk.push(result);
        }
        else if (isFunction(token)) {
            if (stk.empty()) throw runtime_error("Invalid RPN expression");

            double arg = stk.pop();
            double result = calculateFunction(token, arg);
            stk.push(result);
        }
    }

    if (stk.size() != 1) throw runtime_error("Invalid RPN expression");
    return stk.pop();
}

// 综合计算函数（中缀表达式直接求值）
double evalExpr(const string& expr) {
    string rpn = toRPN(expr);
    return calcRPN(rpn);
}