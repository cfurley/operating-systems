#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Structure to represent a process control block.
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
 * A preemptive priority based pcb scheduler, which defaults to round robin
 * scheduling when priorities are equal. The scheduler also prints out various
 * runtime statistics to std-out as specified within the project requirements.
 *
 * @param pcbs Vector of Process Control Blocks
 * @param tq Time Quantum for round robin scheduling
 */
void priority_rr_scheduler(vector<PCB> pcbs, int tq)
{
    int tt = 0;                    // total time, running the scheduler.
    vector<PCB *> pcb_ready_queue; // ptrs to pcbs ready to run.
    PCB *active_pcb;               // ptr to activly running pcb.

    /* Run PP-RR Scheduler */
    while (!pcbs.empty())
    {
        /* Check for new process arrivals and push into the queue. */
        for (PCB pcb : pcbs)
        {
            if (pcb.arrival_time == tt)
            {
                pcb_ready_queue.push_back(&pcb);
                cout << "tt=" << tt;
                pcb.printObject();
            }
        }

        /* Select highest priority process */ // And round robin, how to implement?
        /* Run the process for a single tq */
        /* Update process the state variables */
        /* Handle completion or re-queue process */

        tt++; // increment total time each iteration.

        /******************************************************************/
        if (tt > 120) // REMOVE THIS LINE
            break;    // REMOVE THIS LINE
        /******************************************************************/
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
