#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

constexpr int64_t MOD = 4294967296;

template <typename T>
class SegmentTree {
 private:
    vector<T>* arr;
    vector<T> tree;
    T neutralElement;
    std::function<T(T, T)> operation;

 public:
    explicit SegmentTree(
        size_t size,
        T neutralEl,
        std::function<T(T, T)> operation) {
        tree.resize(size * 4);
        neutralElement = neutralEl;
        this->operation = operation;
        arr = nullptr;
    }

    void setArray(vector<T>* inputData) {
        arr = inputData;
    }

    void build(size_t v, size_t tLeft, size_t tRight) {
        if (tLeft == tRight) {
            tree[v] = (*arr)[tLeft];
        } else {
            size_t tMiddle = (tLeft + tRight) / 2;
            build(v * 2, tLeft, tMiddle);
            build(v * 2 + 1, tMiddle + 1, tRight);
            tree[v] = operation(tree[v * 2], tree[v * 2 + 1]);
        }
    }

    T query(size_t v, size_t tLeft, size_t tRight,
        size_t leftBorder, size_t rightBorder) {
        if (leftBorder > rightBorder) {
            return neutralElement;
        }
        if (leftBorder == tLeft && rightBorder == tRight) {
            return tree[v];
        }
        size_t tMiddle = (tLeft + tRight) / 2;
        return operation(
            query(v * 2, tLeft, tMiddle, leftBorder,
                std::min(rightBorder, tMiddle)),
            query(v * 2 + 1, tMiddle + 1, tRight,
                std::max(leftBorder, tMiddle + 1), rightBorder));
    }

    void update(size_t v, size_t tLeft, size_t tRight, size_t pos, T newVal) {
        if (tLeft == tRight) {
            tree[v] = newVal;
        } else {
            size_t tMiddle = (tLeft + tRight) / 2;
            if (pos <= tMiddle) {
                update(v * 2, tLeft, tMiddle, pos, newVal);
            } else {
                update(v * 2 + 1, tMiddle + 1, tRight, pos, newVal);
            }
            tree[v] = operation(tree[v * 2], tree[v * 2 + 1]);
        }
    }
};

int main() {
	vector<int> a = { 99, 100, 1, 2, 1, 10 };
	SegmentTree<int64_t> tree(a.size(), 0,
                    [](int64_t a, int64_t b) { return a + b; }););
	tree.setArray(&a);
	tree.build(1, 0, a.size() - 1);
	cout << tree.query(1, 0, a.size() - 1, 1, 4) << ' ';
	tree.update(1, 0, a.size() - 1, 3, 33);
	cout << tree.query(1, 0, a.size() - 1, 1, 4);
	return 0;
}
