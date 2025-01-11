//#include "network.hpp"
#include "param/param.hpp"
#include "simulation.hpp"
#include "test.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <random>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: ./aging_network parameterfile\n");
    return -1;
  }
  std::string paramfile = argv[1];
  param::parameter param(paramfile);
  if (!param) {
    std::cerr << "An error occurred while loading " << paramfile << std::endl;
    return -1;
  }
  int seed = param.get<int>("seed", 0);
  std::mt19937 rng(seed);
  simulate_sample(param, rng);
}
