#pragma once
#include <algorithm>
#include <vector>

std::vector<int> calculate_frequency_distribution(std::vector<int> &v) {
  auto max = *std::max_element(v.begin(), v.end());
  std::vector<int> dd(max + 1, 0);
  for (auto d : v) {
    dd[d]++;
  }
  return dd;
}