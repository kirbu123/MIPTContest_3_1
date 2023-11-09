#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Node {
  Node* parent;
  Node* left;
  Node* right;
  int size;
  int x;
  int y;
  explicit Node(int& idx) : parent(nullptr), left(nullptr), right(nullptr), size(1), x(idx) {
    std::random_device rd;
    std::mt19937 gen(rd());
    y = static_cast<int>(gen() % 100000);
  }
  Node() = default;
  ~Node() = default;
};

class CTree {
  Node* root_;
  void Clear(Node*);
  Node* Merge(Node*, Node*);
  Node* Find(Node*, int&);
  std::pair<Node*, Node*> Split(Node*, int& key);
  Node* Kth(Node*, int);
  void FixNode(Node* node);
  int Size(Node*);
  void ShowTree(Node*);

 public:
  CTree() : root_(nullptr) {
  }
  ~CTree();
  void Insert(int&);
  void Delete(int&);
  Node* Find(int&);
  Node* Next(int&);
  Node* Prev(int&);
  Node* Kth(int&);
};

void CTree::FixNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  node->size = 1 + Size(node->left) + Size(node->right);
}

int CTree::Size(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->size;
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

Node* CTree::Kth(int& k) {
  return Kth(root_, k);
}

Node* CTree::Next(int& idx) {
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

Node* CTree::Prev(int& idx) {
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

Node* CTree::Find(int& idx) {
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

void CTree::Delete(int& idx) {
  if (Find(idx) == nullptr) {
    return;
  }
  auto roots1 = Split(root_, idx);
  auto node = Next(idx);
  int key = idx + 1;
  if (node) {
    key = node->x;
  }
  auto roots2 = Split(roots1.second, key);
  delete roots2.first;
  root_ = Merge(roots1.first, roots2.second);
}

void CTree::Insert(int& idx) {
  if (Find(idx) == nullptr) {
    auto roots = Split(root_, idx);
    Node* node = new Node(idx);
    root_ = Merge(Merge(roots.first, node), roots.second);
  }
}

std::pair<Node*, Node*> CTree::Split(Node* root, int& key) {
  if (root == nullptr) {
    return std::make_pair(nullptr, nullptr);
  }
  if (root->x < key) {
    auto roots = Split(root->right, key);
    root->right = roots.first;
    if (roots.first) {
      roots.first->parent = root;
    }
    if (roots.second) {
      roots.second->parent = nullptr;
    }
    FixNode(root);
    return std::make_pair(root, roots.second);
  }
  auto roots = Split(root->left, key);
  root->left = roots.second;
  if (roots.second) {
    roots.second->parent = root;
  }
  if (roots.first) {
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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  srand(time(nullptr));
  CTree tree;
  std::string command;
  int k = 0;
  while (std::cin >> command) {
    if (command == "insert") {
      std::cin >> k;
      tree.Insert(k);
    } else if (command == "delete") {
      int k = 0;
      std::cin >> k;
      tree.Delete(k);
    } else if (command == "exists") {
      int k = 0;
      std::cin >> k;
      tree.Find(k) != nullptr ? std::cout << "true" << '\n' : std::cout << "false" << '\n';
    } else if (command == "next") {
      int k = 0;
      std::cin >> k;
      Node* node = tree.Next(k);
      if (node == nullptr) {
        std::cout << "none" << '\n';
      } else {
        std::cout << node->x << '\n';
      }
    } else if (command == "prev") {
      int k = 0;
      std::cin >> k;
      Node* node = tree.Prev(k);
      if (node == nullptr) {
        std::cout << "none" << '\n';
      } else {
        std::cout << node->x << '\n';
      }
    } else if (command == "kth") {
      int k = 0;
      std::cin >> k;
      Node* node = tree.Kth(k);
      if (node == nullptr) {
        std::cout << "none" << '\n';
      } else {
        std::cout << node->x << '\n';
      }
    } else {
      break;
    }
  }
  return 0;
}