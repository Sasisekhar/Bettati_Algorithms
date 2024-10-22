#ifndef TASK_SET_HPP__
#define TASK_SET_HPP__

#include <vector>
#include <utility>
#include <iomanip>
#include <string>
#include <map>

struct subtasks {
    int spid;
    int processing_time;
    int effective_release_time;
    int effective_deadline;
};

struct tasks {
    int pid;
    int total_processing_time;
    int release_time;
    int deadline;
    std::vector<subtasks> subtask_set;
};

struct process_schedule {
    int processor_id;
    subtasks task;
    int start_time;
    int task_duration; //This value includes artificial idle times
};

// Function to print the data as a table
void print_tasks_as_table(const std::vector<tasks>& task_list) {
    // Print a line separator
    std::cout << "+" << std::string(28, '-') << "+" << std::endl;

    // Print the table header
    std::cout << std::setw(4) << "| Tasks" 
              << std::setw(2) << "|"
              << std::setw(3) << "ri" 
              << std::setw(3) << "di"
              << std::setw(2) << "|"
              << std::setw(4) << "rij"
              << std::setw(4) << "dij"
              << std::setw(5) << "τij"
              << "|"
              << std::endl;

    // Print a line separator
    std::cout << "+" << std::string(28, '-') << "+" << std::endl;

    // Iterate through the list of tasks
    for (const auto& task : task_list) {
        // Print task pid as T1, T2, etc.
        std::cout << "|" << std::setw(6) << "T" + std::to_string(task.pid)
                  << std::setw(2) << "|"
                  << std::setw(3) << task.release_time
                  << std::setw(3) << task.deadline
                  << std::setw(2) << "|";

        // If the task has subtasks, print the first subtask in the same row
        if (!task.subtask_set.empty()) {
            const auto& first_subtask = task.subtask_set[0];
            std::cout << std::setw(4) << first_subtask.effective_release_time
                      << std::setw(4) << first_subtask.effective_deadline
                      << std::setw(4) << first_subtask.processing_time
                      << "|"
                      << std::endl;

            // Print the remaining subtasks in separate rows (without pid, ri, di)
            for (size_t i = 1; i < task.subtask_set.size(); ++i) {
                const auto& subtask = task.subtask_set[i];
                std::cout << "|" << std::setw(6) << ""  // Empty space to align with the task columns
                          << std::setw(2) << "|"
                          << std::setw(6) << ""  // Empty space to align with the task columns
                          << std::setw(2) << "|"
                          << std::setw(4) << subtask.effective_release_time
                          << std::setw(4) << subtask.effective_deadline
                          << std::setw(4) << subtask.processing_time
                          << "|"
                          << std::endl;
            }
        } else {
            // If there are no subtasks, just move to the next line
            std::cout << std::endl;
        }
        // Print a line separator
        std::cout << std::string(30, '-') << std::endl;
    }
}

std::vector<tasks> compute_effective_times(const std::vector<tasks>& _task_set) {
    auto task_set = _task_set;

    for(auto& task : task_set) {
        int ri = task.release_time;
        int di = task.deadline;

        for(int i = 0; i < task.subtask_set.size(); i++) {
            int sum_for_rij = 0;
            int sum_for_dij = 0;

            for(int j = 0; j < i; j++) {
                sum_for_rij += task.subtask_set[j].processing_time;
            }

            for(int j = i + 1; j < task.subtask_set.size(); j++) {
                sum_for_dij += task.subtask_set[j].processing_time;
            }

            task.subtask_set[i].effective_deadline = di - sum_for_dij;
            task.subtask_set[i].effective_release_time = ri + sum_for_rij;
        }
    }

    return task_set;
}

