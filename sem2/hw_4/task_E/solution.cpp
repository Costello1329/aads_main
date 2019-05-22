#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <queue>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

struct Node {
    string str;
    long long priority;
    long long weight;
    Node *left;
    Node *right;
    Node (const string &str = "") {
        this->str = str;
        this->priority = rand();
        this->weight = 1;
        left = nullptr;
        right = nullptr;
    }
};

class StringTreap {
public:
    StringTreap() {
        root = nullptr;
    }
    void insert(long long, const string&);
    void remove(long long, long long);
    string getAt(long long);
private:
    Node *root;
    void update(Node *node) {
        node->weight = 1 + (node->left == nullptr ? 0 : node->left->weight) + (node->right == nullptr ? 0 : node->right->weight);
    }
    void split(Node *, long long, Node *&, Node *&);
    Node* merge(Node *&, Node *&);
};

void StringTreap::split(Node *current, long long implicitKey, Node *&left, Node *&right) {
    if (current == nullptr) {
        left = right = nullptr;
        return;
    }
    const long long currentIndex = (current->left != nullptr ? current->left->weight : 0) + 1;
    if (currentIndex < implicitKey) {
        split(current->right, implicitKey - currentIndex, current->right, right);
        left = current;
        update(current);
    } else {
        split(current->left, implicitKey, left, current->left);
        right = current;
        update(current);
    }
}

Node* StringTreap::merge(Node *&left, Node *&right) {
    if (left == nullptr) {
        return right;
    } else if (right == nullptr) {
        return left;
    } else if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        update(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        update(right);
        return right;
    }
}

void StringTreap::insert(long long position, const string &str) {
    Node *left, *right;
    split(root, position + 1, left, right);
    Node *insertion = new Node(str);
    left = merge(left, insertion);
    root = merge(left, right);
}

void StringTreap::remove(long long from, long long to) {
    Node *left;
    Node *right;
    Node *deletion;
    split(root, from + 1, left, right);
    split(right, to - from + 2, deletion, right);
    root = merge(left, right);
    delete deletion;
}

string StringTreap::getAt(long long position) {
    Node *left;
    Node *middle;
    Node *right;
    split(root, position + 1, left, right);
    split(right, 2, middle, right);
    string out = middle->str;
    right = merge(middle, right);
    root = merge(left, right);
    return out;
}

int main() {
    StringTreap array;
    size_t count;
    cin >> count;
    for (int i = 0; i < count; i ++) {
        char command;
        cin >> command;
        if (command == '+') {
            size_t position;
            string str;
            cin >> position >> str;
            array.insert(position, str);
        } else if (command == '-') {
            size_t left, right;
            cin >> left >> right;
            array.remove(left, right);
        } else {
            size_t position;
            cin >> position;
            cout << array.getAt(position) << endl;
        }
    }
    return 0;
}
