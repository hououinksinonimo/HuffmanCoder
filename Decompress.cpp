#include "Decompress.h"

void read_data_for_decompress(std::vector<uint64_t> &B, std::vector<uint8_t> &shift, uint8_t &length_of_last_byte, uint8_t &last_byte)
{
    std::ifstream file("data_for_decompress.bin", std::ios_base::binary);

    file.read((char* )&length_of_last_byte, 1);

    if ( length_of_last_byte )
        file.read((char* )&last_byte, 1);

    file.read((char* )&( B[0] ), 2048);

    file.read((char* )&( shift[0] ), 256);

    file.close();
}

void create_binary_tree_of_elementary_codes(const std::vector<uint8_t> &alphabet, const std::vector<uint64_t> &B, const std::vector<uint8_t> &shift, Binary_tree &tree)
{
    size_t size = alphabet.size();

    for (size_t i = 0; i < size; i++)
        tree.insert(alphabet[i], B[ alphabet[i] ], shift[ alphabet[i] ]);
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