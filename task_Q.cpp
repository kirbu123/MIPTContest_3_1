#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

struct Vertex {
 public:
  size_t idx;
  int min = 2147483647;
  int sum = 0;
  size_t left;
  size_t right;
  Vertex() = default;
  explicit Vertex(int m, size_t l, size_t r) {
    min = m;
    sum = m;
    left = l;
    right = r;
  }
};

class SegmentTree {
 private:
  std::vector<Vertex> tree_;
  size_t n_;
  size_t Parent(size_t idx) const {
    return (idx - 1) / 2;
  }
  size_t Left(size_t idx) const {
    return idx * 2 + 1;
  }
  size_t Right(size_t idx) const {
    return (idx + 1) * 2;
  }
  void GetTerminal(size_t left, size_t right, size_t lb, size_t rb, size_t v, std::vector<Vertex>& result) {
    if (right < left || left > rb || right < lb) {
      return;
    }
    if (left <= lb && rb <= right) {
      result.emplace_back(tree_[v]);
    } else {
      size_t center = (lb + rb) / 2;
      GetTerminal(left, std::min(right, center), lb, center, Left(v), result);
      GetTerminal(std::max(left, center + 1), right, center + 1, rb, Right(v), result);
    }
  }

 public:
  SegmentTree() = default;
  explicit SegmentTree(std::vector<int> data) {
    n_ = 1 << (static_cast<int>(log2(data.size()) + 1));
    tree_.resize(2 * (1 << (static_cast<int>(log2(data.size()) + 1))) - 1, Vertex());
    for (size_t i = 0; i < n_; ++i) {
      if (i < data.size()) {
        tree_[tree_.size() - n_ + i] = Vertex(data[i], i + 1, i + 1);
      } else {
        tree_[tree_.size() - n_ + i] = Vertex(0, i + 1, i + 1);
      }
    }
    for (int i = static_cast<int>(Parent(tree_.size() - 1)); i >= 0; --i) {
      tree_[i].min = std::min(tree_[Left(i)].min, tree_[Right(i)].min);
      tree_[i].sum = tree_[Left(i)].sum + tree_[Right(i)].sum;
      tree_[i].left = tree_[Left(i)].left;
      tree_[i].right = tree_[Right(i)].right;
    }
    for (size_t i = 0; i < tree_.size(); ++i) {
      tree_[i].idx = i;
    }
  }
  int GetMin(size_t left, size_t right) {
    --left;
    --right;
    std::vector<Vertex> term;
    GetTerminal(left, right, 0, n_ - 1, 0, term);
    int result = term[0].min;
    for (size_t i = 1; i < term.size(); ++i) {
      result = std::min(result, term[i].min);
    }
    return result;
  }
  void PrintOptimum() const {
    std::stack<size_t> stack;
    stack.push(0);
    size_t idx = 0;
    int result = 0;
    size_t left = 0;
    size_t right = 0;
    while (!stack.empty()) {
      idx = stack.top();
      stack.pop();
      std::cout << tree_[idx].min << ' ' << tree_[idx].sum << ' ' << tree_[idx].left << ' ' << tree_[idx].right << '\n';
      if (tree_[idx].min * tree_[idx].sum >= result) {
        result = tree_[idx].min * tree_[idx].sum;
        left = tree_[idx].left;
        right = tree_[idx].right;
      }
      if (Left(idx) < tree_.size()) {
        stack.push(Left(idx));
      }
      if (Right(idx) < tree_.size()) {
        stack.push(Right(idx));
      }
    }
    std::cout << result << '\n';
    std::cout << left << ' ' << right << '\n';
  }
};

int main() {
  /*
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  */
  size_t n = 0;
  std::cin >> n;
  std::vector<int> data(n);
  for (size_t i = 0; i < data.size(); ++i) {
    std::cin >> data[i];
  }
  SegmentTree tree(data);
  tree.PrintOptimum();
  return 0;
}