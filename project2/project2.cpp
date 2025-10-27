#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Structure to represent a process control block.
struct PCB
{
    string id;           // Process identifier (P1, P2, etc.)
    int priority;        // Process priority (higher number = higher priority)
    int burst_time;      // Total CPU time needed
    int arrival_time;    // Time when process arrives
    int remaining_time;  // CPU time still needed
    int completion_time; // Time when process finishes
    int start_time;      // Time when process first runs
};

/**
 * @brief Priority Round Robin Scheduler.
 *
 * A preemptive priority based pcb scheduler, which defaults to round robin
 * scheduling when priorities are equal.
 *
 * @param pcbs Vector of Process Control Blocks
 * @param tq Time Quantum for round robin scheduling
 */
void priority_rr_scheduler(vector<PCB> &pcbs, int tq)
{
    int tt = 0;                // Current time in the simulation
    vector<PCB *> ready_queue; // Pointers to processes ready to run
    PCB *active_pcb = NULL;    // Pointer to currently running process
    int time_slice = 0;        // Time current process has been running
    int cpu_busy_time = 0;     // Total time CPU has been busy

    // Variables to track schedule output
    int schedule_start = 0;       // Start time of current schedule block
    string schedule_process = ""; // Process ID of current schedule block

    // Main scheduler loop - continues until all processes complete
    while (true)
    {
        // Check for new process arrivals at current time
        for (int i = 0; i < pcbs.size(); i++)
        {
            // If process arrives now and still has work to do
            if (pcbs[i].arrival_time == tt && pcbs[i].remaining_time > 0)
            {
                // Add process to ready queue
                ready_queue.push_back(&pcbs[i]);

                // Check if new process should preempt currently running process
                if (active_pcb != NULL && pcbs[i].priority > active_pcb->priority)
                {
                    // New process has higher priority, so preempt current one
                    ready_queue.push_back(active_pcb); // Put current back in queue
                    active_pcb = NULL;                 // Clear active process
                    time_slice = 0;                    // Reset time slice
                }
            }
        }

        // If no process is running and queue is not empty, select one
        if (active_pcb == NULL && ready_queue.size() > 0)
        {
            // Find the process with highest priority in ready queue
            int max_priority_idx = 0; // Index of highest priority process
            for (int i = 1; i < ready_queue.size(); i++)
            {
                // If this process has higher priority than current max
                if (ready_queue[i]->priority > ready_queue[max_priority_idx]->priority)
                {
                    max_priority_idx = i; // Update index of max priority
                }
            }

            // Set the highest priority process as active
            active_pcb = ready_queue[max_priority_idx];

            // Remove it from ready queue
            ready_queue.erase(ready_queue.begin() + max_priority_idx);

            // Reset time slice for new process
            time_slice = 0;
        }

        // Execute one time unit of the active process
        if (active_pcb != NULL)
        {
            // Check if we're switching to a different process
            if (schedule_process != active_pcb->id)
            {
                // Print previous schedule block if it exists
                if (schedule_process != "")
                {
                    // Check if previous was Idle or a process
                    if (schedule_process == "Idle")
                    {
                        // Print Idle without priority
                        cout << "Time " << schedule_start << "-" << tt << ": Idle" << endl;
                    }
                    else
                    {
                        // Find priority of previous process
                        int prev_priority = 0;
                        for (int i = 0; i < pcbs.size(); i++)
                        {
                            if (pcbs[i].id == schedule_process)
                            {
                                prev_priority = pcbs[i].priority;
                                break;
                            }
                        }
                        // Print the schedule entry with priority
                        cout << "Time " << schedule_start << "-" << tt << ": "
                             << schedule_process << " (Priority " << prev_priority << ")" << endl;
                    }
                }
                // Start new schedule block
                schedule_start = tt;
                schedule_process = active_pcb->id;
            }

            // Run process for one time unit
            active_pcb->remaining_time--; // Decrease remaining time
            time_slice++;                 // Increase time slice counter
            cpu_busy_time++;              // CPU is busy

            // Check if process completed
            if (active_pcb->remaining_time == 0)
            {
                // Process is done
                active_pcb->completion_time = tt + 1; // Record completion time
                active_pcb = NULL;                    // Clear active process
                time_slice = 0;                       // Reset time slice
            }
            // Check if time quantum expired
            else if (time_slice >= tq)
            {
                // Time quantum used up, re-queue the process
                ready_queue.push_back(active_pcb); // Add to back of queue
                active_pcb = NULL;                 // Clear active process
                time_slice = 0;                    // Reset time slice
            }
        }
        else
        {
            // CPU is idle (no process to run)
            if (schedule_process != "Idle")
            {
                // Print previous schedule block if it exists
                if (schedule_process != "")
                {
                    // Check if previous was Idle or a process
                    if (schedule_process == "Idle")
                    {
                        // Print Idle without priority
                        cout << "Time " << schedule_start << "-" << tt << ": Idle" << endl;
                    }
                    else
                    {
                        // Find priority of previous process
                        int prev_priority = 0;
                        for (int i = 0; i < pcbs.size(); i++)
                        {
                            if (pcbs[i].id == schedule_process)
                            {
                                prev_priority = pcbs[i].priority;
                                break;
                            }
                        }
                        // Print the schedule entry with priority
                        cout << "Time " << schedule_start << "-" << tt << ": "
                             << schedule_process << " (Priority " << prev_priority << ")" << endl;
                    }
                }
                // Start idle period
                schedule_start = tt;
                schedule_process = "Idle";
            }
        }

        // Move time forward by one unit
        tt++;

        // Check if all processes are complete
        bool all_done = true; // Assume all done
        for (int i = 0; i < pcbs.size(); i++)
        {
            // If any process still has remaining time
            if (pcbs[i].remaining_time > 0)
            {
                all_done = false; // Not all done
                break;
            }
        }

        // If all processes done and nothing in queue and nothing active
        if (all_done && ready_queue.size() == 0 && active_pcb == NULL)
        {
            break; // Exit main loop
        }
    }

    // Print final schedule block
    if (schedule_process != "")
    {
        if (schedule_process == "Idle")
        {
            cout << "Time " << schedule_start << "-" << tt << ": Idle" << endl;
        }
        else
        {
            // Find priority of last process
            int last_priority = 0;
            for (int i = 0; i < pcbs.size(); i++)
            {
                if (pcbs[i].id == schedule_process)
                {
                    last_priority = pcbs[i].priority;
                    break;
                }
            }
            // Print the schedule entry
            cout << "Time " << schedule_start << "-" << tt << ": "
                 << schedule_process << " (Priority " << last_priority << ")" << endl;
        }
    }

    // Print turnaround times
    cout << endl
         << "Turnaround Time" << endl;
    for (int i = 0; i < pcbs.size(); i++)
    {
        // Turnaround time = completion time - arrival time
        int turnaround = pcbs[i].completion_time - pcbs[i].arrival_time;
        cout << pcbs[i].id << " = " << turnaround << endl;
    }

    // Print waiting times
    cout << endl
         << "Waiting Time" << endl;
    for (int i = 0; i < pcbs.size(); i++)
    {
        // Turnaround time = completion time - arrival time
        int turnaround = pcbs[i].completion_time - pcbs[i].arrival_time;
        // Waiting time = turnaround time - burst time
        int waiting = turnaround - pcbs[i].burst_time;
        cout << pcbs[i].id << " = " << waiting << endl;
    }

    // Print CPU utilization
    cout << endl
         << "CPU Utilization Time" << endl;
    cout << cpu_busy_time << "/" << tt << endl;
}

// main entry point
int main()
{
    string line;           // To read lines from input
    char dummy;            // To read the 'q' character
    int time_quantum;      // Time quantum for round robin
    vector<PCB> processes; // Vector to store all processes

    // Read time quantum from standard input (format: q <number>)
    cin >> dummy >> time_quantum;

    // Read processes from standard input
    while (cin >> line)
    {
        PCB p;       // Create new process
        p.id = line; // Set process ID

        // Read priority, burst time, and arrival time
        cin >> p.priority >> p.burst_time >> p.arrival_time;

        // Initialize remaining time to burst time
        p.remaining_time = p.burst_time;

        // Initialize completion time to -1 (not completed yet)
        p.completion_time = -1;

        // Add process to vector
        processes.push_back(p);
    }

    // Run the scheduler
    priority_rr_scheduler(processes, time_quantum);

    return 0;
}
