/*
    Author:     Daniel Foreacre
    Date:       Nov 18, 2021
    Lab:        08
    Desc:       Program to use MPI to distribute calculation of an integral using the trapezoid to
                all nodes defined in Hosts.txt, then collect and report the result.
*/

/*********************************************************************************************/
// System includes
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <functional>

#include <mpi.h>

/*********************************************************************************************/
// Function prototypes

double
integrateTrap(std::function<double(double)> f, double endpoints[], unsigned n, int r, int p);

/*********************************************************************************************/
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int numProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double endpoints[2];
    unsigned n;
    if (rank == 0)
    {
        std::cout << "a ==> ";
        std::cin >> endpoints[0];
        std::cout << "b ==> ";
        std::cin >> endpoints[1];
        std::cout << "n ==> ";
        std::cin >> n;
    }

    MPI_Bcast(&endpoints, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    double localArea = integrateTrap([](double x){return x * x;}, endpoints, n, rank, numProcs);
    double sumArea = 0;

    MPI_Reduce(&localArea, &sumArea, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        std::cout << std::fixed << "With n = " << n << " trapezoids, our estimate\n  of the integral from "
                  << std::setprecision(2) << endpoints[0] << " to " << endpoints[1] << " = " << std::setprecision(5) << sumArea << std::endl;

    MPI_Finalize();

    return EXIT_SUCCESS;
}

double
integrateTrap(std::function<double(double)> f, double endpoints[], unsigned n, int r, int p)
{
    // Calculate height
    double h = (endpoints[1] - endpoints[0]) / n;
    double area = 0;
    // Determine start and end trapezoid for this process, based on rank #
    unsigned startTrap = (r * n) / p,
             endTrap = ((r + 1) * n) / p;
    // Loop through trapezoids to calculate areas
    for (unsigned t = startTrap; t < endTrap; ++t)
        // For x1: number of trapezoid times height; For x2: number of next trapezoid times height
        area += h * ((f(t * h) + f((t + 1) * h)) / 2);
    return area;
}
