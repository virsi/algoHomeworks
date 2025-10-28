/*
Дано множество целых чисел из [0..10^9] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль
Требования: к дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

6_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition реализуйте
методом прохода двумя итераторами от начала массива к концу.

In                     Out
10                     2
1 2 3 4 5 6 7 8 9 10   6
                       10
*/

#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>

int RandomPivot(int left, int right) {
    if (left > right)
        return left;
    return left + rand() % (right - left + 1);
}

template <typename T, typename Comparator>
int Partition(T* array, int left, int right, Comparator cmp = Comparator()) {
    if (left > right)
        return left;

    int pivot_index = RandomPivot(left, right);
    std::swap(array[pivot_index], array[right]);
    T pivot_value = array[right];

    int i = left - 1;

    for (int j = left; j < right; ++j) {
        if (cmp(array[j], pivot_value)) {
            i++;
            std::swap(array[i], array[j]);
        }
    }
    std::swap(array[i + 1], array[right]);
    return i + 1;
}

template <typename T, typename Comparator = std::less<T>>
T KthStatistic(T* array, int k, int left, int right, Comparator cmp = Comparator()) {
    if (k < left || k > right) {
        throw std::out_of_range(
            "k is out of the bounds of the array segment [left, right] in KthStatistic.");
    }

    while (true) {
        int pivotPosition = Partition(array, left, right, cmp);

        if (pivotPosition == k) {
            return array[pivotPosition];
        } else if (pivotPosition > k) {
            right = pivotPosition - 1;
        } else {
            left = pivotPosition + 1;
        }
    }
}

int main() {
    int n = 0;
    if (!(std::cin >> n) || n <= 0)
        return 0;

    int* array = new (std::nothrow) int[n];
    if (array == nullptr) {
        std::cerr << "Memory allocation failed.\n";
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> array[i])) {
            delete[] array;
            return 1;
        }
    }

    int index_10 = n / 10;
    int index_50 = n / 2;
    int index_90 = n - (n / 10);

    index_10 = std::max(0, std::min(n - 1, index_10));
    index_50 = std::max(0, std::min(n - 1, index_50));
    index_90 = std::max(0, std::min(n - 1, index_90));

    int p_10 = KthStatistic(array, index_10, 0, n - 1);
    int median = KthStatistic(array, index_50, 0, n - 1);
    int p_90 = KthStatistic(array, index_90, 0, n - 1);

    std::cout << p_10 << "\n";
    std::cout << median << "\n";
    std::cout << p_90 << "\n";

    delete[] array;
    return 0;
}
