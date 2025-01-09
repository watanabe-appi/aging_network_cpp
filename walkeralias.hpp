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
    // エラー処理: 空のベクトルの場合
    if (weights.empty()) {
      throw std::invalid_argument("The input vector must not be empty.");
    }

    // 重みの合計を計算
    T total_weight = std::accumulate(weights.begin(), weights.end(), static_cast<T>(0));

    // 正規化された確率を計算
    size_t n = weights.size();
    std::vector<double> probabilities(n);
    for (size_t i = 0; i < n; ++i) {
      probabilities[i] = static_cast<double>(weights[i]) / total_weight * n;
    }

    // Small と Large のリストを作成
    std::vector<size_t> small;
    std::vector<size_t> large;
    for (size_t i = 0; i < n; ++i) {
      if (probabilities[i] < 1.0) {
        small.push_back(i);
      } else {
        large.push_back(i);
      }
    }

    // Alias テーブルと Probability テーブルを構築
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

    // 残った要素を処理
    while (!small.empty()) {
      probability[small.back()] = 1.0;
      small.pop_back();
    }
    while (!large.empty()) {
      probability[large.back()] = 1.0;
      large.pop_back();
    }
  }

  std::vector<int> select(size_t m, std::mt19937 &rng) const {
    // ランダムデバイス

    if (m > probability.size()) {
      throw std::invalid_argument("m cannot be greater than the number of elements.");
    }

    std::unordered_set<int> selected_indices;
    std::vector<int> result;

    while (result.size() < m) {
      // インデックスをランダムに選択
      std::uniform_int_distribution<size_t> dist_index(0, probability.size() - 1);
      size_t index = dist_index(rng);

      // 確率的にAliasを使用
      std::uniform_real_distribution<double> dist_prob(0.0, 1.0);
      int chosen = (dist_prob(rng) < probability[index]) ? index : alias[index];

      // 重複しない場合のみ追加
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
