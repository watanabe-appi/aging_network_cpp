#include <numeric>
#include <random>
#include <stdexcept>
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

  int select() const {
    // ランダムデバイス
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());

    // インデックスをランダムに選択
    std::uniform_int_distribution<size_t> dist_index(0, probability.size() - 1);
    size_t index = dist_index(gen);

    // 確率的にAliasを使用
    std::uniform_real_distribution<double> dist_prob(0.0, 1.0);
    if (dist_prob(gen) < probability[index]) {
      return static_cast<int>(index);
    } else {
      return static_cast<int>(alias[index]);
    }
  }

private:
  std::vector<double> probability;
  std::vector<size_t> alias;
};
