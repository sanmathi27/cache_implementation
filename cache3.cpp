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

public:
    // Constructor to initialize the cache with the params
    SetAssociativeCache(int sets, int size, int assoc, int lineSize, int elementSize) : numSets(sets), 
        setSize(size / sets), cacheLineSize(lineSize), singleElementSize(elementSize) {
        //sets based on the number of sets specified
        for (int i = 0; i < numSets; ++i) {
            this->sets.push_back(new Set(assoc)); // Add new set to the cache
        }
    }

    // access a node with the given key
    bool access(int key) {
        int setIndex = key % numSets;   //set index for the key
        return sets[setIndex]->access(key); 
    }

    // add a node to the cache
    void add(int key, int value) {
        int setIndex = key % numSets;   // set index for the key
        sets[setIndex]->add(key, value);    
        cout << "Address " << key << " added to set " << setIndex << endl; 
    }
};

// Main function to test the set-associative cache
int main() {
    int cacheSize = 16;          // Total cache size
    int numSets = 4;             // Number of sets in the cache
    int setSize = 4;             // Associativity 
    int cacheLineSize = 8;       // Cache line size
    int singleElementSize = 4;   // Size of a single element

    SetAssociativeCache cache(numSets, cacheSize, setSize, cacheLineSize, singleElementSize);

    int addresses[] = {100, 101, 102,101, 103, 100,101, 104, 101, 105,101,107, 108, 109 };

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
