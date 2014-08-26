#include "node.h"

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {

  vector<int> const samples({ 1, 2, 3, 4, 5 });

  Node::Ptr list;

  cout << "empty: " << list << endl;
  cout << "reversed empty: " << reverse(list) << endl;

  list = Node::Ptr(make_shared<Node>(0, list));
  cout << "single: " << list << endl;
  cout << "reversed single: " << reverse(list) << endl;

  for (auto value : samples) {
    list = Node::Ptr(make_shared<Node>(value, list));
  }

  cout << "full: " << list << endl;
  cout << "reversed full: " << reverse(list) << endl;

}
