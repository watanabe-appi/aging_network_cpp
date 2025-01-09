#include "network.hpp"
#include <cstdio>

void make_initial_network(int m, Network &network) {
  for (int i = 0; i < m; i++) {
    network.add();
  }
  for (int i = 0; i < m - 1; i++) {
    for (int j = i + 1; j < m; j++) {
      network.connect(i, j);
    }
  }
  network.show_nodes();
  network.show_edges();
}

int main() {
  Network network;
  make_initial_network(4, network);
}