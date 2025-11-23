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

////////////////////////////////////////
code explaination 
Banker’s Algorithm is a deadlock-avoidance algorithm used by operating systems to safely allocate resources to multiple processes. Before granting a resource request, the algorithm checks whether the system would remain in a safe state. A safe state means there exists at least one execution order (safe sequence) in which all processes can finish without causing deadlock.

The algorithm uses the following data:

Allocation Matrix – resources currently allocated to each process.

Max Matrix – maximum resources each process may request.

Need Matrix = Max – Allocation – remaining resources required for each process.

Available Vector = Total Resources – Sum of Allocations

During the safety check, the algorithm:

Looks for a process whose Need ≤ Available.

Pretends that process finishes and releases its resources back.

Repeats until all processes finish or no progress can be made.

If all processes can finish → the system is SAFE and a safe sequence is printed.
If not → the system is UNSAFE, meaning a deadlock may occur if more resources are allocated.
