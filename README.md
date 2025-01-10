# Aging simulation of Network

std::vector<Node *> linksにおいて、指定された要素Node *nを含むか確認し、もしあった場合は削除して間を詰めるコード。

Nodeがそれぞれstd::vector<Node *> linksを持っており、そのNodeの要素がstd::vector<Node *>nodesに格納されています。これらの情報からエッジのvectorを作成するメソッドの名前をスネークケースで提案してください。


以下のコードを、要素を1つだけ選んでそのインデックスを返すように修正してください。

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
