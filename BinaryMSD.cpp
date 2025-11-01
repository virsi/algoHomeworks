/*
7_3. Binary MSD для long long.
Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 106. Отсортировать массив
методом MSD по битам (бинарный QuickSort).

*/

#include <iostream>

long long getBit(unsigned long long x, int bit) {
    return (x >> bit) & 1;
}

void binaryMSDSort(unsigned long long* arr, int left, int right, int bit) {
    if (left >= right || bit < 0) {
        return;
    }

    int i = left;
    int j = right;

    while (i <= j) {
        while (i <= j && getBit(arr[i], bit) == 0) {
            ++i;
        }
        while (i <= j && getBit(arr[j], bit) == 1) {
            --j;
        }
        if (i < j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    binaryMSDSort(arr, left, j, bit - 1);
    binaryMSDSort(arr, i, right, bit - 1);
}

int main() {
    int n = 0;
    std::cin >> n;
    unsigned long long* arr = new unsigned long long[n];
    for (int i = 0; i < n; ++i) {
        unsigned long long x = 0;
        std::cin >> x;
        arr[i] = x;
    }

    binaryMSDSort(arr, 0, n - 1, 63);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    delete[] arr;
    return 0;
}
