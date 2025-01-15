#pragma once
#ifndef UTIL
#define UTIL

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
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

namespace util {

template <class T>
int find_max_index(std::vector<T> &v) {
  auto max_it = std::max_element(v.begin(), v.end());
  return std::distance(v.begin(), max_it);
}

int find(int index, std::vector<int> &cluster) {
  while (index != cluster[index]) {
    index = cluster[index];
  }
  return index;
}

void unite(int i, int j, std::vector<int> &cluster) {
  i = find(i, cluster);
  j = find(j, cluster);
  if (i < j) {
    cluster[j] = i;
  } else {
    cluster[i] = j;
  }
}

void find_all(std::vector<int> &cluster) {
  for (size_t i = 0; i < cluster.size(); ++i) {
    int index = find(i, cluster);
    cluster[i] = index;
  }
}

template <class T>
void show_all(const std::vector<T> &v) {
  for (size_t i = 0; i < v.size(); ++i) {
    std::cout << i << " " << v[i] << std::endl;
  }
}

std::string param2name(const int N, double alpha, double beta) {
  char buf[100];
  int ia = static_cast<int>(alpha * 10);
  int ib = static_cast<int>(beta * 10);
  snprintf(buf, sizeof(buf), "N%05d_a%02d_b%02d", N, ia, ib);
  std::string name = buf;
  return name;
}

template <class T>
void save_vector(std::string filename, const std::vector<T> &v) {
  std::ofstream ofs(filename);
  for (auto i : v) {
    ofs << i << std::endl;
  }
  printf("%s\n", filename.c_str());
}

template <class T1, class T2>
void save_vector(std::string filename, const std::vector<T1> &v1, const std::vector<T2> &v2) {
  assert(v1.size() == v2.size());
  std::ofstream ofs(filename);
  for (size_t i = 0; i < v1.size(); ++i) {
    ofs << v1[i] << " " << v2[i] << std::endl;
  }
  printf("%s\n", filename.c_str());
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

template <class T>
std::vector<double> average_vector(std::vector<std::vector<T>> &v) {
  std::vector<double> average(v[0].size());
  double vinv = 1.0 / static_cast<double>(v.size());
  for (auto &dd : v) {
    for (int i = 0; i < static_cast<int>(dd.size()); i++) {
      average[i] += dd[i] * vinv;
    }
  }
  return average;
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

} // namespace util

#endif