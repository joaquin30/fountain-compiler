#include <iostream>
#include <fstream>
#include "./fountain.hpp"

int main (int argc, char** argv) {
    fountain::parser output(argv[1]);
    std::cout << output.html();
    return 0;
}
