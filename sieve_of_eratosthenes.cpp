// Dylan Lynch
// April 7, 2016
// Implementation of the Sieve of Eratosthenes
// Finds all primes up to a given n and measures the time taken

#include <iostream>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;

// MODIFIES:    cout
int ask_print_preferences();

// REQUIRES:    n >= 2
// EFFECTS:     finds and prints primes up to n
bool* sieve_of_e(const long long max);

// EFFECTS:     Prints either all primes up to n or the last prime below n or
//              the number of primes below n
// MODIFIES:    cout
void print_primes(const int &print_pref, const bool* sieve, const int &n);

int main () {
    // Ask for a number
    long long n;
    cout << "Find primes up to: ";
    cin >> n;

    int print_pref = ask_print_preferences();

    cout << "running..." << flush;

    // Start timer
    clock_t start = clock();

    // Find primes up to that number
    bool* sieve = sieve_of_e(n);

    // Stop timer (don't want to count printing time)
    double duration = (clock() - start) / ((double) CLOCKS_PER_SEC);

    cout << endl;
    print_primes(print_pref, sieve, n);
    cout << "time taken: " << duration << "s";
    cout << " (doesn't include printing time)" << endl;

    delete[] sieve;
}

// FUNCTIONS
int ask_print_preferences() {
    // Ask if user wants to print primes
    // 0 = n, 1 = y, 2 = last, 3 = count
    string input;
    cout << "Print primes? (y/n/last/count): ";
    cin >> input;
    if (input == "y") {
        return 1;
    }
    else if (input == "last") {
        return 2;
    }
    else if (input == "count") {
        return 3;
    }
    return 0;
}

bool* sieve_of_e(const long long max) {
    // sieve[i] true if i is composite
    // Allocate mem on heap because default stack size
    // is too small at around max = 10,000,000
    bool *sieve = new bool[max];

    // Initialize array
    memset(sieve, 0, max + 1);
    sieve[0] = true;    // 0 is not prime
    sieve[1] = true;    // 1 is not prime

    for (int i = 2; i < ceil(sqrt(max + 1)); ++i) {
        if (sieve[i] == false) {
            // Mark off multiples of this prime starting with it's square;
            // any spot less is a multiple of something <i and already marked
            int max_mult = ceil((max / i));   // prevent division comparison in loop
            for (int mult = i; mult <= max_mult; ++mult) {
                sieve[i * mult] = true;
            }
        }
        else {
            // Don't need to mark off multiples, this number is composite and
            // all its multiples have been marked off already
        }
    }
    return sieve;
}

void print_primes(const int &print_pref, const bool* sieve, const int &n) {
    // 0 = n, 1 = y, 2 = last, 3 = count
    if (print_pref == 1) {
        // Print primes
        cout << "Printing primes up to " << n << ":" << endl;
        for (int i = 0; i < n; ++i) {
            if (sieve[i] == false) {
                // Number is prime
                cout << i << endl;
            }
        }
    }
    else if (print_pref == 2) {
        // Print last prime
        cout << "Last prime: ";
        for (int i = n; i >= 0; --i) {
            if (sieve[i] == false) {
                cout << i << endl;
                break;
            }
        }
    }
    else if (print_pref == 3) {
        // Count primes
        int count = 0;
        for (int i = 0; i < n + 1; ++i) {
            if (sieve[i] == false) {
                ++count;
            }
        }
        cout << "Number of primes below " << n << ": " << count << endl;
    }
}
