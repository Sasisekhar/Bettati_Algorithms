#ifndef ALGORITHM_C_HPP__
#define ALGORITHM_C_HPP__

#include "algorithmA.hpp"

std::vector<schedule> algorithm_C(std::vector<schedule>& S, int num_processors, int num_tasks) {
    // Initialize a new compacted schedule
    std::vector<schedule> compacted_schedule;

    // Step 1: Initialize the start times (fij = rij) for all tasks
    for (auto& sched : S) {
        sched.task_at_time.second = sched.task_at_time.second;  // fij = rij (already set from Algorithm A)
    }

    // Step 2: Compact tasks to reduce idle time
    for (int processor = 0; processor < num_processors; ++processor) {
        int current_time = 0;  // Track current time for each processor

        // Compact tasks for the current processor
        for (auto& sched : S) {
            if (sched.processor_id == processor) {
                // Adjust the task's start time based on the current time
                sched.task_at_time.second = std::max(sched.task_at_time.second, current_time);
                
                // Update the current time for the next task
                current_time = sched.task_at_time.second + sched.task_duration;

                // Store the compacted schedule
                compacted_schedule.push_back(sched);
            }
        }
    }

    // Update the schedule with the compacted version
    return compacted_schedule;
}

#endif