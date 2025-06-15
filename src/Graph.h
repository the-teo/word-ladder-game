#pragma once
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <algorithm>

/**
 * @class Graph
 * @brief A generic undirected graph implementation using an adjacency list.
 * @defgroup Graph Graph Structures
 *
 * Supports adding nodes and edges, checking membership, retrieving neighbors,
 * and finding the shortest path using Breadth-First Search (BFS).
 *
 * @tparam T The type of the elements stored in the graph (e.g., std::string).
 */

template <typename T>
class Graph {
private:
    /**
     * @brief Internal representation of the graph using an adjacency list.
     */
    std::map<T, std::set<T>> adjacencyList;

public:
    /**
     * @brief Adds a node to the graph.
     *
     * If the node already exists, this operation has no effect.
     *
     * @param node The node to be added.
     */
    void addNode(const T& node) {
        if (adjacencyList.find(node) == adjacencyList.end()) {
            adjacencyList[node] = std::set<T>();
        }
    }

    /**
     * @brief Adds an undirected edge between two nodes.
     *
     * If either node doesn't exist, it is added automatically.
     *
     * @param node1 One endpoint of the edge.
     * @param node2 The other endpoint of the edge.
     */
    void addEdge(const T& node1, const T& node2) {
        addNode(node1);
        addNode(node2);
        adjacencyList[node1].insert(node2);
        adjacencyList[node2].insert(node1);
    }

    /**
     * @brief Checks if the graph contains a specific node.
     *
     * @param node The node to check.
     * @return True if the node exists, false otherwise.
     */
    bool contains(const T& node) const {
        return adjacencyList.find(node) != adjacencyList.end();
    }

    /**
     * @brief Retrieves the neighbors of a given node.
     *
     * @param node The node whose neighbors are to be retrieved.
     * @return A const reference to the set of neighbors.
     *         Returns an empty set if the node is not found.
     */
    const std::set<T>& getNeighbors(const T& node) const {
        static std::set<T> emptySet;
        auto it = adjacencyList.find(node);
        if (it != adjacencyList.end()) {
            return it->second;
        }
        return emptySet;
    }

    /**
     * @brief Finds the shortest path between two nodes using BFS.
     *
     * @param start The starting node.
     * @param end The target node.
     * @return A vector representing the shortest path from start to end.
     *         Returns an empty vector if no path exists.
     */
    std::vector<T> shortestPath(const T& start, const T& end) const {
        std::map<T, T> parent;
        std::queue<T> q;
        std::set<T> visited;

        q.push(start);
        visited.insert(start);
        parent[start] = start;

        while (!q.empty()) {
            T current = q.front();
            q.pop();

            if (current == end) {
                std::vector<T> path;
                for (T node = end; node != start; node = parent[node]) {
                    path.push_back(node);
                }
                path.push_back(start);
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (const T& neighbor : getNeighbors(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        return {}; ///< No path found
    }
};
