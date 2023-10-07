#include <iostream>
#include <random>
#include <ctime>
using namespace std;

#define LL long long
#define test_size 100

LL modulo(LL, LL, LL);
bool check_negative_or_positive_1(int, int, LL);
bool MillerRabin_primality_testing(int);
void Is_valid_input(int*);
void print_result(int);

int main(void) {
    int n, i;
    double start, end;
    random_device rd;
    uniform_int_distribution<int> dist(2, INT_MAX);

    start = clock();

    do {
        cout << "Enter a integer n which larger than 1 for Miller-Rabin primality testing(enter -1 to quit): ";
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

bool check_negative_or_positive_1(int n, int times, LL input) {
    int i;

    for (i = 1; i < times; i++) {
        input = (input * input) % n;
        if (input == 1) {
            return false;
        }
        else if (input + 1 == n) {
            return true;
        }
    }
    return false;
}

bool MillerRabin_primality_testing(int n) {
    if (n % 2 == 0) {
        return false;
    }

    int r, d, k, i, tester, x;
    int count_2 = n - 1;
    bool flag;

    k = rand() % 100 + 1;

    for (r = 0; count_2 % 2 != 1; r++) {
        count_2 /= 2;
    }

    d = (n - 1) / pow(2, r);

    for (i = 0; i < k; i++) {
        tester = rand() % (n - 4) + 2;
        x = modulo(tester, d, n);

        if (x == 1 || x == n - 1) {
            continue;
        }

        flag = check_negative_or_positive_1(n, r, x);
        if (!flag) {
            return flag;
        }
    }
    return true;
}

void Is_valid_input(int* n) {
    if (*n == -1 || *n == 0) {
        return;
    }
    else if (*n <= 2) {
        cout << "Invalid input, please enter n which is larger than 2: ";
        cin >> *n;
        Is_valid_input(n);
    }
}

void print_result(int n) {
    if (MillerRabin_primality_testing(n)) {
        cout << n << " is a prime number.";
    }
    else {
        cout << n << " is not a prime number.";
    }
}