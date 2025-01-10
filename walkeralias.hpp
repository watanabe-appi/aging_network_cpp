#pragma once

#include <numeric>
#include <random>
#include <stdexcept>
#include <unordered_set>
#include <vector>

template <typename T>
class WalkerAlias {
public:
  WalkerAlias(const std::vector<T> &weights) {
    if (weights.empty()) {
      throw std::invalid_argument("The input vector must not be empty.");
    }

    T total_weight = std::accumulate(weights.begin(), weights.end(), static_cast<T>(0));

    // 正規化された確率を計算
    size_t n = weights.size();
    std::vector<double> probabilities(n);
    for (size_t i = 0; i < n; ++i) {
      probabilities[i] = static_cast<double>(weights[i]) / total_weight * n;
    }

    std::vector<size_t> small;
    std::vector<size_t> large;
    for (size_t i = 0; i < n; ++i) {
      if (probabilities[i] < 1.0) {
        small.push_back(i);
      } else {
        large.push_back(i);
      }
    }

    alias.resize(n);
    probability.resize(n);

    while (!small.empty() && !large.empty()) {
      size_t l = small.back();
      small.pop_back();
      size_t g = large.back();
      large.pop_back();

      probability[l] = probabilities[l];
      alias[l] = g;

      probabilities[g] = (probabilities[g] + probabilities[l]) - 1.0;
      if (probabilities[g] < 1.0) {
        small.push_back(g);
      } else {
        large.push_back(g);
      }
    }

    while (!small.empty()) {
      probability[small.back()] = 1.0;
      small.pop_back();
    }
    while (!large.empty()) {
      probability[large.back()] = 1.0;
      large.pop_back();
    }
  }

  /**
   * Samples a single index based on the probability distribution.
   *
   * This method uses Walker's Alias Method to efficiently sample one index
   * from a predefined probability distribution. The probability vector and
   * alias table are precomputed, and this function uses a random number
   * generator to select an index according to the probabilities.
   *
   * @param rng A reference to a random number generator (std::mt19937).
   * @return The index of the sampled element based on the probability distribution.
   * @throws std::invalid_argument If the probability vector is empty.
   *
   */
  int sample(std::mt19937 &rng) const {
    if (probability.empty()) {
      throw std::invalid_argument("The probability vector is empty.");
    }

    std::uniform_int_distribution<size_t> dist_index(0, probability.size() - 1);
    size_t index = dist_index(rng);

    std::uniform_real_distribution<double> dist_prob(0.0, 1.0);
    int chosen = (dist_prob(rng) < probability[index]) ? index : alias[index];

    return chosen;
  }

  /**
   * Selects `m` unique indices based on a probability distribution.
   *
   * This method uses a probability vector and an alias table to perform efficient
   * sampling of `m` unique indices. The indices are selected according to their
   * probabilities, and the alias table is used to resolve cases where an index
   * is not directly chosen. The result contains `m` unique indices, and the method
   * ensures no duplicates in the selection.
   *
   * @param m The number of unique indices to select.
   * @param rng A reference to a random number generator (std::mt19937).
   * @return A vector of `m` unique indices.
   * @throws std::invalid_argument If `m` is greater than the number of elements.
   */
  std::vector<int> sample_unique(size_t m, std::mt19937 &rng) const {
    if (m > probability.size()) {
      throw std::invalid_argument("m cannot be greater than the number of elements.");
    }

    std::unordered_set<int> selected_indices;
    std::vector<int> result;

    while (result.size() < m) {
      std::uniform_int_distribution<size_t> dist_index(0, probability.size() - 1);
      size_t index = dist_index(rng);

      std::uniform_real_distribution<double> dist_prob(0.0, 1.0);
      int chosen = (dist_prob(rng) < probability[index]) ? index : alias[index];

      if (selected_indices.find(chosen) == selected_indices.end()) {
        selected_indices.insert(chosen);
        result.push_back(chosen);
      }
    }

    return result;
  }

private:
  std::vector<double> probability;
  std::vector<size_t> alias;
};
