#pragma once

#include "utils.hpp"
#include "walkeralias.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

class Node {
public:
  int id;
  bool remove_flag;
  int fitness;
  Node(int _fitness)
      : id(0), fitness(_fitness) {
    remove_flag = false;
  }

  ~Node() {
    debug_printf("Deleted: %d\n", id);
  }

  void add(Node *n) {
    links.push_back(n);
  }

  void remove(Node *n) {
    auto it = std::remove(links.begin(), links.end(), n);
    if (it != links.end()) {
      debug_printf("Node %d removes %d\n", id, n->id);
      links.erase(it, links.end());
    }
  }

  int degree() {
    return static_cast<int>(links.size());
  }

  bool is_isolated() {
    return degree() == 0;
  }

  void show() {
    printf("Node: (%d) fitness = %d\n", id, fitness);
    for (auto n : links) {
      printf("%d,", n->id);
    }
    printf("\n");
  }
  std::vector<Node *> links;
};

struct Edge {
  const Node *left;
  const Node *right;
  Edge(Node *_left, Node *_right)
      : left(_left), right(_right) {
  }
  void show() {
    printf("%d-%d\n", left->id, right->id);
  }
};

class Network {
public:
  void add(std::mt19937 &rng) {
    std::uniform_int_distribution<> ud(1, 100);
    int fitness = ud(rng);
    nodes.push_back(new Node(fitness));
  }

  // adds a node with the Bianconi-Barabasi criterion
  void add_BB(const int m, std::mt19937 &rng) {
    std::vector<int> weights;
    for (auto &n : nodes) {
      int links = n->degree();
      int fitness = n->fitness;
      weights.push_back(links * fitness);
    }
    WalkerAlias<int> alias_method(weights);
    std::vector<int> indices = alias_method.sample_unique(m, rng);

    const int j = size();
    add(rng);
    for (auto i : indices) {
      connect(i, j);
    }
  }

  // adds a node with adding parameter beta
  void add_aging(double beta, int m, std::mt19937 &rng) {
    std::vector<double> weights;
    for (auto &n : nodes) {
      double links = static_cast<double>(n->degree());
      double fitness = static_cast<double>(n->fitness);
      weights.push_back(std::pow(links * fitness, beta));
    }
    WalkerAlias<double> alias_method(weights);
    std::vector<int> indices = alias_method.sample_unique(m, rng);
    const int j = size();
    add(rng);
    for (auto i : indices) {
      connect(i, j);
    }
  }

  void aging_step(double alpha, double beta, int m, int N, std::mt19937 &rng) {
    remove_aging(alpha, rng);
    while (size() < N) {
      add_aging(beta, m, rng);
    }
  }

  void connect(int i, int j) {
    assert(i < static_cast<int>(nodes.size()));
    assert(j < static_cast<int>(nodes.size()));
    Node *ni = nodes[i];
    Node *nj = nodes[j];
    ni->add(nj);
    nj->add(ni);
  }

  std::vector<Edge> generate_edge_list() {
    std::vector<Edge> edges;
    assign_id();
    for (auto ni : nodes) {
      for (auto nj : ni->links) {
        if (ni->id < nj->id) {
          edges.push_back(Edge(ni, nj));
        }
      }
    }
    return edges;
  }

  void remove_aging(double alpha, std::mt19937 &rng) {
    std::vector<double> weights;
    for (auto &n : nodes) {
      double links = static_cast<double>(n->degree());
      weights.push_back(std::pow(links, alpha));
    }
    WalkerAlias<double> alias_method(weights);
    int index = alias_method.sample(rng);
    debug_printf("Network removes %d\n", index);
    remove_at(index);
    remove_isolated_nodes();
  }

  void remove_at(int index) {
    Node *node_to_be_removed = nodes[index];
    nodes.erase(nodes.begin() + index);
    for (auto n : nodes) {
      n->remove(node_to_be_removed);
    }

    delete node_to_be_removed;
  }

