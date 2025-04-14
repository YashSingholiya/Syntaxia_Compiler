#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> program;
unordered_map<string, int> label_map;
int PC = 0;

void load_program(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        size_t colon_pos = line.find(':');
        if (colon_pos != string::npos) {
            string label = line.substr(0, colon_pos);
            label_map[label] = program.size();
            line = line.substr(colon_pos + 1);
        }

        if (!line.empty()) {
            program.push_back(line);
        }
    }
}

void find_main() {
    auto it = label_map.find("main");
    if (it != label_map.end()) {
        PC = it->second;
    } else {
        cerr << "Error: 'main' label not found in any form!" << endl;
        exit(1);
    }
}

void run() {
    for (size_t i = (PC > 0 ? PC - 1 : 0); i < program.size(); ++i) {
        cout << "Executing: " << program[i] << endl;
        // Simulate execution logic here (dummy for now)
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    load_program(argv[1]);
    find_main();
    run();
    return 0;
}
