#pragma once

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

void simulate(const double alpha, const double beta, std::mt19937 &rng) {
  const int m = 4;
  const int N = 10000;
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