#include <iostream>
#include "Compress.h"
#include "Decompress.h"

void compress(const std::string &filename)
{
    std::vector<uint64_t> B(256);
    std::vector<uint8_t> shift(256);
    create_elementary_codes(B, shift, filename);

    compress_file(B, shift, filename);
}

void decompress(const std::string &filename)
{
    uint8_t length_of_last_byte, last_byte;
    Binary_tree bin_tree;

    create_binary_tree_of_elementary_codes(length_of_last_byte, last_byte, bin_tree);

    decompress_data_from_file(bin_tree.root, filename, length_of_last_byte, last_byte);
}

int main(int argc, char* argv[])
{
    if ( ( argc < 3 ) && ( ( *(argv[1]) != 'c' ) && ( *(argv[1]) != 'd' ) ) ) {
        std::cout << "Неверно переданные параметры.\n";
        exit(1);
    } 

    char mode = *(argv[1]);
    std::string filename = argv[2]; 

    if ( mode == 'c' ) {
        compress(filename);
        std::cout << "Сжатие закончено\n";
    }
    else if ( mode == 'd' ) {
        decompress(filename);
        std::cout << "Разжатие закончено\n";
    }
    
    return 0;
}