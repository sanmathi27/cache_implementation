//cache implementation
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

// Struct for nodes in doubly linked list
struct Node {
    int data;
    Node* prev;     // Pointer to the prev node
    Node* next;     // Pointer to the next node

    // Constructor to initialize
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

// Class representing a set in the cache
class Set {
public:
    unordered_map<int, Node*> cache;    // hashtable, key - int, value - pointer
    Node* head; //least recently used node
    Node* tail; //most recently used node
    int capacity; //cap of cache
    int size; //current size of cache

    // Constructor to initialize the set with a capacity
    Set(int cap) : capacity(cap), size(0), head(nullptr), tail(nullptr) {}

    //remove a node from the set
    void removeNode(Node* node) {
        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;

        delete node;    // Delete the node
        size--;         // Decrease the size of the set
    }

    // add a node to the end of the set
    void addToEnd(Node* node) {
        if (!head)
            head = node;
        else
            tail->next = node;
        node->prev = tail;
        tail = node;
        size++;
    }

    // access a node with the given key
    bool access(int key) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            removeNode(node);
            addToEnd(node);
            return true;     //hit
        } else {
            return false;   //miss
        }
    }

    // add a node to the set
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

// set-associative cache
class SetAssociativeCache {
private:
    vector<Set*> sets;
    int numSets;                    // Number of sets in the cache
    int setSize;                    // Size of each set
    int cacheLineSize;              // Cache line size
    int singleElementSize;          // Size of a single element
    int addressWidth;               // Width of the memory addresses
    int tagWidth;                   // Width of tag bits
    int setIndexWidth;              // Width of set index bits

public:
    // Constructor to initialize the cache with the params
    SetAssociativeCache(int sets, int size, int assoc, int lineSize, int elementSize) : numSets(sets),
        setSize(assoc), cacheLineSize(lineSize), singleElementSize(elementSize) {

        addressWidth = cacheLineSize * singleElementSize;
        setIndexWidth = log2(sets); // Number of bits for set index
        tagWidth = addressWidth - setIndexWidth; // Number of bits for tag

        for (int i = 0; i < numSets; ++i) {
            this->sets.push_back(new Set(assoc)); // Add new set to the cache
        }
    }

    // extract tag bits from address
    int extractTag(int address) {
        return address >> setIndexWidth; // shift to the right to exclude set index bits
    }

    // extract set index bits from the address
    int extractSetIndex(int address) {
        return address & ((1 << setIndexWidth) - 1); // bitwise AND to extract set index bits
    }

    // access a node with the given key
    bool access(int key) {
        int setIndex = extractSetIndex(key);   //set index for the key
        return sets[setIndex]->access(key);
    }

    // add a node to the cache
    void add(int key, int value) {
        int setIndex = extractSetIndex(key);   // set index for the key
        sets[setIndex]->add(key, value);
        cout << "Address " << key << " added to set " << setIndex << endl;
    }
};

// main func to test the set-associative cache
int main() {
    int cacheSize = 16;          // total cache size
    int numSets = 4;             // number of sets in the cache
    int setSize = 4;             // associativity 
    int cacheLineSize = 8;       // cache line size
    int singleElementSize = 4;   // size of a single element

    SetAssociativeCache cache(numSets, cacheSize, setSize, cacheLineSize, singleElementSize);

    int addresses[] = {100, 101, 102, 103, 100,101, 104, 101, 105,101,107, 108, 106 };

    int totalAccesses = sizeof(addresses) / sizeof(addresses[0]);
    int cacheHits = 0;
    for (int i = 0; i < totalAccesses; ++i) {
        if (cache.access(addresses[i])) {
            cout << "Cache Hit for address " << addresses[i] << endl;
            cacheHits++;
        } else {
            cout << "Cache Miss for address " << addresses[i] << endl;
            cache.add(addresses[i], addresses[i]);
        }
    }

    double hitRate = (double) cacheHits / totalAccesses * 100.0;
    double missRate = 100.0 - hitRate;

    cout << "Cache Hit Rate: " << hitRate << "%" << endl;
    cout << "Cache Miss Rate: " << missRate << "%" << endl;

    return 0;
}
