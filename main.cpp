#include "network.hpp"
#include "simulation.hpp"
#include "walkeralias.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <unordered_set>

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
  auto ave = util::average_frequency_distribution(data);
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

void test_add_aging(std::mt19937 &rng) {
  Network network;
  network.add(rng);
  network.add(rng);
  network.add(rng);
  network.add(rng);
  network.connect(0, 1);
  network.connect(1, 2);
  network.connect(2, 3);
  const double beta = -2.0;
  const int m = 4;
  network.nodes[0]->fitness = 1;
  network.nodes[1]->fitness = 1;
  network.nodes[2]->fitness = 1;
  network.nodes[3]->fitness = 1;
  network.add_aging(beta, m, rng);
  network.nodes[4]->fitness = 100;
  network.add_aging(beta, m, rng);
  network.show_nodes();
}

void test_aging(std::mt19937 &rng) {
  const int m = 4;
  const int N = 10000;
  const double alpha = -1.5;
  const double beta = 2.0;
  std::vector<double> degree_average, degree_variance;
  Network network;
  make_initial_network(m, rng, network);
  while (network.size() < N) {
    network.add_BB(m, rng);
  }
  for (int i = 0; i < N; ++i) {
    network.aging_step(alpha, beta, m, N, rng);
    degree_average.push_back(network.calculate_degree_average());
    degree_variance.push_back(network.calculate_degree_variance());
  }
  std::string base = util::param2name(alpha, beta);
  std::string degree_distribution_file = "degree_distribution_" + base + ".dat";
  std::string degree_average_file = "degree_average_" + base + ".dat";
  std::string degree_variance_file = "degree_variance_" + base + ".dat";
  util::save_vector(degree_average_file, degree_average);
  util::save_vector(degree_variance_file, degree_variance);
  util::save_vector(degree_distribution_file, network.degree_distribution());
}

int main() {
  int seed = 1;
  std::mt19937 rng(seed);
  simulate(-1.5, 2.0, rng);
  simulate(3.0, 2.5, rng);
  simulate(-1.5, -1.5, rng);
  simulate(2.0, -1.0, rng);
}