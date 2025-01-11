#pragma once
#include "network.hpp"
#include <random>

void test_simulation() {
  /*
  simulate(-1.5, 2.0, rng);
  simulate(3.0, 2.5, rng);
  simulate(-1.5, -1.5, rng);
  simulate(2.0, -1.0, rng);
*/
}

int pos2index(int x, int y, const int L) {
  x = (x + L) % L;
  y = (y + L) % L;
  return x + y * L;
}

void test_percolation(std::mt19937 &rng) {
  const int L = 100;
  Network network;
  for (int i = 0; i < L * L; ++i) {
    network.add(rng);
  }
  for (int ix = 0; ix < L; ++ix) {
    for (int iy = 0; iy < L; ++iy) {
      int i = pos2index(ix, iy, L);
      int j = pos2index(ix + 1, iy, L);
      int k = pos2index(ix, iy + 1, L);
      network.connect(i, j);
      network.connect(i, k);
    }
  }
  const int nd = 100;
  const int n_sample = 1;
  auto result = network.calculate_percolation(nd, n_sample, rng);
  double ninv = 1.0 / static_cast<double>(nd);
  for (int i = 0; i < nd; ++i) {
    double p = i * ninv;
    printf("%f %f\n", p, result[i]);
  }
}

void test_average() {
  std::vector<std::vector<int>> v;
  for (int i = 0; i < 10; i++) {
    std::vector<int> d;
    for (int j = 0; j < 10; j++) {
      d.push_back(i + j);
    }
    v.push_back(d);
  }
  util::show_all(util::average_vector(v));
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
  auto ave = util::average_frequency_distribution(data);
  util::show_all(ave);
}
