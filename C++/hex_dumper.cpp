#include <fstream>
#include <ios>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;

#define IS_MOD_16(n) ((u64)n % (16) == (0))
#define WHITESPACE ' '
#define PRINT_HEX(mod) cout << WHITESPACE << mod << hex << uppercase << chr

vector<unsigned char> dump_file_bytes(string input);
void dump_char_info(vector<unsigned char> vect);


int main(void) {
    string file;
    cout << "Enter File Path: ";
    getline(cin, file);

    ifstream temp_file_check(file);

    if (!temp_file_check) {
        cout << "Unable to Open File.";
        return -1;
    }

    vector<unsigned char> bytes = dump_file_bytes(file);

    dump_char_info(bytes);

    return 0;
}


vector<unsigned char> dump_file_bytes(string input) {
    ifstream rpath(input);

    vector<unsigned char> out(
        istreambuf_iterator<char>(rpath),
        {}
    );

    return out;
}


void dump_char_info(vector<unsigned char> bytes) {
    /*----------------------------------*/
    string ascii;

    for (u64 i{}; i < bytes.size(); ++i) {
        int chr = (int)bytes[i];

        if (chr < 32 || chr > 126) {
            ascii += '.';
            
        } else {
            ascii += bytes[i];
        }


        if (IS_MOD_16(i)) {
            cout << dec; // "Flush" cout to make it quit returning decs as hexadecs.
            cout << "\n"
                << setw(8)
                << setfill('0')
                << i;
        }


        if (chr <= 15) {
            PRINT_HEX('0'); // appends '0' to the start

        } else {
            PRINT_HEX("");
        }


        if (IS_MOD_16(ascii.size())) {
            cout << WHITESPACE << ascii;
            ascii.clear();

        } else if (bytes.size() - i < 2) { /* pseudo: if the last set of bytes are under 16 chrs */
            u8 missing = 16 - ascii.size();
            cout << string((missing * 3) + 1, WHITESPACE); // padding needed to align the ASCII
            cout << ascii;
        }
    }

}
