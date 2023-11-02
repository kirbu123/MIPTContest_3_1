#include <iostream>
#include <vector>
#include <cmath>

struct Query {
 public:
  char command;
  size_t first = 0;
  size_t second = 0;
  int delta = 0;
  Query(char c, size_t f, size_t s, int d = 0) {
    command = c;
    first = f;
    second = s;
    delta = d;
  }
  Query() = default;
};

std::istream& operator>>(std::istream& stream, Query& query) {
  stream >> query.command >> query.first >> query.second;
  if (query.command == 'a') {
    stream >> query.delta;
  }
  --query.first;
  --query.second;
  return stream;
}

struct Vertex {
 public:
  size_t idx;
  int max = -1;
  Vertex() = default;
  explicit Vertex(int m) {
    max = m;
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
    if (Left(v) < tree_.size() && Right(v) < tree_.size()) {
      int local_max = std::max(tree_[Left(v)].max, tree_[Right(v)].max);
      tree_[Left(v)].max += tree_[v].max - local_max;
      tree_[Right(v)].max += tree_[v].max - local_max;
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
        tree_[tree_.size() - n_ + i] = Vertex(data[i]);
      }
    }
    for (int i = static_cast<int>(Parent(tree_.size() - 1)); i >= 0; --i) {
      tree_[i].max = std::max(tree_[Left(i)].max, tree_[Right(i)].max);
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
  int Update(Query query) {
    size_t left = query.first;
    size_t right = query.second;
    std::vector<Vertex> term;
    GetTerminal(left, right, 0, n_ - 1, 0, term);
    for (size_t i = 0; i < term.size(); ++i) {
      tree_[term[i].idx].max += query.delta;
    }
    for (size_t i = 0; i < term.size(); ++i) {
      for (size_t j = term[i].idx; j > 0; j = Parent(j)) {
        tree_[Parent(j)].max = std::max(tree_[Left(Parent(j))].max, tree_[Right(Parent(j))].max);
      }
    }
    return query.delta;
  }
};

void Step(Query query, SegmentTree& st) {
  if (query.command == 'm') {
    std::cout << st.GetMax(query) << ' ';
  } else if (query.command == 'a') {
    st.Update(query);
  }
}

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
    Step(query, st);
  }
  return 0;
}