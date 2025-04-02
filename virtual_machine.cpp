#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

// Define a simple virtual machine with registers, stack, and memory
class VirtualMachine {
public:
    // Registers (x0 to x31)
    int regs[32] = {0};
    // Stack, using a vector to simulate the stack
    vector<int> stack;
    // Program counter (for control flow)
    int pc = 0;
    // Output file stream
    ofstream outputFile;

    // Program (list of instructions)
    vector<string> program;

    // Constructor to open the output file
    VirtualMachine() {
        outputFile.open("final_output.txt");  // Open the output file for writing
        if (!outputFile.is_open()) {
            cerr << "Failed to open output file!" << endl;
            exit(1);
        }
    }

    // Destructor to close the output file
    ~VirtualMachine() {
        if (outputFile.is_open()) {
            outputFile.close();  // Close the output file after execution
        }
    }

    // Read assembly code from a file
    bool loadProgramFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open file: " << filename << endl;
            return false;
        }
        string line;
        while (getline(file, line)) {
            // Skip empty lines and comments (assuming comments start with //)
            if (line.empty() || line.find("//") == 0) {
                continue;
            }
            program.push_back(line);
        }
        file.close();
        return true;
    }

    // Execute the program (assembly code)
    void execute() {
        while (pc < program.size()) {
            auto instr = program[pc];
            executeInstruction(instr);
            pc++;
        }
    }

    // Method to execute individual instructions
    void executeInstruction(const string& instr) {
        // Parse and execute instructions here
        if (instr.substr(0, 4) == "addi") {
            // ADDI: add immediate
            int rd, rs1, imm;
            sscanf(instr.c_str(), "addi x%d, x%d, %d", &rd, &rs1, &imm);
            regs[rd] = regs[rs1] + imm;
            outputFile << "addi x" << rd << ", x" << rs1 << ", " << imm << " -> x" << rd << " = " << regs[rd] << endl;
        } else if (instr.substr(0, 3) == "ld ") {
            // LD: Load from memory (stack)
            int rd, offset;
            sscanf(instr.c_str(), "ld x%d, %d(sp)", &rd, &offset);
            regs[rd] = stack[stack.size() - 1 - offset];  // Pop from stack
            outputFile << "ld x" << rd << ", " << offset << "(sp) -> x" << rd << " = " << regs[rd] << endl;
        } else if (instr.substr(0, 3) == "sd ") {
            // SD: Store to memory (stack)
            int rs, offset;
            sscanf(instr.c_str(), "sd x%d, %d(sp)", &rs, &offset);
            if (stack.size() <= offset) {
                stack.resize(offset + 1);  // Ensure stack is large enough
            }
            stack[stack.size() - 1 - offset] = regs[rs];  // Push to stack
            outputFile << "sd x" << rs << ", " << offset << "(sp) -> stack[" << stack.size() - 1 - offset << "] = " << regs[rs] << endl;
        } else if (instr.substr(0, 3) == "jal") {
            // JAL: Jump and Link (function call)
            string func = instr.substr(4);
            if (func == "testfunction") {
                testFunction();
            }
        }
        // You can add more cases for other instructions like branches, comparisons, etc.
    }

    // Example function: testfunction
    void testFunction() {
        outputFile << "Executing test function" << endl;
        // Function implementation (just simulate for now)
    }
};

int main() {
    VirtualMachine vm;

    // Input assembly code from a text file
    string filename = "assembly_code.txt"; // Change this to the file name
    if (!vm.loadProgramFromFile(filename)) {
        return 1;  // Exit if the file could not be opened
    }

    // Execute the loaded program
    vm.execute();

    return 0;
}
