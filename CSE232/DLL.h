/**************************************
			Project 4 
A project that implements the method
functions for a doubly linked list.
Pointers and dynamic memory allocation
used extensively.
***************************************/
#pragma once

#include<iostream>
using std::ostream; using std::cout;
#include<string>
using std::string;
#include <vector>
using std::vector;
#include <sstream>
using std::ostringstream;



// DO NOT MODIFY
//
// Node
//
struct Node {
	string key;
	int value = 0;
	Node* next = nullptr;
	Node* prev = nullptr;
	Node() = default;
	Node(string, int);
	bool operator==(const Node&) const;
	friend ostream& operator<<(ostream& out, const Node& n) {
		out << n.key << ":" << n.value;
		return out;
	}
};

Node::Node(string key_, int value_) {
	key = key_;
	value = value_;
}

bool Node::operator==(const Node& n) const {
	if ((key == n.key) && (value == n.value)) {
		return true;
	}
	else {
		return false;
	}
}

// DO NOT MODIFY
//
// DLL
//
class DLL {
private:
	Node* head_ = nullptr;
	Node* tail_ = nullptr;
	size_t sz_ = 0;
public:
	DLL() = default;
	DLL(const DLL&);
	DLL(vector<Node>);
	Node* get_head();
	Node* get_tail();
	void shead(Node* n) { head_ = n; }
	void stail(Node* n) { tail_ = n; }
	size_t get_size();
	Node add_back(string, int);
	Node add_front(string, int);
	Node add_after(string, string, int);
	Node search(string);
	Node* Search(string find_key);
	Node remove_front();
	Node remove_back();
	Node remove_key(string);
	int update_value(string, int);
	
	friend ostream& operator<<(ostream& out, const DLL& list) {
		auto itr = list.head_;
		ostringstream oss;
		for (; itr != nullptr; itr = itr->next) {
			oss << itr->key << ":" << itr->value << ", ";
		}
		string ans = oss.str();
		ans = ans.substr(0, ans.size() - 2);
		out << ans;
		return out;
	}
};

Node* DLL::get_head() {
	return head_;
}
Node* DLL::get_tail() {
	return tail_;
}
size_t DLL::get_size() {
	return sz_;
}

DLL::DLL(const DLL& list) {
	head_ = list.head_;
	tail_ = list.tail_;
	sz_ = list.sz_;
}

DLL::DLL(vector<Node> vec) {
	for (const auto& node : vec) {
		add_back(node.key, node.value);
	}
}

/* BEGIN MODIFICATION BELOW */

// adding a node to the end of the list, returning new node

Node DLL::add_back(string str, int num)
{	/**************************/
	struct Node* New = new Node;  // must delete eventually
	/**************************/
	
	New->key = str;
	New->value = num;
	New->next = nullptr;  // setting values for members
	if (sz_)
	{
		get_tail()->next = New;
		New->prev = get_tail();
	}
	else
	{	New->prev = nullptr;
		shead(New);    
		// personally implemented setters, Max said this OK on Piazza
	}

	sz_ = sz_ + 1; // updating size
	stail(New);  // setting tail
	return *New;
}

// adding node to front with return of new node
Node DLL::add_front(string str, int num)
{	// commentary limited due to being mirror image of above
	/**************************/
	struct Node* New = new Node;  // must delete eventually
	/**************************/
	
	New->key = str;
	New->value = num;
	New->prev = nullptr;
	if (sz_)
	{	// note head instead of tail
		get_head()->prev = New;
		New->next = get_head();
	}
	else
	{
		New->next = nullptr;
		stail(New);
	}

	sz_ = sz_ + 1;
	shead(New);
	return *New;
}


/* adding node after a particular node
   and then returning it */
