#include <iostream>
#include <random>
#include <ctime>
using namespace std;

#define test_size 100

bool Basic_prime_testing(int);
void Is_valid_input(int*);
void print_result(int);

int main(void) {
    int n, i;
    double start, end;
    random_device rd;
    uniform_int_distribution<int> dist(2, INT_MAX);

    start = clock();

    do {
        cout << "Enter a integer n which larger than 2 for basic prime testing(enter -1 to quit, 0 to create 100 random n): ";
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

bool Basic_prime_testing(int n) {
    int i;

    for (i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
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
    if (Basic_prime_testing(n)) {
        cout << n << " is a prime number.";
    }
    else {
        cout << n << " is not a prime number.";
    }
}