// Function to print the Gantt chart
void print_gantt_chart(const std::vector<process_schedule>& schedule) {

    int x_axis_start = 0;

    // Create a map to group schedules by processor ID
    std::map<int, std::vector<process_schedule>> processor_schedules;

    // Populate the map with schedules grouped by processor
    for (const auto& sched : schedule) {
        processor_schedules[sched.processor_id].push_back(sched);
    }

    for (auto& [processor_id, schedules] : processor_schedules) {
        std::sort(schedules.begin(), schedules.end(), [](const process_schedule& a, const process_schedule& b) {
            return a.start_time < b.start_time;
        });
    }

    // Print Gantt chart for each processor
    for (const auto& [processor_id, tasks] : processor_schedules) {
        // Print the top border of the chart
        std::cout << "Processor: P" << processor_id << "\n+";

        // First loop: Print the Gantt chart boxes (Txx or ∅ for idle time)
        int current_time = x_axis_start;
        for (const auto& task : tasks) {
            int task_start_time = task.start_time;
            int task_end_time = task_start_time + task.task_duration;

            // Print idle time (∅) if there's a gap between the current time and task start
            if (task_start_time > current_time) {
                int idle_duration = task_start_time - current_time;
                std::cout << std::setw(idle_duration * 4) << std::setfill('-');
                std::cout << "+";  // Separator after idle time
            }

            // Print the task box (Txx) with the task duration (width multiplied by 3)
            std::cout << std::setw(task.task_duration * 4) << std::setfill('-');
            std::cout << "+";  // Separator after the task

            // Update the current time to the end of this task
            current_time = task_end_time;
        }

        std::cout << "\n|";  // Start the line with task names (Txx or ∅)

        // Second loop: Print task names (Txx or ∅ for idle time)
        current_time = x_axis_start;
        for (const auto& task : tasks) {
            int task_start_time = task.start_time;
            int task_end_time = task_start_time + task.task_duration;

            // Print idle time (∅) if there's a gap between the current time and task start
            if (task_start_time > current_time) {
                int idle_duration = task_start_time - current_time;
                std::cout << std::setw(idle_duration * 4) << std::setfill('/');
                std::cout << "|";  // Separator after idle time
            }

            if(task.task.processing_time != task.task_duration){
                // Print the task ID (Txx) with the task duration (width multiplied by 3)
                std::cout   << std::setfill(' ') << std::setw((task.task.processing_time * 4)) << "T" + std::to_string(task.task.spid)
                            << std::setfill(':') << std::setw(((task.task_duration - task.task.processing_time) * 4))
                            << "|";  // Separator after the task
            } else {
                // Print the task ID (Txx) with the task duration (width multiplied by 3)
                std::cout   << std::setfill(' ') << std::setw((task.task.processing_time * 4)) << "T" + std::to_string(task.task.spid) + "|";
            }

            // Update the current time to the end of this task
            current_time = task_end_time;
        }

        std::cout << "\n+";

        // Third loop: Print the bottom border of the chart
        current_time = x_axis_start;
        for (const auto& task : tasks) {
            int task_start_time = task.start_time;
            int task_end_time = task_start_time + task.task_duration;

            // Print idle time border
            if (task_start_time > current_time) {
                int idle_duration = task_start_time - current_time;
                std::cout << std::setw(idle_duration * 4) << std::setfill('-');
                std::cout << "+";  // Separator after idle time
            }

            // Print task border
            std::cout << std::setw(task.task_duration * 4) << std::setfill('-');
            std::cout << "+";  // Separator after the task

            // Update the current time to the end of this task
            current_time = task_end_time;
        }

        std::cout << "\n";

        // Fourth loop: Print time indices below the chart
        current_time = x_axis_start;
        std::cout << std::setfill(' ');
        for (const auto& task : tasks) {
            int task_start_time = task.start_time;
            int task_end_time = task_start_time + task.task_duration;

            // Print idle time (skip time index during idle)
            if (task_start_time > current_time) {
                int idle_duration = task_start_time - current_time;
                std::cout << current_time << std::setw((idle_duration * 4));
            }

            if(task.task.processing_time != task.task_duration) {
                // Print task start time and (end time when inflated)
                std::cout   << task_start_time 
                            << std::setw((task.task.processing_time * 4)) 
                            << task_start_time + task.task.processing_time 
                            << std::setw(((task.task_duration - task.task.processing_time) * 4));
            } else {
                // Print task start time
                std::cout << task_start_time << std::setw((task.task_duration * 4));
            }

            // Update the current time to the end of this task
            current_time = task_end_time;
        }

        std::cout << current_time << "\n\n";  // Add some space before the next processor's chart
    }
}

#endif