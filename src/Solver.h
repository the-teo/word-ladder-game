#pragma once
#include "Graph.h"
#include <string>
#include <vector>
#include <algorithm>

/**
 * @class Solver
 * @brief Provides logic for solving Word Ladder transformations using a graph.
 * @ingroup Core Core Logic
 * 
 * The Solver class uses a pre-built word graph to find shortest paths between words
 * and to generate hints for the player.
 */

class Solver {
private:
    /**
     * @brief The word graph used for transformations.
     *
     * Each node is a word, and edges connect words that differ by one letter.
     */
    Graph<std::string> wordGraph;

public:
    /**
     * @brief Constructs a Solver with the given word graph.
     *
     * @param graph A Graph object where each node is a valid dictionary word.
     */
    Solver(const Graph<std::string>& graph);

    /**
     * @brief Finds the shortest transformation path between two words.
     *
     * Uses Breadth-First Search (BFS) internally to compute the path.
     *
     * @param start The word to start from.
     * @param end The target word to reach.
     * @return A vector of words representing the shortest path.
     *         Returns an empty vector if no path exists.
     */
    std::vector<std::string> findShortestPath(const std::string& start,
        const std::string& end) const;

    /**
     * @brief Provides a hint by returning the next best word in the path from current to target.
     *
     * @param current The current word.
     * @param target The target word.
     * @return A pair containing:
     *         - the next word in the optimal path (or empty string if no path exists),
     *         - the total number of moves in the optimal path.
     */
    std::pair<std::string, int> getHint(const std::string& current, const std::string& target) const;
};
