//cache implementation
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
int singleElementSize = 4;
// struct for nodes in doubly linked list
struct Node {
    int data;
    Node* prev;     // pointer to the prev node
    Node* next;     // pointer to the next node

    // constructor to initialize
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

// class representing a set in cache
class Set {
public:
    unordered_map<int, Node*> cache;    // hashtable, key - int, value - pointer
    Node* head; //least recently used node
    Node* tail; //most recently used node
    int capacity; //cap of cache
    int size; //current size of cache

    // constructor to initialize the set with som capacity
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

        delete node;
        size--; 
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

    // add multiple nodes to the set
    void addMultiple(int startKey, int startValue, int num) {
    for (int i = 0; i < num; ++i) {
        int key = startKey + i * singleElementSize; // addrs based on the size of a single element
        int value = startValue + i; // some value
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
    int numSets;                    // num of sets in cache
    int setSize;                    // size of each set
    int cacheLineSize;              // cache line size
    int singleElementSize;          // size of a single element
    int addressWidth;               // width of the memory addresses
    int tagWidth;                   // width of tag bits
    int setIndexWidth;              // width of set index bits
    int nextAddress;                // next available memory address
    int currentSet;                 // current set index

public:
    // constructor to initialize the cache with the params
    SetAssociativeCache(int sets, int size, int assoc, int lineSize, int elementSize) : numSets(sets),
        setSize(assoc), cacheLineSize(lineSize), singleElementSize(elementSize), nextAddress(0), currentSet(0) {

        addressWidth = cacheLineSize * singleElementSize;
        setIndexWidth = log2(sets); // num of bits for set index
        tagWidth = addressWidth - setIndexWidth; // num of bits for tag

        for (int i = 0; i < numSets; ++i) {
            this->sets.push_back(new Set(assoc)); // add new set to cache
        }
    }

    // generate next mem address for an element
    int getNextAddress() {
        int address = nextAddress;
        nextAddress += singleElementSize;
        return address;
    }

    // extract tag bits from address
    int extractTag(int address) {
        return address >> setIndexWidth; // shift to the right to exclude set index bits
    }

    // access a node with the given key
bool access(int key) {
    for (int setIndex = 0; setIndex < numSets; ++setIndex) {
        if (sets[setIndex]->access(key)) {
            return true; //cache hit
        }
    }
    return false; // cache miss
}


    // add a node to the cache
    void add(int key, int value, int num) {
    int setIndex = currentSet;
    currentSet = (currentSet + 1) % numSets; // switch btw caches to avoid conflict misses
    
    // add the curr address and the next three addresses to cache set
    sets[setIndex]->addMultiple(key, value, 4);
}


};

// main func to test the set-associative cache
int main() {
    int cacheSize = 8;          // total cache size
    int numSets = 2;             // number of sets in the cache
    int setSize = 4;             // associativity
    int cacheLineSize = 4;       // cache line size
    int singleElementSize = 4;   // size of a single element

    SetAssociativeCache cache(numSets, cacheSize, setSize, cacheLineSize, singleElementSize);

    // generate addresses for matrix X
    int xAddresses[1024];
    for (int i = 0; i < 1024; ++i) {
        xAddresses[i] = cache.getNextAddress();
    }

    // generate addresses for matrix Y (start from after matrix X)
    int yAddresses[1024];
    for (int i = 0; i < 1024; ++i) {
        yAddresses[i] = cache.getNextAddress() + 16;
    }
    
    int totalAccesses = 2048; // assuming both matrices have the same number of elements
    int cacheHits = 0;
    int j=0;
    int k=0;
    for (int i = 0; i < totalAccesses; i++) {
        if (i % 2 == 0) {
            if (cache.access(xAddresses[j])) {
                cout << "Cache Hit for address " << xAddresses[j] << endl;
                cacheHits++;
            } else {
                cout << "Cache Miss for address " << xAddresses[j] << endl;
                
                if(j%4==0)
                cache.add(xAddresses[j], xAddresses[j], 16); // add next 4 elements from matrix X
            }
            j++;
        } 
        else {
            // Accessing element from matrix Y
            if (cache.access(yAddresses[k])) {
                cout << "Cache Hit for address " << yAddresses[k] << endl;
                cacheHits++;
            } else {
                cout << "Cache Miss for address " << yAddresses[k] << endl;
                if(k%4==0)
                cache.add(yAddresses[k], yAddresses[k], 16); // add next 4 elements from matrix Y
            }
            k++;
        }
    }

    double hitRate = (double) cacheHits / (totalAccesses ) * 100.0;
    double missRate = 100.0 - hitRate;

    cout << "Cache Hit Rate: " << hitRate << "%" << endl;
    cout << "Cache Miss Rate: " << missRate << "%" << endl;

    return 0;
}
