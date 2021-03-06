#include <iostream>
#include "Blackbird.h"


int main(int argc, const char* argv[])
{
    std::ifstream stream;
    stream.open(argv[1]);

    blackbird::Program* program = blackbird::parse(stream);

    // print device info
    std::cout << std::endl;
    program->print_device_info();
    // print operations
    std::cout << std::endl;
    program->print_operations();

    return 0;
}
