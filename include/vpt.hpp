#include <iostream>
#include <stdio.h>
#include <cmath>
#include "knn.hpp"

class Node {

    public:

    int index;
    int len;
    double *data;
    double *vp;
    double *D;
    double mu;
    int parentIndex;
    int leftIndex;
    int rightIndex;


    Node() {

        index = -1;
        len = 0;
        data = NULL;
        vp = NULL;
        D = NULL;
        mu = -1;
        parentIndex = -1;
        leftIndex = -1;
        rightIndex = -1;
    }

    Node(int index,
    int len,
    double *data,
    int parentIndex) {

        this->index = index;
        this->len = len;
        this->data = data;
        this->vp = data;
        D = new double[len];
        util::computeEuclideanDistance(data, vp, D, len, 1, 1);
        this->mu = computeMu();
        this->parentIndex = parentIndex;
        this->leftIndex = -1;
        this->rightIndex = -1;
    }

    double computeMu() {   // TODO FOR MORE DIMENSIONS
    
        double *sortedD = new double[len];
        memcpy(sortedD, D, len * sizeof(double));
        qsort(sortedD, len, sizeof(double), util::compare);             // TODO more efficient
        mu = sortedD[(len - 1) / 2];
        //prt::rowMajor(sortedD, 1, len);
        delete[] sortedD;
        return mu;
    }
};  // END OF NODE CLASS

double* computeLeftChild(Node *parent) {
    
    int _len = (parent->len + 1) / 2;
    double *_data = new double[_len];
    int idx = 0;
    for (int i = 0; i < parent->len; i++) {
        if (parent->D[i] <= parent->mu)
            _data[idx++] = parent->data[i];
    }
    //std::cout << "Left child:\t"; 
    //prt::rowMajor(_data, 1, _len);
    return _data;
}

double* computeRightChild(Node *parent) {
    
    int _len = parent->len - ((parent->len + 1) / 2);
    double *_data = new double[_len];
    int idx = 0;
    for (int i = 0; i < parent->len; i++) {
        if (parent->D[i] > parent->mu)
            _data[idx++] = parent->data[i];
    }
    //std::cout << "Right child:\t"; 
    //prt::rowMajor(_data, 1, _len);  
    return _data;
}


class VPT {

    public:

    Node root;
    std::vector<Node> tree;
    
    VPT() {
        //std::cout << "VPT constructed\n";
    }

    VPT(Node root) {
        this->root = root;
        tree.push_back(this->root);
    }

    void createVPT() {

        int len = -1;
        int maxIdx = 2 * root.len - 1;
        // std::cout << "Max Idx = " << maxIdx << std::endl; 
        for (int i = 0; i < maxIdx; i++) {
            len = tree[i].len;
            if (len > 1) {
                tree[i].leftIndex = tree.size();
                tree[i].rightIndex = tree.size() + 1;
                createChildren(tree[i], tree[i].len);
            }
        }
    }

    void createChildren(Node curNode, int len) {

        int leftLen = (curNode.len + 1) / 2;
        int rightLen = curNode.len - leftLen;
        Node _left = Node(tree.size(), leftLen, computeLeftChild(&curNode), curNode.index);
        tree.push_back(_left);
        Node _right = Node(tree.size(), rightLen, computeRightChild(&curNode), curNode.index);
        tree.push_back(_right);
    }

    int searchLeaf(double point) {                      // return the node index of the leaf that contains the point

        int curNodeIndex = 0;
        int _leftIdx;
        int _rightIdx;
        while ( !isLeaf(curNodeIndex) ) {
            _leftIdx = tree[curNodeIndex].leftIndex;
            _rightIdx = tree[curNodeIndex].rightIndex; 
            if ( belongsToNode(point, tree[_leftIdx]) )
                curNodeIndex = _leftIdx;
            else
                curNodeIndex = _rightIdx; 
        }
        return tree[curNodeIndex].index;
    }

