int parse_imm(std::string imm_str) {
    try {
        return std::stoi(imm_str);  // Try to convert string to integer
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid immediate value '" << imm_str << "'" << std::endl;
        return 0;  // Default value on error
    }
}
