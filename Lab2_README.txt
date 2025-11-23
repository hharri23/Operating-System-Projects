# Banker's Algorithm – Deadlock Avoidance (CS 33211 Project 2)

This project implements the Banker's Algorithm to determine whether a given
system state is safe. It reads the resource-allocations snapshot from an input
file and prints whether the system is in a safe state, and if so, a safe
sequence of processes.

## Build Instructions

Requirements:
- g++ (C++17 or later)
- Linux/Unix environment (or a VM such as VirtualBox)

To build:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 banker.cpp -o banker
