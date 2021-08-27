#pragma once

#include <exception>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace jcf {

  /** 
      Adjacency list graph representation.

      !@# consider templating over node id and value types !@#
  */

  struct al_graph_t final {

    using node_id_t = size_t;
    using node_value_t = size_t; 

    using node_t = std::pair<node_id_t, node_value_t>;
    using edge_t = std::pair<node_id_t, node_id_t>;

    std::optional<node_value_t> get(node_id_t id) {
      const auto it = m_nodes.find(id);
      return it != m_nodes.end() ? std::optional<node_value_t>(it->second) : std::nullopt;
    }

    bool connected(node_id_t n1, node_id_t n2) {
      const auto it = m_edges.find(n1);
      return it != m_edges.end();
    }

    void insert(node_id_t id, node_value_t value) {
      const auto result = m_nodes.emplace(id, value);
      if (!result.second)
        throw std::invalid_argument("duplicate node id: " + std::to_string(id) +
                                    " value: " + std::to_string(value));
    }

    void connect(edge_t edge) {
      if (edge.first == edge.second)
        throw std::invalid_argument("no links to self id: " + std::to_string(edge.first));
      // !@# ignore duplicates for now !@#
      m_edges.insert(edge);
    }

    static node_id_t id_of(node_t node) { return node.first; }
    static node_id_t value_of(node_t node) { return node.second; }

    void dump() const {
      std::cerr << "nodes" << "-----" << std::endl;
      for (const auto& node: m_nodes)
        std::cerr << "id: " << id_of(node) << " value: " << value_of(node) << std::endl;
      std::cerr << "edges" << "-----" << std::endl;
      for (const auto& edge: m_edges)
        std::cerr << edge.first << " --> " << edge.second << std::endl;
    }

    // FUN models bool(node_id_t, node_value_t), returning true to
    // keep going or false to short_circuit
    template<typename FUN>
    void bfs(node_id_t root, FUN visit) {
      auto visited = std::unordered_set<node_id_t>();
      auto to_visit = std::queue<node_id_t>();
      to_visit.push(root);

      while (!to_visit.empty()) {
        const auto id = to_visit.front();
        to_visit.pop();

        if (visited.find(id) == visited.end()) {

          const auto target_it = m_nodes.find(id);
          assert(target_it != m_nodes.end());

          const auto subject = *target_it;

          const auto keep_going = visit(id_of(subject), value_of(subject));
          if (!keep_going)
            return;

          visited.insert(id);
          const auto children = m_edges.equal_range(id);
          for (auto it = children.first; it != children.second; ++it) {
            to_visit.push(it->second);
          }
        }
      }
    }
    
    template <typename FUN>
    void dfs(node_id_t root, FUN visit) {
      auto visited = std::unordered_set<node_id_t>();
      auto to_visit = std::stack<node_id_t>();
      to_visit.push(root);

      while (!to_visit.empty()) {
        const auto id = to_visit.top();
        to_visit.pop();

        if (visited.find(id) == visited.end()) {

          const auto target_it = m_nodes.find(id);
          assert(target_it != m_nodes.end());

          const auto subject = *target_it;

          const auto keep_going = visit(id_of(subject), value_of(subject));
          if (!keep_going)
            return;

          visited.insert(id);
          const auto children = m_edges.equal_range(id);
          for (auto it = children.first; it != children.second; ++it) {
            to_visit.push(it->second);
          }
        }
      }
    }

  private:
    std::unordered_map<node_id_t, node_value_t> m_nodes;
    std::multimap<node_id_t, node_id_t> m_edges;
  };

  
} // jcf
