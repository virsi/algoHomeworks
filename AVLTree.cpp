/*
4_2. Порядковые статистики. Дано число N и N строк. Каждая строка содержит команду добавления или
удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики. Команда добавления
числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
Запрос на получение k-ой порядковой статистики задается числом k. Требования: скорость выполнения запроса
- O(log n).

in       out
5        40
40 0     40
10 1     10
4 1      4
-10 0    50
50 2
*/

#include <algorithm>
#include <functional>
#include <iostream>

template <typename T>
struct Node {
        T key;
        int height;
        int size;  // размер поддерева
        Node* left;
        Node* right;

        Node(T k) : key(k), height(1), size(1), left(nullptr), right(nullptr) {}
};

template <typename T, typename Compare = std::less<T>>
class AVLTree {
    public:
        AVLTree(Compare cmp = Compare()) : root(nullptr), cmp(cmp) {}

        ~AVLTree() { destroy(root); }

        void insert(const T& key) { root = insert(root, key); }

        void remove(const T& key) { root = remove(root, key); }

        T findKth(int k) const {
            if (!root || k < 0 || k >= root->size) {
                throw std::out_of_range("Index out of bounds or tree is empty");
            }
            return findKth(root, k);
        }

        int size() const { return getSize(root); }

    private:
        Node<T>* root;
        Compare cmp;

        int getHeight(Node<T>* p) const { return p ? p->height : 0; }

        int getSize(Node<T>* p) const { return p ? p->size : 0; }

        void fix(Node<T>* p) {
            int hl = getHeight(p->left);
            int hr = getHeight(p->right);
            p->height = (hl > hr ? hl : hr) + 1;
            p->size = getSize(p->left) + getSize(p->right) + 1;
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
            if (!p)
                return new Node<T>(k);

            if (cmp(k, p->key))
                p->left = insert(p->left, k);
            else if (cmp(p->key, k))  // k > p->key
                p->right = insert(p->right, k);

            return balance(p);
        }

        Node<T>* findMin(Node<T>* p) { return p->left ? findMin(p->left) : p; }

        Node<T>* removeMin(Node<T>* p) {
            if (p->left == 0)
                return p->right;
            p->left = removeMin(p->left);
            return balance(p);
        }

        Node<T>* remove(Node<T>* p, const T& k) {
            if (!p)
                return nullptr;

            if (cmp(k, p->key)) {
                p->left = remove(p->left, k);
            } else if (cmp(p->key, k)) {
                p->right = remove(p->right, k);
            } else {
                Node<T>* left = p->left;
                Node<T>* right = p->right;
                delete p;
                if (!right)
                    return left;
                Node<T>* min = findMin(right);
                min->right = removeMin(right);
                min->left = left;
                return balance(min);
            }
            return balance(p);
        }

        T findKth(Node<T>* p, int k) const {
            int leftSize = getSize(p->left);

            if (k == leftSize) {
                return p->key;
            }

            if (k < leftSize) {
                return findKth(p->left, k);
            } else {
                return findKth(p->right, k - leftSize - 1);
            }
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

    int N;
    if (!(std::cin >> N))
        return 0;

    AVLTree<int> tree;

    for (int i = 0; i < N; ++i) {
        int val, k;
        std::cin >> val >> k;

        if (val > 0) {
            tree.insert(val);
        } else {
            tree.remove(std::abs(val));
        }

        std::cout << tree.findKth(k) << "\n";
    }

    return 0;
}
