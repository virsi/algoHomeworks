// #include <iostream>
// #include <sstream>
// using namespace std;

// struct Node {
//     int value;
//     Node* left;
//     Node* right;
//     Node(int val) : value(val), left(nullptr), right(nullptr) {}
// };

// Node* insert(Node* root, int val) {
//     if (!root) return new Node(val);
//     if (val < root->value)
//         root->left = insert(root->left, val);
//     else
//         root->right = insert(root->right, val);
//     return root;
// }

// bool checkEqualAll(Node* root, int firstVal) {
//     if (!root) return true;
//     if (root->value != firstVal) return false;
//     return checkEqualAll(root->left, firstVal) && checkEqualAll(root->right, firstVal);
// }

// int main() {
//     Node* root = nullptr;
//     string line;


//     getline(cin, line);
//     stringstream ss(line);
//     int val;

//     while (ss >> val) {
//         root = insert(root, val);
//     }

//     if (!root) {
//         cout << 1 << endl;
//     } else {
//         int first = root->value;
//         bool allEqual = checkEqualAll(root, first);
//         cout << (allEqual ? 1 : 0) << endl;
//     }

//     return 0;
// }


// #include <iostream>
// #include <sstream>
// #include <climits>
// using namespace std;

// struct Node {
//     int value;
//     Node* left;
//     Node* right;
//     Node(int val) : value(val), left(nullptr), right(nullptr) {}
// };

// Node* insert(Node* root, int val) {
//     if (!root) return new Node(val);
//     if (val < root->value)
//         root->left = insert(root->left, val);
//     else
//         root->right = insert(root->right, val);
//     return root;
// }

// int minDepth(Node* root) {
//     if (!root) return 0;
//     if (!root->left)
//         return 1 + minDepth(root->right);
//     if (!root->right)
//         return 1 + minDepth(root->left);
//     return 1 + min(minDepth(root->left), minDepth(root->right));
// }

// int main() {
//     Node* root = nullptr;
//     string line;

//     getline(cin, line);
//     stringstream ss(line);
//     int val;

//     while (ss >> val) {
//         root = insert(root, val);
//     }

//     cout << minDepth(root) << endl;

//     return 0;
// }

// #include <iostream>
// #include <unordered_set>
// #include <string>
// using namespace std;

// struct Node {
//     string product_name;
//     int color;
//     int size;
// };

// bool operator==(const Node& lhs, const Node& rhs) {
//     return lhs.product_name == rhs.product_name &&
//            lhs.color == rhs.color &&
//            lhs.size == rhs.size;
// }

// namespace std {
//     template<>
//     struct hash<Node> {
//         size_t operator()(const Node& node) const noexcept {
//             size_t h1 = hash<string>()(node.product_name);
//             size_t h2 = hash<int>()(node.color);
//             size_t h3 = hash<int>()(node.size);
//             return h1 ^ (h2 << 1) ^ (h3 << 2);
//         }
//     };
// }

// int main() {
//     unordered_set<Node> table;
//     char op;
//     string name;
//     int color, size = 0;

//     while (cin >> op >> name >> color >> size) {
//         Node node{name, color, size};

//         if (op == '+') {
//             if (table.find(node) == table.end()) {
//                 table.insert(node);
//                 cout << "OK\n";
//             } else {
//                 cout << "FAIL\n";
//             }
//         } else if (op == '-') {
//             if (table.find(node) != table.end()) {
//                 table.erase(node);
//                 cout << "OK\n";
//             } else {
//                 cout << "FAIL\n";
//             }
//         } else if (op == '?') {
//             if (table.find(node) != table.end()) {
//                 cout << "OK\n";
//             } else {
//                 cout << "FAIL\n";
//             }
//         }
//     }

//     return 0;
// }



#include <algorithm>
#include <iostream>
#include <string>

template <typename T>
struct Node {
    T key;
    int height;
    Node* left;
    Node* right;

    Node(T k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

template <typename T>
class AVLTree {
public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() { destroy(root); }

    void insert(const T& key) { root = insert(root, key); }

    void remove(const T& key) { root = remove(root, key); }

    bool exists(const T& key) const {
        Node<T>* current = root;
        while (current) {
            if (key == current->key) return true;
            if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        return false;
    }

    const T* next(const T& x) const {
        Node<T>* current = root;
        const T* res = nullptr;
        while (current) {
            if (current->key > x) {
                res = &current->key;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return res;
    }

    const T* prev(const T& x) const {
        Node<T>* current = root;
        const T* res = nullptr;
        while (current) {
            if (current->key < x) {
                res = &current->key;
                current = current->right;
            } else {
                current = current->left;
            }
        }
        return res;
    }

private:
    Node<T>* root;

    int getHeight(Node<T>* p) const { return p ? p->height : 0; }

    void fix(Node<T>* p) {
        int hl = getHeight(p->left);
        int hr = getHeight(p->right);
        p->height = (hl > hr ? hl : hr) + 1;
    }

    Node<T>* rotateRight(Node<T>* p) {
        Node<T>* q = p->left;
        p->left = q->right;
        q->right = p;
        fix(p);
        fix(q);
        return q;
    }

    Node<T>* rotateLeft(Node<T>* q) {
        Node<T>* p = q->right;
        q->right = p->left;
        p->left = q;
        fix(q);
        fix(p);
        return p;
    }

    int bfactor(Node<T>* p) const { return getHeight(p->right) - getHeight(p->left); }

    Node<T>* balance(Node<T>* p) {
        fix(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0)
                p->right = rotateRight(p->right);
            return rotateLeft(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0)
                p->left = rotateLeft(p->left);
            return rotateRight(p);
        }
        return p;
    }

    Node<T>* insert(Node<T>* p, const T& k) {
        if (!p) return new Node<T>(k);

        if (k < p->key)
            p->left = insert(p->left, k);
        else if (k > p->key)
            p->right = insert(p->right, k);
        return balance(p);
    }

    Node<T>* findMin(Node<T>* p) { return p->left ? findMin(p->left) : p; }

    Node<T>* removeMin(Node<T>* p) {
        if (p->left == 0) return p->right;
        p->left = removeMin(p->left);
        return balance(p);
    }

    Node<T>* remove(Node<T>* p, const T& k) {
        if (!p) return nullptr;

        if (k < p->key) {
            p->left = remove(p->left, k);
        } else if (k > p->key) {
            p->right = remove(p->right, k);
        } else {
            Node<T>* left = p->left;
            Node<T>* right = p->right;
            delete p;
            if (!right) return left;
            Node<T>* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            return balance(min);
        }
        return balance(p);
    }

    void destroy(Node<T>* p) {
        if (p) {
            destroy(p->left);
            destroy(p->right);
            delete p;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    AVLTree<int> tree;
    std::string op;
    int x;

    while (std::cin >> op >> x) {
        if (op == "insert") {
            tree.insert(x);
        } else if (op == "delete") {
            tree.remove(x);
        } else if (op == "exists") {
            if (tree.exists(x))
                std::cout << "true\n";
            else
                std::cout << "false\n";
        } else if (op == "next") {
            const int* res = tree.next(x);
            if (res)
                std::cout << *res << "\n";
            else
                std::cout << "none\n";
        } else if (op == "prev") {
            const int* res = tree.prev(x);
            if (res)
                std::cout << *res << "\n";
            else
                std::cout << "none\n";
        }
    }

    return 0;
}
