#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <map>

using namespace std;

map<string, int> registers;  // Register file
int PC = 0;  // Program counter

// Trim function to remove leading and trailing spaces
void trim(string& s) {
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
}

// Function to decode and execute the instructions
void executeInstruction(const string& instruction) {
    // Debugging output for current instruction
    cout << "Executing: " << instruction << endl;

    // Example for printing register state and execution
    if (instruction.substr(0, 3) == "addi") {
        // Extract operands for the ADDI instruction (this is just an example)
        stringstream ss(instruction);
        string opcode, dest, src1, imm;
        ss >> opcode >> dest >> src1 >> imm;

        // Handle the register operations here
        int imm_val = stoi(imm);
        registers[dest] = registers[src1] + imm_val;

        // Debugging output
        cout << "Updated register " << dest << " to " << registers[dest] << endl;
    }

    // More instruction handling should go here...

    // Increment PC to the next instruction
    PC++;
    cout << "Program Counter (PC) updated to: " << PC << endl;
}

// Function to run the VM
void run(const vector<string>& program) {
    while (PC < program.size()) {
        string instruction = program[PC];

        // Print out the current instruction
        cout << "PC = " << PC << ": " << instruction << endl;

        // Execute the instruction
        executeInstruction(instruction);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Failed to open file: " << filename << endl;
        return 1;
    }

    vector<string> program;
    string line;
    while (getline(inputFile, line)) {
        trim(line);
        if (!line.empty()) {
            program.push_back(line);
        }
    }

    inputFile.close();

    // Initialize registers (just for example, set all to zero)
    registers["sp"] = 0;
    registers["fp"] = 0;

    // Start the VM
    run(program);

    return 0;
}
