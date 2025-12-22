#pragma once
#include "BinTree.h"
#include "Bitmap.h"
#include <map>
#include <string>
#include <queue>
#include <vector>
#include <cstring>
#include <cctype>
#include <iostream>

using namespace std;

#define N_CHAR (0x80 - 0x20)

struct HuffChar {
    char ch;
    int weight;
    HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {}
    bool operator<(const HuffChar& hc) const { return weight > hc.weight; }
    bool operator==(const HuffChar& hc) const { return weight == hc.weight; }
};

typedef BinTree<HuffChar> HuffTree;

class HuffCode : public Bitmap {
public:
    HuffCode(Rank n = 8) : Bitmap(n) {}
};

int* statistics(const char* sample_text_file) {
    int* freq = new int[N_CHAR]();  // 初始化为0
    FILE* fp = fopen(sample_text_file, "r");
    if (!fp) {
        delete[] freq;
        return nullptr;
    }

    char ch;
    while (fscanf(fp, "%c", &ch) > 0) {
        if (ch >= 0x20 && ch < 0x80) {
            freq[ch - 0x20]++;
        }
    }
    fclose(fp);
    return freq;
}

struct CompareTrees {
    bool operator()(HuffTree* a, HuffTree* b) {
        return a->root()->data.weight > b->root()->data.weight;
    }
};

HuffTree* generateTree(int* freq) {
    priority_queue<HuffTree*, vector<HuffTree*>, CompareTrees> forest;

    for (int i = 0; i < N_CHAR; i++) {
        if (freq[i] > 0) {
            HuffTree* tree = new HuffTree();
            tree->insertAsRoot(HuffChar(0x20 + i, freq[i]));
            forest.push(tree);
        }
    }

    while (forest.size() > 1) {
        HuffTree* T1 = forest.top(); forest.pop();
        HuffTree* T2 = forest.top(); forest.pop();

        HuffTree* S = new HuffTree();
        S->insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
        S->attachAsLC(S->root(), T1);
        S->attachAsRC(S->root(), T2);

        forest.push(S);
    }

    return forest.empty() ? nullptr : forest.top();
}

void generateCT(HuffCode* code, int length, map<char, string>& table, BinNode<HuffChar>* v) {
    if (IsLeaf(*v)) {
        char* bits = code->bits2string(length);
        table[v->data.ch] = string(bits);
        delete[] bits;
        return;
    }
    if (HasLChild(*v)) {
        code->clear(length);
        generateCT(code, length + 1, table, v->lChild);
    }
    if (HasRChild(*v)) {
        code->set(length);
        generateCT(code, length + 1, table, v->rChild);
    }
}

map<char, string> generateTable(HuffTree* tree) {
    map<char, string> table;
    HuffCode* code = new HuffCode();
    generateCT(code, 0, table, tree->root());
    delete code;
    return table;
}

string encode(map<char, string>& table, const string& s) {
    string encoded;
    for (char c : s) {
        // 直接查找字符
        if (table.find(c) != table.end()) {
            encoded += table[c];
        }
    }
    return encoded;
}

string decode(HuffTree* tree, const string& codeStr) {
    string decoded;
    BinNode<HuffChar>* x = tree->root();

    for (char bit : codeStr) {
        x = (bit == '1') ? x->rChild : x->lChild;
        if (IsLeaf(*x)) {
            decoded += x->data.ch;
            x = tree->root();
        }
    }
    return decoded;
}