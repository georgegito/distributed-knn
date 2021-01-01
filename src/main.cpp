#include <iostream>
#include <stdlib.h>

#include "knn.hpp"
#include "utils.hpp"
#include "distributed.hpp"
#include "vpt.hpp"

int main() {

    std::cout << std::endl;
    // const int n = 5;
    // const int d = 1;
    // const int k = 5;

    // // double X[n * d] = {0, 1, 2, 3, 15, 12, 15, 11, 30, 30};
    // // double X[n * d] = {0, 1, 1, 0, 0, 0, 1, 1, 2, 0};

    // double X[n * d] = {0, 10, -10, 20, 3};

    // struct knnresult ans = mpi::distrAllkNN(X, n, d, k);

    double data[]      = {14, 2, 50, 8, 11, 7, 19, 40};
    // double data[]      = {100, 80, 70, 60, 40, 35, 200, 500};
    int k              = 3;
    int d              = 1;
    int len            = 8;
    double queryPoints = 19;
    int m              = 1;
    // Node root          = Node(0, len, data, -1);

    knnresult _ans = knnresult();
    _ans.m         = m;
    _ans.k         = k;
    _ans.nidx      = new int[_ans.m * _ans.k]();
    _ans.ndist     = new double[_ans.m * _ans.k]();

  for (int i = 0; i < _ans.m; i++) {
      for (int j = 0; j < k; j++) {
          _ans.ndist[i * k + j] = D_MAX;
          _ans.nidx[i * k + j]  = -1;
      }
  }

    std::vector<Point> X;

    for (int i = 0; i < len; i += d) {

        double* coords = new double[d];

        for (int j = 0; j < d; j++)
            coords[j] = data[i + j];

        X.push_back(Point(i, coords, d));
    }

    VPT vpt(X);
    vpt.buildTree(0, X.size());
    // std::cout << vpt.buildTree(0, X.size()) << std::endl;

    int cnt = 0;
    for (auto p : vpt._nodes) {
        std::cout << cnt++ << ": " << X[p.vpIndex].coords[0] << "\t"
                  << "mu: " << p.mu << "\t(" << p.leftIndex << ", " << p.rightIndex << ", " << p.parentIndex << ")" << std::endl;
        if (p.points_len) {
            for (int j = 0; j < p.points_len; j++) {
                std::cout << p.points[j].coords[0] << std::endl;
            }
        }
        // std::cout << i.vpIndex << std::endl;
    }
    std::cout << std::endl;

    double* coords = new double[1];
    coords[0] = 41;
    Point p = Point(10, coords, 1);

    // std::cout << std::endl << vpt.leafKNN(p, _ans) << std::endl;

    int leafIndex = vpt.searchLeaf(p, _ans);
    vpt.leafKNN(p, vpt._nodes[leafIndex], _ans);

    int curNodeIndex = vpt.moveUp(leafIndex);
    // curNodeIndex = vpt.moveLeft(curNodeIndex);
    
    vpt.checkInside(p, vpt._nodes[curNodeIndex], _ans.ndist[_ans.k - 1]);
    vpt.checkOutside(p, vpt._nodes[curNodeIndex], _ans.ndist[_ans.k - 1]);
    
    std:: cout << std::endl << "isLeftChild = " << vpt.isLeftChild(curNodeIndex) << std::endl;
    std:: cout << std::endl << "isRightChild = " << vpt.isRightChild(curNodeIndex) << std::endl;

    std::cout << "\nkNN ndist:\t";
    prt::rowMajor(_ans.ndist, 1, _ans.k);
    std::cout << "kNN nidx:\t";
    prt::rowMajor(_ans.nidx, 1, _ans.k);
    std::cout << std::endl;

    // for (auto i : X) {
    //     std::cout << i.coords[0] << std::endl;
    // }

    // std::cout << "mu = " << root.mu << std::endl;
    std::cout << std::endl;

    return 0;
}
