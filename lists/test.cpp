#include "list.h"

#include <iostream>
#include <vector>

using namespace std;

void test_list() {
  using namespace list;
  vector<int> const samples({ 1, 2, 3, 4, 5 });

  Node::Ptr list;

  cout << "empty: " << list << endl;
  cout << "reversed empty: " << reverse(list) << endl;

  list = insert(list, 0);
  cout << "single: " << list << endl;
  cout << "reversed single: " << reverse(list) << endl;

  for (auto sample : samples) {
    list = insert(list, sample);
  }

  cout << "full: " << list << endl;

  cout << "find 0: " << find(list, 0) << endl;
  cout << "find 3: " << find(list, 3) << endl;
  cout << "find 5: " << find(list, 5) << endl;
  cout << "find 7: " << find(list, 7) << endl;

  cout << "reversed full: " << reverse(list) << endl;

  list = Node::Ptr();
  vector<int> const scrambled({0, 3, 1, 5, 2, 0, 0, 6});
  for (auto sample : scrambled) {
    list = insert_sorted(list, sample);
  }
  cout << "sorted: " << list << endl;
  list = insert_sorted(list, 7);
  cout << "sorted: " << list << endl;

  list = remove(list, 0);
  cout << "remove first: " << list << endl;
  list = remove(list, 7);
  cout << "remove last: " << list << endl;
  list = remove(list, 1);
  list = remove(list, 2);
  list = remove(list, 3);
  list = remove(list, 5);
  list = remove(list, 6);
  cout << "remove rest: " << list << endl;
}

int main() {
  test_list();
}
