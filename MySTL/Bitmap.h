#pragma once

typedef int Rank;

class Bitmap {
private:
    unsigned char* M;
    Rank N, _sz;

protected:
    void init(Rank n) {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N);
        _sz = 0;
    }

public:
    Bitmap(Rank n = 8) { init(n); }

    ~Bitmap() { delete[] M; M = NULL; _sz = 0; }

    Rank size() { return _sz; }

    void set(Rank k) {
        expand(k);
        if (!test(k)) {
            _sz++;
            M[k >> 3] |= (0x80 >> (k & 0x07));
        }
    }

    void clear(Rank k) {
        expand(k);
        if (test(k)) {
            _sz--;
            M[k >> 3] &= ~(0x80 >> (k & 0x07));
        }
    }

    bool test(Rank k) {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    char* bits2string(Rank n) {
        expand(n - 1);
        char* s = new char[n + 1];
        s[n] = '\0';
        for (Rank i = 0; i < n; i++)
            s[i] = test(i) ? '1' : '0';
        return s;
    }

    void expand(Rank k) {
        if (k < 8 * N) return;
        Rank oldN = N;
        unsigned char* oldM = M;
        init(2 * k);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }
};

