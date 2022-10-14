#pragma once
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <queue>
#include <limits>
#include "TItemTypes.h"

namespace TDataTypes
{

    struct Node
    {
        int index;
        int hash;
        std::shared_ptr<TItemTypes::TItem> item;
        double threshold;
        Node* left;
        Node* right;

        Node() :
            index(0),
            hash(0),
            item(nullptr),
            threshold(0),
            left(nullptr),
            right(nullptr) {}

        ~Node() {
            delete left;
            delete right;
        }
    };


    /*
    * Based on http://stevehanov.ca/blog/index.php?id=130
    * Distance metric is the hamming distances between the hashes of two TItems' images.
    * Each node stores a shared
    */
    class VPTree {
    public:
        VPTree();




    private:


        //Node* root;
        Node* root;
        // I hope I dont actually have to implement by own VPTree....
    };



};
