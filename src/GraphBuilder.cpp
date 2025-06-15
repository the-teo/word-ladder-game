#include "GraphBuilder.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>


 void GraphBuilder::buildPatternMap(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        for (size_t i = 0; i < word.length(); ++i) {
            std::string pattern = word;
            pattern[i] = '*';
            patternMap[pattern].insert(word);
        }
    }
}

 std::vector<std::string> GraphBuilder::loadDictionary(const std::string& filename, size_t wordLength) {
    std::vector<std::string> words;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open dictionary file");
    }

    std::string word;
    while (file >> word) {
        if (wordLength == 0 || word.length() == wordLength) {
            // Convert to uppercase for consistency
            std::transform(word.begin(), word.end(), word.begin(), ::toupper);
            words.push_back(word);
        }
    }
    return words;
}

 Graph<std::string> GraphBuilder::buildGraph(const std::vector<std::string>& words) {
    buildPatternMap(words);
    Graph<std::string> graph;

    for (const auto& pair : patternMap) {
        const auto& wordSet = pair.second;
        for (auto it1 = wordSet.begin(); it1 != wordSet.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != wordSet.end(); ++it2) {
                graph.addEdge(*it1, *it2);
            }
        }
    }

    return graph;
}
