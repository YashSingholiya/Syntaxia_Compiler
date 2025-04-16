// RISC-V Virtual Machine (Minimal Example)
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>

// 32 general-purpose registers
int64_t regs[32] = {0};

// Simulated memory (1MB)
std::vector<uint8_t> memory(1024 * 1024);

// Stack pointer starts at the end of memory
uint64_t sp = 1024 * 1024;

// Label map
std::unordered_map<std::string, int> labelMap;
std::vector<std::string> instructions;

int getRegister(const std::string &name) {
    if (name[0] == 'x') return std::stoi(name.substr(1));
    return -1;
}

void loadAssembly(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        // Remove comments
        auto comment = line.find('#');
        if (comment != std::string::npos) line = line.substr(0, comment);
        
        // Trim
        if (line.empty()) continue;

        // Label check
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string label = line.substr(0, colon);
            labelMap[label] = instructions.size();
            line = line.substr(colon + 1);
        }

        if (!line.empty()) instructions.push_back(line);
    }
}

void execute() {
    for (int pc = 0; pc < instructions.size(); ++pc) {
        std::istringstream iss(instructions[pc]);
        std::string opcode, arg1, arg2, arg3;
        iss >> opcode >> arg1 >> arg2 >> arg3;

        // Clean trailing commas
        if (!arg1.empty() && arg1.back() == ',') arg1.pop_back();
        if (!arg2.empty() && arg2.back() == ',') arg2.pop_back();
        if (!arg3.empty() && arg3.back() == ',') arg3.pop_back();

        if (opcode == "addi") {
            int rd = getRegister(arg1);
            int rs1 = getRegister(arg2);
            int imm = std::stoi(arg3);
            regs[rd] = regs[rs1] + imm;
        } else if (opcode == "add") {
            int rd = getRegister(arg1);
            int rs1 = getRegister(arg2);
            int rs2 = getRegister(arg3);
            regs[rd] = regs[rs1] + regs[rs2];
        } else if (opcode == "sd") {
            int rs = getRegister(arg1);
            int offset = std::stoi(arg2.substr(0, arg2.find('(')));
            std::string base = arg2.substr(arg2.find('(') + 1);
            base.pop_back(); // remove )
            int baseReg = getRegister(base);
            uint64_t addr = regs[baseReg] + offset;
            *(int64_t*)&memory[addr] = regs[rs];
        } else if (opcode == "ld") {
            int rd = getRegister(arg1);
            int offset = std::stoi(arg2.substr(0, arg2.find('(')));
            std::string base = arg2.substr(arg2.find('(') + 1);
            base.pop_back();
            int baseReg = getRegister(base);
            uint64_t addr = regs[baseReg] + offset;
            regs[rd] = *(int64_t*)&memory[addr];
        } else if (opcode == "jal") {
            regs[1] = pc + 1; // x1 is return address
            pc = labelMap[arg1] - 1;
        } else if (opcode == "jalr") {
            int rd = getRegister(arg1);
            std::string offsetReg = arg2;
            int offset = std::stoi(offsetReg.substr(0, offsetReg.find('(')));
            std::string base = offsetReg.substr(offsetReg.find('(') + 1);
            base.pop_back();
            int baseReg = getRegister(base);
            pc = regs[baseReg] + offset - 1;
        } else if (opcode == "ldi") { // pseudo-instruction
            int rd = getRegister(arg1);
            regs[rd] = std::stoi(arg2);
        } else if (opcode == "bne") {
            int rs1 = getRegister(arg1);
            int rs2 = getRegister(arg2);
            if (regs[rs1] != regs[rs2]) pc = labelMap[arg3] - 1;
        } else if (opcode == "beq") {
            int rs1 = getRegister(arg1);
            int rs2 = getRegister(arg2);
            if (regs[rs1] == regs[rs2]) pc = labelMap[arg3] - 1;
        } else if (opcode == "blt") {
            int rs1 = getRegister(arg1);
            int rs2 = getRegister(arg2);
            if (regs[rs1] < regs[rs2]) pc = labelMap[arg3] - 1;
        }
    }
}

void printRegisters() {
    std::cout << "\nFinal Register State:\n";
    for (int i = 0; i < 32; ++i) {
        if (regs[i] != 0)
            std::cout << "x" << i << ": " << regs[i] << "\n";
    }
}

int main() {
    loadAssembly("assembly_code.txt");
    regs[2] = sp; // Initialize sp in x2
    execute();
    printRegisters();
    return 0;
}

