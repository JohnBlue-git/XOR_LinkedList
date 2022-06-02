
/*
Auther: John Blue
Time: 2022/6
Platform: ATOM with MinGw and g++compiler
Object: Testing template XOR_Doubly_LinkedList
*/

#include <iostream>// for basic i/o
#include <exception>// for exception
#include "XLinkedList.h"
using namespace std;

int main() {
  LinkedList<int> test;
  cout << "Push" << endl;
  for(int i = 0; i < 6; i++) {
    test.Push_back(i);
  }
  cout << "is empty? 1 true 0 false: " << test.isEmpty() << endl;
  cout << "Print" << endl;
  test.PrintList();
  cout << test << endl;
  test.Reverse_PrintList();
  cout << "Size" << endl;
  cout << test.length() << endl;
  cout << endl;

  cout << "Copy" << endl;
  LinkedList<int> copy(test);
  cout << "Print" << endl;
  copy.PrintList();
  cout << endl;

  cout << "Assign" << endl;
  LinkedList<int> assign = test;
  cout << "Print" << endl;
  assign.PrintList();
  cout << endl;

  cout << "Transeverse" << endl;
  LinkedList<int> transe(test.transverse());
  cout << "Print" << endl;
  transe.PrintList();
  cout << endl;

  //cout << "Clear" << endl;
  //test.Clear();
  cout << "Pop" << endl;
  for(int i = 0; i < 6; i++) {
    cout << test.Pop_front() << endl;
  }
  cout << "is empty? 1 true 0 false: " << test.isEmpty() << endl;
  cout << "Print" << endl;
  try {
    test.PrintList();
  }
  catch(exception& e) {
    cout << e.what() << endl;
  }
  cout << endl;
}
