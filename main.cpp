#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class CacheLine {
public:
    bool valid;
    vector<int> data;

    CacheLine(int lineSize) : valid(false), data(lineSize) {}
};

class Cache {
private:
    int cacheSize;
    int lineSize;
    int singleElementSize;
    int associativity;
    int numLines;
    vector<vector<CacheLine> > cache;

    int accessCount;
    int hitCount;

public:
    Cache(int size, int line, int element, int assoc) :
        cacheSize(size), lineSize(line), singleElementSize(element), associativity(assoc) {
        
        numLines = cacheSize / (lineSize * associativity);
        cache.resize(numLines, vector<CacheLine>(associativity, CacheLine(lineSize)));

        accessCount = 0;
        hitCount = 0;
    }

    void access(int address) {
        accessCount++;
        int index = address / (lineSize * singleElementSize);
        int offset = address % (lineSize * singleElementSize);

        for (int i = 0; i < associativity; ++i) {
            if (cache[index][i].valid) {
                if (cache[index][i].data[offset] == address) {
                    hitCount++;
                    cout << "Cache Hit" << endl;
                    return;
                }
            }
        }

        cout << "Cache Miss" << endl;

        // round-robin replacement policy
        int lineToReplace = rand() % associativity;
        cache[index][lineToReplace].valid = true;
        cache[index][lineToReplace].data[offset] = address;
    }

    float getHitRatio() const {
        if (accessCount == 0) return 0.0;
        return static_cast<float>(hitCount) / accessCount;
    }
};

int main() {
    int cacheSize = 1024;
    int lineSize = 32;
    int singleElementSize = 4;
    int associativity = 4;

    Cache cache(cacheSize, lineSize, singleElementSize, associativity);

    
    cache.access(100);
    cache.access(101);
    cache.access(102);
    cache.access(101);
    cache.access(103);

    cout << "Hit Ratio: " << cache.getHitRatio() * 100 << "%" << endl;

    return 0;
}
