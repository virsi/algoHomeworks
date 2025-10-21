/*
2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)).
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для
бинарного поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
Формат входных данных.
В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.
in         out
2 1        1
1 2
2

4 3        1 3 0
2 4 5 7
4 6 1
*/

#include <iostream>

int BinarySearch(int* arr, int left, int right, int target) {
    int result = right + 1;  // По умолчанию - за пределами массива

    while (left <= right) {
        int mid = left + (right - left) / 2;  // т.к. left + right может вызвать переполнение int

        if (arr[mid] >= target) {
            result = mid;     // Нашли кандидата
            right = mid - 1;  // Ищем более левый подходящий элемент
        } else {
            left = mid + 1;
        }
    }
    return result;
}

int ExponentialSearch(int* arr, int size, int target) {
    if (size == 0)
        return 0;

    if (arr[0] >= target)
        return 0;

    if (arr[size - 1] < target)
        return size;

    int i = 1;
    while (i < size && arr[i] < target) {
        i *= 2;
    }

    int left = i / 2;
    int right = std::min(i, size - 1);

    return BinarySearch(arr, left, right, target);
}

void Run() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    int* arrayA = new int[n];
    int* arrayB = new int[m];

    for (int i = 0; i < n; ++i) {
        std::cin >> arrayA[i];
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> arrayB[i];
    }

    for (int i = 0; i < m; ++i) {
        int result = ExponentialSearch(arrayA, n, arrayB[i]);
        std::cout << result;
        if (i < m - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    delete[] arrayA;
    delete[] arrayB;
}

int main() {
    Run();
    return 0;
}
