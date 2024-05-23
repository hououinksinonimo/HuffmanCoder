#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

void create_elementary_codes(std::vector<uint64_t> &B, std::vector<uint8_t> &shift, const std::string &filename);
void compress_file(const std::vector<uint64_t> &B, const std::vector<uint8_t> &shift, const std::string &filename);