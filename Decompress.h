#pragma once

#include "Binary_Tree.h"
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

void read_data_for_decompress(std::vector<uint64_t> &B, std::vector<uint8_t> &shift, uint8_t &length_of_last_byte, uint8_t &last_byte);
void create_binary_tree_of_elementary_codes(const std::vector<uint8_t> &alphabet, const std::vector<uint64_t> &B, const std::vector<uint8_t> &shift, Binary_tree &tree);
uint8_t get_letter_by_way(const uint64_t &way, const uint8_t &way_length, Tree_element* const root);
void decompress_data_from_file(Tree_element* const root, const std::string &filename, uint8_t &length_of_last_byte, const uint8_t &last_byte);
