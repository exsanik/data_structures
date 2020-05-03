class LinkCutTree {
 private:
  class Node {
   public:
    Node *left, *right, *parent;
    Node() : left(nullptr), right(nullptr), parent(nullptr) {}
    bool reverse = false;
    bool isRoot() const {
      return !parent || parent->left != this && parent->right != this;
    }
    bool isLeft() const {
      return this != nullptr && parent && parent->left == this;
    }
    void push() {
      if (this != nullptr && reverse) {
        swap(left, right);
        if (left) left->reverse ^= true;
        if (right) right->reverse ^= true;
        reverse = false;
      }
    }
  };

  Node* tree = nullptr;

  void connect(Node* ver, Node* parent, bool isLeft) {
    if (ver) ver->parent = parent;
    (isLeft ? parent->left : parent->right) = ver;
  }

  void rotate(Node* ver) {
    Node* parent = ver->parent;
    Node* gparent = parent->parent;
    Node* left = ver->left;
    Node* right = ver->right;
    bool verleft = ver->isLeft();
    bool pleft = parent->isLeft();
    bool proot = parent->isRoot();
    connect(verleft ? right : left, parent, verleft);
    connect(parent, ver, !verleft);
    if (proot) {
      ver->parent = gparent;
    } else {
      connect(ver, gparent, pleft);
    }
  }

  void splay(Node* ver) {
    while (!ver->isRoot()) {
      Node* parent = ver->parent;
      Node* gparent = parent->parent;
      gparent->push();
      parent->push();
      ver->push();
      if (!parent->isRoot()) {
        rotate(ver->isLeft() ^ parent->isLeft() ? ver : parent);
      }
      rotate(ver);
    }
    ver->push();
  }

  Node* access(Node* ver) {
    Node* last = 0;
    for (Node* currVer = ver; currVer; currVer = currVer->parent) {
      splay(currVer);
      currVer->right = last;
      last = currVer;
    }
    splay(ver);
    return last;
  }

 public:
  explicit LinkCutTree(int size) { tree = new Node[size]; }
  ~LinkCutTree() { delete[] tree; }

  void make_root(int v) {
    access(&tree[v]);
    tree[v].reverse ^= 1;
  }

  void link(int u, int v) {
    make_root(u);
    access(&tree[v]);
    tree[u].parent = &tree[v];
  }

  void cut(int u, int v) {
    make_root(u);
    access(&tree[v]);
    tree[v].left->parent = nullptr;  // not works with gnu 14 and gnu 17 O_o,
                                     // don't know why, other comilers are ok
    tree[v].left = nullptr;
  }

  bool connected(int u, int v) {
    if (u == v) return true;
    access(&tree[u]);
    access(&tree[v]);
    return tree[u].parent != nullptr;
  }

  Node* lca(int u, int v) {
    access(&tree[u]);
    return access(&tree[v]);
  }
};
