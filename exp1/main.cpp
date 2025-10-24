#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include "MySTL/Vector.h"
#include "MySTL/Stack.h"

using namespace std;

// ==================== ����1��������������� ====================

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double modulus() const {
        return sqrt(real * real + imag * imag);
    }

    bool operator<(const Complex& other) const {
        double mod1 = modulus();
        double mod2 = other.modulus();
        if (fabs(mod1 - mod2) < 1e-9) {
            return real < other.real;
        }
        return mod1 < mod2;
    }

    bool operator==(const Complex& other) const {
        return fabs(real - other.real) < 1e-9 && fabs(imag - other.imag) < 1e-9;
    }

    bool operator>(const Complex& other) const {
        return !(*this < other) && !(*this == other);
    }

    bool operator<=(const Complex& other) const {
        return *this < other || *this == other;
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "(" << c.real << " + " << c.imag << "i)";
        return os;
    }

    double getReal() const { return real; }
    double getImag() const { return imag; }
};

Vector<Complex> findComplexByModulus(const Vector<Complex>& vec, double m1, double m2) {
    Vector<Complex> result;
    for (int i = 0; i < vec.size(); i++) {
        double mod = vec[i].modulus();
        if (mod >= m1 && mod < m2) {
            result.insert(result.size(), vec[i]);
        }
    }
    return result;
}

// ==================== ����2�����ʽ������ ====================

#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;

const char pri[N_OPTR][N_OPTR] = {
    /*           +    -    *    /    ^    !    (    )   \0 */
    /* + */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* - */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* * */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* / */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* ^ */    '>', '>', '>', '>', '>', '<', '<', '>', '>',
    /* ! */    '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /* ( */    '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /* ) */    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* \0 */   '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

Operator charToOperator(char c) {
    switch (c) {
    case '+': return ADD;
    case '-': return SUB;
    case '*': return MUL;
    case '/': return DIV;
    case '^': return POW;
    case '!': return FAC;
    case '(': return L_P;
    case ')': return R_P;
    case '\0': return EOE;
    default: return EOE;
    }
}

long long factorial(int n) {
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double calculate(double a, char op, double b = 0) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (fabs(b) < 1e-9) {
            throw runtime_error("Division by zero");
        }
        return a / b;
    case '^': return pow(a, b);
    default: throw runtime_error("Unknown operator");
    }
}

// ��ѧ��������
double calculateMathFunction(const string& func, double arg) {
    if (func == "sin") return sin(arg);
    if (func == "cos") return cos(arg);
    if (func == "tan") return tan(arg);
    if (func == "log") {
        if (arg <= 0) throw runtime_error("Logarithm of non-positive number");
        return log(arg);
    }
    if (func == "sqrt") {
        if (arg < 0) throw runtime_error("Square root of negative number");
        return sqrt(arg);
    }
    throw runtime_error("Unknown function: " + func);
}

double evaluateExpression(const string& expression) {
    Stack<double> opnd;
    Stack<char> optr;

    optr.push('\0');

    size_t i = 0;
    while (!optr.empty()) {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        // �����ѧ����
        if (isalpha(expression[i])) {
            string func;
            while (i < expression.length() && isalpha(expression[i])) {
                func += expression[i++];
            }

            // ����������
            while (i < expression.length() && isspace(expression[i])) i++;
            if (i >= expression.length() || expression[i] != '(') {
                throw runtime_error("Function call missing opening parenthesis");
            }
            i++; // ���� '('

            // ��������
            string argStr;
            int parenCount = 1;
            while (i < expression.length() && parenCount > 0) {
                if (expression[i] == '(') parenCount++;
                else if (expression[i] == ')') parenCount--;

                if (parenCount > 0) {
                    argStr += expression[i];
                }
                i++;
            }

            if (parenCount != 0) {
                throw runtime_error("Unmatched parentheses in function call");
            }

            // �ݹ�������
            double arg = evaluateExpression(argStr);
            double result = calculateMathFunction(func, arg);
            opnd.push(result);
            continue;
        }

        if (isdigit(expression[i]) || expression[i] == '.') {
            string numStr;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            double num = stod(numStr);
            opnd.push(num);
        }
        else if (expression[i] == '-' && (i == 0 || expression[i - 1] == '(')) {
            i++;
            string numStr = "-";
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            double num = stod(numStr);
            opnd.push(num);
        }
        else {
            char currentOp = expression[i];
            char topOp = optr.top();

            Operator current = charToOperator(currentOp);
            Operator top = charToOperator(topOp);

            char priority = pri[top][current];

            switch (priority) {
            case '<':
                optr.push(currentOp);
                i++;
                break;

            case '=':
                optr.pop();
                i++;
                break;

            case '>': {
                optr.pop();
                if (topOp == '!') {
                    if (opnd.empty()) throw runtime_error("Invalid expression");
                    double a = opnd.pop();
                    if (a < 0 || fabs(a - round(a)) > 1e-9) {
                        throw runtime_error("Factorial requires non-negative integer");
                    }
                    opnd.push(static_cast<double>(factorial(static_cast<int>(round(a)))));
                }
                else {
                    if (opnd.size() < 2) throw runtime_error("Invalid expression");
                    double b = opnd.pop();
                    double a = opnd.pop();
                    opnd.push(calculate(a, topOp, b));
                }
                break;
            }

            default:
                throw runtime_error("Invalid expression");
            }
        }
    }

    if (opnd.size() != 1) {
        throw runtime_error("Invalid expression");
    }

    return opnd.pop();
}

