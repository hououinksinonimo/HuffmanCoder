#include "Binary_Tree.h"

Tree_element::Tree_element() 
{
    data_exist = false;
    left = nullptr;
    right = nullptr;
}

Binary_tree::Binary_tree()
{
    root = new Tree_element();
}

void Binary_tree::delete_tree(Tree_element* current)
{
    if ( current != nullptr ) {
        delete_tree(current->left);
        delete_tree(current->right);
        delete current;
    }
}

Binary_tree::~Binary_tree()
{
    delete_tree(root);
}

void Binary_tree::insert(const uint8_t &letter, const uint64_t &way, const uint8_t &way_length)
{
    uint64_t mask = 1ULL << ( way_length - 1 );

    Tree_element* current = root;

    while ( mask ){
        if ( way & mask ) {
            if ( current->right != nullptr )
                current = current->right;
            else {
                current->right = new Tree_element();
                current = current->right;
            }
        }
        else {
            if ( current->left != nullptr )
                current = current->left;
            else {
                current->left = new Tree_element();
                current = current->left;
            }
        }

        mask >>= 1;
    }

    current->data = letter;
    current->data_exist = true;
}