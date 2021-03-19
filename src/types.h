#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <cstdint>

struct Square {
  int r, c;
  Square(int r, int c) : r(r), c(c) {}
  Square offset(int a, int b) const { return {r + a, c + b}; }
  Square offset(const Square& s) const { return {r + s.r, c + s.c}; }
  bool valid() const { return 0 <= r && r < 5 && 0 <= c && c < 5; }
  bool operator<(const Square& s) const {
    if (r != s.r) return r < s.r;
    return c < s.c;
  }
};

template <typename T>
struct Grid {
  std::array<std::array<T, 5>, 5> data;
  T get(const Square& s) const { return data[s.r][s.c]; }
  void set(const Square& s, const T& v) { data[s.r][s.c] = v; }
  bool operator==(const Grid& rhs) const { return data == rhs.data; }
};

template <class T>
inline void hash_combine(std::size_t& s, const T& v) {
  std::hash<T> h;
  s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

template <typename T>
struct std::hash<Grid<T>> {
  std::size_t operator()(const Grid<T>& grid) const {
    std::size_t hsh = 0;
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        hash_combine(hsh, grid.data[i][j]);
      }
    }
    return hsh;
  }
};

enum AnimalId : int {
  UNKNOWN = -1,
  NOTHING = -2,
};

#endif  // !TYPES_H_INCLUDED
