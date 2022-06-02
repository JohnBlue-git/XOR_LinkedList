
/*
Auther: John Blue
Time: 2022/6
Platform: ATOM with MinGw and g++compiler
Object: Template XOR_Doubly_LinkedList
Disclaimer: The std::string LinkedList haven't been consider throughly in this scope
*/

#include <iostream>// for basic i/o
#include <iomanip>// for fixed and setprecision
#include <stdexcept>// for exception
#include <string>// for string
#include <typeinfo>// for typeid
#include <stdint.h>// for uintptr_t
//#include <fstream>// for file

////// ListNode.h

// set LinkedList as friend of ListNode
template<class T>
class LinkedList;

template<class T>
class ListNode {
private:// content
    T data;
    ListNode* link;

private:// prevent cilent from using these
    // constructor
    ListNode(const ListNode& copy) {}

    // operator
    ListNode& operator=(const ListNode& copy) { return *this; }

public:
	// constructor
	ListNode(): data(0), link(0) {}
	ListNode(T a): data(a), link(0) {}
	~ListNode() {
    data = 0;
    link = 0;
    // or you can use another method
    //https://stackoverflow.com/questions/34170164/destructor-for-binary-search-tree
    //data = 0;
    //delete link;
    //caution the chain reaction when a node is deleted
    //for example: pop current->... then current->link = 0 then delete current (to prevent further delete)
    //and it will simplified deconstructor
    /*
    ~LinkedList() {
      delete first;
      end = 0;
      size = 0;
    }
    */
  }

  // !!! must be public for derived LinkedList
  // function
  T get_data() const { return data; }
  ListNode* get_link() const { return link; }

	// friend class
	friend class LinkedList<T>;
	template<class U> friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& print);
};

////// LinkedList.h

template<class T>
class LinkedList {

protected:
	// size
	int size;

	// head / end pointer
	ListNode<T>* first;
	ListNode<T>* end;

public:
	// constructor
	LinkedList(): first(0), end(0), size(0) {};
	LinkedList(const LinkedList& copy);
	virtual ~LinkedList();

	// operator
	LinkedList& operator=(const LinkedList& copy);
	T& operator[](const unsigned int x);
	//T& operator[](const unsigned int x) const;// the same even with const
	//T operator[](const unsigned int x) const;// not mutable
	template<class U> friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& print);
	//friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& print)// not work
	//friend std::ostream& operator<< <> (std::ostream& os, const LinkedList<T>& print)// not work

	// function
	bool isEmpty() const;// check empty
  ListNode<T>* root() const;// root of list
	int length() const;// size of list
  void Reverse_PrintList() const;// print list
	void PrintList() const;// print list
	void Clear();// clear list
	void Push_back(T x);// add entity
	void Push_front(T x);// add entity
  T Pop_front();// pop entity
  // inline
  //static inline ListNode<T>* XOR(uintptr_t a, uintptr_t b) { return a ^ b; }
  static inline ListNode<T>* XOR(uintptr_t a, uintptr_t b) { return (ListNode<T>*)(a ^ b); }
  // tran
	LinkedList transverse() const;// transverse

	// exception class
	class bad_empty : public std::logic_error
	{
	public:
		explicit bad_empty(const std::string & s = " "): std::logic_error(s + ", Empty error in List object\n") {};
		virtual ~bad_empty() throw() {}
	};
	class bad_index : public std::logic_error
	{
	private:
		int index;
	public:
		explicit bad_index(int ix = 0, const std::string & s = " "): index(ix), std::logic_error(s + ", Index error in List object\n") {};
		int get_index() const { return index; }
		virtual ~bad_index() throw() {}
	};
};

////// LinkedList.cpp

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& copy): first(0), end(0) {
	// copy content
	ListNode<T>* current = copy.first;
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	while (current != 0) {
		this->Push_back(current->data);
		// iteration
    next = XOR((uintptr_t)prev, (uintptr_t)current->link);
    prev = current;
    current = next;
	}
}

template<class T>
LinkedList<T>::~LinkedList() {
	this->Clear();
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& copy) {
	// check
	if (&copy == this) {
		return *this;
	}
	// clear
	this->Clear();
	// copy content
	ListNode<T>* current = copy.first;
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	while (current) {
		this->Push_back(current->data);
		// iteration
    next = XOR((uintptr_t)prev, (uintptr_t)current->link);
    prev = current;
    current = next;
	}
	// return
	return *this;
}

template<class T>
//T& LinkedList<T>::operator[](const unsigned int x) const { // the same even with const
T& LinkedList<T>::operator[](const unsigned int x) {
	// check
	if (first == 0) {                     // id list list is empty
		throw bad_empty("[] operation");
	}
	if (size < x + 1) {                   // bad index
		throw bad_index(x, "[] operation");
	}
	// extract out the i th item
	ListNode<T>* current = first;         // point to first
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	unsigned int i = 0;
	while (i < x) {
    next = XOR((uintptr_t)prev, (uintptr_t)current->link);
    prev = current;
    current = next;
		i++;
	}
	return current->data;
}

