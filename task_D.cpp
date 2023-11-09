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

class StaticRmq {
 private:
  std::vector<std::vector<int>> sparse_table_;
  std::vector<int> steps_;
  int n_ = 0;
  int k_inf_ = 1000000;
  int GCD(int a, int b) const {
    return b ? GCD(b, a % b) : a;
  }

 public:
  explicit StaticRmq(std::vector<int> data) {
    n_ = static_cast<int>(log2(data.size())) + 1;
    steps_.resize(data.size() + 1);
    steps_[0] = 0;
    steps_[1] = 0;
    for (size_t i = 2; i < steps_.size(); ++i) {
      steps_[i] = 1 + static_cast<int>(log2(static_cast<double>(i) / static_cast<double>(2)));
    }
    sparse_table_.resize(data.size(), std::vector<int>(n_, k_inf_));
    for (size_t i = 0; i < data.size(); ++i) {
      sparse_table_[i][0] = data[i];
    }
    for (auto j = 1; j < n_; ++j) {
      for (size_t i = 0; i < data.size(); ++i) {
        if (i + static_cast<size_t>(pow(2, j - 1)) < data.size()) {
          sparse_table_[i][j] =
              GCD(sparse_table_[i][j - 1], sparse_table_[i + static_cast<size_t>(pow(2, j - 1))][j - 1]);
        }
      }
    }
  }
  int GetGCD(size_t left, size_t right) const {
    int res =
        GCD(sparse_table_[left][steps_[right - left + 1]],
            sparse_table_[right - static_cast<size_t>(pow(2, steps_[right - left + 1])) + 1][steps_[right - left + 1]]);
    return res;
  }
  int GetGCD(Query query) const {
    size_t left = query.first;
    size_t right = query.second;
    int res =
        GCD(sparse_table_[left][steps_[right - left + 1]],
            sparse_table_[right - static_cast<size_t>(pow(2, steps_[right - left + 1])) + 1][steps_[right - left + 1]]);
    return res;
  }
};

int main() {
  int n = 0;
  std::cin >> n;
  std::vector<int> data(n);
  for (size_t i = 0; i < static_cast<size_t>(n); ++i) {
    std::cin >> data[i];
  }
  StaticRmq obj(data);
  int k = 0;
  std::cin >> k;
  Query query;
  for (size_t i = 0; i < static_cast<size_t>(k); ++i) {
    std::cin >> query;
    std::cout << obj.GetGCD(query) << std::endl;
  }
}