#include "al_graph.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <vector>

using node_id_t = jcf::al_graph_t::node_id_t;

/*
    1 -> 2 |-> 3
           |-> 4 |-> 5
           |     |-> 6
           |-> 7
 */
jcf::al_graph_t make_test_tree() {
  auto result = jcf::al_graph_t();

  auto ids = std::set<node_id_t>{ { 1, 2, 3, 4, 5, 6, 7 } };
  for (const auto id: ids)
    result.insert(id, id);
  result.connect(std::make_pair(1, 2));
  result.connect(std::make_pair(2, 3));
  result.connect(std::make_pair(2, 4));
  result.connect(std::make_pair(2, 7));
  result.connect(std::make_pair(4, 5));
  result.connect(std::make_pair(4, 6));

  result.dump();

  return result;
}

void test_no_duplicate_ids() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  bool caught = false;
  try {
    auto graph = jcf::al_graph_t();
    graph.insert(42, 0);
    graph.insert(42, 1);
  } catch (const std::invalid_argument& ex) {
    caught = true;
  }
  assert(caught);
}

void test_simple_lookup() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  auto graph = jcf::al_graph_t();
  graph.insert(42, 0xdeadbeef);
  auto value = graph.get(42);
  assert(*value == 0xdeadbeef);
}

void test_simple_edge() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  auto graph = jcf::al_graph_t();
  
  auto ids = std::vector<node_id_t>();
  for (node_id_t id = 0; id < 42; ++id) {
    ids.push_back(id);
    graph.insert(id, 0xdeadbeef);
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  std::shuffle(ids.begin(), ids.end(), gen);
  for (size_t idx = 0; idx < ids.size(); ++idx) {
    const auto next_idx = (idx + 1) % ids.size();
    const auto edge = std::make_pair(ids[idx], ids[next_idx]);
    graph.connect(edge);
  }

  for (size_t idx = 0; idx < ids.size(); ++idx) {
    const auto next_idx = (idx + 1) % ids.size();
    const auto edge = std::make_pair(ids[idx], ids[next_idx]);
    assert(graph.connected(ids[idx], ids[next_idx]));
    assert(graph.connected(ids[next_idx], ids[idx]));
  }
}

void test_no_links_to_self() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  bool caught = false;
  try {
    auto graph = jcf::al_graph_t();
    graph.insert(1, 0xdeadbeef);
    graph.connect(std::make_pair(1, 1));
  }
  catch (std::invalid_argument& ex) {
    caught = true;
  }
  assert(caught);
}

void test_tree_bfs() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  auto graph = make_test_tree();
  auto os = std::ostringstream();
  graph.bfs(1, [&os](const auto id, const auto) {
    os << " " << id;
    return true;
  });
  assert(os.str() == " 1 2 3 4 7 5 6");
}

void test_circular_bfs() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  auto graph = jcf::al_graph_t();

  auto ids = std::set<node_id_t>{ { 1, 2, 3 } };
  for (const auto id: ids)
    graph.insert(id, id);
  graph.connect(std::make_pair(1, 2));
  graph.connect(std::make_pair(2, 3));
  graph.connect(std::make_pair(3, 1));

  graph.dump();

  auto os = std::ostringstream();
  graph.bfs(1, [&os](const auto id, const auto) {
    os << " " << id;
    return true;
  });
  assert(os.str() == " 1 2 3");
}

void test_tree_dfs() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  auto graph = make_test_tree();
  auto os = std::ostringstream();
  graph.dfs(1, [&os](const auto id, const auto) {
    os << " " << id;
    return true;
  });
  std::cerr << os.str() << std::endl;
  assert(os.str() == " 1 2 7 4 6 5 3");
}

void test_circular_dfs() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  auto graph = jcf::al_graph_t();

  auto ids = std::set<node_id_t>{ { 1, 2, 3, 4, 5 } };
  for (const auto id: ids)
    graph.insert(id, id);
  graph.connect(std::make_pair(1, 2));
  graph.connect(std::make_pair(2, 3));
  graph.connect(std::make_pair(2, 4));
  graph.connect(std::make_pair(4, 5));
  graph.connect(std::make_pair(5, 1));
  graph.connect(std::make_pair(3, 1));

  graph.dump();

  auto os = std::ostringstream();
  graph.dfs(1, [&os](const auto id, const auto) {
    os << " " << id;
    return true;
  });
  std::cerr << os.str() << std::endl;
  assert(os.str() == " 1 2 4 5 3");
}

int main(int argc, char* argv[]) {

  test_no_duplicate_ids();
  test_simple_lookup();
  test_simple_edge();
  test_no_links_to_self();
  test_tree_bfs();
  test_circular_bfs();
  test_tree_dfs();
  test_circular_dfs();
}
