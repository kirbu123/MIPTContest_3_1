#include <iostream>
#include <vector>

struct Point {
 public:
  int x = 0;
  int y = 0;
  int z = 0;
  Point() = default;
  Point(int x_inp, int y_inp, int z_inp) {
    x = x_inp;
    y = y_inp;
    z = z_inp;
  }
  Point(Point& point) {
    x = point.x;
    y = point.y;
    x = point.z;
  }
};

std::istream& operator>>(std::istream& stream, Point& point) {
  stream >> point.x >> point.y >> point.z;
  return stream;
}

bool operator==(const Point& point1, const Point& point2) {
  return (point1.x == point2.x && point1.y == point2.y && point1.z == point2.z);
}

bool operator==(Point&& point1, Point&& point2) {
  return (point1.x == point2.x && point1.y == point2.y && point1.z == point2.z);
}

class FenwickTree {
 private:
  std::vector<std::vector<std::vector<int>>> ft_;
  int GetPrefSum(Point& point) const {
    int result = 0;
    for (int x = point.x; x >= 0; x = (x & (x + 1)) - 1) {
      for (int y = point.y; y >= 0; y = (y & (y + 1)) - 1) {
        for (int z = point.z; z >= 0; z = (z & (z + 1)) - 1) {
          result += ft_[x][y][z];
        }
      }
    }
    return result;
  }
  int GetPrefSum(Point&& point) const {
    int result = 0;
    for (int x = point.x; x >= 0; x = (x & (x + 1)) - 1) {
      for (int y = point.y; y >= 0; y = (y & (y + 1)) - 1) {
        for (int z = point.z; z >= 0; z = (z & (z + 1)) - 1) {
          result += ft_[x][y][z];
        }
      }
    }
    return result;
  }

 public:
  explicit FenwickTree(int n) {
    ft_.resize(n, std::vector<std::vector<int>>(n, std::vector<int>(n, 0)));
  }
  int Update(Point& point, int delta) {
    for (int x = point.x; x < static_cast<int>(ft_.size()); x = x | (x + 1)) {
      for (int y = point.y; y < static_cast<int>(ft_.size()); y = y | (y + 1)) {
        for (int z = point.z; z < static_cast<int>(ft_.size()); z = z | (z + 1)) {
          ft_[x][y][z] += delta;
        }
      }
    }
    return delta;
  }
  int GetSum(Point& first, Point& second) const {
    if (first == Point(0, 0, 0)) {
      return GetPrefSum(second);
    }
    if (second == Point(0, 0, 0)) {
      return GetPrefSum(first);
    }
    int x1 = std::min(first.x, second.x);
    int x2 = std::max(first.x, second.x);
    int y1 = std::min(first.y, second.y);
    int y2 = std::max(first.y, second.y);
    int z1 = std::min(first.z, second.z);
    int z2 = std::max(first.z, second.z);
    return GetPrefSum(Point(x2, y2, z2)) - GetPrefSum(Point(x2, y1 - 1, z2)) - GetPrefSum(Point(x1 - 1, y2, z2)) +
           GetPrefSum(Point(x1 - 1, y1 - 1, z2)) - GetPrefSum(Point(x2, y2, z1 - 1)) +
           GetPrefSum(Point(x2, y1 - 1, z1 - 1)) + GetPrefSum(Point(x1 - 1, y2, z1 - 1)) -
           GetPrefSum(Point(x1 - 1, y1 - 1, z1 - 1));
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n = 0;
  std::cin >> n;
  int command = 0;
  int delta = 0;
  Point point1;
  Point point2;
  FenwickTree tree(n);
  while (true) {
    std::cin >> command;
    if (command == 1) {
      std::cin >> point1 >> delta;
      tree.Update(point1, delta);
    } else if (command == 2) {
      std::cin >> point1 >> point2;
      std::cout << tree.GetSum(point1, point2) << '\n';
    } else {
      break;
    }
  }
  return 0;
}