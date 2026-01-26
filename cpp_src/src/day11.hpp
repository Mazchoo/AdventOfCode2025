// Day11
#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

namespace day11
{
    static const int MAX_ITERATIONS = 1000000;

    struct Edge {
        size_t from_index;
        size_t to_index;
    };

    struct Graph {
        std::vector<std::string> nodes;
        size_t start_node_index;
        size_t end_node_index;
        std::vector<Edge> edges;
    };

    // Parse a directed graph from the input format:
    // node_name: neighbor1 neighbor2 neighbor3
    // Returns a Graph structure with nodes, start_node_index, end_node_index, and directed edges
    Graph parse_graph(std::string_view payload) {
        Graph graph;
        std::unordered_map<std::string, size_t> node_to_index;

        // Helper function to get or create node index
        auto get_or_create_node = [&](const std::string& node_name) -> size_t {
            auto it = node_to_index.find(node_name);
            if (it != node_to_index.end()) {
                return it->second;
            }
            size_t index = graph.nodes.size();
            graph.nodes.push_back(node_name);
            node_to_index[node_name] = index;
            return index;
        };

        size_t line_start = 0;
        for (size_t i = 0; i <= payload.length(); i++) {
            if (i == payload.length() || payload[i] == '\n') {
                if (i > line_start) {
                    std::string_view line = payload.substr(line_start, i - line_start);

                    // Skip empty lines and whitespace
                    bool has_content = false;
                    for (char c : line) {
                        if (c != ' ' && c != '\t' && c != '\r') {
                            has_content = true;
                            break;
                        }
                    }

                    if (!has_content) {
                        line_start = i + 1;
                        continue;
                    }

                    // Find the colon separator
                    size_t colon_pos = line.find(':');
                    if (colon_pos == std::string_view::npos) {
                        line_start = i + 1;
                        continue;
                    }

                    // Extract source node name (trim whitespace)
                    std::string_view source_view = line.substr(0, colon_pos);
                    while (!source_view.empty() && (source_view.back() == ' ' || source_view.back() == '\t')) {
                        source_view.remove_suffix(1);
                    }
                    while (!source_view.empty() && (source_view.front() == ' ' || source_view.front() == '\t')) {
                        source_view.remove_prefix(1);
                    }

                    std::string source_node(source_view);
                    size_t source_index = get_or_create_node(source_node);

                    // Parse destination nodes
                    size_t pos = colon_pos + 1;
                    while (pos < line.length()) {
                        // Skip whitespace
                        while (pos < line.length() && (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\r')) {
                            pos++;
                        }

                        if (pos >= line.length()) break;

                        // Extract node name
                        size_t word_start = pos;
                        while (pos < line.length() && line[pos] != ' ' && line[pos] != '\t' && line[pos] != '\r') {
                            pos++;
                        }

                        if (pos > word_start) {
                            std::string dest_node(line.substr(word_start, pos - word_start));
                            size_t dest_index = get_or_create_node(dest_node);

                            // Add directed edge
                            graph.edges.push_back({source_index, dest_index});
                        }
                    }
                }

                line_start = i + 1;
            }
        }

        // Determine start and end nodes
        // Start node is typically the first node in the input or a special name like "aaa" or "you"
        // End node is typically "out" or similar
        graph.start_node_index = 0;
        graph.end_node_index = 0;

        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (graph.nodes[i] == "aaa" || graph.nodes[i] == "you") {
                graph.start_node_index = i;
            }
            if (graph.nodes[i] == "out" || graph.nodes[i] == "end") {
                graph.end_node_index = i;
            }
        }

        return graph;
    }

    // Get direct descendants of a node (immediate children - one edge away)
    std::vector<size_t> get_direct_descendants(const Graph& graph, size_t node_index) {
        std::vector<size_t> descendants;
        for (const auto& edge : graph.edges) {
            if (edge.from_index == node_index) {
                descendants.push_back(edge.to_index);
            }
        }
        return descendants;
    }

