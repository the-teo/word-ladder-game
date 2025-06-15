#pragma once

#include "Graph.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <stdexcept>

/**
 * @class GraphBuilder
 * @brief Responsible for building a word graph used in the Word Ladder problem.
 * @ingroup Graph Graph Structures
 *
 * This class constructs a graph where nodes are words and edges connect words that differ by one character.
 * It uses intermediate pattern mapping to efficiently determine valid connections.
 */

class GraphBuilder {
private:
    /**
     * @brief A map from word patterns to sets of words matching that pattern.
     *
     * For example, the pattern "c*t" could map to {"cat", "cot", "cut"}.
     */
    std::map<std::string, std::set<std::string>> patternMap;

    /**
     * @brief Constructs the pattern map from the given list of words.
     *
     * Each word is transformed into several patterns by replacing each letter with '*', and
     * those patterns are stored as keys in a map pointing to sets of words that match.
     *
     * @param words List of words to process.
     */
    void buildPatternMap(const std::vector<std::string>& words);

public:
    /**
     * @brief Loads a dictionary from a file.
     *
     * Reads a list of words from the specified file, filtering by word length if provided.
     *
     * @param filename Path to the dictionary file.
     * @param wordLength Optional word length to filter the words. If 0, no filtering is applied.
     * @return A vector of words from the file matching the criteria.
     * @throws std::runtime_error If the file cannot be opened.
     */
    static std::vector<std::string> loadDictionary(const std::string& filename, size_t wordLength = 0);

    /**
     * @brief Builds a word graph from the given list of words.
     *
     * Nodes are words, and an edge connects two words if they differ by exactly one character.
     *
     * @param words List of words to include as nodes in the graph.
     * @return A Graph object with connections based on one-letter differences.
     */
    Graph<std::string> buildGraph(const std::vector<std::string>& words);
};
