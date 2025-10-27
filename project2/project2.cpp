#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
    int remaining_quantum;
    int start_time;
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
    PCB *active_pcb = nullptr;     // ptr to activly running pcb.
    int time_slice = 0;            // time slice counter for current process
    int cpu_busy_time = 0;         // track CPU utilization

    // Track schedule for output
    vector<tuple<int, int, string, int>> schedule; // start, end, process_id, priority
    int last_schedule_time = 0;
    string last_process = "";

    /* Run PP-RR Scheduler */
    while (true)
    {
        /* Check for new process arrivals and push into the queue. */
        for (size_t i = 0; i < pcbs.size(); i++)
        {
            if (pcbs[i].arrival_time == tt && pcbs[i].remaining_time > 0)
            {
                pcb_ready_queue.push_back(&pcbs[i]);
                pcbs[i].remaining_quantum = tq;
                if (pcbs[i].start_time == -1)
                {
                    pcbs[i].start_time = tt;
                }

                // Check if new process should preempt current
                if (active_pcb != nullptr && pcbs[i].priority > active_pcb->priority)
                {
                    // Preempt current process
                    pcb_ready_queue.push_back(active_pcb);
                    active_pcb = nullptr;
                    time_slice = 0;
                }
            }
        }

        /* Select highest priority process */
        if (active_pcb == nullptr && !pcb_ready_queue.empty())
        {
            // Find highest priority process
            int max_priority_idx = 0;
            for (size_t i = 1; i < pcb_ready_queue.size(); i++)
            {
                if (pcb_ready_queue[i]->priority > pcb_ready_queue[max_priority_idx]->priority)
                {
                    max_priority_idx = i;
                }
            }

            active_pcb = pcb_ready_queue[max_priority_idx];
            pcb_ready_queue.erase(pcb_ready_queue.begin() + max_priority_idx);
            time_slice = 0;
        }

        /* Run the process for a single time unit */
        if (active_pcb != nullptr)
        {
            // Track schedule changes
            if (last_process != active_pcb->id)
            {
                if (!last_process.empty())
                {
                    schedule.push_back(make_tuple(last_schedule_time, tt, last_process, -1));
                }
                last_schedule_time = tt;
                last_process = active_pcb->id;
            }

            active_pcb->remaining_time--;
            time_slice++;
            cpu_busy_time++;

            /* Handle completion or re-queue process */
            if (active_pcb->remaining_time == 0)
            {
                // Process completed
                active_pcb->completion_time = tt + 1;
                active_pcb = nullptr;
                time_slice = 0;
            }
            else if (time_slice >= tq)
            {
                // Time quantum expired, re-queue
                pcb_ready_queue.push_back(active_pcb);
                active_pcb->remaining_quantum = tq;
                active_pcb = nullptr;
                time_slice = 0;
            }
        }
        else
        {
            // CPU is idle
            if (last_process != "Idle")
            {
                if (!last_process.empty())
                {
                    schedule.push_back(make_tuple(last_schedule_time, tt, last_process, -1));
                }
                last_schedule_time = tt;
                last_process = "Idle";
            }
        }

        tt++; // increment total time each iteration.

        // Check if all processes are done
        bool all_done = true;
        for (const auto &p : pcbs)
        {
            if (p.remaining_time > 0)
            {
                all_done = false;
                break;
            }
        }
        if (all_done && pcb_ready_queue.empty() && active_pcb == nullptr)
        {
            break;
        }
    }

    // Add final schedule entry
    if (!last_process.empty())
    {
        schedule.push_back(make_tuple(last_schedule_time, tt, last_process, -1));
    }

    // Print schedule with priorities
    for (const auto &entry : schedule)
    {
        int start = get<0>(entry);
        int end = get<1>(entry);
        string proc_id = get<2>(entry);

        if (proc_id == "Idle")
        {
            cout << "Time " << start << "-" << end << ": Idle" << endl;
        }
        else
        {
            // Find priority
            int priority = 0;
            for (const auto &p : pcbs)
            {
                if (p.id == proc_id)
                {
                    priority = p.priority;
                    break;
                }
            }
            cout << "Time " << start << "-" << end << ": " << proc_id
                 << " (Priority " << priority << ")" << endl;
        }
    }

    // Print turnaround times
    cout << endl
         << "Turnaround Time" << endl;
    for (const auto &p : pcbs)
    {
        int turnaround = p.completion_time - p.arrival_time;
        cout << p.id << " = " << turnaround << endl;
    }

    // Print waiting times
    cout << endl
         << "Waiting Time" << endl;
    for (const auto &p : pcbs)
    {
        int turnaround = p.completion_time - p.arrival_time;
        int waiting = turnaround - p.burst_time;
        cout << p.id << " = " << waiting << endl;
    }

    // Print CPU utilization
    cout << endl
         << "CPU Utilization Time" << endl;
    cout << cpu_busy_time << "/" << tt << endl;
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
        p.remaining_quantum = time_quantum;
        p.start_time = -1;
        processes.push_back(p);
    }

    /*** Run the scheduler. ***/
    priority_rr_scheduler(processes, time_quantum);

    return 0;
}
