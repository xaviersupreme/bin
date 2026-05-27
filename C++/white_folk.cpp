#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <ios>
#include <iterator>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

int main() {
    string path;
    printf("Enter file: ");
    getline(cin, path);

    ifstream file(path, ios::binary);

    if (!file) {
        printf("Could not open file.");
        return 1;
    }

    vector<unsigned char> bytes(
        istreambuf_iterator<char>(file),
        {}
    );
    
    string ascii;
    for (size_t i{}; i < bytes.size(); ++i) {
        int real_size = bytes.size() - i;
        int int_c = static_cast<int>(bytes[i]);

        if (i % 16 == 0) {
            cout << "\n" << setw(8) << setfill('0') << i;
        }

        if (int_c < 10) {
            cout << "  0x0" << hex << uppercase << int_c;

        } else if (int_c >= 10 && int_c <= 15) {
            cout << "  0x0" << hex << uppercase << int_c;

        } else if (int_c > 15) {
            cout << "  0x" << hex << uppercase << int_c;

        } else {
            cout << "  0x??";
        }
        
        if (int_c < 32 || int_c > 126) {
            ascii += '.';

        } else {
            ascii += bytes[i];
        }

        size_t used = ascii.size();
        size_t missing = 16 - used;

        if (ascii.size() % 16 == 0) {
            cout << "  " << ascii;
            ascii.clear();

        } else if (real_size < 2) {
            cout << string(missing * 6 + 2, ' ');
            cout << ascii;
        }
        
    }

    return 0;
}