Node DLL::add_after(string add_key, string str, int num)
{
	struct Node* add = Search(add_key);
	if (add)  // if found
	{
		/**************************/
		struct Node* New = new Node;  // must delete eventually
		/**************************/
		
		New->key = str; 
		New->value = num;
		New->next = add->next;  // creating appropriate values
		if (add->next) { (add->next)->prev = New; }
		else { stail(New);}
		// case of being tail
		New->prev = add;
		add->next = New;
		sz_ = sz_ + 1; // updating size
		return *New;
	}
	else { return Node("", 0); } //default retrun
}

// removing a node from the rear end of node a returning its values
Node DLL::remove_back()
{
	struct Node ret;
	if (sz_)
	{
		ret.key = get_tail()->key;  // creating return Node 
		ret.value = get_tail()->value;
		ret.prev = get_tail()->prev;
		struct Node* temp = get_tail()->prev; 
		try { delete get_tail(); }   // deleting previously allocated memory
		catch (...) { cout << "Node deletion failed."; }  // if the memory is not from heap
		sz_ = sz_ - 1;
		if (sz_)
		{
			stail(temp); // resetting connections
			tail_->next = nullptr;
		}   // case for last node below
		else { shead(nullptr); stail(nullptr); }

		return ret;
	}
	return ret;
}

// removing the front node and returning Node with its values

Node DLL::remove_front()
{	/* since this is virtually identical to above
	   commentary is limited  */
	struct Node ret;
	if (sz_)
	{
		ret.key = get_head()->key;   // note head instead of tail
		ret.value = get_head()->value;
		ret.next = get_head()->next;
		struct Node* temp = get_head()->next;
		try { delete get_head(); }
		catch (...) { cout << "Node deletion failed."; }  // if the memory is not from heap
		sz_ = sz_ - 1;
		if (sz_)
		{
			shead(temp);
			head_->prev = nullptr;
		}
		else { shead(nullptr); stail(nullptr); }

		return ret;
	}

	return ret;
}

/* removing node based on its key
   and returning its key / value */
Node DLL::remove_key(string to_remove)
{
	struct Node* rem = Search(to_remove);  // using pointer search to find appropriate node
	if (rem)
	{	// for general case 
		if (rem->prev && rem->next) { (rem->prev)->next = rem->next; (rem->next)->prev = rem->prev; }
		// for head 
		else if (rem->next) { (rem->next)->prev = nullptr; shead(rem->next); }
		// for tail
		else if (rem->prev) { (rem->prev)->next = nullptr; stail(rem->prev); }
		// return Node
		struct Node ret = Node(rem->key, rem->value);
		try { delete rem; }   // deleting dynamic memory
		catch (...) { cout << "Node deletion failed."; }  // if the memory is not from heap
		sz_ = sz_ - 1; // size adjustment
		return ret;
	}

	else { return Node("", 0); } // default return
}

/*This function searched for a particular key and returns the Node 
  which contains the key */

Node DLL::search(string find_key)
{
	if (sz_)
	{
		if (get_head()->key == find_key) {return *get_head();}  // case where head is return
		struct Node* npr = get_head(); 
		while (npr->next)    // looping through the list until null
		{
			if (npr->next->key == find_key) {return *(npr->next);} // if succesful return
			else { npr = npr->next; }
		}
		return Node("", 0);
	}     // default returns
	return Node("", 0);
}

/* This Search is functionally equivalent to the one above 
   instead  it returns a pointer and 0 for failure (null)*/

Node* DLL::Search(string find_key)
{
	if (sz_)
	{	if (get_head()->key == find_key){return get_head();}
		
		struct Node* npr = get_head();

		while (npr->next)
		{
			if (npr->next->key == find_key) {return (npr->next);}
			else { npr = npr->next; }
		}
		return 0;
	}
	return 0;
}

// member to change value of key, return it and returning -1 for failure
int DLL::update_value(string find_key, int new_value)
{
	struct Node* npr = Search(find_key);  // finding address
	if (npr) { (npr->value) = new_value; return new_value; }  // sucess and change
	return -1;   // failure return
}
