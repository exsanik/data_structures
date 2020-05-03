#include <vector>
#include <algorithm>

class LinkCutTree {
  class Node {
   public:
    int nodeValue;
    int subTreeValue;
    int size;
    bool revert;
    Node *left;
    Node *right;
    Node *parent;

    Node() {
      size = 1;
      nodeValue = subTreeValue = 0;
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

    int getSubTreeValue(Node *root) {
      return root == nullptr ? 0 : root->subTreeValue;
    }

    void update() {
      subTreeValue = std::max(std::max(nodeValue, getSubTreeValue(left)),
                              getSubTreeValue(right));
      size = 1 + getSize(left) + getSize(right);
    }
  };

  std::vector<Node> nodes;

  void connect(Node *child, Node *parent, int isLeftChild) {
    if (child != nullptr) child->parent = parent;
    if (isLeftChild != -1) {
      if (isLeftChild) {
        parent->left = child;
      } else {
        parent->right = child;
      }
    }
  }

  void rotate(Node *x) {
    Node *parent = x->parent;
    Node *gparent = parent->parent;
    bool isRootP = parent->isRoot();
    bool leftChildX = (x == parent->left);

    connect(leftChildX ? x->right : x->left, parent, leftChildX);
    connect(parent, x, !leftChildX);
    connect(x, gparent, isRootP ? -1 : parent == gparent->left);
    parent->update();
  }

  void splay(Node *x) {
    while (!x->isRoot()) {
      Node *parent = x->parent;
      Node *gparent = parent->parent;
      if (!parent->isRoot()) gparent->push();
      parent->push();
      x->push();
      if (!parent->isRoot()) {
        rotate((x == parent->left) == (parent == gparent->left) ? parent : x);
      }
      rotate(x);
    }
    x->push();
    x->update();
  }

  Node *expose(Node *x) {
    Node *last = nullptr;
    for (Node *curr = x; curr != nullptr; curr = curr->parent) {
      splay(curr);
      curr->left = last;
      last = curr;
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

  void modify(int x, int delta) {
    makeRoot(x);
    nodes[x].nodeValue += delta;
    nodes[x].update();
  }

  int getQueryValue(int x, int y) {
    makeRoot(x);
    expose(&nodes[y]);
    return nodes[y].subTreeValue;
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
