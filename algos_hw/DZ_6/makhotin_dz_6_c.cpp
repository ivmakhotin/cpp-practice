#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <inttypes.h>

struct Cycle_move {
    size_t left, right, shift;
};

template <class T>
class Treap {
    #define cnt(t) (t ? t->c : 0)
    #define updc(t) if (t) t->c = 1 + cnt(t->left) + cnt(t->right)

    struct tree {
        T val;
        int32_t pr, c;
        bool rev;
        tree *left, *right;
        tree() {
            left = right = NULL;
            c = 0;
            rev = false;
        }
        tree(T V, int32_t priority, tree *L, tree*R):val(V) {
            pr = priority;
            left = L;
            right = R;
            c = 1 + cnt(left) + cnt(right);
            rev = false;
        }
    };

    tree *root;
    int32_t length;

    void push(tree *&t) {
        if (t && t->rev) {
            t->rev = false;
            tree *q = t->left;
            t->left = t->right;
            t->right = q;
            if (t->left)
                t->left->rev ^= true;
            if (t->right)
                t->right->rev ^= true;
        }
    }

    void merge(tree *&t, tree *left, tree *right) {
        push(left);
        push(right);
        if (!left) 
            t = right;
        else if (!right)
            t = left;
        else if (left->pr > right->pr) {
            merge(left->right, left->right, right);
            t = left;
        }
        else {
            merge(right->left, left, right->left);
            t = right;
        }
        updc(t);
    }

    void split(tree *t, int32_t sk, tree *&left, tree *&right, int32_t less = 0) {
        if (!t) 
            left = right = NULL;
        else {
            push(t);
            int32_t key = cnt(t->left) + less;
            if (sk <= key) {
                split(t->left, sk, left, t->left, less);
                right = t;
            }
            else {
                split(t->right, sk, t->right, right, less + cnt(t->left) + 1);
                left = t;
            }
            updc(t);
        }
    }

    void print(tree *&t) {
        if (!t)
            return;
        push(t);
        print(t->left);
        std::cout << t->val;
        print(t->right);
    }

    void destroy_treap(tree *root) {
    if (root != NULL) {
      destroy_treap(root->left);
      destroy_treap(root->right);
      delete root;
    }
}

public:
    Treap() {
        root = NULL;
        length = 0;
    }

    ~Treap() {
        destroy_treap(root);
    }

    void insert(int32_t pos, T val) {
        if (pos < 0 || pos > length) return;
        tree *q = new tree(val, rand() << 15 | rand(), NULL, NULL);
        tree *tl, *tr;
        split(root, pos, tl, tr);
        merge(tl, tl, q);
        merge(root, tl, tr);
        ++length;
    }

    void reverse(int32_t left, int32_t right) {
        left = std::max(left, 0);
        right = std::min(right, length - 1);
        if (left > right) return;
        tree *tl, *tm, *tr;
        split(root, left, tl, tr);
        split(tr, right - left + 1, tm, tr);
        tm->rev ^= true;
        merge(tl, tl, tm);
        merge(root, tl, tr);
    }

    void cycle_move(int32_t left, int32_t right, int32_t shift) {
        reverse(left - 1, shift + left - 2);
        reverse(shift + left - 1, right - 1);
        reverse(left - 1, right - 1);
    }

    void print() {
        print(root);
        std::cout << std::endl;
    }
    #undef cnt(t)
    #undef updc(t)
};    

int main() {
    srand(74747);

    Treap<char> treap;
    char c;
    int32_t query_number, left, right, shift;
    std::string cypher;

    std::getline (std::cin, cypher);
    for (size_t i = 0; i < cypher.size(); ++i) {
        treap.insert(i, cypher[i]);
    }

    std::cin >> query_number;
    std::vector<struct Cycle_move> cycle_moves;

    for (size_t i = 0; i < query_number; ++i) {
        std::cin >> left >> right >> shift;
        cycle_moves.push_back({left, right, shift});
    }

    for (int32_t i = query_number - 1; i >= 0; --i) {
        treap.cycle_move(cycle_moves[i].left, cycle_moves[i].right,
                         cycle_moves[i].shift);
    }
    treap.print();

    return 0;
}
