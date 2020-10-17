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
	Node(int k, int val) :prev(nullptr), next(nullptr), key(k), value(val) {};
};

class Cache {

protected:
	map<int, unique_ptr<Node>> mp; //map the key to the node in the linked list
	Node* tail = nullptr; // double linked list tail pointer
	Node* head = nullptr; // double linked list head pointer
	int cp = 0;  //capacity
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function
};

class LRUCache : protected Cache {
	void updateExistingNode(int key, int value) {
		if (head == nullptr) throw exception("Head not set up when it should.");
		if (tail == nullptr) throw exception("Tail not set up when it should.");

		Node* containingNode = mp[key].get();
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
		auto newNode = make_unique<Node>(Node(key, value));

		mp.insert(make_pair(key, move(newNode)));

		head = newNode.get();
		tail = newNode.get();
	}

	void addNode(int key, int value) {
		auto newNode = make_unique<Node>(Node(nullptr, head, key, value));
		if (head == nullptr) throw exception("Head not set up when it should.");
		if (tail == nullptr) throw exception("Tail not set up when it should.");

		if (mp.size() == cp) {
			mp.erase(tail->key);
			auto temp = tail;
			tail = temp->prev;
		}

		head->prev = newNode.get();
		mp.insert(make_pair(key, move(newNode)));
		head = newNode.get();
	}

public:
	LRUCache(int capacity) {
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
		mp.clear();
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
