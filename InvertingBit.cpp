/*
1.4 Инвертируйте значение бита в числе N по его номеру K.
Формат входных данных. Число N, номер бита K.
Формат выходных данных. Число с инвертированным битом в десятичном виде.
in    out
25 1  27
25 4   9
*/

#include <iostream>

int InvertingBit(unsigned int num, int position) {
    return num ^ (1 << position);
}

int main() {
    unsigned int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::cout << InvertingBit(n, k) << std::endl;
    return 0;
}