  void remove_isolated_nodes() {
    nodes.erase(
        std::remove_if(nodes.begin(), nodes.end(),
                       [](Node *node) {
                         bool is_isolated = node->is_isolated();
                         if (is_isolated) {
                           delete node;
                         }
                         return is_isolated;
                       }),
        nodes.end());
  }

  // エッジで接続されているノードのうち、最大のクラスターのみ残す
  void filter_largest_cluster() {
    std::vector<int> cluster;
    for (int i = 0; i < size(); ++i) {
      cluster.push_back(i);
    }
    auto edges = generate_edge_list();
    assign_id();
    for (auto e : edges) {
      int i = e.left->id;
      int j = e.right->id;
      util::unite(i, j, cluster);
    }
    util::find_all(cluster);
    // Find the cluster index of the maximum cluster
    std::vector<int> cluster_size(cluster.size());
    for (auto i : cluster) {
      cluster_size[i]++;
    }
    int index_of_max_cluster = util::find_max_index(cluster_size);
    for (auto n : nodes) {
      if (cluster[n->id] != index_of_max_cluster) {
        n->remove_flag = true;
      }
    }
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [](Node *n) { return n->remove_flag; }), nodes.end());
  }

  // 現在のネットワークにおいて、与えられた確率pでエッジをactivateした時の最大クラスターの割合を返す。

  double calculate_perocaltion_probability(const double p, std::mt19937 &rng) {
    std::uniform_real_distribution<> ud(0.0, 1.0);
    std::vector<int> cluster;
    for (int i = 0; i < size(); ++i) {
      cluster.push_back(i);
    }
    auto edges = generate_edge_list();
    assign_id();
    for (auto e : edges) {
      int i = e.left->id;
      int j = e.right->id;
      if (ud(rng) < p) {
        util::unite(i, j, cluster);
      }
    }
    // Find the cluster index of the maximum cluster
    util::find_all(cluster);
    std::vector<int> cluster_size(cluster.size());
    for (auto i : cluster) {
      cluster_size[i]++;
    }
    int index_of_max_cluster = util::find_max_index(cluster_size);
    double probability = cluster_size[index_of_max_cluster];
    probability /= size();
    return probability;
  }

  std::vector<double> calculate_percolation(const int nd, const int n_sample, std::mt19937 &rng) {
    //最大クラスターノードのみ残す
    filter_largest_cluster();
    std::vector<double> result;
    double ninv = 1.0 / static_cast<double>(nd);
    for (int i = 0; i <= nd; ++i) {
      double p = i * ninv;
      double probability = 0.0;
      for (int j = 0; j < n_sample; ++j) {
        probability += calculate_perocaltion_probability(p, rng);
      }
      probability /= n_sample;
      result.push_back(probability);
    }
    return result;
  }

  // for observation
  double calculate_degree_average() {
    double average = 0;
    for (auto n : nodes) {
      average += n->degree();
    }
    average /= nodes.size();
    return average;
  }

  double calculate_degree_variance() {
    double average = calculate_degree_average();
    double variance = 0.0;
    for (auto n : nodes) {
      double diff = n->degree() - average;
      variance += diff * diff;
    }
    variance /= nodes.size();
    return variance;
  }

  std::vector<int> degree_distribution() {
    std::vector<int> degrees;
    for (auto n : nodes) {
      degrees.push_back(n->degree());
    }
    return util::calculate_frequency_distribution(degrees);
  }

  int size() {
    return static_cast<int>(nodes.size());
  }

  // For Debug

  void assign_id() {
    int id = 0;
    for (auto &n : nodes) {
      n->id = id;
      id++;
    }
  }

  void show_nodes() {
    assign_id();
    printf("-------------------------------\n");
    printf("Nodes:\n");
    for (auto n : nodes) {
      n->show();
    }
  }

  void show_edges() {
    auto edges = generate_edge_list();
    printf("-------------------------------\n");
    printf("Edges:\n");
    for (auto e : edges) {
      e.show();
    }
  }

  std::vector<Node *> nodes;

private:
};