// ==================== ����3����״ͼ��������� ====================

int largestRectangleArea(Vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;

    Stack<int> st;
    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        int currentHeight = (i == n) ? 0 : heights[i];

        while (!st.empty() && currentHeight < heights[st.top()]) {
            int height = heights[st.pop()];
            int width = st.empty() ? i : i - st.top() - 1;
            maxArea = max(maxArea, height * width);
        }

        st.push(i);
    }

    return maxArea;
}

// �򵥵�ð������ʵ�֣���ΪVector�е�bubbleSort��protected��
template <typename T>
void bubbleSort(Vector<T>& vec, int lo, int hi) {
    if (hi - lo < 2) return;
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = lo + 1; i < hi; i++) {
            if (vec[i - 1] > vec[i]) {
                swap(vec[i - 1], vec[i]);
                sorted = false;
            }
        }
        hi--;
    }
}

// ==================== ���Ժ��� ====================

void testComplexOperations() {
    cout << "=== ���Ը������� ===" << endl;

    Vector<Complex> complexVec;
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < 10; i++) {
        double real = rand() % 10 - 5;
        double imag = rand() % 10 - 5;
        complexVec.insert(complexVec.size(), Complex(real, imag));
    }

    complexVec.insert(complexVec.size(), Complex(1, 1));
    complexVec.insert(complexVec.size(), Complex(1, 1));

    cout << "ԭʼ����: ";
    for (int i = 0; i < complexVec.size(); i++) {
        cout << complexVec[i] << " ";
    }
    cout << endl;

    // ���Բ��ҹ���
    Complex target(1, 1);
    int foundIndex = complexVec.find(target);
    if (foundIndex >= 0) {
        cout << "���Ҹ���(1+1i): �ҵ����±�Ϊ " << foundIndex << endl;
    }
    else {
        cout << "���Ҹ���(1+1i): δ�ҵ�" << endl;
    }

    complexVec.unsort();
    cout << "�����: ";
    for (int i = 0; i < complexVec.size(); i++) {
        cout << complexVec[i] << " ";
    }
    cout << endl;

    // �ٴβ��Բ���
    foundIndex = complexVec.find(target);
    if (foundIndex >= 0) {
        cout << "�������Ҹ���(1+1i): �ҵ����±�Ϊ " << foundIndex << endl;
    }
    else {
        cout << "�������Ҹ���(1+1i): δ�ҵ�" << endl;
    }

    complexVec.insert(3, Complex(10, 10));
    cout << "������С: " << complexVec.size() << endl;

    complexVec.remove(3);
    cout << "ɾ�����С: " << complexVec.size() << endl;

    int removedCount = complexVec.deduplicate();
    cout << "ȥ�غ�ɾ�� " << removedCount << " ���ظ�Ԫ��" << endl;
}

