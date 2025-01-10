#include "network.hpp"
#include "walkeralias.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <unordered_set>

void make_initial_network(int m, std::mt19937 &rng, Network &network) {
  for (int i = 0; i < m; i++) {
    network.add(rng);
  }
  for (int i = 0; i < m - 1; i++) {
    for (int j = i + 1; j < m; j++) {
      network.connect(i, j);
    }
  }
}

void test_alias(std::mt19937 &rng) {
  std::vector<double> weights = {1.0, 3.0, 2.0, 4.0};

  WalkerAlias<double> alias_method(weights);

  // 結果を統計的に確認
  const int trials = 100000;
  const size_t m = 2;
  std::map<int, int> counts;

  for (int i = 0; i < trials; ++i) {
    std::vector<int> indices = alias_method.select(m, rng);
    for (int index : indices) {
      counts[index]++;
    }
  }

  // 結果を出力
  std::cout << "Index selection ratios:" << std::endl;
  for (size_t i = 0; i < weights.size(); ++i) {
    double ratio = static_cast<double>(counts[i]) / (trials * m);
    std::cout << "Index " << i << ": " << counts[i] << " times ("
              << (ratio * 100) << "%)\n";
  }
}

template <class T>
void show_all(std::vector<T> &v) {
  for (size_t i = 0; i < v.size(); ++i) {
    std::cout << i << " " << v[i] << std::endl;
  }
}

std::vector<int> calc(int m, int N, std::mt19937 &rng) {
  Network network;
  make_initial_network(m, rng, network);
  while (network.size() < N) {
    network.add_BB(m, rng);
  }
  return network.degree_distribution();
}

int main() {
  int seed = 0;
  const int m = 4;
  const int N = 10000;
  std::mt19937 rng(seed);
  std::vector<std::vector<int>> data;
  for (int i = 0; i < 10; i++) {
    data.push_back(calc(m, N, rng));
  }
  auto ave = average_frequency_distribution(data);
  show_all(ave);
}