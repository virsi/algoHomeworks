/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне 0..232 -1
Требования:
B-дерево должно быть реализовано в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Формат входных данных
Сначала вводится минимальный порядок дерева t.
Затем вводятся элементы дерева.
Формат выходных данных
Программа должна вывести B-дерево по слоям. Каждый слой на новой строке, элементы должны выводится в том
порядке, в котором они лежат в узлах.

stdin                    stdout
2                        3
0 1 2 3 4 5 6 7 8 9      1 5 7
                         0 2 4 6 8 9

4                        3
0 1 2 3 4 5 6 7 8 9      0 1 2 4 5 6 7 8 9
*/

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class BTree {
    private:
        struct BNode {
                std::vector<T> keys;
                std::vector<BNode*> children;
                BNode* parent;
                bool leaf;

                BNode(bool is_leaf = true) : parent(nullptr), leaf(is_leaf) {}

                bool is_leaf() const { return leaf; }

                ~BNode() {}
        };

        BNode* root;
        int t_value;
        Compare comp;

        int max_keys() const { return 2 * t_value - 1; }
        int min_keys() const { return t_value - 1; }
        int median_index() const { return t_value - 1; }

        void find_insert_position(BNode* node, const T& key, int& index);
        void split_child(BNode* parent_node, int child_index, BNode* child_node);
        void deletenode(BNode* node);
        bool searchKey(const T& key, BNode* node);
        void print_level_order(std::ostream& out);

    public:
        BTree(int t, Compare c = Compare()) : t_value(t), root(nullptr), comp(c) {
            if (t < 2) {
                throw std::invalid_argument("B-Tree minimum degree t must be >= 2.");
            }
        }
        ~BTree();

        void insert(T key);
        bool search(T key);
        void print();
};

template <typename T, typename Compare>
BTree<T, Compare>::~BTree() {
    if (root != nullptr)
        deletenode(root);
}

template <typename T, typename Compare>
void BTree<T, Compare>::deletenode(BNode* node) {
    if (node == nullptr)
        return;

    for (BNode* child : node->children) {
        deletenode(child);
    }
    delete node;
}

template <typename T, typename Compare>
void BTree<T, Compare>::find_insert_position(BNode* node, const T& key, int& index) {
    index = 0;
    while (index < node->keys.size() && comp(node->keys[index], key)) {
        index++;
    }
}

template <typename T, typename Compare>
void BTree<T, Compare>::split_child(BNode* parent_node, int child_index, BNode* child_node) {
    BNode* new_sibling = new BNode(child_node->is_leaf());

    // ключ который поднимается (индекс t-1)
    T median_key = child_node->keys[median_index()];

    // переносим ключи в нового правого брата
    new_sibling->keys.assign(child_node->keys.begin() + t_value, child_node->keys.end());

    // удаляем ключи из исходного узла (с медианного ключа до конца)
    child_node->keys.erase(child_node->keys.begin() + median_index(), child_node->keys.end());

    // если узел не лист, переносим потомков
    if (!child_node->is_leaf()) {
        new_sibling->children.assign(child_node->children.begin() + t_value, child_node->children.end());

        for (BNode* child : new_sibling->children) {
            child->parent = new_sibling;
        }

        child_node->children.erase(child_node->children.begin() + t_value, child_node->children.end());
    }

    parent_node->children.insert(parent_node->children.begin() + child_index + 1, new_sibling);
    new_sibling->parent = parent_node;

    parent_node->keys.insert(parent_node->keys.begin() + child_index, median_key);
}

template <typename T, typename Compare>
void BTree<T, Compare>::insert(T key) {
    if (root == nullptr) {
        root = new BNode();
        root->keys.push_back(key);
        return;
    }

    BNode* ptr = root;

    if (ptr->keys.size() == max_keys()) {
        BNode* new_root = new BNode(false);
        new_root->children.push_back(ptr);
        ptr->parent = new_root;
        root = new_root;
        split_child(new_root, 0, ptr);
        ptr = new_root;
    }

    while (!ptr->is_leaf()) {
        int i;
        find_insert_position(ptr, key, i);

        if (ptr->children[i]->keys.size() == max_keys()) {
            split_child(ptr, i, ptr->children[i]);
            if (comp(ptr->keys[i], key)) {
                i++;
            }
        }
        ptr = ptr->children[i];
    }

    int i;
    find_insert_position(ptr, key, i);
    ptr->keys.insert(ptr->keys.begin() + i, key);
}

template <typename T, typename Compare>
bool BTree<T, Compare>::searchKey(const T& key, BNode* node) {
    if (node == nullptr)
        return false;

    int i;
    find_insert_position(node, key, i);

    if (i < node->keys.size() && !comp(node->keys[i], key) && !comp(key, node->keys[i])) {
        return true;
    }

    if (node->is_leaf()) {
        return false;
    }

    return searchKey(key, node->children[i]);
}

template <typename T, typename Compare>
bool BTree<T, Compare>::search(T key) {
    return searchKey(key, this->root);
}

template <typename T, typename Compare>
void BTree<T, Compare>::print_level_order(std::ostream& out) {
    if (root == nullptr)
        return;

    std::queue<BNode*> q;
    q.push(root);
    q.push(nullptr);

    bool first_key_on_level = true;

    while (!q.empty()) {
        BNode* node = q.front();
        q.pop();

        if (node == nullptr) {
            if (!q.empty()) {
                out << std::endl;
                q.push(nullptr);
                first_key_on_level = true;
            }
            continue;
        }

        if (!first_key_on_level) {
            out << " ";
        }

        for (size_t i = 0; i < node->keys.size(); ++i) {
            out << node->keys[i];
            if (i < node->keys.size() - 1) {
                out << " ";
            }
        }

        first_key_on_level = false;

        for (BNode* child : node->children) {
            if (child != nullptr) {
                q.push(child);
            }
        }
    }
    out << std::endl;
}

template <typename T, typename Compare>
void BTree<T, Compare>::print() {
    print_level_order(std::cout);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // 1. Считываем t (первая строка)
    int t_input;
    if (!(std::cin >> t_input) || t_input < 2) {
        return 0;
    }

    BTree<int> tree(t_input);

    // 2. Считываем элементы дерева (вторая строка) — без использования std::string
    // Сначала снимем оставшийся символ новой строки (если он есть)
    if (std::cin.peek() == '\n') {
        std::cin.get();
    }

    // Читаем символы до первого '\n' (то есть до нажатия Enter) и парсим числа вручную
    std::vector<char> buffer;
    char ch;
    while (std::cin.get(ch)) {
        if (ch == '\n')
            break;
        buffer.push_back(ch);
    }

    // Разбор чисел из буфера (пропускаем пробелы, поддерживаем знак '-')
    long long cur = 0;
    bool in_number = false;
    bool negative = false;
    for (size_t i = 0; i < buffer.size(); ++i) {
        char c = buffer[i];
        if (c == '-' && !in_number) {
            negative = true;
            in_number = true;
            cur = 0;
        } else if (c >= '0' && c <= '9') {
            in_number = true;
            cur = cur * 10 + (c - '0');
        } else {
            if (in_number) {
                int value = negative ? -static_cast<int>(cur) : static_cast<int>(cur);
                tree.insert(value);
                in_number = false;
                negative = false;
                cur = 0;
            }
            // иначе просто пропускаем разделители
        }
    }
    // Последнее число (если строка не заканчивалась пробелом)
    if (in_number) {
        int value = negative ? -static_cast<int>(cur) : static_cast<int>(cur);
        tree.insert(value);
    }

    // 3. Вывод B-дерева по слоям
    tree.print();

    return 0;
}
