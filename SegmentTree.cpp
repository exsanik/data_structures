#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;

class SegmentTree {
private:
	vector<int> arr;
	vector<int> tree;

	int op(int a, int b) {
		return a + b;
	}
public:
	SegmentTree(vector<int>& a) {
		arr = a;
		tree.resize(a.size() * 4);
	}

	void build(int v, int tLeft, int tRight) {
		if (tLeft == tRight) {
			tree[v] = arr[tLeft];
		} else {
			int tMiddle = (tLeft + tRight) / 2;
			build(v * 2, tLeft, tMiddle);
			build(v * 2 + 1, tMiddle + 1, tRight);
			tree[v] = op(tree[v * 2], tree[v * 2 + 1]);
		}
	}

	int query(int v, int tLeft, int tRight, int l, int r) {
		if (l > r) {
			return 0; //neutral element
		}
		if (l == tLeft && r == tRight) {
			return tree[v];
		}
		int tMiddle = (tLeft + tRight) / 2;
		return op(query(v * 2, tLeft, tMiddle, l, std::min(r, tMiddle)),
			query(v * 2 + 1, tMiddle + 1, tRight, std::max(l, tMiddle + 1), r));
	}

	void update(int v, int tLeft, int tRight, int pos, int newVal) {
		if (tLeft == tRight) {
			tree[v] = newVal;
		} else {
			int tMiddle = (tLeft + tRight) / 2;
			if (pos <= tMiddle) {
				update(v * 2, tLeft, tMiddle, pos, newVal);
			} else {
				update(v * 2 + 1, tMiddle + 1, tRight, pos, newVal);
			}
			tree[v] = op(tree[v * 2], tree[v * 2 + 1]);
		}
	}
};


int main() {
	vector<int> a = { 99, 100, 1, 2, 1, 10 };
	SegmentTree tree(a);
	tree.build(1, 0, a.size() - 1);
	cout << tree.query(1, 0, a.size() - 1, 1, 4) << ' ';
	tree.update(1, 0, a.size() - 1, 3, 33);
	cout << tree.query(1, 0, a.size() - 1, 1, 4);
	return 0;
}
