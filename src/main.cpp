#include <iostream>
#include <stdlib.h>

#include "knn.hpp"
#include "utils.hpp"
#include "distributed.hpp"

int main() {

    /* ---------------------------------- DATA ---------------------------------- */

    // double X[n * d] = {3, 3, 5, 1};
    // double Y[m * d] = {0, 1, 2, 5, 4, 1};

    // double Y[m * d] = {1, 3, 9};

    const int n = 5;
    const int d = 2;
    const int k = 1;

    double X[n * d] = {0, 3, 4, 10, 0, 7, 15, 14, 100, 2};

    std::cout << "X matrix: \n";
    prt::rowMajor(X, n, d);
    std::cout << "\n";

    // double X[n * d] = {0, 10, -10, 20, 3};

    /* ----------------------------------- MPI ---------------------------------- */

    struct knnresult res = mpi::distrAllkNN(X, n, d, k);

    /* --------------------------------- PRINTS --------------------------------- */

    // std::cout << "kNN distances: " << std::endl;
    // prt::twoDim(res.ndist, res.m, res.k);
    // prt::twoDim(res.nidx, res.m, res.k);

    // std::cout << std::endl << "kNN indices: " << std::endl;
    // prt::twoDim(res.nidx, m, k);

    // std::cout << std::endl << "Distance matrix: " << std::endl;
    // prt::rowMajor(D, n, m);

    return 0;
}
