#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node {
	Node* next;
	Node* prev;
	int value;
	int key;
	Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
	Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
	map<int, Node*> mp; //map the key to the node in the linked list
	int cp;  //capacity
	Node* tail; // double linked list tail pointer
	Node* head; // double linked list head pointer
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function
};

class LRUCache : protected Cache {
	void updateExistingNode(int key, int value) {
		if (head == nullptr) throw exception("Head not set up when it should.");
		if (tail == nullptr) throw exception("Tail not set up when it should.");

		Node* containingNode = mp[key];
		if (containingNode != head) {
			if (containingNode->prev == nullptr) throw exception("The previous node for the existing node is null when it's not the head of the linked list.");

			if (containingNode != tail) {
				if (containingNode->next == nullptr) throw exception("The next node for the existing node is null when it's not the tail of the linked list.");
				containingNode->next->prev = containingNode->prev;
			}
			else {
				tail = containingNode->prev;
			}

			containingNode->prev->next = containingNode->next;
			containingNode->next = head;
			containingNode->prev = nullptr;

			head->prev = containingNode;
			head = containingNode;
		}

		containingNode->value = value;
	}

	void initializeMap(int key, int value) {
		Node* newNode = new Node(key, value);

		mp[key] = newNode;

		head = newNode;
		tail = newNode;
	}

	void addNode(int key, int value) {
		Node* newNode = new Node(nullptr, head, key, value);
		if (head == nullptr) throw exception("Head not set up when it should.");
		if (tail == nullptr) throw exception("Tail not set up when it should.");

		if (mp.size() == cp) {
			mp.erase(tail->key);
			Node* temp = tail;
			tail = temp->prev;
			delete temp;
		}

		head->prev = newNode;
		mp[key] = newNode;
		head = newNode;
	}

public:
	LRUCache(int capacity) {
		tail = nullptr;
		head = nullptr;
		cp = capacity;
	}

	void set(int key, int value) {
		try
		{
			if (mp.count(key)) {
				updateExistingNode(key, value);
			}
			else {
				if (tail == nullptr && head == nullptr) { //This is the first Node on the map
					initializeMap(key, value);
				}
				else {
					addNode(key, value);
				}
			}
		}
		catch (const std::exception&)
		{
			
		}
	}

	int get(int key) {
		if (mp.count(key)) {
			return mp[key]->value;
		}
		else {
			return -1;
		}
	}

	~LRUCache() {
		
	}
};

int main() {
	int n, capacity, i;
	cin >> n >> capacity;
	LRUCache l(capacity);
	for (i = 0; i < n; i++) {
		string command;
		cin >> command;
		if (command == "get") {
			int key;
			cin >> key;
			cout << l.get(key) << endl;
		}
		else if (command == "set") {
			int key, value;
			cin >> key >> value;
			l.set(key, value);
		}
	}
	return 0;
}
