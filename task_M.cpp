#include <iostream>
#include <vector>

uint32_t NextRand(const uint32_t& a, const uint32_t& b, uint32_t& cur) {
  cur = cur * a + b;
  return cur >> 8;
}

int main() {
  uint32_t m = 0;
  uint32_t q = 0;
  std::cin >> m >> q;
  uint32_t a = 0;
  uint32_t b = 0;
  std::cin >> a >> b;
  uint32_t n = 1 << 24;
  std::vector<uint32_t> smeshariki(n + 2, 0);
  uint32_t cur = 0;
  for (uint32_t i = 0; i < m; ++i) {
    uint32_t delta = NextRand(a, b, cur);
    uint32_t l = NextRand(a, b, cur);
    uint32_t r = NextRand(a, b, cur);
    if (l > r) {
      uint32_t temp = l;
      l = r;
      r = temp;
    }
    smeshariki[l] += delta;
    smeshariki[r + 1] -= delta;
  }
  std::vector<uint32_t> pref(n + 1);
  pref[0] = smeshariki[0];
  pref[1] = 2 * smeshariki[0] + smeshariki[1];
  uint32_t add = smeshariki[0] + smeshariki[1];
  for (uint32_t i = 2; i <= n; ++i) {
    pref[i] = pref[i - 1] + smeshariki[i] + add;
    add += smeshariki[i];
  }
  uint32_t result = 0;
  for (uint32_t i = 0; i < q; ++i) {
    uint32_t l = NextRand(a, b, cur);
    uint32_t r = NextRand(a, b, cur);
    if (l > r) {
      uint32_t temp = l;
      l = r;
      r = temp;
    }
    if (l == 0) {
      result += pref[r];
    } else {
      result += (pref[r] - pref[l - 1]);
    }
  }
  std::cout << result;
  return 0;
}
// NOLINT