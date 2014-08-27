#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <memory>
#include <stdexcept>

class Node {
 public:
  typedef std::shared_ptr<Node> Ptr;

 Node(int value, Ptr next=Ptr(nullptr)) : _value(value), _next(next) { }

  int value() const { return _value; }

  Ptr const & next() const { return _next; }
  Ptr &       next()       { return _next; }

 private:
  int _value;
  Ptr _next;
};

std::ostream & operator<<(std::ostream & os, Node::Ptr list) {
  if (!list) {
    std::cout << "[]";
    return os;
  }

  Node::Ptr cursor(list);
  while (cursor) {
    std::cout << cursor->value();
    cursor = cursor->next();
    if (cursor) std::cout << " ";
  }
  return os;
}

Node::Ptr insert(Node::Ptr list, int value) {
  return std::make_shared<Node>(value, list);
}

Node::Ptr insert_sorted(Node::Ptr list, int value) {
  if (!list) return std::make_shared<Node>(value, list);
  if (list->value() > value) return std::make_shared<Node>(value, list);
  Node::Ptr current(list);
  while (current) {
    Node::Ptr next(current->next());
    if (!next || next->value() > value) {
      current->next() = std::make_shared<Node>(value, next);
      break;
    }
    current = current->next();
  }
  if (!current) throw std::runtime_error("WTF!?");
  return list;
}

Node::Ptr find(Node::Ptr list, int value) {
  Node::Ptr result(list);
  while (result && result->value() != value) {
    result = result->next();
  }
  return result;
}

Node::Ptr reverse(Node::Ptr list) {
  if (!list) return list;
  Node::Ptr current(list), next(current->next());
  while (next) {
    Node::Ptr tmp(next->next());
    next->next() = current;
    current = next;
    next = tmp;
  }
  list->next() = Node::Ptr();
  return current;
}

#endif
