class FenwickTree {
private:
	vector<int> tree;
	const int neutralEl = 0;

	void init(int sz) {
		tree.assign(sz, neutralEl);
	}

	int op(int a, int b) {
		return a + b;
	}

public:
	FenwickTree(int sz) {
		init(sz);
	}
	FenwickTree(vector<int>& a) {
		init(a.size());
		for (int i = 0; i < a.size(); ++i) {
			update(i, a[i]);
		}
	}
	void update(int x, int delta) {
		for (int i = x; i < tree.size(); i = (i | (i + 1))) {
			tree[i] = op(delta, tree[i]);
		}
	}
	int query(int x) {
		int result = neutralEl;
		for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
			result = op(result, tree[i]);
		}
		return result;
	}
	int query(int x, int y) {
		return query(y) - query(x - 1); //1D

		//query(x2, y2) - query(x2, y1 - 1) - query(x1 - 1, y2) + query(x1 - 1, y1 - 1) //2D
		
		//int value1 = query(x2, y2, z2) - query(x1 - 1, y2, z2) - //3D
		//    query(x2, y1 - 1, z2) + query(x1 - 1, y1 - 1, z2);
		//int value2 = query(x2, y2, z1 - 1) - query(x1 - 1, y2, z1 - 1) -
		//    query(x2, y1 - 1, z1 - 1) + query(x1 - 1, y1 - 1, z1 - 1);
		//value1 - value2 
	}
};

int main() {
	vector<int> a = { 99, 100, 1, 2, 1, 10 };
	FenwickTree tree(a);
	cout << tree.query(0, 1) << ' ';
	tree.update(0, 2);
	cout << tree.query(0, 1) << ' ';
	return 0;
}
