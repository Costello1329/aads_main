#include <iostream>
#include <vector>
#include <cmath>
using std::pair;
using std::vector;
using std::make_pair;
using std::cin;


class SparseTable {
public:
    void setup(const vector<size_t>&);
    pair<size_t, size_t> rangeMinimumQuery (const size_t, const size_t);
private:
    vector<vector<pair<size_t, size_t>> > table;
};

void SparseTable::setup(const vector<size_t>& sequence) {
    const size_t length = sequence.size();
    const size_t height = static_cast<size_t>(log2(sequence.size())) + 1;
    table.resize(height);

    for (size_t i = 0; i < length; i ++) {
        table[0].push_back(make_pair(sequence[i], i));
    }

    for (size_t i = 1; i < height; i ++) {
        const size_t currentLength = length - pow(2, i) + 1;
        table[i].resize(currentLength);

        for (size_t j = 0; j < currentLength; j ++) {
            table[i][j] = std::min<pair<size_t, size_t>>(table[i - 1][j], table[i - 1][j + pow(2, i - 1)]);
        }
    }
}

pair<size_t, size_t> SparseTable::rangeMinimumQuery(const size_t left, const size_t right) {
    const size_t length = right - left + 1;
    const size_t power = static_cast<size_t>(log2(length));
    const size_t flooredLength = pow(2, power);

    const pair<size_t, size_t> leftValue = table[power][left];
    const size_t padding = length - flooredLength;
    const pair<size_t, size_t> rightValue = table[power][left + padding];

    return std::min<pair<size_t, size_t>>(leftValue, rightValue);
}

class Tree {
public:
    Tree(size_t);
    void addEdge(size_t, size_t);
    void lockTreeAndStartProcessingQueries();
    size_t leastCommonAncestor (size_t, size_t);
private:
    vector<vector<size_t> > children;
    vector<size_t> depthList;
    vector<size_t> verticesList;
    vector<size_t> indexes;
    SparseTable sparseTable;
    bool locked;
    void DFS(size_t vertex = 0, size_t depth = 0) {
        depthList.push_back(depth);
        verticesList.push_back(vertex);
        indexes[vertex] = verticesList.size() - 1;
        for (const size_t &child: children[vertex]) {
            DFS(child, depth + 1);
            depthList.push_back(depth);
            verticesList.push_back(vertex);
            indexes[vertex] = verticesList.size() - 1;
        }
    }
};

Tree::Tree (size_t verticesCount) {
    children.resize(verticesCount);
    depthList.resize(0);
    verticesList.resize(0);
    indexes.resize(verticesCount);
    locked = false;
}

void Tree::addEdge(size_t parent, size_t child) {
    if (locked)
        return;
    children[parent].push_back(child);
}

void Tree::lockTreeAndStartProcessingQueries () {
    if (locked) {
        return;
    }
    locked = true;
    DFS();
    sparseTable.setup(depthList);
}

size_t Tree::leastCommonAncestor(size_t firstVertex, size_t secondVertex) {
    if (!locked) {
        return -1;
    }
    const size_t firstIndex = indexes[firstVertex];
    const size_t secondIndex = indexes[secondVertex];
    const size_t minIndex = std::min<size_t>(firstIndex, secondIndex);
    const size_t maxIndex = std::max<size_t>(firstIndex, secondIndex);
    const size_t index = sparseTable.rangeMinimumQuery(minIndex, maxIndex).second;
    const size_t vertex = verticesList[index];
    return vertex;
}

class Progression {
public:
    Progression(size_t x, size_t y, size_t z, size_t n);
    void set(size_t a, size_t b);
    pair<size_t, size_t> get() { return make_pair(a, b); }
    void iterate();
private:
    size_t x, y, z, n;
    size_t a, b;
};

Progression::Progression(size_t x, size_t y, size_t z, size_t n) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->n = n;
}

void Progression::set(size_t a, size_t b) {
    this->a = a;
    this->b = b;
}

void Progression::iterate() {
    const size_t c = (x*a + y*b + z) % n;
    a = b;
    b = c;
}

int main () {
    size_t verticesCount;
    cin >> verticesCount;
    size_t queriesCount;
    cin >> queriesCount;
    Tree tree(verticesCount);
    for (size_t child = 1; child < verticesCount; child ++) {
        size_t parent;
        cin >> parent;
        tree.addEdge(parent, child);
    }
    size_t a, b;
    cin >> a >> b;
    size_t x, y, z;
    cin >> x >> y >> z;
    Progression progression(x, y, z, verticesCount);
    progression.set(a, b);
    tree.lockTreeAndStartProcessingQueries();
    size_t vertex = 0;
    size_t sum = 0;
    for (size_t i = 0; i < queriesCount; i ++) {
        size_t firstVertex = (progression.get().first + vertex) % verticesCount;
        size_t secondVertex = progression.get().second;
        vertex = tree.leastCommonAncestor(firstVertex, secondVertex);
        sum += vertex;
        progression.iterate();
        progression.iterate();
    }
    std::cout << sum << std::endl;
    return 0;
}

