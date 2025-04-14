#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>
using namespace std;

unordered_map<string, int> labelToIndex; // Map to store label and its corresponding line number
vector<string> program; // Vector to store the entire program
int PC = 0; // Program counter

// Function to load the program into memory
void loadProgram(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        program.push_back(line);
    }
}

// Function to parse labels and store their line numbers
void parseLabels() {
    for (int i = 0; i < program.size(); ++i) {
        string line = program[i];
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string label = line.substr(0, pos);
            label.erase(remove_if(label.begin(), label.end(), ::isspace), label.end());
            labelToIndex[label] = i + 1; // Store label and line number (1-based index)
        }
    }

    // Debugging: Print out all labels
    cout << "Labels found in the program:" << endl;
    for (auto& label : labelToIndex) {
        cout << label.first << " -> " << label.second << endl;
    }
}

// Function to set the Program Counter (PC) to the address of the 'main' label
void setProgramCounterToMain() {
    if (labelToIndex.find("main") != labelToIndex.end()) {
        PC = labelToIndex["main"]; // Set program counter to the location of 'main' label
        cout << "Program counter set to main at line: " << PC << endl;
    } else {
        cout << "Error: 'main' label not found in the program!" << endl;
        exit(1); // Exit with error if 'main' label is not found
    }
}

// Function to run the program (simplified)
void run() {
    // Set the program counter to the 'main' label
    setProgramCounterToMain();

    // Run the program starting from the 'main' label
    for (int i = PC - 1; i < program.size(); ++i) { // Starting from the 'main' label
        string instruction = program[i];
        cout << "Executing: " << instruction << endl;
    }
}

int main() {
    string filename = "input.asm"; // Your RISC-V assembly file
    loadProgram(filename);         // Load the program
    parseLabels();                 // Parse labels and store their line numbers
    run();                          // Run the program starting from 'main'
    return 0;
}
