#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Structure to represent a process control block
struct PCB
{
    string id;
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    int completion_time;
    void printObject();
};

// Prints the PCB object to standard output.
void PCB::printObject()
{
    cout << " ID:" << this->id;
    cout << " Priority:" << this->priority;
    cout << " Burst:" << this->burst_time;
    cout << " Arrival:" << this->arrival_time;
    cout << " Remaining:" << this->remaining_time;
    cout << " Completion:" << this->completion_time;
    cout << endl;
}

/**
 * @brief Priority Round Robin Scheduler.
 *
 * Implements a preemptive priority-based round robin scheduling algorithm.
 * The scheduler re-evaluates process priorities at each time quantum and
 * selects the highest priority process to run. When multiple processes have
 * the same priority, round robin scheduling is used to ensure fairness.
 *
 * @param pcbs Vector of Process Control Blocks to be scheduled
 * @param tq Time quantum for round robin scheduling
 */
void priority_rr_scheduler(vector<PCB> pcbs, int tq)
{
    int tt = 0;    // total time, running the scheduler.
    PCB activePCB; // the actively running pcb on the cpu.

    /* Run PP-RR Scheduler */
    while (!pcbs.empty())
    {
        /* Handle interupts */
        if (tt % tq == 0)
        {
            // interupt
        }

        /* Handle arrival times */
        // if pcb->arrival >= tt : the process can run.

        tt++; // increment total time.
    }
}

// main entry point, test the scheduler here.
int main(int argc, char **argv)
{
    string line;
    char dummy;
    int time_quantum;
    vector<PCB> processes;

    // Read time quantum from standard input
    cin >> dummy >> time_quantum;

    // Read processes from standard input
    while (cin >> line)
    {
        PCB p;
        p.id = line;
        cin >> p.priority >> p.burst_time >> p.arrival_time;
        p.remaining_time = p.burst_time;
        p.completion_time = -1;
        /* p.remaining_quantum = time_quantum; */
        processes.push_back(p);
    }

    /*** Run the scheduler. ***/
    priority_rr_scheduler(processes, time_quantum);

    return 0;
}
