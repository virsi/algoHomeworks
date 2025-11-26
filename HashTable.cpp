/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой
адресацией. Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть
реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен
быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент
заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции добавления строки в
множество, удаления строки из множества и проверки принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.

Требования: В таблице запрещено хранение указателей на описатель элемента. Формат входных данных Каждая
строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и
следующей за ним через пробел строки, над которой проводится операция. Тип операции  – один из трех
символов:
    +  означает добавление данной строки в множество;
    -  означает удаление  строки из множества;
    ?  означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении
элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве. Формат выходных данных
Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того,
встречается ли данное слово в нашем множестве. stdin      stdout
+ hello    ok
+ bye      ok
? bye      ok
+ bye      fail
- bye      ok
? bye      fail
? hello    ok
*/

#include <iostream>
#include <string>
#include <vector>

const size_t INITIAL_SIZE = 8;

struct StringHasher {
        size_t operator()(const std::string& str, size_t prime_factor) const {
            size_t hash = 0;
            for (char c : str) {
                // 'a' + 1 гарантирует, что значения букв от 1 до 26
                hash = hash * prime_factor + (c - 'a' + 1);
            }
            return hash;
        }
};

template <typename T, typename H>
class HashTable {
    public:
        HashTable(H hasher);
        ~HashTable() = default;

        bool add(const T& key);
        bool remove(const T& key);
        bool has(const T& key);

    private:
        struct Node {
                T value;
                bool isEmpty = true;
                bool isDeleted = false;
        };

        std::vector<Node> table;
        size_t itemsCount;  // только живые
        H hasher;

        void rehashIfNeeded();
        void resize();
};

template <typename T, typename H>
HashTable<T, H>::HashTable(H h) : hasher(h), itemsCount(0) {
    table.resize(INITIAL_SIZE);
}

template <typename T, typename H>
bool HashTable<T, H>::add(const T& key) {
    rehashIfNeeded();

    size_t h1 = hasher(key, 137);  // хеш для индекса
    size_t h2 = hasher(key, 71);   // хеш для шага

    size_t step = (2 * h2 + 1);
    size_t m = table.size();

    int firstDeletedIndex = -1;

    for (size_t i = 0; i < m; ++i) {
        size_t index = (h1 + i * step) % m;

        if (table[index].isEmpty) {
            // Вставляем либо сюда, либо в ранее найденный deleted
            if (firstDeletedIndex != -1) {
                table[firstDeletedIndex].value = key;
                table[firstDeletedIndex].isDeleted = false;
                table[firstDeletedIndex].isEmpty = false;
            } else {
                table[index].value = key;
                table[index].isEmpty = false;
                table[index].isDeleted = false;
            }
            itemsCount++;
            return true;
        }

        if (!table[index].isEmpty && !table[index].isDeleted && table[index].value == key) {
            return false;
        }

        if (table[index].isDeleted && firstDeletedIndex == -1) {
            firstDeletedIndex = (int)index;
        }
    }

    // Если таблица забита deleted элементами
    if (firstDeletedIndex != -1) {
        table[firstDeletedIndex].value = key;
        table[firstDeletedIndex].isDeleted = false;
        table[firstDeletedIndex].isEmpty = false;
        itemsCount++;
        return true;
    }

    return false;
}

template <typename T, typename H>
bool HashTable<T, H>::remove(const T& key) {
    size_t h1 = hasher(key, 137);
    size_t h2 = hasher(key, 71);
    size_t step = (2 * h2 + 1);
    size_t m = table.size();

    for (size_t i = 0; i < m; ++i) {
        size_t index = (h1 + i * step) % m;

        if (table[index].isEmpty) {
            return false;
        }

        if (!table[index].isDeleted && table[index].value == key) {
            table[index].isDeleted = true;
            itemsCount--;
            return true;
        }
    }
    return false;
}

template <typename T, typename H>
bool HashTable<T, H>::has(const T& key) {
    size_t h1 = hasher(key, 137);
    size_t h2 = hasher(key, 71);
    size_t step = (2 * h2 + 1);
    size_t m = table.size();

    for (size_t i = 0; i < m; ++i) {
        size_t index = (h1 + i * step) % m;

        if (table[index].isEmpty) {
            return false;
        }

        if (!table[index].isDeleted && table[index].value == key) {
            return true;
        }
    }
    return false;
}

template <typename T, typename H>
void HashTable<T, H>::rehashIfNeeded() {
    // itemsCount / size >= 3/4
    if (itemsCount * 4 >= table.size() * 3) {
        resize();
    }
}

template <typename T, typename H>
void HashTable<T, H>::resize() {
    std::vector<Node> oldTable = std::move(table);

    size_t newSize = oldTable.size() * 2;
    table.resize(newSize);
    // все новые элементы по умолчанию isEmpty = true

    itemsCount = 0;

    for (const auto& node : oldTable) {
        if (!node.isEmpty && !node.isDeleted) {
            add(node.value);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    StringHasher hasher;
    HashTable<std::string, StringHasher> set(hasher);

    char op;
    std::string word;

    while (std::cin >> op >> word) {
        bool result = false;
        switch (op) {
            case '+':
                result = set.add(word);
                break;
            case '-':
                result = set.remove(word);
                break;
            case '?':
                result = set.has(word);
                break;
            default:
                continue;
        }
        std::cout << (result ? "OK" : "FAIL") << "\n";
    }

    return 0;
}
