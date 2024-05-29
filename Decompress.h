#pragma once

#include "Binary_Tree.h"
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

void create_binary_tree_of_elementary_codes(uint8_t &length_of_last_byte, uint8_t &last_byte, Binary_tree &tree);
void decompress_data_from_file(Tree_element* const root, const std::string &filename, uint8_t &length_of_last_byte, const uint8_t &last_byte);