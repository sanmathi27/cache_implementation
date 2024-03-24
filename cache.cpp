#include <iostream>
#include <unordered_map>

using namespace std;

//struct for nodes in doubly linked list
struct Node {
    int data;
    Node* prev;
    Node* next;
    
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<int, Node*> cache; //hashtable , key - int, value - pointer
    Node* head; //least recently used node
    Node* tail; //most recently used node
    int capacity; //cap of cache
    int size; //current size of cache

public:
    //constructor to initialize lru cache
    LRUCache(int cap) : capacity(cap), size(0), head(nullptr), tail(nullptr) {}

    //remove a node from the linked list
    void removeNode(Node* node) {
        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;

        delete node;
        size--;
    }

    // add node to the end of linked list
    void addToEnd(Node* node) {
        if (!head)
            head = node;
        else
            tail->next = node;
        node->prev = tail;
        tail = node;
        size++;
    }

    // access node of given key
    bool access(int key) {
        if (cache.find(key) != cache.end()) { 
            Node* node = cache[key];
            removeNode(node);
            addToEnd(node);
            cout << "Cache Hit for address " << key << endl;
            return true;
        } else { // Cache miss
            cout << "Cache Miss for address " << key << endl;
            return false;
        }
    }

    // add element to cache
    void add(int key, int value) {
        if (cache.find(key) != cache.end()) { 
            cache[key]->data = value;
            access(key); 
        } else {
            if (size >= capacity) { 
                cache.erase(head->data);
                removeNode(head);
            }
            Node* newNode = new Node(value);
            cache[key] = newNode;
            addToEnd(newNode);
        }
    }

};

int main() {
    int cacheSize = 4;
    int cacheLineSize = 64; //havent considered
    int singleElementSize = 4; //havent considered
    int associativity = 2; //havent considered yet

    LRUCache cache(cacheSize);

   
    int addresses[] = {100, 101, 102, 103, 100, 104,101, 105, 106, 107, 108, 109, 110};

   
    int totalAccesses = sizeof(addresses) / sizeof(addresses[0]);
    int cacheHits = 0;
    for (int i = 0; i < totalAccesses; ++i) {
        if (cache.access(addresses[i]))
            cacheHits++;
        else
            cache.add(addresses[i], addresses[i]);
    }

    double hitRate = (double) cacheHits / totalAccesses * 100.0;
    double missRate = 100.0 - hitRate;

    cout << "Cache Hit Rate: " << hitRate << "%" << endl;
    cout << "Cache Miss Rate: " << missRate << "%" << endl;

    return 0;
}
