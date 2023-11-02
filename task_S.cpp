#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Node {
  Node* parent;
  Node* left;
  Node* right;
  int size;
  int min;
  int reverse;
  int x;
  int y;
  explicit Node(int idx) : parent(nullptr), left(nullptr), right(nullptr), x(idx) {
    reverse = false;
    size = 1;
    min = x;
    y = rand() / 1000000;
  }
  explicit Node(size_t idx) : parent(nullptr), left(nullptr), right(nullptr), x(static_cast<int>(idx)) {
    reverse = false;
    size = 1;
    min = x;
    y = rand() / 1000000;
  }
  Node() = default;
  ~Node() = default;
};

class CTree {
 private:
  Node* root_;
  void Clear(Node*);
  Node* Merge(Node*, Node*);
  Node* Find(Node*, int&);
  std::pair<Node*, Node*> Split(Node*, int key);
  Node* Kth(Node*, int);
  void FixNode(Node* node);
  int Size(Node*);
  int Min(Node*);
  void ShowTree(Node*);

 public:
  CTree() : root_(nullptr) {
  }
  ~CTree();
  void Insert(size_t, int);
  void Delete(int);
  Node* Find(int);
  Node* Next(int);
  Node* Prev(int);
  Node* Kth(int);
  void ShowTree();
  int GetMin(size_t, size_t);
  void Reverse(size_t, size_t);
};

void CTree::FixNode(Node* node) {
  if (node != nullptr) {
    node->size = 1 + Size(node->left) + Size(node->right);
    node->min = std::min(std::min(node->x, Min(node->left)), Min(node->right));
    if (node->reverse) {
      node->reverse = false;
      Node* temp = node->left;
      node->left = node->right;
      node->right = temp;
      if (node->left != nullptr) {
        node->left->reverse = !node->left->reverse;
      }
      if (node->right != nullptr) {
        node->right->reverse = !node->right->reverse;
      }
    }
  }
}

int CTree::Size(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->size;
}

int CTree::Min(Node* node) {
  if (node == nullptr) {
    return 2147483647;
  }
  return node->min;
}

Node* CTree::Kth(Node* node, int k) {
  if (Size(node) < k + 1) {
    return nullptr;
  }
  while (true) {
    if (Size(node->left) < k) {
      k -= Size(node->left) + 1;
      node = node->right;
    } else if (Size(node->left) > k) {
      node = node->left;
    } else {
      return node;
    }
  }
  return nullptr;
}

Node* CTree::Kth(int k) {
  return Kth(root_, k);
}

Node* CTree::Next(int idx) {
  Node* next = nullptr;
  Node* node = root_;
  while (node != nullptr) {
    if (node->x > idx) {
      next = node;
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return next;
}

Node* CTree::Prev(int idx) {
  Node* prev = nullptr;
  Node* node = root_;
  while (node != nullptr) {
    if (node->x < idx) {
      prev = node;
      node = node->right;
    } else {
      node = node->left;
    }
  }
  return prev;
}

Node* CTree::Find(int idx) {
  return Find(root_, idx);
}

Node* CTree::Find(Node* node, int& idx) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->x == idx) {
    return node;
  }
  if (idx > node->x) {
    return Find(node->right, idx);
  }
  return Find(node->left, idx);
}

void CTree::Delete(int idx) {
  if (Find(idx) == nullptr) {
    return;
  }
  auto roots1 = Split(root_, idx);
  auto node = Next(idx);
  int key = idx + 1;
  if (node != nullptr) {
    key = node->x;
  }
  auto roots2 = Split(roots1.second, key);
  delete roots2.first;
  root_ = Merge(roots1.first, roots2.second);
}

void CTree::Insert(size_t i, int x) {
  auto roots = Split(root_, static_cast<int>(i));
  Node* node = new Node(x);
  root_ = Merge(Merge(roots.first, node), roots.second);
}

std::pair<Node*, Node*> CTree::Split(Node* root, int k) {
  if (root == nullptr) {
    return std::make_pair(nullptr, nullptr);
  }
  FixNode(root);
  if (Size(root->left) < k) {
    auto roots = Split(root->right, k - Size(root->left) - 1);
    root->right = roots.first;
    if (roots.first != nullptr) {
      roots.first->parent = root;
    }
    if (roots.second != nullptr) {
      roots.second->parent = nullptr;
    }
    FixNode(root);
    return std::make_pair(root, roots.second);
  }
  auto roots = Split(root->left, k);
  root->left = roots.second;
  if (roots.second != nullptr) {
    roots.second->parent = root;
  }
  if (roots.first != nullptr) {
    roots.first->parent = nullptr;
  }
  FixNode(root);
  return std::make_pair(roots.first, root);
}

Node* CTree::Merge(Node* root_left, Node* root_right) {
  if (root_left == nullptr) {
    return root_right;
  }
  if (root_right == nullptr) {
    return root_left;
  }
  FixNode(root_left);
  FixNode(root_right);
  if (root_left->y < root_right->y) {
    root_right->left = Merge(root_left, root_right->left);
    if (root_right->left) {
      root_right->left->parent = root_right;
    }
    FixNode(root_right);
    return root_right;
  }
  root_left->right = Merge(root_left->right, root_right);
  if (root_left->right != nullptr) {
    root_left->right->parent = root_left;
  }
  FixNode(root_left);
  return root_left;
}

void CTree::Clear(Node* node) {
  if (node != nullptr) {
    if (node->left) {
      Clear(node->left);
    }
    if (node->right) {
      Clear(node->right);
    }
  }
  delete node;
}

void CTree::ShowTree(Node* node) {
  if (node != nullptr) {
    ShowTree(node->left);
    std::cout << node->x << ' ';
    ShowTree(node->right);
  }
}

void CTree::ShowTree() {
  ShowTree(root_);
  std::cout << '\n';
}

int CTree::GetMin(size_t l, size_t r) {
  auto trees_prev = Split(root_, static_cast<int>(l - 1));
  auto trees_finall = Split(trees_prev.second, static_cast<int>(r - l + 1));
  FixNode(trees_finall.first);
  int result = Min(trees_finall.first);
  root_ = Merge(Merge(trees_prev.first, trees_finall.first), trees_finall.second);
  return result;
}

void CTree::Reverse(size_t l, size_t r) {
  auto trees_prev = Split(root_, static_cast<int>(l - 1));
  auto trees_finall = Split(trees_prev.second, static_cast<int>(r - l + 1));
  trees_finall.first->reverse = !trees_finall.first->reverse;
  FixNode(trees_finall.first);
  root_ = Merge(Merge(trees_prev.first, trees_finall.first), trees_finall.second);
}

CTree::~CTree() {
  Clear(root_);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  srand(time(nullptr));
  CTree tree;
  size_t n = 0;
  size_t m = 0;
  std::cin >> n >> m;
  int elem = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> elem;
    tree.Insert(i, elem);
  }
  int command = 0;
  size_t l = 0;
  size_t r = 0;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> command;
    if (command == 1) {
      std::cin >> l >> r;
      tree.Reverse(l, r);
    } else if (command == 2) {
      std::cin >> l >> r;
      std::cout << tree.GetMin(l, r) << '\n';
    }
  }
  return 0;
}