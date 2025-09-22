#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

// Definition of the PCB Structure.
struct PCB {
    int pid;
    int pc;
    std::string state;
    int total_work;

    PCB(int _pid, int _total_work)
        : pid(_pid), pc(0), state("READY"), total_work(_total_work) {};
};

// Print states of all processes, sorted by PID (PROVIDED - DO NOT MODIFY)
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) {
    std::cout << "Interrupt " << timeSlice << ":" << std::endl;
    std::vector<PCB> sorted_pcbs = pcbs;
    std::sort(sorted_pcbs.begin(), sorted_pcbs.end(), 
              [](const PCB& a, const PCB& b) { return a.pid < b.pid; });
    for (const PCB& pcb : sorted_pcbs) {
        std::cout << "PID " << pcb.pid << ": " << pcb.state  << ", at pc " << pcb.pc << std::endl;
    }
    std::cout << std::flush;
}

// Kernel simulator
void kernelSimulator(std::vector<PCB>& pcbs, int timeQuantum) {
    //Implement scheduling as decribed in the project 1 description
    //For each timed interrupt call printProcessStates
    //You may create helper functions as needed
    //Add comments to describe your implementation of this function and/or other helper functions
}


int main() {
    // Read from standard input via redirection (PROVIDED)
    int numProcesses;
    if (!(std::cin >> numProcesses)) {
        std::cerr << "Error: Invalid input for number of processes" << std::endl;
        return 1;
    }
    if (numProcesses <= 0) {
        std::cerr << "Error: Invalid number of processes" << std::endl;
        return 1;
    }
    
    std::vector<PCB> pcbs;
    std::set<int> pids; // Track unique PIDs
    for (int i = 0; i < numProcesses; ++i) {
        int pid, work;
        if (!(std::cin >> pid >> work)) {
            std::cerr << "Error: Invalid input format for process data" << std::endl;
            return 1;
        }
        if (work <= 0) {
            std::cerr << "Error: Invalid work units for PID " << pid << std::endl;
            return 1;
        }
        if (pids.count(pid) > 0) {
            std::cerr << "Error: Duplicate PID " << pid << " detected" << std::endl;
            return 1;
        } else { // insert new job into the pcbs & pids data structures.
            pcbs.emplace_back(PCB(pid,work));
            pids.insert(pid);
        }
    }
    
    int timeQuantum = 2;
    kernelSimulator(pcbs, timeQuantum);
    
    std::cout << "All processes completed." << std::endl;
    return 0;
}