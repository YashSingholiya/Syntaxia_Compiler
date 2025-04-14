#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

unordered_map<string, int64_t> registers;
unordered_map<int64_t, int64_t> memory;
unordered_map<string, int> labels;
vector<string> instructions;
int pc = 0;

int64_t get_register(const string& reg) {
    if (reg == "x0") return 0; // x0 is hardwired to 0
    if (registers.find(reg) == registers.end()) registers[reg] = 0;
    return registers[reg];
}

void set_register(const string& reg, int64_t value) {
    if (reg == "x0") return;
    cout << "Setting register " << reg << " to " << value << endl;  // Debugging line
    registers[reg] = value;
}

void parse_labels_and_instructions(const string& filename) {
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        cout << "Original line: " << line << endl;  // Log the raw line
        // Remove comments (if any) for clarity
        size_t comment_pos = line.find('#');
        if (comment_pos != string::npos) {
            line = line.substr(0, comment_pos);
        }

        line = line.substr(0, line.find_last_not_of(" \t\n\r") + 1);  // Strip trailing whitespace

        if (line.empty()) continue; // Skip empty lines

        // Log the cleaned line
        cout << "Cleaned line: " << line << endl;

        size_t colon = line.find(':');
        if (colon != string::npos) {
            string label = line.substr(0, colon);
            labels[label] = instructions.size();
            cout << "Parsed label: " << label << " at instruction " << instructions.size() << endl;  // Log label
            line = line.substr(colon + 1);
        }

        if (!line.empty()) {
            instructions.push_back(line);
        }
    }
}

void execute(const string& inst_line) {
    istringstream iss(inst_line);
    string opcode, rd, rs1, rs2, imm;
    iss >> opcode;
    cout << "Parsed opcode: " << opcode << endl;  // Log parsed opcode

    if (opcode == "addi") {
        iss >> rd >> rs1 >> imm;
        set_register(rd, get_register(rs1) + stoi(imm));
    } else if (opcode == "add") {
        iss >> rd >> rs1 >> rs2;
        set_register(rd, get_register(rs1) + get_register(rs2));
    } else if (opcode == "ldi") {
        iss >> rd >> imm;
        set_register(rd, stoi(imm));
    } else if (opcode == "sd") {
        iss >> rs1 >> imm;
        int offset = stoi(imm.substr(0, imm.find('(')));
        string base = imm.substr(imm.find('(') + 1);
        base.pop_back(); // remove ')'
        memory[get_register(base) + offset] = get_register(rs1);
    } else if (opcode == "ld") {
        iss >> rd >> rs1;
        set_register(rd, memory[get_register(rs1)]);
    } else if (opcode == "jal") {
        iss >> rd >> rs1;
        set_register(rd, pc + 1);
        pc = labels[rs1] - 1;
    } else if (opcode == "jalr") {
        iss >> rd >> imm;
        set_register(rd, 0);
        pc = get_register("x1") - 1;
    } else if (opcode == "beq") {
        iss >> rs1 >> rs2 >> imm;
        if (get_register(rs1) == get_register(rs2)) pc = labels[imm] - 1;
    } else if (opcode == "bne") {
        iss >> rs1 >> imm >> rd;
        if (get_register(rs1) != stoi(imm)) pc = labels[rd] - 1;
    } else if (opcode == "blt") {
        iss >> rs1 >> rs2 >> rd;
        if (get_register(rs1) < get_register(rs2)) pc = labels[rd] - 1;
    }
}

void run_vm(const string& filename) {
    parse_labels_and_instructions(filename);
    while (pc < static_cast<int>(instructions.size())) {
        cout << "Current PC: " << pc << endl;  // Debugging line
        execute(instructions[pc]);
        pc++;
    }
}

void print_registers() {
    cout << "\nFinal Register Values:\n";
    if (registers.empty()) {
        cout << "No registers updated." << endl;  // Debugging line
    }
    for (auto it = registers.begin(); it != registers.end(); ++it) {
        cout << setw(4) << it->first << ": " << it->second << endl;
    }
}

int main() {
    run_vm("assembly_code.txt");
    print_registers();
    return 0;
}
