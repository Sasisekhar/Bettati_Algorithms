#ifndef ALGORITHM_A_HPP__
#define ALGORITHM_A_HPP__

#include <iostream>
#include "EEDF.hpp"

// Algorithm A: Feasibility schedule with precedence
std::pair<bool, std::vector<process_schedule>> algorithm_A( const std::vector<tasks>& task_set, 
                                                            int num_processors, 
                                                            const std::vector<tasks>& original_task_set = {}, 
                                                            const std::pair<bool, int> force_bottleneck = {false, 0} ) {
    std::cout << "\nperforming Algorithm A" << std::endl;
    int total_deadline = 0;
    std::vector<process_schedule> final_schedule;

    for(auto task : task_set) {
        if(task.deadline > total_deadline) {
            total_deadline = task.deadline;
        }
    }
    
    // Step 1: Determine the bottleneck processor
    int bottleneck_processor = 0;
    int max_total_time = 0;

    if(!force_bottleneck.first) {
        for (int processor = 0; processor < num_processors; ++processor) {
            int total_processing_time = 0;
            for (auto task : task_set) {
                total_processing_time += task.subtask_set[processor].processing_time;
            }
            if (total_processing_time > max_total_time) {
                max_total_time = total_processing_time;
                bottleneck_processor = processor;
            }
        }
    } else {
        bottleneck_processor = force_bottleneck.second;
    }

    std::cout << "bottleneck processor: P" << bottleneck_processor << std::endl;

    // Step 2: Schedule subtasks on the bottleneck processor using EEDF
    auto bottleneck_schedule = schedule_EEDF(task_set, bottleneck_processor, original_task_set);

    // Check for feasibility on the bottleneck processor
    for (const auto& sched : bottleneck_schedule) {
        if (sched.start_time + sched.task_duration > total_deadline) {
            std::cout << "no feasible schedule exists for the bottleneck processor.\n";
            return std::make_pair(false, final_schedule);
        }
    }

    std::cout << "schedule on bottleneck processor:" << std::endl;
    print_gantt_chart(bottleneck_schedule);

    // Step 3: Propagate the schedule to other processors
    final_schedule = bottleneck_schedule;  // Start with the bottleneck processor's schedule

    // Propagate tasks to the other processors
    for (auto& task : task_set) {
        for (int processor = bottleneck_processor + 1; processor < num_processors; ++processor) {
            const auto& prev_task = task.subtask_set[processor - 1];
            const auto& curr_task = task.subtask_set[processor];

            // Find the start time of the previous task and schedule the current task immediately after
            for (auto& sched : final_schedule) {
                if (sched.processor_id == processor - 1 && sched.task.spid == prev_task.spid) {
                    final_schedule.push_back({
                        .processor_id = processor,
                        .task = ((original_task_set.empty())?   curr_task :
                                                                std::find_if(original_task_set.begin(), original_task_set.end(), 
                                                                    [task](const tasks& search_task) {
                                                                        return search_task.pid == task.pid;
                                                                    }
                                                                )->subtask_set[processor]),
                        .start_time = sched.start_time + sched.task_duration,
                        .task_duration = curr_task.processing_time,
                    });
                    break;
                }
            }
        }

        // For processors before the bottleneck processor
        for (int processor = bottleneck_processor - 1; processor >= 0; --processor) {
            const auto& curr_task = task.subtask_set[processor];
            const auto& next_task = task.subtask_set[processor + 1];

            // Schedule the current task before the next one starts
            for (auto& sched : final_schedule) {
                if (sched.processor_id == processor + 1 && sched.task.spid == next_task.spid) {
                    int start_time = sched.start_time - curr_task.processing_time;

                    final_schedule.push_back({
                        .processor_id = processor,
                        .task = ((original_task_set.empty())?   curr_task :
                                                                std::find_if(original_task_set.begin(), original_task_set.end(), 
                                                                            [task](const tasks& search_task) {
                                                                                return search_task.pid == task.pid;
                                                                            }
                                                                        )->subtask_set[processor]),
                        .start_time = start_time,
                        .task_duration = curr_task.processing_time
                    });
                    break;
                }
            }
        }
    }

    return std::make_pair(true, final_schedule);
}


#endif