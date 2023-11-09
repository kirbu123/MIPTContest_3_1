#include <iostream>
#include <vector>
#include <cmath>
#include <string>

struct Query {
 public:
  size_t first = 0;
  size_t second = 0;
  Query(size_t f, size_t s) {
    first = f;
    second = s;
  }
  Query() = default;
};

std::istream& operator>>(std::istream& stream, Query& query) {
  stream >> query.first >> query.second;
  --query.first;
  --query.second;
  return stream;
}

struct Vertex {
 public:
  size_t idx;
  int left = 0;
  int max = 0;
  int right = 0;
  Vertex() = default;
  explicit Vertex(int l, int m, int r) {
    left = l;
    max = m;
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
  size_t TransformIdx(size_t idx) {
    return idx + tree_.size() - n_;
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
  Vertex SwapVertex(Vertex u, Vertex v) {
    Vertex target;
    target.max = u.max + v.max + 2 * std::min(u.right, v.left);
    target.right = u.right + v.right - std::min(u.right, v.left);
    target.left = u.left + v.left - std::min(u.right, v.left);
    return target;
  }

 public:
  SegmentTree() = default;
  explicit SegmentTree(std::vector<int> data) {
    n_ = 1 << (static_cast<int>(log2(data.size()) + 1));
    tree_.resize(2 * (1 << (static_cast<int>(log2(data.size()) + 1))) - 1, Vertex());
    for (size_t i = 0; i < n_; ++i) {
      if (i < data.size()) {
        if (data[i] == 1) {
          tree_[tree_.size() - n_ + i] = Vertex(0, 0, 1);
        } else if (data[i] == -1) {
          tree_[tree_.size() - n_ + i] = Vertex(1, 0, 0);
        }
      }
    }
    for (int i = static_cast<int>(Parent(tree_.size() - 1)); i >= 0; --i) {
      tree_[i] = SwapVertex(tree_[Left(i)], tree_[Right(i)]);
    }
    for (size_t i = 0; i < tree_.size(); ++i) {
      tree_[i].idx = i;
    }
  }
  int GetMax(Query query) {
    size_t left = query.first;
    size_t right = query.second;
    std::vector<Vertex> term;
    GetTerminal(left, right, 0, n_ - 1, 0, term);
    Vertex result = term[0];
    for (size_t i = 1; i < term.size(); ++i) {
      result = SwapVertex(result, term[i]);
    }
    return result.max;
  }
  void ShowTree() {
    for (size_t i = 0; i < tree_.size(); ++i) {
      std::cout << tree_[i].idx << ": " << tree_[i].left << ' ' << tree_[i].max << ' ' << tree_[i].right << '\n';
    }
  }
};

void Step(Query query, SegmentTree& st) {
  std::cout << st.GetMax(query) << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::vector<int> data;
  std::string string;
  std::getline(std::cin, string);
  for (auto i = string.begin(); i != string.end(); ++i) {
    if (*i == '(') {
      data.emplace_back(1);
    } else if (*i == ')') {
      data.emplace_back(-1);
    }
  }
  size_t k = 0;
  std::cin >> k;
  Query query;
  SegmentTree st(data);
  for (size_t i = 0; i < k; ++i) {
    std::cin >> query;
    Step(query, st);
  }
  return 0;
}