void testSortingPerformance() {
    cout << "\n=== ������������ ===" << endl;

    const int TEST_SIZE = 500;

    // ����˳��������������
    Vector<Complex> sortedVec, reversedVec, randomVec;

    // ˳������
    for (int i = 0; i < TEST_SIZE; i++) {
        sortedVec.insert(sortedVec.size(), Complex(i, i));
    }

    // ��������
    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        reversedVec.insert(reversedVec.size(), Complex(i, i));
    }

    // ��������
    for (int i = 0; i < TEST_SIZE; i++) {
        randomVec.insert(randomVec.size(), Complex(i, i));
    }
    randomVec.unsort();

    // ���Թ鲢��������
    Vector<Complex> testVec1 = sortedVec;
    clock_t start = clock();
    testVec1.sort();
    clock_t end = clock();
    cout << "˳�������鲢����ʱ��: " << (double)(end - start) / CLOCKS_PER_SEC << "��" << endl;

    Vector<Complex> testVec2 = reversedVec;
    start = clock();
    testVec2.sort();
    end = clock();
    cout << "���������鲢����ʱ��: " << (double)(end - start) / CLOCKS_PER_SEC << "��" << endl;

    Vector<Complex> testVec3 = randomVec;
    start = clock();
    testVec3.sort();
    end = clock();
    cout << "���������鲢����ʱ��: " << (double)(end - start) / CLOCKS_PER_SEC << "��" << endl;

    // ����ð���������ܣ�ʹ���ⲿ������
    Vector<Complex> testVec4 = sortedVec;
    start = clock();
    bubbleSort(testVec4, 0, testVec4.size());
    end = clock();
    cout << "˳������ð������ʱ��: " << (double)(end - start) / CLOCKS_PER_SEC << "��" << endl;

    Vector<Complex> testVec5 = reversedVec;
    start = clock();
    bubbleSort(testVec5, 0, testVec5.size());
    end = clock();
    cout << "��������ð������ʱ��: " << (double)(end - start) / CLOCKS_PER_SEC << "��" << endl;

    Vector<Complex> testVec6 = randomVec;
    start = clock();
    bubbleSort(testVec6, 0, testVec6.size());
    end = clock();
    cout << "��������ð������ʱ��: " << (double)(end - start) / CLOCKS_PER_SEC << "��" << endl;
}

void testRangeSearch() {
    cout << "\n=== ���Է�Χ���� ===" << endl;

    Vector<Complex> complexVec;
    for (int i = 0; i < 10; i++) {
        complexVec.insert(complexVec.size(), Complex(i, i));
    }

    complexVec.sort();

    cout << "����������: ";
    for (int i = 0; i < complexVec.size(); i++) {
        cout << complexVec[i] << " ";
    }
    cout << endl;

    Vector<Complex> result = findComplexByModulus(complexVec, 3, 7);
    cout << "ģ��[3, 7)��Χ�ڵĸ���: ";
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << endl;
}

void testCalculator() {
    cout << "\n=== ���Ա��ʽ������ ===" << endl;

    const char* expressions[] = {
        "2 + 3 * 4",
        "(2 + 3) * 4",
        "3 + 4 * 2",
        "5!",
        "2^3 + 4",
        "10 / 2",
        "sin(0)",
        "cos(0)",
        "log(1)",
        "sqrt(4)",
        "2 * sin(1.57) + 3 * cos(0)"
    };
    int numExpr = sizeof(expressions) / sizeof(expressions[0]);

    for (int i = 0; i < numExpr; i++) {
        try {
            string expr = expressions[i];
            double result = evaluateExpression(expr);
            cout << expr << " = " << result << endl;
        }
        catch (const exception& e) {
            cout << expressions[i] << " -> ����: " << e.what() << endl;
        }
    }
}

void testLargestRectangle() {
    cout << "\n=== ������״ͼ��������� ===" << endl;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int test = 1; test <= 3; test++) {
        int n = rand() % 10 + 5;
        Vector<int> heights;

        cout << "����" << test << " - ��������: ";
        for (int i = 0; i < n; i++) {
            int height = rand() % 10 + 1;
            heights.insert(heights.size(), height);
            cout << height << " ";
        }

        int maxArea = largestRectangleArea(heights);
        cout << "-> ���������: " << maxArea << endl;
    }
}

int main() {
    cout << "��ʼ�������й���..." << endl;

    try {
        testComplexOperations();
        testSortingPerformance();
        testRangeSearch();
        testCalculator();
        testLargestRectangle();
    }
    catch (const exception& e) {
        cout << "�������г���: " << e.what() << endl;
        return 1;
    }

    cout << "\n���в������!" << endl;
    return 0;
}