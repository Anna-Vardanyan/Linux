#include <iostream>
#include "super_rm.h"

int main(int argc, char** argv){
    int size = std::atoi(argv[2]);
    super_rm(argc, argv, size);
    return 0;
}