/*
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется
в правое поддерево root; иначе в левое поддерево root. Требования: Рекурсия запрещена. Решение должно
поддерживать передачу функции сравнения снаружи.

2_3. Выведите элементы в порядке post-order (снизу вверх).
in      out
3
2 1 3   1 3 2

3
1 2 3   3 2 1

3
3 1 2   2 1 3
*/

#include <iostream>
#include <vector>

template <typename T>
struct treeNode {
        T value;
        treeNode* left;
        treeNode* right;

        treeNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename T>
struct DefaultComparator {
        bool operator()(const T& left, const T& right) const { return left < right; }
};

template <typename T, typename Comparator = DefaultComparator<T>>
class Tree {
    public:
        Tree() : root(nullptr) {}
        ~Tree();
        void addElement(const T& val);
        void printPostOrder();

    private:
        treeNode<T>* root;
        Comparator cmp;
};

template <typename T, typename Comparator>
Tree<T, Comparator>::~Tree() {
    if (root == nullptr) {
        return;
    }

    std::vector<treeNode<T>*> stack{};
    stack.push_back(root);

    while (!stack.empty()) {
        treeNode<T>* node = stack.back();
        stack.pop_back();

        if (node->left != nullptr) {
            stack.push_back(node->left);
        }
        if (node->right != nullptr) {
            stack.push_back(node->right);
        }
        delete node;
    }
    root = nullptr;
}

template <typename T, typename Comparator>
void Tree<T, Comparator>::addElement(const T& value) {
    if (root == nullptr) {
        root = new treeNode<T>(value);
        return;
    }

    treeNode<T>* current = root;

    while (true) {
        // value >= current->value
        if (!cmp(value, current->value)) {
            // правое поддерево
            if (current->right == nullptr) {
                current->right = new treeNode<T>(value);
                return;
            }
            current = current->right;
        } else {
            // левое поддерево
            if (current->left == nullptr) {
                current->left = new treeNode<T>(value);
                return;
            }
            current = current->left;
        }
    }
}

template <typename T, typename Comparator>
void Tree<T, Comparator>::printPostOrder() {
    if (root == nullptr) {
        return;
    }

    std::vector<treeNode<T>*> stack{};

    treeNode<T>* previous = nullptr;
    treeNode<T>* current = root;

    while (!stack.empty() || current != nullptr) {
        // влево до упора и кладём узлы на стек
        if (current != nullptr) {
            stack.push_back(current);
            current = current->left;
        } else {
            treeNode<T>* node = stack.back();

            // проверяем правого потомка
            if (node->right != nullptr && node->right != previous) {
                current = node->right;
            } else {
                std::cout << node->value << " ";
                previous = node;  // посещённый
                stack.pop_back();
            }
        }
    }

    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    int value = 0;

    Tree<int> tree;

    if (!(std::cin >> n)) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> value)) {
            break;
        }
        tree.addElement(value);
    }

    tree.printPostOrder();

    return 0;
}
