/*
    Filename    : Lab04.cc
    Author      : Daniel Foreacre
    Course      : CSCI 476-01
    Date        : 9/30/21
    Assignment  : 4
    Description : Program to practice utilizing the parallel vector operations of
                  a SIMD system to add vectors.
*/

/****************************************************************/
// System includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <immintrin.h>

/****************************************************************/
// Local includes
#include "Timer.hpp"

/****************************************************************/
// Function Prototypes / Constants
size_t const RAND_SEED = 0;

std::vector<int>
getRandomVector(unsigned n);

std::vector<int>
sumVectorSimd(std::vector<int> const &x, std::vector<int> const &y);

std::vector<int>
sumVector(std::vector<int> const &x, std::vector<int> const &y);

void
getChecksum(std::vector<int> &z);

/****************************************************************/
int main(int argc, char* argv[])
{
    // Get size of array from user
    unsigned n;
    std::cout << "N ==> ";
    std::cin >> n;

    // Generate two arrays of size n
    std::vector<int> x = getRandomVector(n);
    std::vector<int> y = getRandomVector(n);

    std::vector<int> z(n);

    // Get sum of x + y using SIMD
    Timer<> t;
    z = sumVectorSimd(x, y);
    t.stop();

    getChecksum(z);
    std::cout << std::fixed;
    std::cout << "\nSIMD sum:  " << z[0];
    std::cout << "\nSIMD time: " << std::setprecision(2) << t.getElapsedMs();

    // Get sum of x + y using scalar logic
    t.start();
    z = sumVector(x, y);
    t.stop();

    getChecksum(z);
    std::cout << "\n\nSerial sum:  " << z[0];
    std::cout << "\nSerial time: " << std::setprecision(2) << t.getElapsedMs() << std::endl;

    return EXIT_SUCCESS;
}

/* Create a vector of random integers in the range [0, 4] following a uniform distribution
   @param   n       Size of the vector
   @return  The vector containing random integers */
std::vector<int>
getRandomVector(unsigned n)
{
    std::vector<int> v(n);
    std::generate(v.begin (), v.end (), [=]
    {
        static std::minstd_rand gen (RAND_SEED);
        static std::uniform_int_distribution<> dis (0, 4);
        return dis (gen);
    });
    return v;
}

/* Computes sum of two vectors using SIMD parallelism
   @param   x       Constant reference to first vector
   @param   y       Constant reference to second vector
   @return  Sum of x and y as a vector */
std::vector<int>
sumVectorSimd(std::vector<int> const &x, std::vector<int> const &y)
{
    std::vector<int> z(x.size());

    // Split the arrays into groups of 256 bits for computation
    for (unsigned i = 0; i < x.size(); i += 8)
    {
        __m256i a = _mm256_loadu_si256((__m256i *)&x[i]);
        __m256i b = _mm256_loadu_si256((__m256i *)&y[i]);
        __m256i c = _mm256_add_epi32(a, b);
        _mm256_storeu_si256((__m256i *)&z[i], c);
    }
    return z;
}

/* Computes sum of two vectors using scalar logic
   @param   x       Constant reference to first vector
   @param   y       Constant reference to second vector
   @return  Sum of x and y as a vector */
std::vector<int>
sumVector(std::vector<int> const &x, std::vector<int> const &y)
{
    std::vector<int> z(x.size());
    std::transform (x.begin(), x.end(), y.begin(), z.begin(), std::plus<int>());
    return z;
}

/* Computes checksum by summing the first 10,000 values of given vector and placing
   the results in index 0 of the vector.
   @param   z       Reference to a vector to sum. */
void
getChecksum(std::vector<int> &z)
{
    // Limit summation to first 10000 elements
    unsigned size = z.size();
    if (size >= 10000)
        size = 10000;
    
    // Split the array into groups of 256 bits for summation
    for (unsigned i = 8; i < size; i += 8)
    {
        __m256i a = _mm256_loadu_si256((__m256i *)&z[0]);
        __m256i b = _mm256_loadu_si256((__m256i *)&z[i]);
        __m256i c = _mm256_add_epi32(a, b);
        _mm256_storeu_si256((__m256i *)&z[0], c);
    }

    // Sum the first 8 integers of the vector
    for (unsigned i = 1; i < 8; ++i)
        z[0] += z[i];
}

/* (Very small) Portion of generated assembly code:
    .L129:
	vmovdqu	(%rcx,%rdx,4), %ymm0
	movq	0(%r13), %rax
	vpaddd	(%rax,%rdx,4), %ymm0, %ymm0
	movq	(%r12), %rax
	vmovdqu	%ymm0, (%rax,%rdx,4)

    The vmovdqu command is to move unaligned packed integer values. It takes the vector in memory
    and stores it in the vector register %ymm0. The next vector operation is vpaddd which adds
    the packed doubleword integers from %ymm0, adds it to itself and stores the result in
    the %rax and %rdx registers. Finally it uses vmovdqu again to move that result back into
    register %ymm0. Because of it using the same register, %ymm0, I think that this portion
    of the assembly code is for the checksum calculation being folded back into the z vector.
*/
