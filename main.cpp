#include <iostream>
#include "Compiler.h"

int main(int argc, const char *argv[]) {
    Compiler cc(argc, argv);
    std::cout << "this is a compiler\n";
    return 0;
}
