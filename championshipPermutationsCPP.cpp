#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// https://www.geeksforgeeks.org/multithreading-in-cpp/
// auto filter out anyone out of championship contention

int SCORE_SYSTEM[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1, 0};
const int NUM_PLACES = sizeof(SCORE_SYSTEM) / sizeof(SCORE_SYSTEM[0]);

const int NUM_DRIVERS = 3;
const int NUM_DIVS = 10;

const int DRIVERX_POINTS[NUM_DIVS][NUM_DRIVERS] =   {
                                                        {134, 120,  79}, {110,  92,  82}, {150,  80,  78}, {118,  90,  85}, {106, 100,  84},
                                                        {118, 107,  94}, {172, 162, 117}, {141, 105,  81}, {129, 125,  88}, {  0,   0,   0}
                                                    };

long long int factorial(int num);
int nPr(int n, int r);


int perms_count = 0;
int one_race_perms_count = nPr(NUM_PLACES, NUM_DRIVERS) * NUM_PLACES;
int** one_race_perms;


int giveFastestLap(int points) {
    if (points > 0) {
        return points + 1;
    } else {
        return points;
    }
}

void recordPermutation(int a[], int n) {

    int fastestLap = 0;

    // calculate NUM_PLACES for the same set (for a fastest lap in each place)
    for (int j = 0; j < NUM_PLACES; j++) {

        // add one permutation of race results
        for (int i=0; i<n; i++) {

            if (j < NUM_DRIVERS && i == j && a[i] > 0) {
                fastestLap = 1;
            } else {
                fastestLap = 0;
            }

            one_race_perms[perms_count][i] = a[i] + fastestLap;
        }
        perms_count += 1;
    }
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
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


    // initialize one_race_perms array
    one_race_perms = new int*[one_race_perms_count];
    for (int i = 0; i < one_race_perms_count; i++) {
        one_race_perms[i] = new int[NUM_DRIVERS];
    }

    cout << "There are " << one_race_perms_count << " permutations of one race (with the given inputs)" << endl;

    // no need to recalculate one_race_perms if the number of drivers is constant
    perms_count = 0;
    kCombination(SCORE_SYSTEM, NUM_PLACES, NUM_DRIVERS);

    for (int z = 0; z < NUM_DIVS; z++) {

        unsigned long long int total = 0;
        unsigned long long int driver_wins[NUM_DRIVERS] = {0};
        unsigned long long int draws = 0;

        auto start = high_resolution_clock::now();

        for (int i = 0; i < one_race_perms_count; i++) {
            for (int j = 0; j < one_race_perms_count; j++) {

                int driver_points[NUM_DRIVERS] = {0};

                for(int a = 0; a < NUM_DRIVERS; a++) {
                    int points = DRIVERX_POINTS[z][a] + one_race_perms[i][a] + one_race_perms[j][a];


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

        auto stop = high_resolution_clock::now();

        cout << "------------------------" << z+1 << "------------------------" << endl;
        for (int i = 0; i < NUM_DRIVERS; i++) {
            cout << driver_wins[i] << '/' << total << '(' << setprecision(4) << ((float) driver_wins[i]/ (float) total) * 100.0 << "%)\n";
        }
        cout << draws << '/' << total << '(' << setprecision(4) << ((float) draws/ (float) total) * 100.0 << "%)\n";
        cout << total << '/' << total << '(' << setprecision(4) << ((float) total/ (float) total) * 100.0 << "%)\n";

        auto duration = duration_cast<microseconds>(stop - start);
        double durationInSeconds = duration.count()/1000000.0; 
        cout << "completed in " << durationInSeconds << setprecision(7) << " seconds" << endl;

        cout << "~" << int(total/durationInSeconds) << " permutations/s" << endl;
    }

    return 0;
}



long long int factorial(int num) {
    long long int fact = 1;
    for (int i = 1; i < num+1; i++) {
        fact = fact * i;
    }

    return fact;
}

int nPr(int n, int r) {
    return (int) (factorial(n) / factorial(n-r));
}