template<class T>
bool LinkedList<T>::isEmpty() const {
	if (first == 0) {                      // if list is empty
		return true;
	}
	else {
		return false;
	}
}

template<class T>
ListNode<T>* LinkedList<T>::root() const {
  return first;
}

template<class T>
int LinkedList<T>::length() const {
	return size;
}

template<class T>
void LinkedList<T>::Reverse_PrintList() const {
	if (first == 0) {                         // if list is empty
		throw bad_empty("Reverse_PrintList()");
	}
	ListNode<T>* current = end;             // point to first
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	std::cout << "R List: ";
	while (current != 0) {                    // execute to last NULL
		if (typeid(T) == typeid(double)) {
			std::cout << std::fixed << std::setprecision(3) << current->data << " ";
		}
		else if (typeid(T) == typeid(int) || typeid(T) == typeid(float) || typeid(T) == typeid(std::string)) {
			std::cout << current->data << " ";
		}
    // iteration
    next = XOR((uintptr_t)prev, (uintptr_t)current->link);
    prev = current;
    current = next;
	}
	std::cout << std::endl;
}

template<class T>
void LinkedList<T>::PrintList() const {
	if (first == 0) {                         // if list is empty
		throw bad_empty("PrintList()");
	}
	ListNode<T>* current = first;             // point to first
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	std::cout << "List: ";
	while (current != 0) {                    // execute to last NULL
		if (typeid(T) == typeid(double)) {
			std::cout << std::fixed << std::setprecision(3) << current->data << " ";
		}
		else if (typeid(T) == typeid(int) || typeid(T) == typeid(float) || typeid(T) == typeid(std::string)) {
			std::cout << current->data << " ";
		}
    // iteration
    next = XOR((uintptr_t)prev, (uintptr_t)current->link);
    prev = current;
    current = next;
	}
	std::cout << std::endl;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& print) {
	// check
	if (print.first == 0) {                         // if list is empty
		// not yet know what to implment exception for operator
		//return os << print.bad_empty("operator<<");
    os << "operator<<, Empty error in List object\n";
    return os;
	}
	// print out
	ListNode<T>* current = print.first;             // point to first
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	os << "List: ";
	while (current != 0) {                          // execute to last NULL
		if (typeid(T) == typeid(double)) {
			os << std::fixed << std::setprecision(3) << current->data << " ";
		}
		else if (typeid(T) == typeid(int) || typeid(T) == typeid(float) || typeid(T) == typeid(std::string)) {
			os << current->data << " ";
		}
    // iteration
    next = (ListNode<T>*)((uintptr_t)prev ^ (uintptr_t)current->link);
    prev = current;
    current = next;
	}
	// return
	return os;
}

template<class T>
void LinkedList<T>::Clear() {
	ListNode<T>* current;
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	while (first != 0) {
		current = first;
    next = XOR((uintptr_t)prev, (uintptr_t)first->link);
    prev = first;
    first = next;
		// !!! delete and prevent bug
    current->link = 0;
		delete current;
		current = 0;
	}
	// !!! remembr to 0 pointer
	end = 0;
	// !!! reset size
	size = 0;
}

template<class T>
void LinkedList<T>::Push_back(T x) {
	// add new node
	ListNode<T>* newNode = new ListNode<T>(x);
  //newNode->link = 0;
	// connection
	if (first == 0 && end == 0) {              // indicate list is empty now
		first = newNode;                         // fist and end are all point to new node
		end = newNode;
	}
	else {                                     // link new node to end pointer
		newNode->link = end;                // default link for new node
    end->link = XOR((uintptr_t)end->link, (uintptr_t)newNode);// update previous link
		end = newNode;
	}
	// increase size number
	size++;
}

template<class T>
void LinkedList<T>::Push_front(T x) {
	// add new node
	ListNode<T>* newNode = new ListNode<T>(x);
	// connection
	if (first == 0 && end == 0) {              // indicate list is empty now
		first = newNode;                         // fist and end are all point to new node
		end = newNode;
	}
	else {                                     // link new node to end pointer
    newNode->link = first;                // default link for new node
    first->link = XOR((uintptr_t)first->link, (uintptr_t)newNode);// update previous link
		first = newNode;
	}
	// increase size number
	size++;
}

template<class T>
T LinkedList<T>::Pop_front() {
  // check
  if (this->first == 0) {                         // if list is empty
    throw bad_empty("Pop_front()");
  }
  // pop
  ListNode<T>* current = first;
  //rt = current->data; it is protected data from Node (not parent class)
  T rt = current->data;
  first = first->link;
  if (first) {
    first->link = XOR((uintptr_t)current, (uintptr_t)first->link);
  }
  current->link = 0;
  delete current;
  // decrese size
  size--;
  // return
  return rt;
}

template<class T>
LinkedList<T> LinkedList<T>::transverse() const {
	// list for retrun
	LinkedList<T> RT;
	// copy content
	ListNode<T>* current = first;
  ListNode<T>* prev = 0;
  ListNode<T>* next;
	while (current != 0) {
		RT.Push_front(current->data);
		// iteration
    next = XOR((uintptr_t)prev, (uintptr_t)current->link);
    prev = current;
    current = next;
	}
	// return
	return RT;
}
