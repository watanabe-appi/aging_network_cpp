#pragma once

#include "utils.hpp"
#include "walkeralias.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

class Node {
public:
  int id;
  const int fitness;
  Node(int _fitness)
      : id(0), fitness(_fitness) {
  }

  ~Node() {
    printf("Deleted: %d\n", id);
  }

  void add(Node *n) {
    links.push_back(n);
  }

  void remove(Node *n) {
    auto it = std::remove(links.begin(), links.end(), n);
    if (it != links.end()) {
      printf("Node %d removes %d\n", id, n->id);
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

  void connect(int i, int j) {
    assert(i < static_cast<int>(nodes.size()));
    assert(j < static_cast<int>(nodes.size()));
    Node *ni = nodes[i];
    Node *nj = nodes[j];
    ni->add(nj);
    nj->add(ni);
  }

  void generate_edge_list() {
    edges.clear();
    assign_id();
    for (auto ni : nodes) {
      for (auto nj : ni->links) {
        if (ni->id < nj->id) {
          edges.push_back(Edge(ni, nj));
        }
      }
    }
  }

  void remove_aging(double alpha, std::mt19937 &rng) {
    std::vector<double> weights;
    for (auto &n : nodes) {
      double links = static_cast<double>(n->degree());
      weights.push_back(std::pow(links, alpha));
    }
    WalkerAlias<double> alias_method(weights);
    int index = alias_method.sample_unique(1, rng)[0];
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

  std::vector<int> degree_distribution() {
    std::vector<int> degrees;
    for (auto n : nodes) {
      degrees.push_back(n->degree());
    }
    return calculate_frequency_distribution(degrees);
  }

  int size() {
    return static_cast<int>(nodes.size());
  }

  std::vector<Node *> nodes;

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
    generate_edge_list();
    printf("-------------------------------\n");
    printf("Edges:\n");
    for (auto e : edges) {
      e.show();
    }
  }

private:
  std::vector<Edge> edges;
};