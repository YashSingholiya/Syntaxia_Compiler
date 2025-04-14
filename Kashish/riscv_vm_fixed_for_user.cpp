#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <bitset>
#include <iomanip>

using namespace std;

class RISC_V_VM {
public:
    unordered_map<string, int> labels;  // Store labels and their line numbers
    vector<string> program;             // Store the program code
    int registers[32] = {0};            // 32 registers
    int PC = 0;                         // Program Counter

    RISC_V_VM() {
        // Initialize registers (optional)
        fill(begin(registers), end(registers), 0);
    }

    void load_program(const string &filename) {
        ifstream file(filename);
        string line;
        int line_num = 0;

        if (!file) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        // Read the program code from the file
        while (getline(file, line)) {
            // Remove extra spaces
            trim(line);
            if (line.empty()) continue;

            // Handle labels (like 'main:')
            if (line.back() == ':') {
                labels[line.substr(0, line.size() - 1)] = line_num;
            }

            program.push_back(line);
            line_num++;
        }
    }

    void run() {
        while (PC < program.size()) {
            string line = program[PC];
            PC++;  // Increment PC after each instruction

            // Parse instruction and execute
            istringstream ss(line);
            string instr;
            ss >> instr;

            if (instr == "addi") {
                int rd, rs1, imm;
                ss >> rd >> rs1 >> imm;
                addi(rd, rs1, imm);
            }
            else if (instr == "ld") {
                int rd, offset;
                ss >> rd >> offset;
                ld(rd, offset);
            }
            else if (instr == "jal") {
                string label;
                ss >> label;
                jal(label);
            }
            else if (instr == "beq") {
                int rs1, rs2, label;
                ss >> rs1 >> rs2 >> label;
                beq(rs1, rs2, label);
            }
            // Add more instructions as needed

        }
    }

    void addi(int rd, int rs1, int imm) {
        registers[rd] = registers[rs1] + imm;
    }

    void ld(int rd, int offset) {
        registers[rd] = registers[offset]; // Simulating loading from memory
    }

    void jal(const string &label) {
        int target = labels[label];   // Get the line number from the label
        PC = target;
    }

    void beq(int rs1, int rs2, int label) {
        if (registers[rs1] == registers[rs2]) {
            PC = label;
        }
    }

    void print_registers() {
        for (int i = 0; i < 32; i++) {
            cout << "x" << i << ": " << registers[i] << endl;
        }
    }

private:
    void trim(string &s) {
        // Remove leading and trailing spaces, tabs, newlines
        s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
        s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    RISC_V_VM vm;
    vm.load_program(argv[1]);
    vm.run();
    vm.print_registers();

    return 0;
}