    // Get direct ancestors of a node (immediate parents - one edge away)
    std::vector<size_t> get_direct_ancestors(const Graph& graph, size_t node_index) {
        std::vector<size_t> ancestors;
        for (const auto& edge : graph.edges) {
            if (edge.to_index == node_index) {
                ancestors.push_back(edge.from_index);
            }
        }
        return ancestors;
    }

    // Get all descendants of a node (nodes reachable by following edges forward)
    std::vector<size_t> get_all_descendants(const Graph& graph, size_t node_index) {
        std::vector<size_t> descendants;
        std::vector<bool> visited(graph.nodes.size(), false);
        std::vector<size_t> queue;

        // Start with direct children
        for (const auto& edge : graph.edges) {
            if (edge.from_index == node_index && !visited[edge.to_index]) {
                visited[edge.to_index] = true;
                queue.push_back(edge.to_index);
            }
        }

        // BFS to find all descendants
        size_t queue_pos = 0;
        while (queue_pos < queue.size()) {
            size_t current = queue[queue_pos++];
            descendants.push_back(current);

            for (const auto& edge : graph.edges) {
                if (edge.from_index == current && !visited[edge.to_index]) {
                    visited[edge.to_index] = true;
                    queue.push_back(edge.to_index);
                }
            }
        }

        return descendants;
    }

    uint32_t get_number_of_paths(std::string_view payload) {
        Graph graph = parse_graph(payload);
        std::vector<uint32_t> number_paths(graph.nodes.size(), 0);
        number_paths[graph.start_node_index] = 1;

        std::vector<bool> green(graph.nodes.size(), false);

        std::vector<size_t> candidates = {};
        std::vector<size_t> new_candidates = {graph.start_node_index};
        std::vector<size_t> working_candidates = {};

        auto descendents = get_all_descendants(graph, graph.start_node_index);

        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (std::find(descendents.begin(), descendents.end(), i) == descendents.end() && i != graph.start_node_index) {
                green[i] = true;
            }
        }

        int i = 0;
        while (i++ < MAX_ITERATIONS && !green[graph.end_node_index]) {

            for (auto& candidate: new_candidates) {
                auto decendents = get_direct_descendants(graph, candidate);
                for (auto& decendent: decendents) {

                    if (std::find(candidates.begin(), candidates.end(), decendent) == candidates.end() &&
                        std::find(new_candidates.begin(), new_candidates.end(), decendent) == new_candidates.end() &&
                        std::find(working_candidates.begin(), working_candidates.end(), decendent) == working_candidates.end()) {
                            working_candidates.push_back(decendent);
                    }
                }
            }

            candidates.insert(candidates.end(),
                std::make_move_iterator(new_candidates.begin()),
                std::make_move_iterator(new_candidates.end()));

            new_candidates.clear();

            new_candidates.insert(new_candidates.end(),
                std::make_move_iterator(working_candidates.begin()),
                std::make_move_iterator(working_candidates.end()));

            working_candidates.clear();

            for (auto& candidate: candidates) {
                auto ancestors = get_direct_ancestors(graph, candidate);
                bool all_green = true;
                for (auto& ancestor: ancestors) {
                    if (!green[ancestor]) {
                        all_green = false;
                        break;
                    }
                }

                if (all_green) {
                    green[candidate] = true;
                    working_candidates.push_back(candidate);

                    auto decendents = get_direct_descendants(graph, candidate);
                    for (auto& decendent: decendents)
                        number_paths[decendent] += number_paths[candidate];
                }
            }

            candidates.erase(
                std::remove_if(candidates.begin(), candidates.end(),
                    [&](int x) {
                        return std::find(working_candidates.begin(), working_candidates.end(), x) != working_candidates.end();
                    }),
                candidates.end()
            );

            working_candidates.clear();
        }

        return number_paths[graph.end_node_index];
    }
}
