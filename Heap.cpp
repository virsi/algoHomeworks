/*
4.3 Планировщик процессов
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется:
приоритетом P
временем, которое он уже отработал t
временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.
Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
Формат выходных данных: Кол-во переключений процессора.
in      out
3       18
1 10
1 5
2 5
*/

#include <functional>
#include <stdexcept>
#include <iostream>

template<typename T, typename Comparator = std::less<T>>
class Heap {
public:
    Heap(Comparator comp = Comparator()) : comparator(comp), _size(0), _capacity(0), data(nullptr) {}

    Heap(const Heap& other) : comparator(other.comparator), _size(other._size), _capacity(other._capacity) {
        if (_capacity) {
            data = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) data[i] = other.data[i];
        } else {
            data = nullptr;
        }
    }

    Heap& operator=(const Heap& other) {
        Heap temp(other);
        std::swap(comparator, temp.comparator);
        std::swap(data, temp.data);
        std::swap(_size, temp._size);
        std::swap(_capacity, temp._capacity);
        return *this;
    }

    ~Heap() {
        clear();
    }

    bool empty() const noexcept { return _size == 0; }
    size_t size() const noexcept { return _size; }

    const T& top() const {
        if (empty()) throw std::out_of_range("Heap is empty");
        return data[0];
    }

    void push(const T& value) {
        ensure_capacity_for_push();
        data[_size] = value;
        sift_up(_size);
        ++_size;
    }

    void push(T&& value) {
        ensure_capacity_for_push();
        data[_size] = value;
        sift_up(_size);
        ++_size;
    }

    void pop() {
        if (empty()) throw std::out_of_range("Heap is empty");
        if (_size > 1) {
            data[0] = std::move(data[_size - 1]);
        }
        --_size;
        if (!empty()) sift_down(0);
    }

    void clear() noexcept {
        delete[] data;
        data = nullptr;
        _size = 0;
        _capacity = 0;
    }

private:
    Comparator comparator;
    T* data;
    size_t _size;
    size_t _capacity;

    void ensure_capacity_for_push() {
        if (_size < _capacity) return;
        size_t new_capacity = (_capacity == 0) ? 4 : _capacity * 2;
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < _size; ++i) new_data[i] = std::move(data[i]);
        delete[] data;
        data = new_data;
        _capacity = new_capacity;
    }

    void sift_up(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) / 2;
            if (!comparator(data[idx], data[parent])) break;
            std::swap(data[idx], data[parent]);
            idx = parent;
        }
    }

    void sift_down(size_t idx) {
        while (true) {
            size_t left = idx * 2 + 1;
            size_t right = idx * 2 + 2;
            size_t best = idx;
            if (left < _size && comparator(data[left], data[best])) best = left;
            if (right < _size && comparator(data[right], data[best])) best = right;
            if (best == idx) break;
            std::swap(data[idx], data[best]);
            idx = best;
        }
    }
};

struct Technux {
    Technux() : P(0), t(0), T(0) {}
    ~Technux() {}

    long long P, t, T;

    long long getValue() const {
        return P * (t + 1);
    }

    friend std::istream& operator>>(std::istream &input, Technux &technux) {
        input >> technux.P >> technux.T;
        technux.t = 0;
        return input;
    }
};

struct TechnuxComparator {
    bool operator()(const Technux &l, const Technux &r) const {
        return l.getValue() < r.getValue();
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) return 0;

    TechnuxComparator cmp;
    Heap<Technux, TechnuxComparator> heap(cmp);

    for (int i = 0; i < n; ++i) {
        Technux technux;
        std::cin >> technux;
        heap.push(technux);
    }

    long long switches = 0;

    while (!heap.empty()) {
        Technux current = heap.top();
        heap.pop();

        current.t += current.P;
        ++switches;

        if (current.t < current.T) {
            heap.push(std::move(current));
        }
    }

    std::cout << switches << std::endl;
    return 0;
}
