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
#include <new>

struct Client {
    int arrivalTime = 0;
    int leavingTime = 0;

    friend std::istream& operator>>(std::istream& input, Client& client) {
        input >> client.arrivalTime >> client.leavingTime;
        return input;
    }
};

bool compareClients(const Client& c1, const Client& c2) {
    if (c1.leavingTime == c2.leavingTime) {
        return c1.arrivalTime < c2.arrivalTime;
    }
    return c1.leavingTime < c2.leavingTime;
}

void merge(Client* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    //буферы
    Client* L = new(std::nothrow) Client[n1];
    Client* R = new(std::nothrow) Client[n2];

    if (L == nullptr || R == nullptr) {
        delete[] L;
        delete[] R;
        throw std::bad_alloc();
    }

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (compareClients(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}


void mergeSort(Client* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int calculateMinAdvertisements(Client* clients, int clientCount) {
    if (clientCount == 0) return 0;


    // максимум 2n реклам
    int max_ad_count = clientCount * 2;
    int* ad_times = new(std::nothrow) int[max_ad_count];
    if (ad_times == nullptr) throw std::bad_alloc();
    int ad_size = 0;

    try {
        mergeSort(clients, 0, clientCount - 1);
    } catch (...) {
        delete[] ad_times;
        throw;
    }

    for (int i = 0; i < clientCount; ++i) {
        const Client& currentClient = clients[i];
        int l = currentClient.arrivalTime;
        int r = currentClient.leavingTime;

        int seen_count = 0;
        for (int j = 0; j < ad_size; ++j) {
            int ad_time = ad_times[j];
            if (ad_time >= l && ad_time <= r) {
                seen_count++;
            }
        }

        int needed = 2 - seen_count;

        if (needed > 0) {
            int t1 = r;
            bool t1_exists = false;
            for (int j = 0; j < ad_size; ++j) {
                if (ad_times[j] == t1) {
                    t1_exists = true;
                    break;
                }
            }

            if (!t1_exists) {
                ad_times[ad_size++] = t1;
                needed--;
            }

            // показ 2 (если еще нужен): в r-1
            if (needed > 0) {
                int t2 = r - 1;

                if (t2 >= l) {

                    bool t2_exists = false;
                    for (int j = 0; j < ad_size; ++j) {
                        if (ad_times[j] == t2) {
                            t2_exists = true;
                            break;
                        }
                    }

                    if (!t2_exists) {
                        ad_times[ad_size++] = t2;
                    }
                }
            }
        }
    }

    int result = ad_size;
    delete[] ad_times;
    return result;
}

int main() {
    // ускорение ввода/вывода
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    if (!(std::cin >> n) || n <= 0) {
        return 0;
    }

    Client* clients = new(std::nothrow) Client[n];
    if (clients == nullptr) {
        std::cerr << "Error: Failed to allocate memory for clients.\n";
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> clients[i])) {
            n = i;
            break;
        }
    }

    int result = 0;

    result = calculateMinAdvertisements(clients, n);
    std::cout << result << "\n";

    delete[] clients;
    return 0;
}
