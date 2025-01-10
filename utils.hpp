#pragma once
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <vector>

template <typename... Args>
#ifdef DEBUG
void debug_printf(const char *format, Args const &...args) {
  printf(format, args...);
}
#else
void debug_printf(const char *, Args const &...) {
}
#endif

template <class T>
void save_vector(std::string filename, std::vector<T> &v) {
  std::ofstream ofs(filename);
  for (auto i : v) {
    ofs << i << std::endl;
  }
}

int find_max(std::vector<std::vector<int>> &v) {
  int max_value = 0;
  for (const auto &row : v) {
    for (int value : row) {
      if (value > max_value) {
        max_value = value;
      }
    }
  }
  return max_value;
}

std::vector<double> average_frequency_distribution(std::vector<std::vector<int>> &v) {
  std::vector<size_t> array_size;
  for (auto &dd : v) {
    array_size.push_back(dd.size());
  }
  auto max_size = *std::max_element(array_size.begin(), array_size.end());
  std::vector<double> ave(max_size, 0.0);
  double vinv = 1.0 / static_cast<double>(v.size());
  for (auto &dd : v) {
    for (int i = 0; i < static_cast<int>(dd.size()); i++) {
      ave[i] += dd[i] * vinv;
    }
  }
  return ave;
}

std::vector<int> calculate_frequency_distribution(std::vector<int> &v) {
  auto max = *std::max_element(v.begin(), v.end());
  std::vector<int> dd(max + 1, 0);
  for (auto d : v) {
    dd[d]++;
  }
  return dd;
}