#include <iostream>
#include "fountain.hpp"

int main (int argc, char** argv) {
    fountain::parser output(argv[1]);
    std::cout << output.tex();
    return 0;
}
