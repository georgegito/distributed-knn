#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>

#include "knn.hpp"
#include "utils.hpp"
#include "distributed.hpp"
#include "vpt.hpp"

int main()
{

    std::cout << std::endl;
    // const int n = 5;
    // const int d = 1;
    // const int k = 5;

    // // double X[n * d] = {0, 1, 2, 3, 15, 12, 15, 11, 30, 30};
    // // double X[n * d] = {0, 1, 1, 0, 0, 0, 1, 1, 2, 0};

    // double X[n * d] = {0, 10, -10, 20, 3};

    // struct knnresult ans = mpi::distrAllkNN(X, n, d, k);

    // double corpusData[]      = {100, 80, 70, 60, 40, 35, 200, 500};
    std::vector<double> corpusData = {14, 2, 50, 8, 11, 7, 19, 40};
    std::vector<double> queryData  = {8, 100, 10, 52};

    int d = 2;
    int k = 4;

    std::vector<Point> corpus(corpusData.size() / d);
    std::vector<Point> query(queryData.size() / d);

    for (int i = 0; i < corpusData.size() / d; i++)
        corpus[i] = Point(i, &corpusData[i * d], d);

    for (int i = 0; i < queryData.size() / d; i++)
        query[i] = Point(i, &queryData[i * d], d);

    std::cout << "Corpus points:\t";
    prt::points(corpus);
    std::cout << "Query points:\t";
    prt::points(query);

    knnresult ans = knnresult();
    ans.m         = query.size();
    ans.k         = k;
    ans.nidx      = new int[ans.m * ans.k];
    ans.ndist     = new double[ans.m * ans.k];

    std::fill_n(ans.nidx, ans.m * ans.k, D_MAX);
    std::fill_n(ans.ndist, ans.m * ans.k, -1);

    /* ------------------------------ Construct VPT ----------------------------- */

    VPT vpt(corpus);
    vpt.buildTree(0, corpus.size());

    int cnt = 0;
    std::cout << "\nVantage point tree:\n\n";
    for (auto p : vpt._nodes) {
        std::cout << "nodes[" << cnt++ << "]:\tvp = ";
        prt::point(corpus[p.vpIndex]);
        std::cout << "\t\t"
                  << "mu = " << p.mu << "\t\t(" << p.leftIndex << ", " << p.rightIndex << ", " << p.parentIndex << ")"
                  << std::endl;
        if (p.leafPointsLen) {
            for (int j = 0; j < p.leafPointsLen; j++) {
                std::cout << p.leafPoints[j].coords[0] << std::endl;
            }
        }
    }
    std::cout << std::endl;

    /* --------------------------------- VPT kNN -------------------------------- */

    for (auto p : query) {
        vpt.kNN(p, ans, p.index);
    }

    prt::kNN(ans);

    return 0;
}
