/* Matt Smith
 * Professor Watts
 * CS 460 Programming Languages
 * Homework 4
 * 18 November 2015
*/

#include <stdio.h>
#include <stdlib.h>

// C Boolean
typedef int bool;
#define true 1
#define false 0

// Basic struct to represent a binomial
typedef struct Binomial {
        float coefficient;
        float constant;
} Binomial;

// Linear array resizer
Binomial* arrayResize(Binomial* array, unsigned long size, unsigned long* maxSize) {
    unsigned long newMax = *maxSize * 2;
    Binomial* newArray = malloc(sizeof(Binomial) * newMax);
    for (unsigned long i=0; i<size; i++) {
        newArray[i] = array[i];
    }
    free(array);
    *maxSize = newMax;
    return newArray;
}

// qsort comparator for two binomials
bool binomComparator(const void* avoid, const void* bvoid) {
    const Binomial* a = (const Binomial*)avoid;
    const Binomial* b = (const Binomial*)bvoid;
    if (a->coefficient != b->coefficient) {
        return a->coefficient > b->coefficient;
    }
    else {
        return a->constant > b->constant;
    }
}

int main() {
    // Create large dynamic array
    unsigned long binomialMax = 5000000;
    unsigned long binomialSize = 0;
    Binomial* binomials = malloc(sizeof(Binomial) * binomialMax);
    
    // Continually read stdin until EOF
    float coeff;
    float consta;
    char x;
    while (scanf("%f%c%f", &coeff, &x, &consta) != EOF) {
        binomials[binomialSize].coefficient = coeff;
        binomials[binomialSize].constant = consta;
        binomialSize++;
        if (binomialSize >= binomialMax) {
            binomials = arrayResize(binomials, binomialSize, &binomialMax);
        }
    }
    
    // Sort binomials
    //quicksort(binomials, 0, binomialSize-1);
    qsort(binomials, binomialSize, sizeof(Binomial), binomComparator);

    // Print sorted binomials list
    for (unsigned int i=0; i<binomialSize; i++) {
        printf("%0.2fx%+0.2f\n", binomials[i].coefficient, binomials[i].constant);
    }
}