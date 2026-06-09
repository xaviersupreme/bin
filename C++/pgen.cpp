#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;

int main(void) {
    const string chrs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+-=<>/?";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(0, chrs.size());

    string final;
    for (int i{}; i < 12; ++i) {
        final += chrs.substr(distrib(gen), 1);
    }
    cout << final << endl;

    ofstream of;

    of.open("passwords.txt", ios::app);
    if (!of)
        cout << "passwords.txt not found";
    else {
        of << final << '\n';
        of.close();
    }
}
