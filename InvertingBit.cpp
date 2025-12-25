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
