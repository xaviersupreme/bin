#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

void append_file(stringstream* str);
void calculate(int op1, int op2, char r_op);
void calculate_ss(int op1, int op2, char r_op, stringstream &ss);
void puts_history();

int main(void) {
    cout << "calc or history: ";
    string input;
    cin >> input;

    if (input == "calc") {
        int op1, op2;
        char r_op;

        cout << "enter op1: ";
        cin >> op1;

        cout << "enter r_op: ";
        cin >> r_op;

        cout << "enter op2: ";
        cin >> op2;

        calculate(op1, op2, r_op);

        stringstream out;

        calculate_ss(op1, op2, r_op, out);

        append_file(&out);
    } else if (input == "history") {
        puts_history();
    }

}

void calculate(int op1, int op2, char r_op) {
    /*  */ if (r_op == '*') {
        cout << op1 * op2 << endl;
    } else if (r_op == '+') {
        cout << op1 + op2 << endl;
    } else if (r_op == '-') {
        cout << op1 - op2 << endl;
    } else if (r_op == '/') {
        if (op2 == 0) {
            cout << "undefined (division by zero)\n";
        } else {
            cout << op1 / op2 << "\n";
        }
    } else if (r_op == '%') {
        if (op2 == 0) {
            cout << "undefined (modulo by zero)\n";
        } else {
            cout << op1 % op2 << "\n";
        }
    } else {
        cout << "choose a real operator" << endl;
    }
}

void calculate_ss(int op1, int op2, char r_op, stringstream &ss) {
    if (r_op == '*') {
        ss << op1 << " * " << op2 << " = " << op1 * op2 << "\n";
    }
    else if (r_op == '+') {
        ss << op1 << " + " << op2 << " = " << op1 + op2 << "\n";
    }
    else if (r_op == '-') {
        ss << op1 << " - " << op2 << " = " << op1 - op2 << "\n";
    }
    else if (r_op == '/') {
        if (op2 == 0) {
            ss << op1 << " / " << op2 << " = undefined (division by zero)\n";
        } else {
            ss << op1 << " / " << op2 << " = " << op1 / op2 << "\n";
        }
    }
    else if (r_op == '%') {
        if (op2 == 0 || op1 == 0) {
            ss << op1 << " % " << op2 << " = undefined (modulo by zero)\n";
        } else {
            ss << op1 << " % " << op2 << " = " << op1 % op2 << "\n";
        }
    }
    else {
        ss << "Unknown operator: " << r_op << "\n";
    }
}

void append_file(stringstream* str) {
    ofstream of;

    of.open("history.txt", ios::app);
    if (!of)
        cout << "history not found";
    else {
        of << str->str();
        of.close();
    }
}

void puts_history() {
    ifstream file("history.txt");
    string str; 
    while (getline(file, str))
    {
        cout << str << endl;
    }
}
