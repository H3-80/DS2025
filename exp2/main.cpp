#include <iostream>
#include <fstream>
#include <cctype>
#include "../MySTL/huffman.h"

using namespace std;

string readTextFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return "";
    }

    string content((istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>());
    file.close();
    return content;
}

string filterAlphabetic(const string& text) {
    string result;
    for (char c : text) {
        if (isalpha(c)) {
            result += tolower(c);
        }
    }
    return result;
}

int main() {
    cout << "=== Huffman编码实验 ===" << endl;

    // 尝试在两个可能的位置读取文件
    string text = readTextFile("104.txt");
    if (text.empty()) {
        text = readTextFile("MySTL/104.txt");
    }
    if (text.empty()) {
        cerr << "无法读取文本文件" << endl;
        return 1;
    }

    cout << "原始文本长度: " << text.length() << " 个字符" << endl;

    // 过滤出字母字符并转换为小写
    string filteredText = filterAlphabetic(text);
    cout << "处理后的文本长度: " << filteredText.length() << " 个字母" << endl;

    if (filteredText.empty()) {
        cerr << "过滤后文本为空" << endl;
        return 1;
    }

    // 将文本写入临时文件用于统计
    ofstream temp("temp.txt");
    temp << filteredText;
    temp.close();

    // 统计频率
    int* freq = statistics("temp.txt");
    if (!freq) {
        cerr << "频率统计失败" << endl;
        return 1;
    }

    // 生成Huffman树
    HuffTree* tree = generateTree(freq);
    if (!tree) {
        cerr << "生成Huffman树失败" << endl;
        delete[] freq;
        return 1;
    }

    // 生成编码表
    map<char, string> codeTable = generateTable(tree);

    // 显示部分编码表
    cout << "\n=== 部分Huffman编码表 ===" << endl;
    int count = 0;
    for (const auto& pair : codeTable) {
        if (count++ < 10) {
            cout << "'" << pair.first << "': " << pair.second << endl;
        }
    }

    // 测试编码和解码
    vector<string> testWords = { "dream", "freedom", "justice", "nation", "brotherhood" };

    cout << "\n=== 编码测试 ===" << endl;
    for (const string& word : testWords) {
        // 将测试单词转换为小写以匹配编码表
        string lowercaseWord;
        for (char c : word) {
            lowercaseWord += tolower(c);
        }

        string encoded = encode(codeTable, lowercaseWord);
        string decoded = decode(tree, encoded);

        cout << "单词: " << word << endl;
        cout << "编码: " << encoded << endl;
        cout << "解码: " << decoded << endl;
        cout << "编码长度: " << encoded.length() << " bits" << endl;
        if (!encoded.empty()) {
            cout << "压缩比: " << (word.length() * 8.0 / encoded.length()) << ":1" << endl;
        }
        cout << "---" << endl;
    }

    // 计算整体压缩效果
    string fullEncoded = encode(codeTable, filteredText);
    cout << "\n=== 整体统计 ===" << endl;
    cout << "原始文本大小: " << filteredText.length() * 8 << " bits" << endl;
    cout << "编码后大小: " << fullEncoded.length() << " bits" << endl;
    if (!fullEncoded.empty()) {
        cout << "压缩比: " << (filteredText.length() * 8.0 / fullEncoded.length()) << ":1" << endl;
    }

    // 清理资源
    delete[] freq;
    delete tree;

    // 删除临时文件
    remove("temp.txt");

    return 0;

}
