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

void test_alias_sample(std::mt19937 &rng) {
  std::vector<double> weights = {1.0, 3.0, 2.5, 3.5};
  WalkerAlias<double> alias_method(weights);
  const int trials = 100000;
  std::vector<int> counts(weights.size());
  for (int i = 0; i < trials; ++i) {
    int index = alias_method.sample(rng);
    counts[index]++;
  }
  for (size_t i = 0; i < weights.size(); ++i) {
    double ratio = static_cast<double>(counts[i]) / (trials);
    std::cout << "Index " << i << ": " << counts[i] << " times ("
              << (ratio * 100) << "%)\n";
  }
}

void test_alias_sample_unique(std::mt19937 &rng) {
  std::vector<double> weights = {1.0, 3.0, 2.0, 4.0};
  WalkerAlias<double> alias_method(weights);

  // 結果を統計的に確認
  const int trials = 100000;
  const size_t m = 2;
  std::map<int, int> counts;

  for (int i = 0; i < trials; ++i) {
    std::vector<int> indices = alias_method.sample_unique(m, rng);
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

// BB modelの分布を確認するコード
void test_BB(std::mt19937 &rng) {
  const int m = 4;
  const int N = 10000;
  std::vector<std::vector<int>> data;
  for (int i = 0; i < 10; i++) {
    data.push_back(calc(m, N, rng));
  }
  auto ave = average_frequency_distribution(data);
  show_all(ave);
}

void test_remove(std::mt19937 &rng) {
  Network network;
  network.add(rng);
  network.add(rng);
  network.add(rng);
  network.add(rng);
  network.connect(1, 0);
  network.connect(2, 1);
  network.connect(3, 1);
  const double alpha = 10;
  network.assign_id();
  network.remove_aging(alpha, rng);
}

int main() {
  int seed = 0;
  std::mt19937 rng(seed);
  test_remove(rng);
}