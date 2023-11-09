#include <iostream>
#include <vector>
#include <cmath>

class SparseTable {
 private:
  std::vector<std::vector<std::vector<std::vector<int>>>> sparse_table_;

 public:
  explicit SparseTable(std::vector<std::vector<int>> data) {
    sparse_table_.resize(data.size() + 1);
    for (size_t i = 1; i < data.size() + 1; ++i) {
      sparse_table_[i].resize(data[0].size() + 1);
      size_t count = (static_cast<size_t>(std::log2(data.size() - (i - 1))) + 1);
      for (size_t j = 1; j < data[0].size() + 1; ++j) {
        sparse_table_[i][j].resize(count);
        size_t count1 = (static_cast<size_t>(std::log2(data[0].size() - (j - 1))) + 1);
        for (size_t k = 0; k < count; ++k) {
          sparse_table_[i][j][k].resize(count1);
        }
      }
    }
    for (size_t i = 1; i < data.size() + 1; ++i) {
      for (size_t j = 1; j < data[0].size() + 1; ++j) {
        sparse_table_[i][j][0][0] = data[i - 1][j - 1];
      }
    }
    for (size_t i = 1; i < data.size() + 1; ++i) {
      for (size_t k1 = 1, size_of_j_line = 0; k1 < static_cast<size_t>(log2(static_cast<double>(data[0].size()))) + 1;
           ++k1) {
        size_of_j_line = data[0].size() - (1 << k1) + 1;
        for (size_t j = 1; j < size_of_j_line + 1; ++j) {
          auto ind = j + (1 << (k1 - 1));
          sparse_table_[i][j][0][k1] = std::min(sparse_table_[i][j][0][k1 - 1], sparse_table_[i][ind][0][k1 - 1]);
        }
      }
    }
    for (size_t k1 = 1, size_of_i_line = 0; k1 < static_cast<size_t>(log2(static_cast<double>(data.size()))) + 1;
         ++k1) {
      size_of_i_line = data.size() - (1 << k1) + 1;
      for (size_t i = 1; i < size_of_i_line + 1; ++i) {
        for (size_t k2 = 0, size_of_j_line = 0; k2 < static_cast<size_t>(log2(static_cast<double>(data[0].size()))) + 1;
             ++k2) {
          size_of_j_line = data[0].size() - (1 << k2) + 1;
          for (size_t j = 1; j < size_of_j_line + 1; ++j) {
            sparse_table_[i][j][k1][k2] =
                std::min(sparse_table_[i][j][k1 - 1][k2], sparse_table_[i + (1 << (k1 - 1))][j][k1 - 1][k2]);
          }
        }
      }
    }
  }
  int GetMin(size_t x1, size_t x2, size_t y1, size_t y2) {
    auto x = static_cast<size_t>(std::log2(x2 - x1 + 1));
    auto y = static_cast<size_t>(std::log2(y2 - y1 + 1));
    auto k1 = (1 << x) - 1;
    auto k2 = (1 << y) - 1;
    int result1 = std::min(sparse_table_[x1][y1][x][y], sparse_table_[x2 - k1][y1][x][y]);
    int result2 = std::min(sparse_table_[x1][y2 - k2][x][y], sparse_table_[x2 - k1][y2 - k2][x][y]);
    return std::min(result1, result2);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n = 0;
  size_t m = 0;
  std::cin >> n >> m;
  std::vector<std::vector<int>> data(n, std::vector<int>(m));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      std::cin >> data[i][j];
    }
  }
  SparseTable st(data);
  size_t k = 0;
  std::cin >> k;
  size_t x1 = 0;
  size_t y1 = 0;
  size_t x2 = 0;
  size_t y2 = 0;
  for (size_t i = 0; i < k; ++i) {
    std::cin >> x1 >> y1 >> x2 >> y2;
    std::cout << st.GetMin(x1, x2, y1, y2) << '\n';
  }
  return 0;
}