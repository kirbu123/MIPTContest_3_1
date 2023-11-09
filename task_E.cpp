#include <iostream>
#include <vector>

struct Query {
 public:
  char id;
  int64_t first = 0;
  int64_t second = 0;
  Query(char simb, int64_t f, int64_t s) {
    id = simb;
    first = f;
    second = s;
  }
  Query() = default;
};

std::istream& operator>>(std::istream& stream, Query& query) {
  stream >> query.id >> query.first >> query.second;
  if (query.id == 's') {
    --query.first;
    --query.second;
  } else if (query.id == 'u') {
    --query.first;
  }
  return stream;
}

class FenwickTree {
 private:
  std::vector<int64_t> ft_;
  std::vector<int64_t> pref_;
  int64_t GetPrefSum(int64_t idx) const {
    int64_t result = 0;
    for (int64_t i = idx; i >= 0; i = (i & (i + 1)) - 1) {
      result += ft_[i];
    }
    return result;
  }

 public:
  explicit FenwickTree(std::vector<int64_t> data) {
    pref_.resize(data.size(), data[0]);
    for (int64_t i = 0; i < static_cast<int64_t>(data.size()); ++i) {
      pref_[i] = pref_[i - 1] + data[i];
    }
    ft_.resize(data.size(), 0);
    for (int64_t i = 0; i < static_cast<int64_t>(data.size()); ++i) {
      ft_[i] = pref_[i] - pref_[(i & (i + 1)) - 1];
    }
  }
  int64_t Update(int64_t idx, int64_t delta) {
    for (int64_t i = idx; i < static_cast<int64_t>(ft_.size()); i = i | (i + 1)) {
      ft_[i] += delta;
    }
    return delta;
  }
  int64_t Update(Query query) {
    int64_t idx = query.first;
    int64_t delta = query.second;
    for (int64_t i = idx; i < static_cast<int64_t>(ft_.size()); i = i | (i + 1)) {
      ft_[i] += delta;
    }
    return delta;
  }
  int64_t GetSum(int64_t left, int64_t right) const {
    if (left == 0) {
      return GetPrefSum(right);
    }
    return GetPrefSum(right) - GetPrefSum(left - 1);
  }
  int64_t GetSum(Query query) const {
    int64_t left = query.first;
    int64_t right = query.second;
    if (left == 0) {
      return GetPrefSum(right);
    }
    return GetPrefSum(right) - GetPrefSum(left - 1);
  }
};

int main() {
  int64_t n = 0;
  std::cin >> n;
  std::vector<int64_t> data(n);
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> data[i];
  }
  FenwickTree fet(data);
  int64_t m = 0;
  std::cin >> m;
  Query query;
  for (int64_t i = 0; i < m; ++i) {
    std::cin >> query;
    if (query.id == 's') {
      std::cout << '\n' << fet.GetSum(query) << '\n';
    } else if (query.id == 'u') {
      int64_t temp = query.second;
      query.second -= data[query.first];
      data[query.first] = temp;
      fet.Update(query);
    }
  }
}