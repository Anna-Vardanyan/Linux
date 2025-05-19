#ifndef COPY_COPY_FUNCTION_H
#define COPY_COPY_FUNCTION_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <ftw.h>
#include <fcntl.h>
#include <vector>
#include <string>

char* files[2];
bool recursive = false;
bool noOverwrite = false;
bool forceOverwrite = false;

bool is_empty(std::ifstream& pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void copy_file(bool noOverwrite_, bool forceOverwrite_, char* files_[2]){
    int file1;
    int file2;
    file1 = open(files_[0], O_RDONLY);
    if (file1 == -1) {
        throw std::logic_error("Couldn't open the source file.");
    }
    file2 = open(files_[1], O_WRONLY | O_CREAT, 0666);
    if (file2 == -1) {
        close(file1);
        throw std::logic_error("Couldn't create or open the destination file.");
        exit(EXIT_FAILURE);
    }

    std::ifstream for_checking(files_[1]);

    char buffer[1024];
    int count;

    if (noOverwrite_) {
        if (forceOverwrite_) {
            forceOverwrite_ = false;
        }
        if (!is_empty(for_checking)) {
            throw std::logic_error("Can't overwrite an existing file.");
        } else {
            while ((count = read(file1, buffer, sizeof(buffer))) != 0) {
                write(file2, buffer, count);
            }
        }
    } else if (forceOverwrite_) {
        while ((count = read(file1, buffer, sizeof(buffer))) != 0) {
            write(file2, buffer, count);
        }
    }
}

int ftw_callback(const char *fpath, const struct stat *sb,
                  int typeflag) {
    if(S_ISREG(sb->st_mode)){
        copy_file(noOverwrite, forceOverwrite, files);
    }
    else if(S_ISDIR(sb->st_mode)){
        ftw(fpath, ftw_callback, 1);
    }
    else{
        mkdir(files[1], sb->st_mode);
    }
    return 0;
}

void copy(int argc_, char** argv_) {
    if (argc_ < 3) {
        throw std::logic_error("Not enough arguments.");
    }

    int opt;
    while((opt = getopt(argc_, argv_, "rnf")) != -1){
        switch(opt){
            case 'r':
                recursive = true;
                break;
            case 'n':
                noOverwrite = true;
                break;
            case 'f':
                forceOverwrite = true;
                break;
        }
    }

    std::size_t j = 0;
    for (std::size_t i = 1; i < argc_; ++i) {
        if (argv_[i][0] != '-') {
            files[j] =  argv_[i];
            ++j;
        }
    }

    if(recursive == true){
       ftw(files[0], ftw_callback, 1);
    }

    else {
        copy_file(noOverwrite, forceOverwrite, files);
    }
}


#endif //COPY_COPY_FUNCTION_H