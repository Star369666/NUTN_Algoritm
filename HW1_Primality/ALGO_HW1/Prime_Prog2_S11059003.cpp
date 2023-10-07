#include <iostream>
#include <vector>
#include <random>
using namespace std;

void fill_all_multiple_to_1(vector<int>&, int, int);
vector<int> Sieve_of_eratosthenes(int);
void Is_valid_input(int*);
void print_result(vector<int>, int);

int main(void) {
    int n;
    vector<int> result;
    double start, end;
    random_device rd;
    uniform_int_distribution<int> dist(2, sqrt(INT_MAX));

    start = clock();

    do {
        cout << "Enter a integer n which larger than 1 for Sieve of Eratosthenes(enter -1 to quit, 0 to create a random n): ";
        cin >> n;

        Is_valid_input(&n);

        if (n == -1) {
            end = clock();
            cout << "The program run " << (end - start) << "ms";
            return 0;
        }
        else if (n == 0) {
            n = dist(rd);
            result = Sieve_of_eratosthenes(n);
            print_result(result, n);
        }

        result = Sieve_of_eratosthenes(n);
        print_result(result, n);
    } while (true);
}

void fill_all_multiple_to_1(vector<int>& result, int start, int n) {
    int i = 2;

    for (i = 2; start * i <= n; i++) {
        result[start * i - 1] = 1;
    }
}

vector<int> Sieve_of_eratosthenes(int n) {
    vector<int> result(n, 0);
    int i;

    result[0] = 1;
    for (i = 1; i <= sqrt(n); i++) {
        if (result[i] == 0) {
            fill_all_multiple_to_1(result, i + 1, n);
        }
    }
    return result;
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

void print_result(vector<int> result, int n) {
    cout << "These are prime numbers that small than " << n << ": ";

    for (int i = 0; i < n; i++) {
        if (result[i] == 0) {
            cout << i + 1 << ", ";
        }
    }

    cout << "end.\n\n";
}
