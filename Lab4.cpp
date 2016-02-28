#include <thread>
#include <iostream>
#include <cmath>
#include <mutex>
#include <vector>
#include <atomic>
using namespace std;

atomic_flag lock1 = ATOMIC_FLAG_INIT;

long int n = 25, k = 100, maxThs = 10;
double SUMM = 0;
int totalThs = 0;

void func(int i)
{
    double s = 0;
    for (int j = 1; j<k+1; j++) {
        s += 1/(i*i+log(5*j)+8);
    }
    while (lock1.test_and_set()) { }
    SUMM += s;
    lock1.clear();

    if (i+maxThs<n+1) {
        func(i+maxThs);
    }// настоятельно рекомендуется сделать ре рекурсией а циклом на всю функцию с таким же условием
    // рекурсия - плохо при большом количестве итераций
}
// можно многопоточность организовать средствами WinApi или c++14 - я выбрал второй вариант
int main()
{
    vector<thread> thrs;
    int i = 0;
    cout << "Введите N и K " << endl;
    cin >> n >> k;
    if (n<maxThs) {
        maxThs = n;
    }

    while (i<n+1 && totalThs<maxThs) {
        thrs.emplace_back(func, i+1);
        totalThs++;
        i++;
    }

    for (int k = 0; k<maxThs; k++) thrs[k].join();

    cout << "сумма ряда : " << SUMM << endl;

    cout << "использовано " << totalThs << " потоков" << endl;

    return 0;

}
