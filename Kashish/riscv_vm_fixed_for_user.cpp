#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cassert>

// Define register names
std::unordered_map<std::string, int> registers = {
    {"x0", 0}, {"x1", 0}, {"x2", 0}, {"x3", 0}, {"x4", 0}, {"x5", 0},
    {"x6", 0}, {"x7", 0}, {"x8", 0}, {"x9", 0}, {"x10", 0}, {"x11", 0},
    {"x12", 0}, {"x13", 0}, {"x14", 0}, {"x15", 0}, {"x16", 0}, {"x17", 0},
    {"x18", 0}, {"x19", 0}, {"x20", 0}, {"x21", 0}, {"x22", 0}, {"x23", 0},
    {"x24", 0}, {"x25", 0}, {"x26", 0}, {"x27", 0}, {"x28", 0}, {"x29", 0},
    {"x30", 0}, {"x31", 0}, {"sp", 0}, {"fp", 0}
};

// Memory initialization (simulated memory as a large array)
std::unordered_map<int, int> memory;

// Program counter
int PC = 0;

// Utility function to parse immediate values
int parse_imm(std::string imm_str) {
    if (imm_str[0] == '-') {
        return std::stoi(imm_str);
    } else {
        return std::stoi(imm_str);
    }
}

// Handle different RISC-V instructions
void execute_instruction(const std::string& instruction) {
    std::stringstream ss(instruction);
    std::string opcode;
    ss >> opcode;

    if (opcode == "addi") {
        std::string rd, rs1, imm;
        ss >> rd >> rs1 >> imm;
        int imm_val = parse_imm(imm);
        registers[rd] = registers[rs1] + imm_val;
        std::cout << "Executing addi: " << rd << " = " << registers[rs1] << " + " << imm_val << std::endl;
    }
    else if (opcode == "ld") {
        std::string rd, rs1, imm;
        ss >> rd >> rs1 >> imm;
        int imm_val = parse_imm(imm);
        int address = registers[rs1] + imm_val;
        registers[rd] = memory[address];
        std::cout << "Executing ld: " << rd << " = memory[" << address << "] = " << memory[address] << std::endl;
    }
    else if (opcode == "sd") {
        std::string rs1, rs2, imm;
        ss >> rs1 >> rs2 >> imm;
        int imm_val = parse_imm(imm);
        int address = registers[rs1] + imm_val;
        memory[address] = registers[rs2];
        std::cout << "Executing sd: memory[" << address << "] = " << registers[rs2] << std::endl;
    }
    else if (opcode == "jalr") {
        std::string rd, rs1, imm;
        ss >> rd >> rs1 >> imm;
        int imm_val = parse_imm(imm);
        registers[rd] = PC + 4;  // Store the return address
        PC = registers[rs1] + imm_val;
        std::cout << "Executing jalr: PC = " << PC << std::endl;
    }
    else if (opcode == "bne") {
        std::string rs1, rs2, label;
        ss >> rs1 >> rs2 >> label;
        if (registers[rs1] != registers[rs2]) {
            // Jump to label
            std::cout << "Branch taken (bne): " << rs1 << " != " << rs2 << ", Jumping to " << label << std::endl;
            // Note: Label resolving needs to be implemented
        }
    }
    else if (opcode == "beq") {
        std::string rs1, rs2, label;
        ss >> rs1 >> rs2 >> label;
        if (registers[rs1] == registers[rs2]) {
            // Jump to label
            std::cout << "Branch taken (beq): " << rs1 << " == " << rs2 << ", Jumping to " << label << std::endl;
            // Note: Label resolving needs to be implemented
        }
    }
    else if (opcode == "jal") {
        std::string label;
        ss >> label;
        registers["x1"] = PC + 4;  // Save return address in x1
        // Jump to label
        std::cout << "Executing jal: Jumping to " << label << std::endl;
        // Note: Label resolving needs to be implemented
    }
    else {
        std::cout << "Unknown instruction: " << instruction << std::endl;
    }
}

// Main function to run the virtual machine
int main() {
    // Direct file path for the assembly code
    std::string file_path = "C:\\Users\\lenovo\\Desktop\\syntaxia\\SyntaxiaCompiler\\Krishna\\assembly_code.txt";  // Direct file path
    std::ifstream file(file_path);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_path << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip lines that are assembler directives (starting with '.')
        if (line.empty() || line[0] == '.') {
            continue;
        }

        std::cout << "PC = " << PC << ": " << line << std::endl;
        execute_instruction(line);
        PC += 4;  // Move to the next instruction
    }

    return 0;
}
