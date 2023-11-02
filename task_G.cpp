#include <iostream>
#include <vector>
#include <cmath>

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
  int max = -1;
  int count = 0;
  Vertex() = default;
  explicit Vertex(int m, int c = 1) {
    max = m;
    count = c;
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

 public:
  SegmentTree() = default;
  explicit SegmentTree(std::vector<int> data) {
    n_ = 1 << (static_cast<int>(log2(data.size()) + 1));
    tree_.resize(2 * (1 << (static_cast<int>(log2(data.size()) + 1))) - 1, Vertex());
    for (size_t i = 0; i < n_; ++i) {
      if (i < data.size()) {
        tree_[tree_.size() - n_ + i] = Vertex(data[i], 1);
      }
    }
    for (int i = static_cast<int>(Parent(tree_.size() - 1)); i >= 0; --i) {
      tree_[i].max = std::max(tree_[Left(i)].max, tree_[Right(i)].max);
      if (tree_[Left(i)].max == tree_[i].max) {
        tree_[i].count += tree_[Left(i)].count;
      }
      if (tree_[Right(i)].max == tree_[i].max) {
        tree_[i].count += tree_[Right(i)].count;
      }
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
    int result = term[0].max;
    for (size_t i = 1; i < term.size(); ++i) {
      result = std::max(result, term[i].max);
    }
    return result;
  }
  int GetCount(Query query) {
    size_t left = query.first;
    size_t right = query.second;
    std::vector<Vertex> term;
    GetTerminal(left, right, 0, n_ - 1, 0, term);
    int max = GetMax(query);
    int result = 0;
    for (size_t i = 0; i < term.size(); ++i) {
      if (max == term[i].max) {
        result += term[i].count;
      }
    }
    return result;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n = 0;
  std::cin >> n;
  std::vector<int> data(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> data[i];
  }
  size_t k = 0;
  std::cin >> k;
  Query query;
  SegmentTree st(data);
  for (size_t i = 0; i < k; ++i) {
    std::cin >> query;
    std::cout << st.GetMax(query) << ' ' << st.GetCount(query) << std::endl;
  }
  return 0;
}