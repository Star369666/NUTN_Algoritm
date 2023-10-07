#include <iostream>
#include <random>
#include <ctime>
using namespace std;

#define LL long long
#define test_size 100

LL modulo(LL, LL, LL);
bool Fermat_primality_testing(int);
void Is_valid_input(int*);
void print_result(int);

int main(void) {
    int n, i;
    double start, end;
    random_device rd;
    uniform_int_distribution<int> dist(2, INT_MAX);

    start = clock();

    do {
        cout << "Enter a integer n which larger than 1 for Fermat primality testing(enter -1 to quit, 0 to create 100 random n): ";
        cin >> n;

        Is_valid_input(&n);
        if (n == -1) {
            end = clock();
            cout << "The program run " << (end - start) << "ms";
            return 0;
        }
        else if (n == 0) {
            for (i = 0; i < test_size; i++) {
                n = dist(rd);
                print_result(n);
                cout << "\n";
            }
        }

        print_result(n);
        cout << "\n\n";
    } while (true);
}

LL modulo(LL base, LL exponential, LL n) {
    LL a = 1;
    LL b = base;
    while (exponential > 0) {
        if (exponential % 2 == 1)
            a = (a * b) % n;
        b = (b * b) % n;
        exponential = exponential / 2;
    }
    return a % n;
}

bool Fermat_primality_testing(int n) {
    int i, k, tester;

    k = rand() % 1000 + 1;

    for (i = 0; i < k; i++) {
        tester = rand() % (n - 1) + 1;
        if (modulo(tester, n - 1, n) != 1) {
            return false;
        }
    }
    return true;
}

void Is_valid_input(int* n) {
    if (*n == -1 || *n == 0) {
        return;
    }
    else if (*n <= 1) {
        cout << "Invalid input, please enter n which is larger than 1: ";
        cin >> *n;
        Is_valid_input(n);
    }
}

void print_result(int n) {
    if (Fermat_primality_testing(n)) {
        cout << n << " is a prime number.";
    }
    else {
        cout << n << " is not a prime number.";
    }
}