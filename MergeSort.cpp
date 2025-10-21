/*
5_1. Реклама.
В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей (два целых числа).
Каждому покупателю необходимо показать минимум 2 рекламы.  Рекламу можно транслировать только в целочисленные моменты времени.
Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно.
Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
Требуется определить минимальное число показов рекламы.
In         Out
5          5
1 10
10 12
1 10
1 10
23 24
*/

#include <iostream>
#include <stdexcept>

template <typename T>
class Array {
    public:
        Array() : size(0), capacity(5) { arr = new T[capacity]; }

        ~Array() { delete[] arr; }

        Array(const Array& other) : size(other.size), capacity(other.capacity) {
            if (!capacity)
                arr = nullptr;
            arr = new T[capacity];
            for (int i = 0; i < size; ++i) {
                arr[i] = other.arr[i];
            }
        }

        Array& operator=(const Array& other) {
            if (this != &other) {
                delete[] arr;

                size = other.size;
                capacity = other.capacity;

                arr = new T[capacity];
                for (int i = 0; i < size; ++i) {
                    arr[i] = other.arr[i];
                }
            }
            return *this;
        }

        T& operator[](int i) {
            if (i < 0 || i >= size) {
                throw std::out_of_range("Index out of range");
            }
            return arr[i];
        }

        const T& operator[](int i) const {
            if (i < 0 || i >= size) {
                throw std::out_of_range("Index out of range");
            }
            return arr[i];
        }

        void add(T element) {
            if (size >= capacity) {
                extend();
            }
            arr[size++] = element;
        }

        bool isEmpty() const { return size == 0; }

        int getSize() const { return size; }

        friend std::ostream& operator<<(std::ostream& out, const Array& array) {
            out << "[ ";
            for (int i = 0; i < array.size; ++i) {
                out << array[i];
                if (i < array.size - 1) {
                    out << ", ";
                }
            }
            out << " ]";
            return out;
        }

    private:
        T* arr;
        int size = 0;
        int capacity = 5;

        void extend() {
            T* newArr = new T[capacity * 2];
            for (int i = 0; i < size; ++i) {
                newArr[i] = arr[i];
            }

            delete[] arr;
            arr = newArr;
            capacity *= 2;
        }
};

struct Client {
        int arrivalTime = 0;
        int leavingTime = 0;
        Client() : arrivalTime(0), leavingTime(0) {}
        Client(int arrival, int leaving) : arrivalTime(arrival), leavingTime(leaving) {}
        ~Client() {}

        friend std::istream& operator>>(std::istream& input, Client& client) {
            input >> client.arrivalTime >> client.leavingTime;
            return input;
        }

        friend std::ostream& operator<<(std::ostream& out, const Client& client) {
            out << "(" << client.arrivalTime << ", " << client.leavingTime << ")";
            return out;
        }
};

template <typename T>
struct MergeSortComparator {
        bool operator()(const T& c1, const T& c2) const {
            if (c1.leavingTime == c2.leavingTime) {
                return c1.arrivalTime < c2.arrivalTime;
            } else {
                return c1.leavingTime < c2.leavingTime;
            }
        }
};

template <typename T, typename Comparator>
Array<T> merge(const Array<T>& arr1, const Array<T>& arr2, Comparator comp) {
    Array<T> result;
    int i = 0;
    int j = 0;
    int n1 = arr1.getSize();
    int n2 = arr2.getSize();

    while (i < n1 && j < n2) {
        if (comp(arr1[i], arr2[j])) {
            result.add(arr1[i]);
            ++i;
        } else {
            result.add(arr2[j]);
            ++j;
        }
    }

    while (i < n1) {
        result.add(arr1[i]);
        ++i;
    }
    while (j < n2) {
        result.add(arr2[j]);
        ++j;
    }

    return result;
}

// Рекурсивная сортировка слиянием
template <typename T, typename Comparator>
Array<T> mergeSort(const Array<T>& arr, Comparator comp) {
    int size = arr.getSize();
    if (size <= 1) {
        return arr;
    }

    int mid = size / 2;
    Array<T> left;
    Array<T> right;

    for (int i = 0; i < mid; ++i) {
        left.add(arr[i]);
    }
    for (int i = mid; i < size; ++i) {
        right.add(arr[i]);
    }

    Array<T> sortedLeft = mergeSort(left, comp);
    Array<T> sortedRight = mergeSort(right, comp);

    return merge(sortedLeft, sortedRight, comp);
}

int calculateMinAdvertisements(const Array<Client>& clients) {
    Array<int> ads;

    for (int i = 0; i < clients.getSize(); ++i) {
        int l = clients[i].arrivalTime;
        int r = clients[i].leavingTime;

        int seen = 0;
        for (int j = 0; j < ads.getSize(); ++j) {
            if (ads[j] >= l && ads[j] <= r) {
                ++seen;
            }
        }

        if (seen == 0) {
            // две рекламы: в r-1 и r
            ads.add(r - 1);
            ads.add(r);
        } else if (seen == 1) {
            // только одна реклама в r
            bool exists = false;
            for (int j = 0; j < ads.getSize(); ++j) {
                if (ads[j] == r) {
                    exists = true;
                    break;
                }
            }
            if (!exists)
                ads.add(r);
        }
    }

    return ads.getSize();
}

// int main() {
//     int n;
//     std::cin >> n;

//     Array<Client> arr;
//     for (int i = 0; i < n; ++i) {
//         Client client;
//         std::cin >> client;
//         arr.add(client);
//     }

//     MergeSortComparator<Client> comp;
//     Array<Client> sorted = mergeSort(arr, comp);

//     int result = calculateMinAdvertisements(sorted);
//     std::cout << result << std::endl;

//     return 0;
// }

int main() {
    auto test = [](std::initializer_list<Client> list) {
        Array<Client> arr;
        for (auto& c : list)
            arr.add(c);
        MergeSortComparator<Client> comp;
        Array<Client> sorted = mergeSort(arr, comp);
        int result = calculateMinAdvertisements(sorted);
        return result;
    };

    std::cout << "=== TESTS ===" << std::endl;

    // пример из условия
    std::cout << "Test 1: " << test({{1, 10}, {10, 12}, {1, 10}, {1, 10}, {23, 24}}) << " (ожидается 5)"
              << std::endl;

    // один клиент
    std::cout << "Test 2: " << test({{1, 5}}) << " (ожидается 2)" << std::endl;

    // два полностью совпадающих клиента
    std::cout << "Test 3: " << test({{1, 5}, {1, 5}}) << " (ожидается 2)" << std::endl;

    // клиенты без пересечения
    std::cout << "Test 4: " << test({{1, 2}, {3, 4}, {5, 6}}) << " (ожидается 6)" << std::endl;

    // клиенты, частично перекрывающиеся
    std::cout << "Test 5: " << test({{1, 4}, {2, 5}, {3, 6}}) << " (ожидается 2)" << std::endl;

    // длинные пересечения
    std::cout << "Test 6: " << test({{1, 10}, {2, 9}, {3, 8}, {4, 7}}) << " (ожидается 2)" << std::endl;

    // один уходит в момент прихода другого
    std::cout << "Test 7: " << test({{1, 3}, {3, 5}, {5, 7}}) << " (ожидается 4)" << std::endl;

    // много коротких интервалов внутри большого
    std::cout << "Test 8: " << test({{1, 20}, {5, 6}, {6, 7}, {7, 8}, {8, 9}}) << " (ожидается 5)"
              << std::endl;

    return 0;
}
