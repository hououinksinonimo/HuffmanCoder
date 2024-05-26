#include "Compress.h"

void count_frequency(const std::string &filename, std::vector<uint64_t> &letter_count)
{
    std::ifstream file(filename, std::ios_base::binary);

    uint8_t c;

    while ( file.read((char* )&c, 1) )
        letter_count[c]++;

    file.close();
}

struct data_about_letter_vector
{
    std::vector<uint8_t> letter_vector;
    uint64_t count;
};

void create_alphabet(std::vector<data_about_letter_vector> &alphabet, const std::vector<uint64_t> &letter_count)
{
    size_t size = 0;
    
    for (size_t i = 0; i < 256; i++) 
        if ( letter_count[i] ) {
            alphabet[size].letter_vector.push_back(i);
            alphabet[size].count = letter_count[i];

            size++;
        }
    
    alphabet.resize(size);
}

void count_frequency_and_create_alphabet(const std::string &filename, std::vector<data_about_letter_vector> &alphabet)
{
    std::vector<uint64_t> letter_count(256);
    count_frequency(filename, letter_count);

    create_alphabet(alphabet, letter_count);
}

bool compare(const data_about_letter_vector &left, const data_about_letter_vector &right)
{
    return left.count > right.count;
}

void create_elementary_codes(std::vector<uint64_t> &B, std::vector<uint8_t> &shift, const std::string &filename)
{   
    std::vector<data_about_letter_vector> alphabet(256);
    count_frequency_and_create_alphabet(filename, alphabet);

    size_t size = alphabet.size();
    
    std::sort(alphabet.begin(), alphabet.end(), compare);

    while ( size > 2 ) {
        size_t pos0 = size - 2, pos1 = pos0 + 1;

        for ( uint8_t a : alphabet[pos0].letter_vector )
            shift[a]++;

        for ( uint8_t a : alphabet[pos1].letter_vector ) {
            B[a] = ( 1ULL << shift[a] ) | B[a];
            shift[a]++;
        }

        data_about_letter_vector temp;

        for ( uint8_t a : alphabet[pos0].letter_vector )
            temp.letter_vector.push_back(a);

        for ( uint8_t a : alphabet[pos1].letter_vector )
            temp.letter_vector.push_back(a);
        
        temp.count = alphabet[pos0].count + alphabet[pos1].count;

        size -= 2;

        alphabet.resize(size);

        uint8_t pos_to_insert = size;

        while ( pos_to_insert && ( alphabet[ pos_to_insert - 1 ].count < temp.count ) )
            pos_to_insert--;

        alphabet.insert(alphabet.begin() + pos_to_insert, temp);

        size++;
    }
    
    if ( size == 1 ) 
        shift[ alphabet[0].letter_vector[0] ]++;
    else {
        size_t pos0 = size - 2, pos1 = pos0 + 1;

        for ( uint8_t a : alphabet[pos0].letter_vector )
            shift[a]++;

        for ( uint8_t a : alphabet[pos1].letter_vector ) {
            B[a] = ( 1ULL << shift[a] ) | B[a];
            shift[a]++;
        }
    }
}

void write_elementary_codes_into_file(const std::vector<uint64_t> &B, const std::vector<uint8_t> &shift, const uint8_t &length_of_last_byte, const uint8_t &last_byte)
{
    std::ofstream out("data_for_decompress.bin", std::ios_base::binary);

    out.write((char* )&length_of_last_byte, 1);
    
    out.write((char* )&last_byte, 1);

    out.write((char* )&( B[0] ), 2048);

    out.write((char* )&( shift[0] ), 256);

    out.close();
}

void compress_file(const std::vector<uint64_t> &B, const std::vector<uint8_t> &shift, const std::string &filename)
{
    std::cout << "Начало сжатия\n";

    uint8_t byte = 0, length = 0;

    std::ofstream out("compressed_data.bin", std::ios_base::binary);
    std::ifstream file(filename, std::ios_base::binary);

    uint8_t c;

    while ( file.read((char* )(&c), 1) ) {
        uint64_t elementary_code = B[c];
        uint8_t s = shift[c];
        uint64_t mask = ( 1ULL << s ) - 1;

        while ( s ) {
            uint8_t need_bit_length = 8 - length;

            if ( need_bit_length > s ) {
                byte |= elementary_code << ( need_bit_length - s );
                length += s;
                s = 0;
            }
            else {
                s -= need_bit_length;
                byte |= elementary_code >> s;
                mask >>= need_bit_length;
                elementary_code &= mask;
                out.write((char* )&byte, 1);
                byte = 0;
                length = 0;
            }
        }
    }

    out.close();
    file.close();

    write_elementary_codes_into_file(B, shift, length, byte);
}