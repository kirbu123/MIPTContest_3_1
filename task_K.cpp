#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Node {
  Node* parent;
  Node* left;
  Node* right;
  int64_t size;
  int64_t x;
  int64_t y;
  explicit Node(int64_t& idx) : parent(nullptr), left(nullptr), right(nullptr), x(idx) {
    size = x;
    y = rand() / 100000;
  }
  Node() = default;
  ~Node() = default;
};

class CTree {
  Node* root_;
  void Clear(Node*);
  Node* Merge(Node*, Node*);
  Node* Find(Node*, int64_t&);
  std::pair<Node*, Node*> Split(Node*, int64_t key);
  Node* Kth(Node*, int64_t);
  void FixNode(Node* node);
  int64_t Size(Node*);
  void ShowTree(Node*);

 public:
  CTree() : root_(nullptr) {
  }
  ~CTree();
  void Insert(int64_t&);
  void Delete(int64_t&);
  Node* Find(int64_t&);
  Node* Next(int64_t&);
  Node* Prev(int64_t&);
  Node* Kth(int64_t&);
  int64_t GetSum(int64_t&, int64_t&);
};

void CTree::FixNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  node->size = node->x + Size(node->left) + Size(node->right);
}

int64_t CTree::Size(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->size;
}

Node* CTree::Kth(Node* node, int64_t k) {
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

Node* CTree::Kth(int64_t& k) {
  return Kth(root_, k);
}

Node* CTree::Next(int64_t& idx) {
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

Node* CTree::Prev(int64_t& idx) {
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

Node* CTree::Find(int64_t& idx) {
  return Find(root_, idx);
}

Node* CTree::Find(Node* node, int64_t& idx) {
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

void CTree::Delete(int64_t& idx) {
  if (Find(idx) == nullptr) {
    return;
  }
  auto roots1 = Split(root_, idx);
  auto node = Next(idx);
  int64_t key = idx + 1;
  if (node) {
    key = node->x;
  }
  auto roots2 = Split(roots1.second, key);
  delete roots2.first;
  root_ = Merge(roots1.first, roots2.second);
}

void CTree::Insert(int64_t& idx) {
  if (Find(idx) == nullptr) {
    auto roots = Split(root_, idx);
    Node* node = new Node(idx);
    root_ = Merge(Merge(roots.first, node), roots.second);
  }
}

std::pair<Node*, Node*> CTree::Split(Node* root, int64_t key) {
  if (root == nullptr) {
    return std::make_pair(nullptr, nullptr);
  }
  if (root->x < key) {
    auto roots = Split(root->right, key);
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
  auto roots = Split(root->left, key);
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
    std::cout << node->x << ' ' << node->y << '\n';
    ShowTree(node->right);
  }
}

CTree::~CTree() {
  Clear(root_);
}

int64_t CTree::GetSum(int64_t& l, int64_t& r) {
  auto trees_prev = Split(root_, l);
  auto trees_finall = Split(trees_prev.second, r + 1);
  int64_t result = Size(trees_finall.first);
  root_ = Merge(Merge(trees_prev.first, trees_finall.first), trees_finall.second);
  return result;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  srand(time(nullptr));
  CTree tree;
  size_t n = 0;
  std::cin >> n;
  char command = 0;
  int64_t k = 0;
  int64_t l = 0;
  int64_t r = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> command;
    if (command == '+') {
      std::cin >> k;
      tree.Insert(k);
    } else if (command == '?') {
      std::cin >> l >> r;
      std::cout << tree.GetSum(l, r) << '\n';
    }
  }
  return 0;
}