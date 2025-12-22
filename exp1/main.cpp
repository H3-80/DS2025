#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include "../MySTL/Vector.h"
#include "../MySTL/Stack.h"

using namespace std;

// ==================== 功能1：复数运算和排序 ====================

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

// ==================== 功能2：增强版表达式计算器测试 ====================

void testCalculator() {
    cout << "\n=== 测试增强版表达式计算器 ===" << endl;

    // 测试用例：包含复杂运算的中缀表达式
    const char* expressions[] = {
        // 基本运算
        "3 + 4",
        "5 + (1 + 2) * 4 - 3",
        "2 + 3 * 4",
        "15 / (7 - (1 + 1)) * 3 - (2 + (1 + 1))",
        "3 + 4 * 2 / (1 - 5) ^ 2",
        "10 / 2",
        "2 ^ 3",
        "(5 - 3) * 2",

        // 三角函数
        "sin(0)",
        "cos(0)",
        "tan(0)",
        "sin(3.14159 / 2)",
        "cos(3.14159)",
        "sin(3.14159 / 4) + cos(3.14159 / 4)",

        // 对数函数
        "log(100)",
        "ln(2.71828)",
        "log(1000) - ln(10)",

        // 平方根和幂运算
        "sqrt(16)",
        "sqrt(2) + sqrt(8)",
        "4 ^ 0.5",  // 应该等于 sqrt(4)

        // 混合复杂运算
        "sin(0) + cos(0) * sqrt(4)",
        "log(100) * 2 + ln(2.71828) ^ 2",
        "(sin(3.14159/6) + cos(3.14159/3)) * 2",
        "sqrt(sin(3.14159/2)^2 + cos(0)^2)",
        "log(100) / log(10) + ln(2.71828) / ln(2.71828)"
    };

    int numExpr = sizeof(expressions) / sizeof(expressions[0]);

    for (int i = 0; i < numExpr; i++) {
        try {
            string expr = expressions[i];
            double result = evalExpr(expr);  // 直接使用evalExpr处理复杂表达式

            cout << "表达式: " << expr << endl;
            cout << "计算结果: " << result << endl;
            cout << "------------------------" << endl;
        }
        catch (const exception& e) {
            cout << "表达式: \"" << expressions[i] << "\" -> 错误: " << e.what() << endl;
            cout << "------------------------" << endl;
        }
    }
}

// ==================== 功能3：柱状图最大矩形面积 ====================

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

// ==================== 测试函数 ====================

void testComplexOperations() {
    cout << "=== 测试复数功能 ===" << endl;

    Vector<Complex> complexVec;
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < 10; i++) {
        double real = rand() % 10 - 5;
        double imag = rand() % 10 - 5;
        complexVec.insert(complexVec.size(), Complex(real, imag));
    }

    complexVec.insert(complexVec.size(), Complex(1, 1));
    complexVec.insert(complexVec.size(), Complex(1, 1));

    cout << "初始向量: ";
    for (int i = 0; i < complexVec.size(); i++) {
        cout << complexVec[i] << " ";
    }
    cout << endl;

    // 测试查找功能
    Complex target(1, 1);
    int foundIndex = complexVec.find(target);
    if (foundIndex >= 0) {
        cout << "查找复数(1+1i): 找到，下标为 " << foundIndex << endl;
    }
    else {
        cout << "查找复数(1+1i): 未找到" << endl;
    }

    complexVec.unsort();
    cout << "乱序后: ";
    for (int i = 0; i < complexVec.size(); i++) {
        cout << complexVec[i] << " ";
    }
    cout << endl;

    // 再次测试查找
    foundIndex = complexVec.find(target);
    if (foundIndex >= 0) {
        cout << "乱序后查找复数(1+1i): 找到，下标为 " << foundIndex << endl;
    }
    else {
        cout << "乱序后查找复数(1+1i): 未找到" << endl;
    }

    complexVec.insert(3, Complex(10, 10));
    cout << "插入后大小: " << complexVec.size() << endl;

    complexVec.remove(3);
    cout << "删除后大小: " << complexVec.size() << endl;

    int removedCount = complexVec.deduplicate();
    cout << "去重后删除 " << removedCount << " 个重复元素" << endl;
}

