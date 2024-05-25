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
    std::vector<uint64_t> B(256);
    std::vector<uint8_t> shift(256);
    uint8_t length_of_last_byte, last_byte;

    read_data_for_decompress(B, shift, length_of_last_byte, last_byte);

    std::vector<uint8_t> alphabet;
    for (size_t i = 0; i < 256; i++)
        if ( shift[i] )
            alphabet.push_back(i);

    Binary_tree bin_tree;
    create_binary_tree_of_elementary_codes(alphabet, B, shift, bin_tree);

    size_t size = alphabet.size(), count = 0;
    for (size_t i = 0; i < size; i++)
        count += (size_t)( get_letter_by_way(B[ alphabet[i] ], shift[ alphabet[i] ], bin_tree.root) == alphabet[i] );

    if ( count != size ) {
        std::cout << "Ошибка в создании двоичного дерева.\n";
        exit(2);
    }

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