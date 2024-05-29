#include "Decompress.h"

void read_data_for_decompress(std::vector<uint64_t> &B, std::vector<uint8_t> &shift, uint8_t &length_of_last_byte, uint8_t &last_byte)
{
    std::ifstream file("data_for_decompress.bin", std::ios_base::binary);

    file.read((char* )&length_of_last_byte, 1);
    
    file.read((char* )&last_byte, 1);

    file.read((char* )&( B[0] ), 2048);

    file.read((char* )&( shift[0] ), 256);

    file.close();
}

uint8_t get_letter_by_way(const uint64_t &way, const uint8_t &way_length, Tree_element* const root)
{
    Tree_element* current = root;

    uint64_t mask = 1ULL << ( way_length - 1 );

    while ( mask && !current->data_exist ) {
        if ( way & mask )
            current = current->right;
        else
            current = current->left;

        mask >>= 1;
    }

    return ( current->data );
}

void create_binary_tree_of_elementary_codes(uint8_t &length_of_last_byte, uint8_t &last_byte, Binary_tree &tree)
{
    std::vector<uint64_t> B(256);
    std::vector<uint8_t> shift(256);

    read_data_for_decompress(B, shift, length_of_last_byte, last_byte);

    std::vector<uint8_t> alphabet;
    for (size_t i = 0; i < 256; i++)
        if ( shift[i] )
            alphabet.push_back(i);

    size_t size = alphabet.size(), count = 0;

    for (size_t i = 0; i < size; i++)
        tree.insert(alphabet[i], B[ alphabet[i] ], shift[ alphabet[i] ]);
    
    for (size_t i = 0; i < size; i++)
        count += (size_t)( get_letter_by_way(B[ alphabet[i] ], shift[ alphabet[i] ], tree.root) == alphabet[i] );

    if ( count != size ) {
        std::cout << "Ошибка в создании двоичного дерева поиска.\n";
        exit(2);
    }
}


void decompress_data_from_file(Tree_element* const root, const std::string &filename, uint8_t &length_of_last_byte, const uint8_t &last_byte)
{
    std::cout << "Начало разжатия\n";

    std::ifstream reading("compressed_data.bin", std::ios_base::binary);
    std::ofstream writing(filename, std::ios_base::binary);
    Tree_element* current = root;
    uint8_t byte = 0;

    while ( reading.read((char* )&byte, 1) ) {
        uint8_t mask = 1 << 7;

        while ( mask ) {  
            do {
                if ( byte & mask )
                    current = current->right;
                else
                    current = current->left;
                
                mask >>= 1;
            } while ( mask && !current->data_exist );

            if ( current->data_exist ) {
                writing.write( (char* )&( current->data ), 1);
                current = root;
            } 
        }
    }

    uint8_t mask = 1 << 7;

    while ( length_of_last_byte ) {          
        do {
            if ( last_byte & mask )
                current = current->right;
            else
                current = current->left;
            
            mask >>= 1;
            length_of_last_byte--;
        } while ( !current->data_exist );

        writing.write( (char* )&( current->data ), 1);
        current = root;
    }
    
    reading.close();
    
    writing.close();
}