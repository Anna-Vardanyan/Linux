# Linux

This repository contains a set of Linux-based projects demonstrating various system programming tasks and utilities. Each project is implemented from scratch to explore Linux internals, concurrency, and file system operations.

## Projects Overview

### 1. Linux Copy Command (`cp`) Implementation  
A custom implementation of the `cp` command, replicating file copy functionality with low-level system calls.

### 2. Directory Watcher  
A logger that monitors a directory and records events when files or directories are opened or modified. Useful for tracking file access.

### 3. Regular Logger  
A basic logging utility to record system or application events.

### 4. Advanced `rm` Command Implementation  
A secure removal tool that overrides file content before deleting it, adding an extra layer of data safety compared to the standard `rm`.

### 5. Thread-Related Projects  
- Thread Implementation: A simple thread creation and management.
- Mutex Implementation: Custom mutual exclusion primitives to handle concurrency.
- Blocking Queue: A thread-safe queue that blocks on empty/full conditions.  
- Thread Pool: A pool of worker threads for efficient task execution.  

## Getting Started

Each project contains its own folder with source code. The projects are written in C/C++ and use standard Linux system calls and pthreads.

### Requirements  
- Linux environment  
- GCC or Clang compiler  
- Make

## How to Use

1. Clone the repository:  
   ```bash
   git clone https://github.com/yourusername/linux-projects.git
   cd linux-projects
