#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>

class Node {
public:
  int id;
  Node(int _id)
      : id(_id) {
  }
  void add(Node *n) {
    links.push_back(n);
  }

  void show() {
    printf("Node: %d\n", id);
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
  Network() {
    last_id = 0;
  }
  void add() {
    nodes.push_back(Node(last_id));
    last_id++;
  }

  void show_nodes() {
    printf("-------------------------------\n");
    printf("Nodes:\n");
    for (auto n : nodes) {
      n.show();
    }
  }

  void show_edges() {
    printf("-------------------------------\n");
    printf("Edges:\n");
    for (auto e : edges) {
      e.show();
    }
  }

  void connect(int i, int j) {
    Node *ni = &(nodes[i]);
    Node *nj = &(nodes[j]);
    ni->add(nj);
    nj->add(ni);
    edges.push_back(Edge(ni, nj));
  }

  std::vector<Node> nodes;

private:
  int last_id;
  std::vector<Edge> edges;
};