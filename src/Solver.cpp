#include "Solver.h"
#include "Graph.h"
#include <utility>

 Solver::Solver(const Graph<std::string>& graph) : wordGraph(graph) {}

 std::vector<std::string> Solver::findShortestPath(const std::string& start, const std::string& end) const {
    std::string startUpper = start;
    std::string endUpper = end;
    std::transform(startUpper.begin(), startUpper.end(), startUpper.begin(), ::toupper);
    std::transform(endUpper.begin(), endUpper.end(), endUpper.begin(), ::toupper);

    if (!wordGraph.contains(startUpper) || !wordGraph.contains(endUpper)) {
        return {};
    }

    return wordGraph.shortestPath(startUpper, endUpper);
}

 std::pair<std::string, int> Solver::getHint(const std::string& current, const std::string& target) const {
    auto path = findShortestPath(current, target);
    if (path.size() < 2) return { "", -1 };

    const std::string& nextWord = path[1];
    for (size_t i = 0; i < current.size(); ++i) {
        if (current[i] != nextWord[i]) {
            return { nextWord, static_cast<int>(i) };
        }
    }
    return { "", -1 };
}
