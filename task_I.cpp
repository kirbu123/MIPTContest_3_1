#include <iostream>
#include <vector>
#include <cmath>

struct Node {
 public:
  int num;
  int key;
  int value;
  Node* father = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
  Node() = default;
  Node(int key_inp, int value_inp) {
    key = key_inp;
    value = value_inp;
  }
};

std::istream& operator>>(std::istream& stream, Node* node) {
  stream >> node->key >> node->value;
  return stream;
}

std::ostream& operator<<(std::ostream& stream, Node* node) {
  if (node->father == nullptr) {
    stream << 0 << ' ';
  } else {
    stream << node->father->num << ' ';
  }
  if (node->left == nullptr) {
    stream << 0 << ' ';
  } else {
    stream << node->left->num << ' ';
  }
  if (node->right == nullptr) {
    stream << 0 << '\n';
  } else {
    stream << node->right->num << '\n';
  }
  return stream;
}

class CTree {
 private:
  void BulatOrder(Node* node) const {
    if (node != nullptr) {
      BulatOrder(node->left);
      std::cout << node;
      BulatOrder(node->right);
    }
  }

 public:
  Node* root = nullptr;
  CTree() = default;
  explicit CTree(Node* root_inp) {
    root = root_inp;
  }
  CTree(int key_inp, int value_inp) {
    root = new Node;
    root->key = key_inp;
    root->value = value_inp;
  }
  int BuildByRaw(std::vector<Node*> data) {
    if (data.empty()) {
      return -1;
    }
    root = data[0];
    Node* u = nullptr;
    Node* v = nullptr;
    for (size_t i = 1; i < data.size(); ++i) {
      v = data[i];
      u = data[i - 1];
      while (u != nullptr && u->value > v->value) {
        u = u->father;
      }
      if (u == nullptr) {
        root->father = v;
        v->left = root;
        root = v;
      } else {
        v->father = u;
        v->left = u->right;
        u->right = v;
        if (v->left != nullptr) {
          v->left->father = v;
        }
      }
    }
    return 0;
  }
  void BulatOrder() const {
    BulatOrder(root);
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n = 0;
  std::cin >> n;
  std::vector<Node*> data(n);
  for (size_t i = 0; i < n; ++i) {
    data[i] = new Node;
    std::cin >> data[i];
    data[i]->num = static_cast<int>(i) + 1;
  }
  CTree tree;
  tree.BuildByRaw(data);
  std::cout << "YES\n";
  tree.BulatOrder();
  return 0;
}