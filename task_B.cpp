#include <iostream>
#include <vector>

struct AnswerStruct {
 public:
  int64_t left;
  int64_t right;
  int64_t sum;
  AnswerStruct(int64_t l, int64_t r, int64_t s) {
    left = l;
    right = r;
    sum = s;
  }
};

std::ostream& operator<<(std::ostream& stream, AnswerStruct ans) {
  stream << ans.left + 1 << ' ' << ans.right + 1 << ' ' << ans.sum << std::endl;
  return stream;
}

class SolveClass {
 private:
  std::vector<int64_t> pref_sum_;
  std::vector<int64_t> pref_min_;

 public:
  explicit SolveClass(std::vector<int64_t> data) {
    pref_sum_.resize(data.size(), data[0]);
    pref_min_.resize(data.size(), -1);
    for (size_t i = 1; i < data.size(); ++i) {
      pref_sum_[i] = data[i] + pref_sum_[i - 1];
    }
    if (pref_sum_[0] < 0 && 1 < data.size()) {
      pref_min_[1] = 0;
    }
    for (size_t i = 1; i < data.size() - 1; ++i) {
      if (pref_sum_[pref_min_[i]] > pref_sum_[i]) {
        pref_min_[i + 1] = static_cast<int64_t>(i);
      } else {
        pref_min_[i + 1] = pref_min_[i];
      }
    }
  }
  AnswerStruct MaxSegment() {
    AnswerStruct result(0, 0, pref_sum_[0]);
    for (size_t i = 1; i < pref_sum_.size(); ++i) {
      if (result.sum < pref_sum_[i]) {
        result.left = 0;
        result.right = static_cast<int64_t>(i);
        result.sum = pref_sum_[i];
      }
    }
    for (size_t i = 0; i < pref_sum_.size(); ++i) {
      if (result.sum < pref_sum_[i] - pref_sum_[pref_min_[i]]) {
        result = AnswerStruct(pref_min_[i] + 1, static_cast<int64_t>(i), pref_sum_[i] - pref_sum_[pref_min_[i]]);
      }
    }
    return result;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t n = 0;
  std::cin >> n;
  std::vector<int64_t> data(n);
  for (auto i = 0; i < n; ++i) {
    std::cin >> data[i];
  }
  SolveClass solve(data);
  std::cout << solve.MaxSegment();
}