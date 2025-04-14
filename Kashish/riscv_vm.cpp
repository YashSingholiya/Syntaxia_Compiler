#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class RiscVVM {
public:
    RiscVVM();
    void run(const std::string& filename);
    void printRegisters();

private:
    std::map<std::string, int> registers; // x0 - x31
    std::vector<int> memory;
    int pc; // Program counter

    void executeInstruction(const std::string& instruction);
    void parseAndExecute(const std::string& line);
};

RiscVVM::RiscVVM() : pc(0) {
    // Initialize all registers to 0
    for (int i = 0; i < 32; ++i) {
        registers["x" + std::to_string(i)] = 0;
    }
    memory.resize(1024, 0); // Simple memory size
}

void RiscVVM::run(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        parseAndExecute(line);
    }

    printRegisters();
}

void RiscVVM::parseAndExecute(const std::string& line) {
    std::istringstream ss(line);
    std::string instruction;
    ss >> instruction;
    executeInstruction(instruction);
}

void RiscVVM::executeInstruction(const std::string& instruction) {
    if (instruction == "addi") {
        // Example of addi instruction: addi x1, x2, 10
        std::string rd, rs1;
        int imm;
        std::cin >> rd >> rs1 >> imm;
        registers[rd] = registers[rs1] + imm;
    } else if (instruction == "sd") {
        // Example of sd: sd x8, 0(x9)
        std::string rs1, rd;
        int offset;
        std::cin >> rd >> offset >> rs1;
        memory[registers[rs1] + offset] = registers[rd];
    } else if (instruction == "ld") {
        // Example of ld: ld x8, 0(x9)
        std::string rd, rs1;
        int offset;
        std::cin >> rd >> offset >> rs1;
        registers[rd] = memory[registers[rs1] + offset];
    } else {
        std::cout << "Unknown instruction: " << instruction << std::endl;
    }

    pc += 4; // Increment the program counter (assuming all instructions are 4 bytes)
}

void RiscVVM::printRegisters() {
    std::cout << "Final Registers State:" << std::endl;
    for (int i = 0; i < 32; ++i) {
        std::cout << "x" << i << ": " << registers["x" + std::to_string(i)] << std::endl;
    }
}
