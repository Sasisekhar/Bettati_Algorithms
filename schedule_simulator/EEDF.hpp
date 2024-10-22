#ifndef EEDF_HPP__
#define EEDF_HPP__

#include <iostream>
#include <algorithm>
#include "data_structures.hpp"

std::vector<process_schedule> schedule_EEDF(const std::vector<tasks>& _task_set, int processor, const std::vector<tasks>& original_task_set = {}) {
    auto task_set = _task_set;
    std::vector<process_schedule> EEDF_schedule;

    int total_deadline = 0;

    for(auto task : task_set) {
        if(task.deadline > total_deadline) {
            total_deadline = task.deadline;
        }
    }

    std::vector<subtasks> subtask_set; // for this processor
    std::vector<subtasks> original_subtask_set; //non-inflated (if algorithm inflates)

    // Get subtasks for the current processor
    for(auto task : task_set) {
        subtask_set.push_back(task.subtask_set[processor]);
    }

    if(!original_task_set.empty()){
        for(const auto task : original_task_set) {
            original_subtask_set.push_back(task.subtask_set[processor]);
        }
    }

    // Sort the subtask_set by effective_deadline (smallest to largest)
    std::sort(subtask_set.begin(), subtask_set.end(), [](const subtasks& a, const subtasks& b) {
        return a.effective_deadline < b.effective_deadline;
    });

    int current_time = 0;

    // Keep track of which subtasks have been completed
    std::vector<bool> completed(subtask_set.size(), false);

    while(current_time < total_deadline) {
        bool task_scheduled = false;

        // Try to find a subtask that can be executed at current_time
        for (size_t i = 0; i < subtask_set.size(); ++i) {
            if (!completed[i] && current_time >= subtask_set[i].effective_release_time) {
                // Schedule the subtask
                EEDF_schedule.push_back({
                    .processor_id = processor,
                    .task = ((original_task_set.empty())? subtask_set[i] : original_subtask_set[i]),
                    .start_time = current_time,
                    .task_duration = subtask_set[i].processing_time,
                });

                // Update the current time and mark this subtask as completed
                current_time += subtask_set[i].processing_time;
                completed[i] = true;
                task_scheduled = true;
                break; // Exit the loop after scheduling a task
            }
        }

        // If no task was scheduled, move to the next earliest release time
        if (!task_scheduled) {
            // Find the next release time for an unscheduled subtask
            int next_release_time = total_deadline; // set a high value as default
            for (size_t i = 0; i < subtask_set.size(); ++i) {
                if (!completed[i]) {
                    next_release_time = std::min(next_release_time, subtask_set[i].effective_release_time);
                }
            }
            // Move the current time to the next release time (idle period)
            current_time = next_release_time;
        }
    }

    return EEDF_schedule;
}


#endif