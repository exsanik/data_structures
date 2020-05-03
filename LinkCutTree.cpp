#include <iostream>
#include <vector>

class LinkCutTree {
  class Node {
   public:
    int size;
    bool revert;
    Node *left;
    Node *right;
    Node *parent;

    Node() {
      size = 1;
      revert = false;
      left = right = parent = nullptr;
    }

    bool isRoot() {
      return parent == nullptr ||
             (parent->left != this && parent->right != this);
    }

    void push() {
      if (revert) {
        revert = false;
        std::swap(left, right);
        if (left != nullptr) left->revert = !left->revert;
        if (right != nullptr) right->revert = !right->revert;
      }
    }

    int getSize(Node *root) { return root == nullptr ? 0 : root->size; }
    void update() { size = 1 + getSize(left) + getSize(right); }
  };

  std::vector<Node> nodes;

  void connect(Node *ch, Node *p, int isLeftChild) {
    if (ch != nullptr) ch->parent = p;
    if (isLeftChild != -1) {
      if (isLeftChild) {
        p->left = ch;
      } else {
        p->right = ch;
      }
    }
  }

  void rotate(Node *x) {
    Node *p = x->parent;
    Node *g = p->parent;
    bool isRootP = p->isRoot();
    bool leftChildX = (x == p->left);

    connect(leftChildX ? x->right : x->left, p, leftChildX);
    connect(p, x, !leftChildX);
    connect(x, g, isRootP ? -1 : p == g->left);
    p->update();
  }

  void splay(Node *x) {
    while (!x->isRoot()) {
      Node *p = x->parent;
      Node *g = p->parent;
      if (!p->isRoot()) g->push();
      p->push();
      x->push();
      if (!p->isRoot()) {
        rotate((x == p->left) == (p == g->left) ? p : x);
      }
      rotate(x);
    }
    x->push();
    x->update();
  }

  Node *expose(Node *x) {
    Node *last = nullptr;
    for (Node *y = x; y != nullptr; y = y->parent) {
      splay(y);
      y->left = last;
      last = y;
    }
    splay(x);
    return last;
  }

 public:
  explicit LinkCutTree(int size) { nodes.resize(size); }

  void makeRoot(int x) {
    expose(&nodes[x]);
    nodes[x].revert = !nodes[x].revert;
  }

  bool connected(int x, int y) {
    if (x == y) return true;
    expose(&nodes[x]);
    expose(&nodes[y]);
    return nodes[x].parent != nullptr;
  }

  void link(int x, int y) {
    makeRoot(x);
    nodes[x].parent = &nodes[y];
  }

  void cut(int x, int y) {
    makeRoot(x);
    expose(&nodes[y]);
    nodes[y].right->parent = nullptr;
    nodes[y].right = nullptr;
  }

  int getPathLength(int x, int y) {
    if (x == y) {
      return 0;
    }
    makeRoot(x);
    expose(&nodes[y]);
    if (!connected(x, y)) {
      return -1;
    } else {
      return nodes[x].size;
    }
  }
};
