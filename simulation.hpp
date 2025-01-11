#pragma once

#include "network.hpp"
#include "param/param.hpp"
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

void simulate(param::parameter &param, Data &data, std::mt19937 &rng) {
  const double alpha = param.get<double>("alpha");
  const double beta = param.get<double>("beta");
  const int m = 4;
  const int N = param.get<int>("system_size");
  std::vector<double> degree_average, degree_variance;
  Network network;
  make_initial_network(m, rng, network);
  while (network.size() < N) {
    network.add_BB(m, rng);
  }

  const int aging_step = param.get<int>("aging_step", N);
  for (int i = 0; i < aging_step; ++i) {
    network.aging_step(alpha, beta, m, N, rng);
    degree_average.push_back(network.calculate_degree_average());
    degree_variance.push_back(network.calculate_degree_variance());
  }
  const int nd = param.get<int>("percolation_num_dividions", 100);
  const int percolation_sample = param.get<int>("percolation_sample");
  auto percolation = network.calculate_percolation(nd, percolation_sample, rng);

  data.degree_distribution.push_back(network.degree_distribution());
  data.degree_average.push_back(degree_average);
  data.degree_variance.push_back(degree_variance);
  data.percolation.push_back(percolation);
}

void simulate_sample(param::parameter &param, std::mt19937 &rng) {
  const int n_sample = param.get<int>("n_sample");
  Data data;
  for (int i = 0; i < n_sample; ++i) {
    simulate(param, data, rng);
  }

  const int nd = 100;
  double ninv = 1.0 / static_cast<double>(nd);
  std::vector<double> probability;
  for (int i = 0; i <= nd; ++i) {
    probability.push_back(i * ninv);
  }

  const double alpha = param.get<double>("alpha");
  const double beta = param.get<double>("beta");
  const int N = param.get<int>("system_size");
  std::string base = util::param2name(N, alpha, beta);
  std::string degree_distribution_file = "degree_distribution_" + base + ".dat";
  std::string degree_average_file = "degree_average_" + base + ".dat";
  std::string degree_variance_file = "degree_variance_" + base + ".dat";
  std::string percolation_file = "percolation_" + base + ".dat";
  util::save_vector(degree_distribution_file, util::average_frequency_distribution(data.degree_distribution));
  util::save_vector(degree_average_file, util::average_vector(data.degree_average));
  util::save_vector(degree_variance_file, util::average_vector(data.degree_variance));
  util::save_vector(percolation_file, util::average_vector(data.percolation));
}