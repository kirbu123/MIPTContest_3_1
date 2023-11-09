#include <iostream>
#include <vector>

class StaticRSQ {
 private:
  std::vector<int64_t> data_;
  std::vector<int64_t> pref_;

 public:
  explicit StaticRSQ(std::vector<int64_t> data) {
    data_ = data;
    pref_.emplace_back(0);
    for (size_t i = 0; i < data.size(); ++i) {
      pref_.emplace_back(pref_[i] + data[i]);
    }
  }
  int64_t RSQ(size_t left, size_t right) const {
    if (right < left) {
      auto temp = left;
      left = right;
      right = temp;
    }
    return pref_[right + 1] - pref_[left];
  }
};

int main() {
  int64_t n = 0;
  int64_t k = 0;
  std::cin >> n;
  int64_t inp = 0;
  std::vector<int64_t> data;
  for (auto i = 0; i < n; ++i) {
    std::cin >> inp;
    data.emplace_back(inp);
  }
  StaticRSQ rsq(data);
  std::cin >> k;
  size_t left = 0;
  size_t right = 0;
  for (auto i = 0; i < k; ++i) {
    std::cin >> left >> right;
    std::cout << rsq.RSQ(left - 1, right - 1) << ' ';
  }
}