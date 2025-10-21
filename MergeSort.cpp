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

        bool contains(T element) const {
            for (int i = 0; i < size; ++i) {
                if (arr[i] == element) {
                    return true;
                }
            }
            return false;
        }

        void addUnique(T element) {
            if (!contains(element)) {
                add(element);
            }
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
            capacity *= 2;
            T* newArr = new T[capacity];
            for (int i = 0; i < size; ++i) {
                newArr[i] = arr[i];
            }

            delete[] arr;
            arr = newArr;
        }
};

struct Client {
    int arrivalTime = 0;
    int leavingTime = 0;
    Client() = default;
    Client(int arrival, int leaving) : arrivalTime(arrival), leavingTime(leaving) {}
    ~Client() = default;

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
    Array<int> ad_times;

    for (int i = 0; i < clients.getSize(); ++i) {
        const Client& currentClient = clients[i];
        int l = currentClient.arrivalTime;
        int r = currentClient.leavingTime;

        int seen_count = 0;

        for (int j = 0; j < ad_times.getSize(); ++j) {
            int ad_time = ad_times[j];
            if (ad_time >= l && ad_time <= r) {
                seen_count++;
            }
        }

        int needed = 2 - seen_count;

        if (needed > 0) {

            //предотвращениe дубликатов
            if (!ad_times.contains(r)) {
                ad_times.add(r);
                needed--;
            }

            if (needed > 0) {
                int t2 = r - 1;

                if (t2 >= l && !ad_times.contains(t2)) {
                    ad_times.add(t2);
                }
            }
        }
    }
    return ad_times.getSize();
}

int main() {
    int n;

    if (!(std::cin >> n))
        return 0;

    Array<Client> arr;
    for (int i = 0; i < n; ++i) {
        Client client;
        if (!(std::cin >> client))
            break;
        arr.add(client);
    }

    MergeSortComparator<Client> comp;
    Array<Client> sorted = mergeSort(arr, comp);

    int result = calculateMinAdvertisements(sorted);
    std::cout << result << std::endl;

    return 0;
}
