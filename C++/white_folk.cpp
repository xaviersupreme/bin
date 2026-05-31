#include <cstddef>
#include <cstdio>
#include <ios>
#include <iterator>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

vector<string> split(const string& s, char delimiter) {
    vector<string> parts;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delimiter)) {
        parts.push_back(item);
    }

    return parts;
}

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
        // int real_size = bytes.size() - i;
        int int_c = static_cast<int>(bytes[i]);

        if (i % 16 == 0) {
            cout << "\n" << setw(8) << setfill('0') << i;
        }

        if (int_c <= 15) {
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

        size_t missing = 16 - ascii.size();
 
        if (ascii.size() % 16 == 0) {
            cout << "  " << ascii;
            ascii.clear();

        } else if (bytes.size() - i < 2) {
            cout << string(missing * 6 + 2, ' ');
            cout << ascii;
        }

    }

    string bytes_input;
    cin.clear();
    cout << "\n" << "Search for hex (sepereate by ' '): ";
    getline(cin, bytes_input);

    vector<string> splitted = split(bytes_input, ' ');

    for (size_t i{}; i < bytes.size(); ++i) {
        int int_c = static_cast<int>(bytes[i]);
        size_t rounded = (i + 15) & ~15; // round to the nearest 10th

        stringstream stream;

        if (int_c <= 15) {
            stream << "0x0" << hex << uppercase << int_c;
        } else if (int_c > 15) {
            stream << "0x" << hex << uppercase << int_c;
        }

        string result( stream.str() ); // hexified int

        int cur{};
        for (const auto& split : splitted) {
            if (result == static_cast<const string&>(split)) {
                cout << "\n" << "FOUND @ OFFSET: " << setw(8) << setfill('0') << rounded;

                /* TERRIBLE IMPLEMENTATION BUT I DONT KNOW HOW TO DO IT BETTER */
                i += 16 - cur; // skip to the end of the line
                cur = 0; // clear ascii for the next line
            }
        }
    }

    return 0;
}
