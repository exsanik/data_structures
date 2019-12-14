struct Node {
	int prior, sz, val; 
	Node* l, * r; 

	Node(int val) {
		prior = rand();
		sz = 1;
		this->val = val;
		l = r = nullptr;
	}
};

ostream& operator <<(ostream& out, const Node* tree) {
	if (tree == nullptr) {
		return out;
	}
	out << tree->l;
	out << tree->val << " ";
	out << tree->r;
	return out;
}


class ImplicitTree {
private:
	Node* tree;

	Node* merge(Node* tLeft, Node* tRight) {
		if (tLeft == nullptr) { 
			return tRight; 
		}
		if (tRight == nullptr) { 
			return tLeft; 
		}

		if (tLeft->prior > tRight->prior) {
			tLeft->r = merge(tLeft->r, tRight);
			updateSize(tLeft);
			return tLeft;
		} else {
			tRight->l = merge(tLeft, tRight->l);
			updateSize(tRight);
			return tRight;
		}
	}

	void split(Node* resTree, int pos, Node*& tLeft, Node*& tRight) {
		if (resTree == nullptr) {
			tLeft = tRight = nullptr;
			return;
		}

		if (getSize(resTree->l) < pos) {
			split(resTree->r, pos - getSize(resTree->l) - 1, resTree->r, tRight);
			tLeft = resTree;
		} else {
			split(resTree->l, pos, tLeft, resTree->l);
			tRight = resTree;
		}
		updateSize(resTree);
	}

	void updateSize(Node* tree) {
		if (tree == nullptr) {
			return;
		}
		tree->sz = 1 + getSize(tree->l) + getSize(tree->r);
	}

	int getValue(Node* t, int pos) {
		int myIdx = getSize(t->l);
		if (pos < myIdx) {
			return getValue(t->l, pos);
		} else if (pos == myIdx) {
			return t->val;
		} else {
			return getValue(t->r, pos - myIdx - 1);
		}
	}

	int getSize(Node* tree) {
		if (tree == nullptr) {
			return 0;
		}
		return tree->sz;
	}

public:
	ImplicitTree() {
		tree = nullptr;
	}

	ImplicitTree(const vector<int>& arr) {
		tree = nullptr;
		for (int i = 0; i < arr.size(); ++i) {
			tree = merge(tree, new Node(arr[i]));
		}
	}

	int size() {
		if (tree == nullptr) {
			return 0;
		}
		return tree->sz;
	}

	void add(int pos, int val) {
		Node* t1, * t2;
		split(tree, pos, t1, t2);
		Node* new_tree = new Node(val);
		tree = merge(merge(t1, new_tree), t2);
	}

	void remove(int pos) {
		Node* t1, * t2, * t3, * t4;
		split(tree, pos, t1, t2);
		split(t2, pos + 1, t3, t4);
		tree = merge(t1, t4);
		delete t3;
	}

	void toFront(int l, int r) {
		Node* t1, * t2, * t3, * t4;
		split(tree, r + 1, t1, t2);
		split(t1, l, t3, t4);
		tree = merge(merge(t4, t3), t2);
	}

	void push_back(int val) {
		return add(size(), val);
	}

	void push_front(int val) {
		return add(0, val);
	}

	const int& operator[] (const int &index) {
		return getValue(tree, index);
	}

	friend ostream& operator <<(ostream& out, const ImplicitTree& imTree);
};

ostream& operator <<(ostream& out, const ImplicitTree& imTree) {
	out << imTree.tree;
	return out;
}