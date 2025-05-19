#ifndef SUPER_RM_SUPER_RM_H
#define SUPER_RM_SUPER_RM_H

#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <ftw.h>
#include <algorithm>
#include "/home/annavardanyan/linux-2024-25/utils/logging/logger.h"

bool r = false;
bool v = false;
const int buffer_size = 1024;
char* file_to_remove;

std::vector<char> generate_random_vector() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 mt{rd()};
    static thread_local std::uniform_int_distribution<int> dist(0,255);
    std::vector<char> vec(buffer_size);
    std::generate(vec.begin(), vec.end(), [](){return dist(mt);});
    return vec;
}

int remove_and_overwrite(bool v_, const char* file_, int size_){

    int file_descriptor = open(file_, O_WRONLY | O_TRUNC, 0666);
    if (file_descriptor == -1) {
        LOG::ERROR(__func__, __LINE__, "Error opening the file for overwriting");
        return 1;
    }

    std::vector<char> buffer = generate_random_vector();

    while(size_ > buffer_size) {
        int bytes_written = write(file_descriptor, buffer.data(), buffer_size);
        if(bytes_written == -1){
            LOG::ERROR(__func__, __LINE__, "Error overwriting to the file");
        }
        size_ -= buffer_size;
    }
    int bytes_written = write(file_descriptor, buffer.data(), size_);
    if(bytes_written == -1){
        LOG::ERROR(__func__, __LINE__, "Error overwriting to the file");
    }

    close(file_descriptor);

    if (remove(file_) != 0) {
        LOG::ERROR(__func__, __LINE__, "Error removing the file");
        return 1;
    }

    if(v_ == true){
        LOG::INFO(__func__, __LINE__, "Removed the file ", file_);
    }

    return 0;
}

int ftw_callback(const char *fpath, const struct stat *sb,
                 int typeflag) {
    if(typeflag == FTW_F){
        remove_and_overwrite(v, fpath, sb->st_size);
    }
    return 0;
}

void super_rm(int argc_, char** argv_, int _size){

    int opt;
    while((opt = getopt(argc_, argv_, "rv")) != -1) {
        switch (opt) {
            case 'r':
                r = true;
                break;
            case 'v':
                v = true;
                break;
        }
    }

    bool check = false;
    for (std::size_t i = 1; i < argc_; ++i) {
        if (argv_[i][0] != '-') {
            file_to_remove =  argv_[i];
            check = true;
        }
        if(check == true)
            break;
    }

    if(r == true){
        ftw(file_to_remove, ftw_callback, 1);
    }
    else{
        remove_and_overwrite(v, file_to_remove, _size);
    }
}

#endif //SUPER_RM_SUPER_RM_H