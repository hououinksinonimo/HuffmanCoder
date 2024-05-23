#pragma once

#include <cstdint>

class Tree_element
{
    public:
        bool data_exist;
        uint8_t data;
        Tree_element* left;
        Tree_element* right;
        Tree_element();
};

class Binary_tree
{
    public:
        Tree_element* root;
        Binary_tree();
        ~Binary_tree();
        void delete_tree(Tree_element* current);
        void insert(const uint8_t &letter, const uint64_t &way, const uint8_t &way_length);
};