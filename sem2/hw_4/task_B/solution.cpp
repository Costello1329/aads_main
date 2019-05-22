#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using std::vector;
using std::numeric_limits;
using std::max;
using std::cin;
using std::cout;
using std::endl;

struct Node {
    int maxSubsegmentPrefix;
    int maxSubsegmentSuffix;
    int segmentSum;
    int maxSumOnSubsegment;
    Node (int node = numeric_limits<int>::min()) {
        maxSubsegmentPrefix = node;
        maxSubsegmentSuffix = node;
        segmentSum = (node == numeric_limits<int>::min() ? 0 : node);
        maxSumOnSubsegment = node;
    }
};

class SegmentTree {
private:
    int size;
    vector<Node> tree;
    Node createNode (const Node &, const Node &);

public:
    SegmentTree (const vector<int> &);
    Node maxSubsegment (const int &, const int &, const int &, const int &, const int &);
};

Node SegmentTree::createNode (const Node& left, const Node& right) {
    Node node;
    node.maxSubsegmentPrefix = max(left.maxSubsegmentPrefix, left.segmentSum + right.maxSubsegmentPrefix);
    node.maxSubsegmentSuffix = max(right.maxSubsegmentSuffix, right.segmentSum + left.maxSubsegmentSuffix);
    node.segmentSum = left.segmentSum + right.segmentSum;
    node.maxSumOnSubsegment = max(
            max(left.maxSumOnSubsegment, right.maxSumOnSubsegment),
            left.maxSubsegmentSuffix + right.maxSubsegmentPrefix
    );
    return node;
}

SegmentTree::SegmentTree (const vector<int> &sequence) {
    size = sequence.size();
    const int ceiledSize = pow(2, ceil(log2(size)));
    tree.resize(ceiledSize * 2 - 1, Node());
    for (int index = ceiledSize - 1; index < ceiledSize + size - 1; index ++) {
        tree[index] = Node(sequence[index - ceiledSize + 1]);
    }
    for (int index = ceiledSize - 2; index >= 0; index --) {
        const int leftChildIndex = 2 * index + 1;
        const int rightChildIndex = 2 * index + 2;
        tree[index] = createNode(tree[leftChildIndex], tree[rightChildIndex]);
    }
}

Node SegmentTree::maxSubsegment (const int &node, const int &left, const int &right, const int &queryLeft, const int &queryRight) {
    const int rightChildIndex = 2 * node + 2;
    const int leftChildIndex = 2 * node + 1;
    const int middle = (queryLeft + queryRight) / 2;
    if (queryLeft == left && queryRight == right) {
        return tree[node];
    }
    if (left > middle) {
        return maxSubsegment(rightChildIndex, left, right, middle + 1, queryRight);
    }
    if (right <= middle) {
        return maxSubsegment(leftChildIndex, left, right, queryLeft, middle);
    }
    return createNode(
            maxSubsegment(leftChildIndex, left, middle, queryLeft, middle),
            maxSubsegment(rightChildIndex, middle + 1, right, middle + 1, queryRight)
    );
}


int main () {
    while (true) {
        int size, queriesCount;
        if (!(cin >> size >> queriesCount)) {
            break;
        }
        vector<int> sequence(size);
        for (int i = 0; i < size; ++i) {
            cin >> sequence[i];
        }
        SegmentTree tree(sequence);
        for (int i = 0; i < queriesCount; ++i) {
            int left;
            int right;
            cin >> left;
            cin >> right;
            --left;
            --right;
            const int ceiledSize = pow(2, ceil(log2(size))) - 1;
            cout << tree.maxSubsegment(0, left , right, 0, ceiledSize).maxSumOnSubsegment << endl;
        }
    }
    return 0;
}
