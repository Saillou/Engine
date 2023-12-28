#include "function.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void Engine::sayHello() {
    std::cout << "Hello world!" << std::endl;
}

void Engine::readContent(const char* file_path) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        std::cerr << "Not opened" << std::endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>())   );
    std::cout << content << std::endl;
}