// Dylan Lynch
// April 7, 2016
// Implementation of the Sieve of Eratosthenes
// Finds all primes up to a given n and measures the time taken

#include <iostream>
#include <cmath>
#include <ctime>
#include <ctype.h>

using namespace std;

// REQUIRES:    n >= 2
// EFFECTS:     finds and prints primes up to n
void find_primes(const long long max, bool print_request);

int main () {
    // Ask for a number
    long long n;
    cout << "Find primes up to: ";
    cin >> n;

    // Ask if user wants to print primes
    bool print_request = false;
    char input;
    cout << "Print primes? (y/n): ";
    cin >> input;
    if (tolower(input) == 'y') {
        print_request = true;
    }

    // Find primes up to that number
    find_primes(n, print_request);
}

void find_primes(const long long max, bool print_request) {
    // Start timer
    clock_t start = clock();

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

    // Stop timer (don't want to count printing time)
    double duration = (clock() - start) / ((double) CLOCKS_PER_SEC);

    if (print_request) {
        // Print primes
        cout << "Printing primes up to " << max << ":" << endl;
        for (int i = 0; i < max; ++i) {
            if (sieve[i] == false) {
                // Number is prime
                cout << i << endl;
            }
        }
    }

    cout << "time taken: " << duration << "s" << endl;
    cout << "(doesn't include printing time)" << endl;

    delete[] sieve;
}
