#pragma once

#include "network.hpp"
#include "walkeralias.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <unordered_set>

struct Data {
  std::vector<std::vector<int>> degree_distribution;
  std::vector<std::vector<double>> degree_average, degree_variance, percolation;
};

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

void simulate(const double alpha, const double beta, Data &data, std::mt19937 &rng) {
  const int m = 4;
  const int N = 100;
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
  const int nd = 100;
  const int n_sample = 100;
  auto percolation = network.calculate_percolation(nd, n_sample, rng);

  data.degree_distribution.push_back(network.degree_distribution());
  data.degree_average.push_back(degree_average);
  data.degree_variance.push_back(degree_variance);
  data.percolation.push_back(percolation);
}

void simulate_sample(const double alpha, const double beta, const int n_sample, std::mt19937 &rng) {
  Data data;
  for (int i = 0; i < n_sample; ++i) {
    simulate(alpha, beta, data, rng);
  }

  const int nd = 100;
  double ninv = 1.0 / static_cast<double>(nd);
  std::vector<double> probability;
  for (int i = 0; i <= nd; ++i) {
    probability.push_back(i * ninv);
  }

  std::string base = util::param2name(alpha, beta);
  std::string degree_distribution_file = "degree_distribution_" + base + ".dat";
  std::string degree_average_file = "degree_average_" + base + ".dat";
  std::string degree_variance_file = "degree_variance_" + base + ".dat";
  std::string percolation_file = "percolation_" + base + ".dat";
  util::save_vector(degree_distribution_file, util::average_frequency_distribution(data.degree_distribution));
  util::save_vector(degree_average_file, util::average_vector(data.degree_average));
  util::save_vector(degree_variance_file, util::average_vector(data.degree_variance));
  util::save_vector(percolation_file, util::average_vector(data.percolation));
}