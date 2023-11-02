#include <iostream>
#include <vector>
#include <cmath>

class StaticRmq {
 private:
  std::vector<std::vector<int64_t>> sparse_table_;
  std::vector<int64_t> steps_;
  int64_t n_ = 0;
  int64_t k_inf_ = 1000000;

 public:
  explicit StaticRmq(std::vector<int64_t> data) {
    n_ = static_cast<int64_t>(log2(data.size())) + 1;
    steps_.resize(data.size() + 1);
    steps_[0] = 0;
    steps_[1] = 0;
    for (size_t i = 2; i < steps_.size(); ++i) {
      steps_[i] = 1 + static_cast<int64_t>(log2(static_cast<double>(i) / static_cast<double>(2)));
    }
    sparse_table_.resize(data.size(), std::vector<int64_t>(n_, k_inf_));
    for (size_t i = 0; i < data.size(); ++i) {
      sparse_table_[i][0] = data[i];
    }
    for (auto j = 1; j < n_; ++j) {
      for (size_t i = 0; i < data.size(); ++i) {
        if (i + static_cast<size_t>(pow(2, j - 1)) < data.size()) {
          sparse_table_[i][j] =
              std::min(sparse_table_[i][j - 1], sparse_table_[i + static_cast<size_t>(pow(2, j - 1))][j - 1]);
        }
      }
    }
  }
  int64_t GetSum(size_t left, size_t right) const {
    int64_t res = std::min(
        sparse_table_[left][steps_[right - left + 1]],
        sparse_table_[right - static_cast<size_t>(pow(2, steps_[right - left + 1])) + 1][steps_[right - left + 1]]);
    return res;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t n = 0;
  int64_t q = 0;
  std::cin >> n >> q;
  int64_t inp = 0;
  std::vector<int64_t> data;
  for (auto i = 0; i < n; ++i) {
    std::cin >> inp;
    data.emplace_back(inp);
  }
  StaticRmq rsq(data);
  size_t left = 0;
  size_t right = 0;
  for (auto i = 0; i < q; ++i) {
    std::cin >> left >> right;
    std::cout << rsq.GetSum(left - 1, right - 1) << '\n';
  }
}