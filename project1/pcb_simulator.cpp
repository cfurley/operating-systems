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
        : pid(_pid), pc(0), state("Ready"), total_work(_total_work) {};
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
//  printf("%d\n", ready_processes.front().pid);

    // Create a Queue for Pointers to the Processes.
    std::queue<PCB*> ready_processes;

    // Place all processes in the queue with the "Ready" state.
    for (PCB& pcb : pcbs) {
        pcb.state = "Ready";
        ready_processes.push(&pcb);
    }

    /* Run the processes for $timeQuantum amount of time. */

    // while there are ready processes..
    int interupt_counter = 0;
    while (!(ready_processes.empty())) {
        // Get the first process of the queue.
        PCB* current_process = ready_processes.front();
        // Pop that process from the queue.
        ready_processes.pop();
        // Set that process's state to "Running".
        current_process->state = "Running";

        // Run the current process for its allotted Quantum Time.
        int current_quantum_work = 0;      
        while(current_quantum_work < timeQuantum) { // While the process is running..
            // Check if the process has more work to do.
            if (current_process->pc < current_process->total_work) {
                current_process->pc += 1; // increment the process program counter;
            } else {
                break; // process is completed, should break execution.
            }
            current_quantum_work += 1; // increment to reflect current work.
        }

        // Increment the interupt counter, once the work is complete.
        interupt_counter += 1;

        // Check if the process has completed execution.
        if (current_process->total_work <= current_process->pc) {
            // Set the process state to terminated.
            current_process->state = "Terminated";
        } else {
            // Set the process state to ready.
            current_process->state = "Ready";
            // Push the process back into the queue.
            ready_processes.push(current_process);
        }

        // Finally, print the state off the current processes.
        printProcessStates(pcbs, interupt_counter);

    } //endwhile
}//endfunc


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