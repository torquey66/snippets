#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <iostream>
#include <memory>
#include <stdexcept>

namespace circular_list {

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

inline
std::ostream & operator<<(std::ostream & os, Node::Ptr list) {
  if (!list) {
    std::cout << "[]";
    return os;
  }

  Node::Ptr cursor(list);
  while (cursor) {
    std::cout << cursor->value();
    cursor = cursor->next();
    if (cursor == list) break;
    std::cout << " ";
  }
  return os;
}

inline
Node::Ptr insert(Node::Ptr list, int value) {
  if (!list) {
    Node::Ptr result(make_shared<Node>(value));
    result->next = result;
    return result;
  }
  return std::make_shared<Node>(value, list);
}

} // namespace circular_list

#endif
