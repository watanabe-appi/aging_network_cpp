#include "network.hpp"
#include "walkeralias.hpp"
#include <cstdio>
#include <iostream>
#include <random>

void make_initial_network(int m, std::mt19937 &rng, Network &network) {
  for (int i = 0; i < m; i++) {
    network.add(rng);
  }
  for (int i = 0; i < m - 1; i++) {
    for (int j = i + 1; j < m; j++) {
      network.connect(i, j);
    }
  }
  network.show_nodes();
  network.show_edges();
}

void test_alias(std::mt19937 &rng) {
  std::vector<double> weights = {1.5, 1.5, 4, 3};

  WalkerAlias<double> alias_method(weights);

  // 結果を統計的に確認
  std::vector<int> counts(weights.size(), 0);
  const int trials = 100000;
  for (int i = 0; i < trials; ++i) {
    int index = alias_method.select(rng);
    counts[index]++;
  }

  // 結果を出力
  for (size_t i = 0; i < counts.size(); ++i) {
    std::cout << "Index " << i << ": " << counts[i] << " times ("
              << (static_cast<double>(counts[i]) / trials * 100) << "%)\n";
  }
}

int main() {
  int seed = 1;
  std::mt19937 rng(seed);
  Network network;
  //make_initial_network(4, mt, network);
  test_alias(rng);
}