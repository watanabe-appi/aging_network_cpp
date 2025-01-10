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
  void add(Node *n) {
    links.push_back(n);
  }

  int degree() {
    return static_cast<int>(links.size());
  }

  void show() {
    printf("Node: (%d) fitness = %d\n", id, fitness);
    for (auto n : links) {
      printf("%d,", n->id);
    }
    printf("\n");
  }

private:
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
  // 初期ネットワーク追加用
  void add(std::mt19937 &rng) {
    std::uniform_int_distribution<> ud(1, 100);
    int fitness = ud(rng);
    nodes.push_back(new Node(fitness));
  }

  // Bianconi-Barabasiで追加
  void add_BB(const int m, std::mt19937 &rng) {
    // あらかじめ重みを作成しておく
    std::vector<int> weights;
    for (auto &n : nodes) {
      int links = n->degree();
      int fitness = n->fitness;
      weights.push_back(links * fitness);
    }
    WalkerAlias<int> alias_method(weights);
    std::vector<int> indices = alias_method.select(m, rng);

    // 新しいノードを追加
    const int j = size();
    add(rng);
    for (auto i : indices) {
      connect(i, j);
    }
  }

  // 番号を振り直す
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
    assign_id();
    printf("-------------------------------\n");
    printf("Edges:\n");
    for (auto e : edges) {
      e.show();
    }
  }

  void connect(int i, int j) {
    assert(i < static_cast<int>(nodes.size()));
    assert(j < static_cast<int>(nodes.size()));
    Node *ni = nodes[i];
    Node *nj = nodes[j];
    ni->add(nj);
    nj->add(ni);
    edges.push_back(Edge(ni, nj));
    // printf("connect %d-%d\n", i, j);
  }

  // 次数分布関数を返す
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

private:
  std::vector<Edge> edges;
};