void testSortingPerformance() {
    cout << "\n=== 测试排序性能 ===" << endl;

    const int TEST_SIZE = 500;

    // 创建顺序、逆序、乱序向量
    Vector<Complex> sortedVec, reversedVec, randomVec;

    // 顺序向量
    for (int i = 0; i < TEST_SIZE; i++) {
        sortedVec.insert(sortedVec.size(), Complex(i, i));
    }

    // 逆序向量
    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        reversedVec.insert(reversedVec.size(), Complex(i, i));
    }

    // 乱序向量
    for (int i = 0; i < TEST_SIZE; i++) {
        randomVec.insert(randomVec.size(), Complex(i, i));
    }
    randomVec.unsort();

    // 测试归并排序性能 - 使用Vector的公有排序函数
    Vector<Complex> testVec1 = sortedVec;
    clock_t start = clock();
    testVec1.mergeSort(0, testVec1.size());
    clock_t end = clock();
    cout << "顺序向量归并排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;

    Vector<Complex> testVec2 = reversedVec;
    start = clock();
    testVec2.mergeSort(0, testVec2.size());
    end = clock();
    cout << "逆序向量归并排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;

    Vector<Complex> testVec3 = randomVec;
    start = clock();
    testVec3.mergeSort(0, testVec3.size());
    end = clock();
    cout << "乱序向量归并排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;

    // 测试起泡排序性能 - 使用Vector的公有排序函数
    Vector<Complex> testVec4 = sortedVec;
    start = clock();
    testVec4.bubbleSort(0, testVec4.size());
    end = clock();
    cout << "顺序向量起泡排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;

    Vector<Complex> testVec5 = reversedVec;
    start = clock();
    testVec5.bubbleSort(0, testVec5.size());
    end = clock();
    cout << "逆序向量起泡排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;

    Vector<Complex> testVec6 = randomVec;
    start = clock();
    testVec6.bubbleSort(0, testVec6.size());
    end = clock();
    cout << "乱序向量起泡排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;

    // 测试选择排序性能
    Vector<Complex> testVec7 = randomVec;
    start = clock();
    testVec7.selectionSort(0, testVec7.size());
    end = clock();
    cout << "乱序向量选择排序时间: " << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;
}

void testRangeSearch() {
    cout << "\n=== 测试范围查找 ===" << endl;

    Vector<Complex> complexVec;
    for (int i = 0; i < 10; i++) {
        complexVec.insert(complexVec.size(), Complex(i, i));
    }

    complexVec.sort();

    cout << "排序后的向量: ";
    for (int i = 0; i < complexVec.size(); i++) {
        cout << complexVec[i] << " ";
    }
    cout << endl;

    Vector<Complex> result = findComplexByModulus(complexVec, 3, 7);
    cout << "模在[3, 7)范围内的复数: ";
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << endl;
}

void testLargestRectangle() {
    cout << "\n=== 测试柱状图最大矩形面积 ===" << endl;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int test = 1; test <= 3; test++) {
        int n = rand() % 10 + 5;
        Vector<int> heights;

        cout << "测试" << test << " - 柱高序列: ";
        for (int i = 0; i < n; i++) {
            int height = rand() % 10 + 1;
            heights.insert(heights.size(), height);
            cout << height << " ";
        }

        int maxArea = largestRectangleArea(heights);
        cout << "-> 最大矩形面积: " << maxArea << endl;
    }
}

int main() {
    cout << "开始测试所有功能..." << endl;

    try {
        testComplexOperations();
        testSortingPerformance();
        testRangeSearch();
        testCalculator();  // 测试增强版表达式计算器
        testLargestRectangle();
    }
    catch (const exception& e) {
        cout << "程序运行出错: " << e.what() << endl;
        return 1;
    }

    cout << "\n所有测试完成!" << endl;
    return 0;

}