    bool belongsToNode(double point, Node curNode) {

        for (int i = 0; i < curNode.len; i++) {
            if (curNode.data[i] == point)
                return true;
        }
        return false;
    }

    bool isLeaf(int nodeIdx) {
        if (tree[nodeIdx].leftIndex == -1)
            return true;
        else 
            return false;
    }

    int moveUp(int curNodeIndex) {
        return tree[curNodeIndex].parentIndex;
    }

    int moveLeft(int curNodeIndex) {
        return tree[curNodeIndex].leftIndex;
    }

    int moveRight(int curNodeIndex) {
        return tree[curNodeIndex].rightIndex;
    }

    void vptKnn(knnresult res, double* y, int leafIndex, int displacement, int m, int d, int k) {

        int curNodeIndex = leafIndex;
        int curNodeLen = tree[curNodeIndex].len;
        double* tau = &res.ndist[k - 1];
        // std::cout << *tau << std::endl;
        kNN(res, tree[curNodeIndex].data, y, displacement, curNodeLen, m, d, k);

        // if ( isLeftChild(curNodeIndex) ) {
        //     if ( checkOutsideIntersection(*tree[curNodeIndex].vp, *y, tree[curNodeIndex].mu, *tau) ) {
        //         std::cout << "Outside intersection" << std::endl;
        //         curNodeIndex = moveRight(curNodeIndex);
        //         curNodeLen = tree[curNodeIndex].len;
        //         subtreeKnn(res, *y, displacement, m, d, k, curNodeIndex, *tau);
        //     }  
        // }
        // else if ( isRightChild(curNodeIndex) ) {
        //     if ( checkInsideIntersection(*tree[curNodeIndex].vp, *y, tree[curNodeIndex].mu, *tau) ) {
        //         std::cout << "Inside intersection" << std::endl;
        //         curNodeIndex = moveLeft(curNodeIndex);
        //         curNodeLen = tree[curNodeIndex].len;
        //         subtreeKnn(res, *y, displacement, m, d, k, curNodeIndex, *tau);
        //     }
        // }
        // // kNN(res, tree[curNodeIndex].data, y, displacement, curNodeLen, m, d, k);  
    }

    // void subtreeKnn(knnresult res, double y, int displacement, int m, int d, int k, int curNodeIndex, double tau) {

    //     curNodeIndex = moveRight(curNodeIndex);
    //     while ( !isLeaf(curNodeIndex) ) {
    //         if ( checkInsideIntersection(*tree[curNodeIndex].vp, y, tree[curNodeIndex].mu, tau) ) {
    //             curNodeIndex = moveLeft(curNodeIndex);
    //         }
    //         else if ( checkOutsideIntersection(*tree[curNodeIndex].vp, y, tree[curNodeIndex].mu, tau) ) {
    //             curNodeIndex = moveRight(curNodeIndex);
    //         }
    //     }
    //     vptKnn(res, &y, curNodeIndex, displacement, m, d, k);
    // }

    bool checkInsideIntersection(double vp, double y, double mu, double tau) {

        double dist;
        util::computeEuclideanDistance(&vp, &y, &dist, 1, 1, 1);
        if (dist < mu + tau)
            return true;
        
        return false;
    }

    bool checkOutsideIntersection(double vp, double y, double mu, double tau) {

        double dist;
        util::computeEuclideanDistance(&vp, &y, &dist, 1, 1, 1);
        if (dist > mu - tau)
            return true;
        
        return false;
    }

    bool isLeftChild(int curNodeIndex) {

        if (curNodeIndex == 0)
            return false;
        if (tree[tree[curNodeIndex].parentIndex].leftIndex == curNodeIndex)
            return true;
        
        return false;
    }

    bool isRightChild(int curNodeIndex) {
        
        if (curNodeIndex == 0)
            return false;
        if (tree[tree[curNodeIndex].parentIndex].rightIndex == curNodeIndex)
            return true;
        
        return false;
    }
};  // END OF VPT CLASS