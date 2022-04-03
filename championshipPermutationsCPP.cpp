#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

// https://www.geeksforgeeks.org/multithreading-in-cpp/

int SCORE_SYSTEM[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1, 0};
const int NUM_PLACES = sizeof(SCORE_SYSTEM) / sizeof(SCORE_SYSTEM[0]);

const int DRIVERX_POINTS[] = {134, 120, 79};
const int NUM_DRIVERS = sizeof(DRIVERX_POINTS) / sizeof(DRIVERX_POINTS[0]);

int factorial(int num);
int nPr(int n, int r);


int perms_count = 0;
int one_race_perms_count = nPr(NUM_PLACES, NUM_DRIVERS);
int** one_race_perms;


void recordPermutation(int a[], int n) {

    for (int i=0; i<n; i++) {
        one_race_perms[perms_count][i] = a[i];
    }
    perms_count += 1;
}

// Generating permutation using Heap Algorithm
void heapPermutation(int a[], int n, int size) {
    // if size becomes 1 then prints the obtained permutation
    if (size == 1) {
        recordPermutation(a, n);
        return;
    }

    for (int i=0; i<size; i++) {
        heapPermutation(a, n, size-1);
        // if size is odd, swap first and last element, otherwise swap ith and last element
        swap(a[size%2 == 1 ? 0 : i], a[size-1]);
    }
}

void doKCombination(int a[], int n, int p[], int k, int size, int start) {
    int picked[size + 1];
    for (int i = 0; i < size; ++i) picked[i] = p[i];
    if (size == k) {
        // We got a valid combination, use the heap permutation algorithm to generate all permutations out of it.
        heapPermutation(p, k, k);
    } else {
        if (start < n) {
            doKCombination(a, n, picked, k, size, start + 1);
            picked[size] = a[start];
            doKCombination(a, n, picked, k, size + 1, start + 1);
        }
    }
}

// Generate combination of k elements out of a set of n
void kCombination(int a[], int n, int k) {
    doKCombination(a, n, nullptr, k, 0, 0);
}

int main() {

    // initialize one_race_perms array
    one_race_perms = new int*[one_race_perms_count];
    for (int i = 0; i < one_race_perms_count; i++) {
        one_race_perms[i] = new int[NUM_DRIVERS];
    }

    kCombination(SCORE_SYSTEM, NUM_PLACES, NUM_DRIVERS);

    unsigned long long int total = 0;
    unsigned long long int driver_wins[] = {0, 0, 0};
    unsigned long long int draws = 0;

    for (int i = 0; i < one_race_perms_count; i++) {
        for (int j = 0; j < one_race_perms_count; j++) {
            for (int k = 0; k < one_race_perms_count; k++) {

                for (int l = 0; l < NUM_PLACES; l++) {
                    int driver_points[] = {0, 0, 0};

                    for(int a = 0; a < NUM_DRIVERS; a++) {
                        int points = DRIVERX_POINTS[a] + one_race_perms[i][a] + one_race_perms[j][a] + one_race_perms[k][a];

                        if (l > 10 or l == a) {
                            points += 1;
                        }

                        driver_points[a] = points;
                    }

                    // find max
                    int max = driver_points[0];
                    int max_index = 0;
                    for (int a = 1; a < NUM_DRIVERS; a++) {
                        if (driver_points[a] > max) {
                            max = driver_points[a];
                            max_index = a;
                        }
                    }

                    //count max
                    int max_count = 0;
                    for (int a = 0; a < NUM_DRIVERS; a++) {
                        if (driver_points[a] == max) {
                            max_count += 1;
                        }
                    }

                    if (max_count == 1) {
                        driver_wins[max_index] += 1;
                    } else {
                        draws += 1;
                    }
                    total += 1;
                }
            }
        }
    }

    for (int i = 0; i < NUM_DRIVERS; i++) {
        cout << driver_wins[i] << '/' << total << '(' << setprecision(4) << ((float) driver_wins[i]/ (float) total) * 100.0 << "%)\n";
    }
    cout << draws << '/' << total << '(' << setprecision(4) << ((float) draws/ (float) total) * 100.0 << "%)\n";
    cout << total << '/' << total << '(' << setprecision(4) << ((float) total/ (float) total) * 100.0 << "%)\n";

    return 0;
}



int factorial(int num) {
    int fact = 1;
    for (int i = 1; i < num+1; i++) {
        fact = fact * i;
    }

    return fact;
}

int nPr(int n, int r) {
    return (int) (factorial(n) / factorial(n-r));
}